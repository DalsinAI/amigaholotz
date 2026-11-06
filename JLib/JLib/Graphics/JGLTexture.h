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

/** Clase para gestión de texturas. El objeto JGLTexture consta de las 
 * coordenadas de textura dentro de la superficie de datos y de un índice 
 * de textura OpenGL. El llamador es responsable de que la textura sea válida 
 * (por ejemplo de ancho y alto como potencia de 2, donde se aplique o 
 * dimensiones mínimas y/o máximas dependientes de la implementación OpenGL).
 * @file    JGLTexture.h
 * @author  Juan Carlos Seijo Pérez
 * @date    01/04/2003
 * @version 0.0.1 - 01/04/2003 - Primera Versión.
 */

#ifndef _JGLTEXTURE_INCLUDED
#define _JGLTEXTURE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>
#include <JLib/Graphics/JImage.h>
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE GL_CLAMP
#endif//GL_CLAMP_TO_EDGE

/** Clase para gestión de texturas
 */
class JGLTexture : public JObject
{
	/** Instancia de textura. Sirve para llevar cuenta de las referencias a una misma textura creadas.
	 */
	struct JInstance
	{
		JInstance() : id(0), refCount(0) {}
		u32 id;
		bool hasMipmaps;
		s32 width;
		s32 height;
		s32 bpp;
		u32 refCount;
	} *instance;

  // Crea la textura.
  u32 Create();

public:
  // Constructor
  JGLTexture();

  // Constructor copia, referencia la textura.
  JGLTexture(JGLTexture &other);

  // Constructor copia, referencia la textura.
  JGLTexture(const JGLTexture &other);

	// Determina si esta textura es válida
	bool Valid() {return instance != 0;}

  // Crea la textura a partir de un buffer.
  u32 Create(JImage *image, bool withMipmaps = true, s32 internalFormat = 4, s32 border = 0, s32 format = GL_RGBA, s32 type = GL_UNSIGNED_BYTE);

  // Crea la textura desde un fichero bmp, targa, jpg, png, xpm o pcx.
  // Devuelve el id de textura y no se queda una copia.
  u32 Load(const char *fileName, bool withMipmaps = true, s32 internalFormat = 4, s32 border = 0, s32 format = GL_RGBA, s32 type = GL_UNSIGNED_BYTE);

  // Libera la memoria asociada
  void Destroy();

	// Devuelve la siguiente potencia de dos por encima del valor dado.
	static u32 Next2Power(u32 value);

  // Devuelve el identificador de textura
  u32 ID() {return instance ? instance->id : 0;}

  // Devuelve la superficie de textura, debe estar enlazada a OpenGL (bind)
  u8* Data() {u8 *data; glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); return data;}

  // Devuelve la anchura de la textura
  u32 Width() {return instance->width;}

  // Devuelve la altura de la textura
  u32 Height() {return instance->height;}

  // Devuelve la profundidad de color
  u32 BitsPP() {return instance->bpp;}

  // Enlaza la textura a OpenGL
  void GLBind() {glBindTexture(GL_TEXTURE_2D, instance->id);}

  // Establece la repetición x e y
  static void GLRepeat() {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);}

  // Establece clamp x e y
  static void GLClamp() {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);}

  // Establece clamp to edge x e y
  static void GLClampToEdge() {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);}

  // Establece el parámetro OpenGL dado para las texturas 2D
  static void GLParameter(u32 param, u32 value) {glTexParameteri(GL_TEXTURE_2D, param, value);}

  // Establece las coordenadas de textura
  void GLCoord2f(float s, float t) {glTexCoord2f(s, t);}

  // Establece las coordenadas de textura
  void GLCoord2fv(const float *v) {glTexCoord2fv(v);}

  // Establece las coordenadas de textura
  void GLCoord2d(s32 s, s32 t) {glTexCoord2i(s, t);}

  // Establece las coordenadas de textura
  void GLCoord2dv(const s32 *v) {glTexCoord2iv(v);}

  // Referencia la textura dada.
  void Ref(JGLTexture &other);

  // Referencia la textura dada.
	JGLTexture & operator=(JGLTexture &other);

  // Referencia la textura dada.
  void Ref(const JGLTexture &other);

  // Referencia la textura dada.
	JGLTexture & operator=(const JGLTexture &other);

  // Destructor
  virtual ~JGLTexture() {Destroy();}
};

#endif  // _JGLTEXTURE_INCLUDED
