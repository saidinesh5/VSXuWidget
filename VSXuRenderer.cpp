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


void VSXuRenderer::run()
{
    //HACK: Waiting till the QGLWidget has been actually created
    msleep(20);
    // init manager with the shared path and sound input type.
    m_widget->makeCurrent();

    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // A nice black screen till VSXu actually loads itself
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_manager = manager_factory();
    m_manager->init(0,"pulseaudio");
    //for manual sound injection, use: manager->init( path.c_str() , "media_player");  
    m_widget->swapBuffers();

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
    if(m_manager)
        manager_destroy(m_manager);
}
