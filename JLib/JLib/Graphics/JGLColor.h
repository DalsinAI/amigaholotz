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
// @description: Funciones y estructuras para color.
///////////////////////////////////////////////////////////////////////////////

#ifndef _JGLCOLOR_INCLUDED
#define _JGLCOLOR_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>

// Estructura de definición de color tipo float
class JGLColorf : public JObject
{
public:
  float r;                                        // Componente roja (0.0f - 1.0f)
  float g;                                        // Componente verde (0.0f - 1.0f)
  float b;                                        // Componente azul (0.0f - 1.0f)
  float a;                                        // Componente alfa (0.0f - 1.0f)
  
  // Constructor
  JGLColorf() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {};

  // Constructor
  JGLColorf(float _r, float _g, float _b, float _a = 0.0f) 
  : r(_r), g(_g), b(_b), a(_a) {};
  
  // Constructor
  JGLColorf(float *arr) 
  : r(arr[0]), g(arr[1]), b(arr[2]), a(arr[3]) {};
  
  // Asignación de colores
  const JGLColorf& operator = (const JGLColorf &c)
  {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    
    return *this;
  }

  // Adición de colores
  const JGLColorf& operator += (const JGLColorf &c)
  {
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;
    
    return *this;
  }
};

const JGLColorf RED = JGLColorf(1.0f, 0.0f, 0.0f, 1.0f);
const JGLColorf GREEN = JGLColorf(0.0f, 1.0f, 0.0f, 1.0f);
const JGLColorf BLUE = JGLColorf(0.0f, 0.0f, 1.0f, 1.0f);
const JGLColorf BLACK = JGLColorf(0.0f, 0.0f, 0.0f, 1.0f);
const JGLColorf YELLOW = JGLColorf(1.0f, 1.0f, 0.0f, 1.0f);
const JGLColorf CYAN = JGLColorf(0.0f, 1.0f, 1.0f, 1.0f);
const JGLColorf MAGENTA = JGLColorf(1.0f, 0.0f, 1.0f, 1.0f);
const JGLColorf ORANGE = JGLColorf(1.0f, 0.5f, 0.0f, 1.0f);
const JGLColorf PINK = JGLColorf(1.0f, 0.0f, 0.5f, 1.0f);
const JGLColorf PURPLE = JGLColorf(0.5f, 0.0f, 1.0f, 1.0f);
const JGLColorf INDIGO = JGLColorf(0.0f, 0.5f, 1.0f, 1.0f);
const JGLColorf YELLOWGREEN = JGLColorf(0.5f, 1.0f, 0.0f, 1.0f);
const JGLColorf TURQUOISE = JGLColorf(0.0f, 1.0f, 0.5f, 1.0f);
const JGLColorf GRAY = JGLColorf(0.5f, 0.5f, 0.5f, 1.0f);
const JGLColorf WHITE = JGLColorf(1.0f, 1.0f, 1.0f, 1.0f);
const JGLColorf LIGHTGRAY = JGLColorf(0.25f, 0.25f, 0.25f, 1.0f);

// Estructura de definición de color tipo unsigned byte
class JGLColorub
{
public:
  u8 r;                                // Componente roja (0 - 255)
  u8 g;                                // Componente verde (0 - 255)
  u8 b;                                // Componente azul (0 - 255)
  u8 a;                                // Componente alfa (0 - 255)
  
  // Constructor
  JGLColorub() : r(255), g(255), b(255), a(255) {};

  // Constructor
  JGLColorub(u8 _r, u8 _g, u8 _b, u8 _a = 255)
  : r(_r), g(_g), b(_b), a(_a) {};

  // Asignación de colores
  const JGLColorub& operator = (JGLColorub& c)
  {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
  }
};

// Pasa de color decimal (0-1) a byte (0-255)
inline JGLColorub JGLColorf2ub(JGLColorf &color)
{
  return JGLColorub((u8)color.r*255,
                    (u8)color.g*255,
                    (u8)color.b*255,
                    (u8)color.a*255);
};

// Pasa de color byte (0-255) a decimal (0-1)
inline JGLColorf JGLColorub2f(JGLColorub &color)
{
  return JGLColorf(color.r/255.0f,
                   color.g/255.0f,
                   color.b/255.0f,
                   color.a/255.0f);
};

#endif  // _JGLCOLOR_INCLUDED
