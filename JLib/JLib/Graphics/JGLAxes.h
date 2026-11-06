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

#ifndef _JGLAXES_INCLUDED
#define _JGLAXES_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <SDL.h>
#include <GL/gl.h>

class JGLAxes : public JDrawable
{
public:
  GLuint nList;
  float length;
  float xRed, xGreen, xBlue;
  float yRed, yGreen, yBlue;
  float zRed, zGreen, zBlue;
  bool showDivs;
  
  // Crea unos ejes coordenados
  JGLAxes(bool bshowDivs = true, 
        float flength = 2.0f, 
        float fxRed = 1.0f, float fxGreen = 0.0f, float fxBlue = 0.0f, 
        float fyRed = 0.0f, float fyGreen = 1.0f, float fyBlue = 0.0f, 
        float fzRed = 0.0f, float fzGreen = 0.0f, float fzBlue = 1.0f);

  // Crea la lista de OpenGL
  void RebuildList();

  // Pinta los ejes
  void Draw();
};

#endif  // _JGLAXES_INCLUDED
