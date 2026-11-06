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

#include <JLib/Graphics/JGLConsole.h>

// Constructor
JGLConsole::JGLConsole()
{
  nextLine = 0;
  lines = 0;
  visible = true;
  isFull = false;
  chars = (u8 (*)[8])JGLCONSOLEFONT1;
  ChangeSettings(10, JGLCONSOLE_FONT1);
}

// Destructor
JGLConsole::~JGLConsole()
{
  if (lines)
    delete[] lines;
}

// Cambia los valores por defecto de la consola
void JGLConsole::ChangeSettings(s32 _maxLines, JGLCONSOLE_FONT _font)
{
  if (lines)
  {
    delete[] lines;
  }

  maxLines = _maxLines > 0 ? _maxLines : 10;
  font = _font;

  switch (font)
  {
    case JGLCONSOLE_FONT1:
      chars = (u8 (*)[8])JGLCONSOLEFONT2;
      break;

    default:
    case JGLCONSOLE_FONT2:
      chars = (u8 (*)[8])JGLCONSOLEFONT1;
      break;
  }

  lines = new JGLConsoleLine[maxLines];
  for (s32 i = 0; i < maxLines; ++i)
  {
    *lines[i].str = 0;
  }
  nextLine = 0;
}

// Dibuja la consola si está visible
void JGLConsole::Draw()
{
  if (visible)
  {
    //s32 rp[4];
    //glGetIntegerv(GL_CURRENT_RASTER_POSITION, rp);

    for (s32 i = 0, len = 0; i < maxLines; ++i)
    {
      if (*lines[i].str)
      {
        len = (u32)strlen(lines[i].str);
        //Printf("(%d, %d, %d, %d)", rp[0], rp[1], rp[2], rp[3]);
        //glRasterPos2d(rp[0], rp[1] + (8 * lines[i].line));
        //glRasterPos2d(rp[0], 10);
        glRasterPos2d(0, 10);
        
        for (s32 j = 0; j < len; ++j)
        {
          GLenum e;
          glBitmap(8, 8, 0, 0, 0, 0, ((u8 *)&chars[(u8)lines[i].str[j]]));
          e = glGetError();
          e = e;
        }
      }
    }
  }
}

// Añade una cadena a la consola (tipo 'printf()')
void JGLConsole::Printf(const s8 *str, ...)
{
  if (nextLine == maxLines)
  {
    nextLine = 0;
    isFull = true;
  }

  if (isFull)
  {
    // Desplaza el texto de la consola una línea hacia arriba
    for (s32 i = 0; i < maxLines; ++i)
    {
      --lines[i].line;
    }
  }

  va_list vlist;
  s8 _str[JGLCONSOLE_MAX_LINE];

  va_start(vlist, str);
  vsprintf(_str, str, vlist);
  va_end(vlist);

  strcpy(lines[nextLine].str, _str);
  lines[nextLine].line = isFull ? maxLines - 1 : nextLine;
  ++nextLine;
}

// Borra la consola
void JGLConsole::Clear()
{
  for (s32 i = 0; i < maxLines; ++i)
  {
    *lines[i].str = 0;
  }

  nextLine = 0;
}
