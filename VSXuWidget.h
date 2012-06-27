/*
    The QGLWidget based VSXuWidget class, ready to be embeddable in any QtGUI App
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


#ifndef VSXuWidget_H
#define VSXuWidget_H

#include <QGLWidget>
#include <QStringList>
#include <QResizeEvent>

#include "VSXuRenderer.h"

class VSXuWidget : public QGLWidget
{
Q_OBJECT
private:
    VSXuRenderer m_renderer;
    QStringList m_visuals;
    // We need float samples of 512 bytes each
    float soundData[512];

protected:
    void resizeEvent(QResizeEvent* event);
    //Will be taken Care by the Rendering thread
    void paintEvent(QPaintEvent* ){;}
  
public:
    VSXuWidget(QWidget *parent = NULL);
    ~VSXuWidget();
    /**
     * Inject 512 wave samples of sound data meant to be Visualized
     * 
     */
    void injectSound(float soundData[]){
        m_renderer.injectSound(soundData);
    }
};

#endif // VSXuWidget_H
