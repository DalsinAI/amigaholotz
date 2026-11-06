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
// @date: 01/11/2003
// @description: Luz OpenGL.
///////////////////////////////////////////////////////////////////////////////

#ifndef _JGLLIGHT_INCLUDED
#define _JGLLIGHT_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>
#include <JLib/Math/JVector.h>
#include <JLib/Graphics/JGLColor.h>
#include <SDL.h>
#include <GL/gl.h>

// Clase para gestión de texturas
class JGLLight : public JObject
{
  s32 number;                               // Número de luz OpenGL
  JVector pos;                              // Posición
  JVector dir;                              // Dirección
  float cutOff;                             // Ángulo de apertura
  JGLColorf ambient;                        // Color ambiente
  JGLColorf diffuse;                        // Color difuso
  JGLColorf specular;                       // Color especular

public:
  // Constructor
  JGLLight(s32 index = 0) : number(index){}

  // Destructor
  ~JGLLight() {}

  // Devuelve el número de orden OpenGL (GL_LIGHT0, etc.)
  s32 Number() {return number;}
  
  // Establece el número de orden (GL_LIGHT0, etc.)
  void Number(s32 n) {number = n;}
  
  // Devuelve la posición
  JVector & Pos() {return pos;}

  // Establece la posición
  void Pos(float x, float y, float z) {pos.x = x; pos.y = y; pos.z = z; glLightfv(number, GL_POSITION, (float *)&pos);}

  // Establece la posición
  void Pos(JVector &newPos) {pos = newPos;  glLightfv(number, GL_POSITION, (float *)&pos);}

  // Devuelve la dirección
  const JVector & GetDir() {return dir;}

  // Establece la dirección
  void Dir(float x, float y, float z) {dir.x = x; dir.y = y; dir.z = z;  glLightfv(number, GL_POSITION, (float *)&dir);}

  // Establece la dirección
  void Dir(JVector &newDir) {dir = newDir; glLightfv(number, GL_SPOT_DIRECTION, (float *)&dir);}

  // Devuelve el ángulo de apertura
  float CutOff() {return cutOff;}

  // Establece el ángulo de apertura
  void CutOff(float _cutOff) {cutOff = _cutOff; glLightfv(number, GL_SPOT_CUTOFF, &cutOff);}

  // Devuelve el color ambiente
  JGLColorf & Ambient() {return ambient;}

  // Establece el color ambiente
  void Ambient(float r, float g, float b, float a) 
  {ambient.r = r; ambient.g = g; ambient.b = b; ambient.a = a;  glLightfv(number, GL_AMBIENT, (float *)&ambient);}

  // Establece el color ambiente
  void Ambient(JGLColorf &color) {ambient = color; glLightfv(number, GL_AMBIENT, (float *)&ambient);}

  // Devuelve el color difuso
  JGLColorf & Diffuse() {return diffuse;}

  // Establece el color difuso
  void Diffuse(float r, float g, float b, float a) 
  {diffuse.r = r; diffuse.g = g; diffuse.b = b; diffuse.a = a; glLightfv(number, GL_DIFFUSE, (float *)&diffuse);}

  // Establece el color difuso
  void Diffuse(JGLColorf &color) {diffuse = color; glLightfv(number, GL_DIFFUSE, (float *)&diffuse);}

  // Devuelve el color especular
  JGLColorf & Specular() {return specular;}

  // Establece el color especular
  void Specular(float r, float g, float b, float a) 
  {specular.r = r; specular.g = g; specular.b = b; specular.a = a; glLightfv(number, GL_SPECULAR, (float *)&specular);}

  // Establece el color especular
  void Specular(JGLColorf &color) {specular = color; glLightfv(number, GL_SPECULAR, (float *)&specular);}
};

#endif  // _JGLLIGHT_INCLUDED
