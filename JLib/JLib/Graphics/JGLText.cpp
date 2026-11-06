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
// @description: Texto en pantalla con funciones de fonto (OpenGL)
///////////////////////////////////////////////////////////////////////////////

#include <JLib/Graphics/JGLText.h>

extern JGLApp *g_pApp;

// Constructor
JGLText::JGLText()
{
  list = 0;
}

// Destructor
JGLText::~JGLText()
{
  // Borra la lista
  if (list != 0)
    glDeleteLists(list, 255);
}

// Inicializa la fuente
bool JGLText::Init(const char* name /* = "Courier New"*/,
                   int fontSize /*= 12*/,
                   int fontWeight /*= 30*/,
                   bool cursive /*= false*/,
                   bool underscore /*= false*/,
                   bool bitmapFont /*= true*/)
{
  strcpy(font.name, name);
  font.fontSize = fontSize;
  font.fontWeight = fontWeight;
  font.cursive = cursive;
  font.underscore = underscore;
  font.bitmapFont = bitmapFont;
  
  return BuildFont();
}

// Inicializa la fuente
bool JGLText::Init(JFont *_font)
{
  if (!_font)
  {
    // Pone la fuente por defecto
    return Init();
  }
  else
  {
    memcpy(&font, _font, sizeof(JFont));
  }

  return BuildFont();
}

// Crea las listas de fuente
bool JGLText::BuildFont()
{
  if (list != 0)
    glDeleteLists(list, 255);
  
  list = glGenLists(255);
  
  if (list != 0)
  {
    // Crea la fuente a partir de las de windows
    HFONT fnt = CreateFont(-font.fontSize,
                           0,
                           0,
                           0,
                           font.fontWeight,
                           font.cursive,
                           font.underscore,
                           0,
                           ANSI_CHARSET,
                           OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS,
                           ANTIALIASED_QUALITY,
                           DEFAULT_PITCH | FF_SWISS,
                           font.name);

    HFONT fntOld = (HFONT)SelectObject(g_pApp->hDC, fnt);

    // Crea la lista de OpenGL
    if (font.bitmapFont)
    {
      wglUseFontBitmaps(g_pApp->hDC, 0, 255, list);
    }
    else
    {
      wglUseFontOutlines(g_pApp->hDC, 0, 255, list, 0.0f, 0.0f, WGL_FONT_POLYGONS, 0);
    }

    // Toma la anchura de cada caracter
    GetCharWidth32(g_pApp->hDC, 0, 255, widths);

    SelectObject(g_pApp->hDC, fntOld);
    DeleteObject(fnt);

    return true;
  }

  return false;
}

// Comienza la escritura de texto
void JGLText::Begin()
{
  // Cambia a 2D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, g_pApp->width, g_pApp->height, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

	// PARA TEXTURAS

  // Push the neccessary Attributes on the stack
	/*
  glPushAttrib(GL_TEXTURE_BIT|GL_ENABLE_BIT);

	glBindTexture(GL_TEXTURE_2D, fontID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	glEnable(GL_TEXTURE_2D);*/

	// Always Draw in Front
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
}

// Dibuja en pantalla la cadena dada en la posición dada
void JGLText::Draw(int x, int y, const char *text)
{
  float xoff = (float)x, yoff = (float)y;
  size_t len = strlen(text);
  glRasterPos2f(xoff, yoff);

  // Dibuja los caracteres en pantalla
  for (unsigned int i = 0; i < len; ++i)
  {
    if (text[i] == 10)
    {
      yoff += font.fontSize;
      xoff = (float)x;
      glRasterPos2f(xoff, yoff);
    }
    else
    {
      glCallList(text[i]);
      xoff += widths[text[i]];
    }

    glRasterPos2f(xoff, yoff);
  }
}

// Muestra un mensaje (tipo 'printf()')
void JGLText::Printf(int x, int y, const char *str, ...)
{
  va_list vlist;
  char _str[1024];

  va_start(vlist, str);
  vsprintf(_str, str, vlist);
  va_end(vlist);

  Draw(x, y, _str);
}

// Finaliza la escritura de texto
void JGLText::End()
{
	// Restaura las matrices originales
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
