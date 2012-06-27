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


#ifndef VSXURENDERER_H
#define VSXURENDERER_H

#include <QThread>
#include <vsx_manager.h>

class VSXuWidget;
class VSXuRenderer: public QThread
{
  Q_OBJECT
  
    vsx_manager_abs *m_manager;
    VSXuWidget *m_widget;
    bool m_isRunning, m_doResize;
    int m_width,m_height;
    float m_soundData[500];
    
    //The Main Loop for VSXu Renderer
    void run();
public:
    VSXuRenderer(VSXuWidget* parent);
    ~VSXuRenderer();
    void injectSound(float soundData[]);
    void stop(){ m_isRunning = false;}
    void resize(int w, int h);
};

#endif // VSXURENDERER_H
