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

#include <JLib/Graphics/JGLAxes.h>

// Crea unos ejes coordenados
JGLAxes::JGLAxes(bool bshowDivs, 
             float flength, 
             float fxRed, float fxGreen, float fxBlue, 
             float fyRed, float fyGreen, float fyBlue, 
             float fzRed, float fzGreen, float fzBlue)
{
  showDivs = bshowDivs;
  length = flength;
  xRed = fxRed; xGreen = fxGreen; xBlue = fxBlue;
  yRed = fyRed; yGreen = fyGreen; yBlue = fyBlue;
  zRed = fzRed; zGreen = fzGreen; zBlue = fzBlue;
  nList = 0;

  RebuildList();
}

// Crea la lista de OpenGL
void JGLAxes::RebuildList()
{
  if (nList != 0)
    glDeleteLists(nList, 1);

  nList = glGenLists(1);

  glNewList(nList, GL_COMPILE);

  glPointSize(2.0f);
  glBegin(GL_LINES);
  glColor3f(xRed, xGreen, xBlue);
  glVertex3f(-length, 0.0f, 0.0f);
  glVertex3f( length, 0.0f, 0.0f);
  glEnd();

  // Pone varias divisiones
  if (showDivs)
  {
    glBegin(GL_POINTS);
    for (float i = -length + (0.1f * length); i < length; i += (0.1f * length))
      glVertex3f(i, 0.0f, 0.0f);
    glEnd();
  }

  glBegin(GL_LINES);
  glColor3f(yRed, yGreen, yBlue);
  glVertex3f(0.0f, -length, 0.0f);
  glVertex3f(0.0f, length, 0.0f);
  glEnd();

  // Pone varias divisiones
  if (showDivs)
  {
    glBegin(GL_POINTS);
    for (float i = -length + (0.1f * length); i < length; i += (0.1f * length))
      glVertex3f(0.0f, i, 0.0f);
    glEnd();
  }

  glBegin(GL_LINES);
  glColor3f(zRed, zGreen, zBlue);
  glVertex3f(0.0f, 0.0f, -length);
  glVertex3f(0.0f, 0.0f,  length);
  glEnd();

  // Pone varias divisiones
  if (showDivs)
  {
    glBegin(GL_POINTS);
    for (float i = -length + (0.1f * length); i < length; i += (0.1f * length))
      glVertex3f(0.0f, 0.0f, i);
    glEnd();
  }

  glEndList();
}

// Pinta los ejes
void JGLAxes::Draw()
{
  glCallList(nList);
}
