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
// @description: Cuadrícula
///////////////////////////////////////////////////////////////////////////////

#ifndef _JGLGRID_INCLUDED
#define _JGLGRID_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JGLColor.h>
#include <SDL.h>
#include <GL/gl.h>

class JGLGrid : public JDrawable
{
public:
  GLuint nList;                                     // Índice de lista de OpengGL
  float scale;                                      // Escala de cuadrante
  s32 divs;                                         // Divisiones por cuadrante
  JGLColorf color;                                  // Color
  u8 orient;                                        // Orientación

  static const u8 XY;                               // Cuadrícula en plano XY
  static const u8 YZ;                               // Cuadrícula en plano YZ
  static const u8 ZX;                               // Cuadrícula en plano ZX
  
  // Crea unos ejes coordenados
  JGLGrid(s32 _divs = 10, 
          float fScale = 10.0f, 
          JGLColorf _color = JGLColorf(0.0f, 1.0f, 0.0f, 1.0f),
          const u8 _orient = ZX);

  // Crea la lista de OpenGL
  void RebuildList();

  // Pinta la cuadrícula
  void Draw();
};

#endif  // _JGLGRID_INCLUDED
