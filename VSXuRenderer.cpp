/*
    The Rendering Thread doing the main VSXu Rendering
    Copyright (C) 2012  Dinesh Manajipet <saidinesh5@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <vector>
#include <string>

#include <QFileInfo>
#include <QImage>

#include <GL/glu.h>
#include "VSXuRenderer.h"
#include "VSXuWidget.h"

VSXuRenderer::VSXuRenderer(VSXuWidget* parent):
  m_widget(parent),
  m_manager(0),
  m_isRunning(true),
  m_doResize(true),
  m_isActive(true),
  m_width(640),
  m_height(480)
{

}

QStringList VSXuRenderer::getVisuals()
{
    QStringList result;
    if(!m_manager)
      return result;

    std::vector<std::string> visuals = m_manager->get_visual_filenames();
    for(std::vector<std::string>::iterator it = visuals.begin(); it < visuals.end() ; it++){
      result<< QFileInfo( QString::fromStdString(*it)).fileName().replace(".vsx","");
    }

    return result;
}


void VSXuRenderer::resize(int w, int h)
{
    m_width = w;
    m_height = h;
    m_doResize = true;
}

void VSXuRenderer::injectSound(float soundData[])
{
  /* uncomment for manual sound injection
  for (int i = 0; i < 512; i++){
      //Making a local copy of the sound data for updating the sound data
      m_soundData[i] = soundData[i];
  }
  manager->set_sound_wave(m_soundData);
  */
}

void VSXuRenderer::drawSplashScreen()
{
    QImage splash = QGLWidget::convertToGLFormat(QImage(":luna.png"));
    int edge = m_width > m_height? m_height:m_width;

    glEnable(GL_TEXTURE_2D);
    GLuint texture_splash;
    glGenTextures(1,&texture_splash);
    glBindTexture(GL_TEXTURE_2D, texture_splash);
    glTexImage2D(GL_TEXTURE_2D, 0, 3 , splash.width(), splash.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,splash.bits());

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport((m_width-edge)/2, (m_height-edge)/2, edge, edge);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(-0.5,-0.5,0);
    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(0.0, 0.0);
      glVertex2f(0.0,0.0);

      glTexCoord2f(0.0, 1.0);
      glVertex2f(0.0,1.0);

      glTexCoord2f(1.0, 0.0);
      glVertex2f(1.0,0.0);

      glTexCoord2f(1.0, 1.0);
      glVertex2f(1.0,1.0);

    glEnd();

    m_widget->swapBuffers();
}


void VSXuRenderer::run()
{
    //HACK: Waiting till the QGLWidget has been actually created
    msleep(20);
    // init manager with the shared path and sound input type.
    m_widget->makeCurrent();

    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // A nice splash screen till VSXu actually loads itself
    drawSplashScreen();

    m_manager = manager_factory();
    m_manager->init(0,"pulseaudio");
    //for manual sound injection, use: manager->init( path.c_str() , "media_player");


    while (m_isRunning){
      if(!m_isActive){
        //Saving CPU cycles when not active
        //Just to not flood the system with too many rendering calls.
        msleep(10);
        continue;
      }
      m_widget->makeCurrent();
      if (m_doResize){
          glViewport(0, 0, m_width, m_height);
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          // set origin to bottom left corner
          gluOrtho2D(0, m_width, 0, m_height);
          m_doResize = false;
      }
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if(m_manager)
          m_manager->render();
      m_widget->swapBuffers();
    //m_widget->doneCurrent();
  }
}

VSXuRenderer::~VSXuRenderer()
{
    stop();
    if(m_manager)
        manager_destroy(m_manager);
}
