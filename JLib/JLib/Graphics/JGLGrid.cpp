/*
 *  JLib - Jacob's Library.
 *  Copyright (C) 2003, 2004  Juan Carlos Seijo Pérez
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 * 
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 * 
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

///////////////////////////////////////////////////////////////////////////////
// @author: Juan Carlos Seijo Pérez
// @date: 01/04/2003
// @description: Ejes coordenados
///////////////////////////////////////////////////////////////////////////////

#include <JLib/Graphics/JGLGrid.h>

const u8 JGLGrid::XY = 0;                // Cuadrícula en plano XY
const u8 JGLGrid::YZ = 1;                // Cuadrícula en plano YZ
const u8 JGLGrid::ZX = 2;                // Cuadrícula en plano ZX

// Crea unos ejes coordenados
JGLGrid::JGLGrid(s32 _divs,
                 float fScale,
                 JGLColorf _color,
                 const u8 _orient)
{
  scale = fScale;
  color = _color;
  divs = _divs;
  orient = _orient;
  nList = 0;

  RebuildList();
}

// Crea la lista de OpenGL
void JGLGrid::RebuildList()
{
  if (nList != 0)
    glDeleteLists(nList, 1);

  nList = glGenLists(1);

  float delta;
  delta = scale/divs;

  glNewList(nList, GL_COMPILE);

  // Pinta la cuadrícula
  glBegin(GL_LINES);
  glColor3f(color.r, color.g, color.b);
  
  switch(orient)
  {
    case XY:
    {
      for (s32 i = 0; i <= 2 * divs; ++i)
      {
        glVertex3f(scale - (i * delta), scale, 0.0f);
        glVertex3f(scale - (i * delta), -scale, 0.0f);
        glVertex3f(scale, scale - (i * delta), 0.0f);
        glVertex3f(-scale, scale - (i * delta), 0.0f);
      }
      break;
    }
    case YZ:
    {
      for (s32 i = 0; i <= 2 * divs; ++i)
      {
        glVertex3f(0.0f, scale - (i * delta), scale);
        glVertex3f(0.0f, scale - (i * delta), -scale);
        glVertex3f(0.0f, scale, scale - (i * delta));
        glVertex3f(0.0f, -scale, scale - (i * delta));
      }
      break;
    }
    default:
    case ZX:
    {
      for (s32 i = 0; i <= 2 * divs; ++i)
      {
        glVertex3f(scale - (i * delta), 0.0f, scale);
        glVertex3f(scale - (i * delta), 0.0f, -scale);
        glVertex3f(scale, 0.0f, scale - (i * delta));
        glVertex3f(-scale, 0.0f, scale - (i * delta));
      }
      break;
    }
  }

  glEnd();

  glEndList();
}

// Pinta la cuadrícula
void JGLGrid::Draw()
{
  glCallList(nList);
}
