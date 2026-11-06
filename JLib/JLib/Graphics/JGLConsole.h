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
// @date: 20/10/2003
// @description: Consola de texto para OpenGL independiente de la plataforma.
///////////////////////////////////////////////////////////////////////////////

#ifndef _JGLCONSOLE_INCLUDED
#define _JGLCONSOLE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JGLConsoleFont.h>
#include <GL/gl.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define JGLCONSOLE_MAX_LINE 128

typedef enum
{
  JGLCONSOLE_FONT1 = 1,
  JGLCONSOLE_FONT2
} JGLCONSOLE_FONT;

// Línea de texto en la consola
typedef struct _JGLConsoleLine
{
  s8 str[JGLCONSOLE_MAX_LINE];          // Cadena a mostrar
  s32 line;                               // Línea en la que se muestra
} JGLConsoleLine;

class JGLConsole : public JDrawable
{
  s32 maxLines;                           // Nº de líneas en pantalla
  bool visible;                           // Flag de visibilidad
  s32 nextLine;                           // Siguiente línea libre
  bool isFull;                            // Indicador de texto en la última línea
  u8 (*chars)[8];              // Puntero a los caracteres
  JGLCONSOLE_FONT font;                   // Fuente usada
  JGLConsoleLine *lines;                  // Líneas en pantalla

public:

  // Constructor
  JGLConsole();

  // Destructor
  ~JGLConsole();

  // Inicializa la consola
  void ChangeSettings(s32 _maxLines, JGLCONSOLE_FONT _font);

  // Dibuja la consola si está visible
  void Draw();

  // Añade una cadena a la consola (tipo 'printf()')
  void Printf(const s8 *str, ...);

  // Borra la consola
  void Clear();

  // Muestra/oculta la consola
  void SetVisible(bool b) {visible = b;}

  // Determina si la consola está visible
  bool IsVisible() {return visible;}
};

#endif  // _JGLCONSOLE_INCLUDED
