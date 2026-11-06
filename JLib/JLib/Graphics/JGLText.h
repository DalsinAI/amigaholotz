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
// @date: 30/04/2003
// @description: Texto en pantalla con funciones de formato (OpenGL)
///////////////////////////////////////////////////////////////////////////////

#ifndef _JGLTEXT_INCLUDED
#define _JGLTEXT_INCLUDED

#include <JLib/Util/JObject.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JGLApp.h>
#include <JLib/Graphics/JFont.h>
#include <SDL.h>
#include <GL/gl.h>

class JGLText : public JObject
{
public:
  GLuint list;                      // Lista base OpenGL
  JFont font;                       // Formato de fuente
  int widths[256];                  // Anchuras por caracter
  int lineHeight;                   // Altura de línea

  // Constructor
  JGLText();

  // Destructor
  ~JGLText();

  // Inicializa la fuente
  bool Init(const char* name = "Courier New",
            int fontSize = 12,
            int fontWeight = 30,
            bool cursive = false,
            bool underscore = false,
            bool bitmapFont = true);

  // Inicializa la fuente
  bool Init(JFont *_font);

  // Crea las listas de fuente
  bool BuildFont();

  // Comienza la escritura de texto
  void Begin();

  // Dibuja en pantalla la cadena dada en la posición dada
  void Draw(int x, int y, const char *text);

  // Muestra un mensaje (tipo 'printf()')
  void Printf(int x, int y, const char *str, ...);

  // Finaliza la escritura de texto
  void End();
};

#endif  // _JGLTEXT_INCLUDED
