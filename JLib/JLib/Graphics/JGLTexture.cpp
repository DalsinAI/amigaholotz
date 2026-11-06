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
 *  License along with this library; if not, write to the Free Software
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
 * @file    JGLTexture.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    01/04/2003
 * @version 0.0.1 - 01/04/2003 - Primera Versión.
 */

#include <JLib/Graphics/JGLTexture.h>

// Constructor
JGLTexture::JGLTexture() : instance(0)
{}

// Libera la memoria asociada
void JGLTexture::Destroy()
{
  if (instance)
  {
		--(instance->refCount);

		if (instance->refCount == 0)
		{
			{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
			if (!glIsTexture(instance->id)) printf("JGLTexture::Destroy() id es %d, IsTexture=%d\n", instance->id, glIsTexture(instance->id));
			{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
			glDeleteTextures(1, (GLuint *)&instance->id);
			delete instance;
		}

		instance = 0;
  }
}

u32 JGLTexture::Next2Power(u32 value)
{
	u32 newVal = 1;
	while (value > newVal)
	{
		newVal <<= 1;
	}

	return newVal;
}

// Crea la textura a partir de un buffer.
u32 JGLTexture::Create(JImage *image, bool withMipmaps /*= false*/, s32 internalFormat /*= 4*/, s32 border /*= 0*/, s32 format /*= GL_RGBA*/, s32 type /*= GL_UNSIGNED_BYTE*/)
{
	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};

	// Si no hay datos
  if(!image)
		return 0;

	Destroy();
	
	instance = new JGLTexture::JInstance;
	++(instance->refCount);

  instance->width = image->Width();
  instance->height = image->Height();
  instance->bpp = image->BitsPP();

	instance->hasMipmaps = withMipmaps;

	// Genera la textura con el ID asociado en el array
	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
	glGenTextures(1, (GLuint *)&instance->id);
	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};

	if (0 == instance->id)
	{
		fprintf(stderr, "JGLTexture: No se pudo generar la textura.\n");
		JDELETE(instance);

		return 0;
	}

	// Enlaza el ID a la textura
	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
	glBindTexture(GL_TEXTURE_2D, instance->id);
	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};

	// Crea los mipmaps, si es necesario
	if (instance->hasMipmaps)
	{
	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
    if (0 != gluBuild2DMipmaps(GL_TEXTURE_2D,
															 internalFormat,
															 image->Width(),
															 image->Height(),
															 format,
															 type,
															 image->Pixels()))
		{
			fprintf(stderr, "No se pudieron construir los mipmaps: %s\n", gluErrorString(glGetError()));
			Destroy();
			
			return 0;
		}
	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
	}
  else
	{
		glGetError();

		image->Lock();
    glTexImage2D(GL_TEXTURE_2D, 
								 0, 
								 internalFormat,
								 image->Width(), 
								 image->Height(), 
								 border, 
								 format,
								 type,
								 image->Pixels());
		image->Unlock();
	
		u32 ret = glGetError();
		if (ret != 0)
		{
			fprintf(stderr, "Fallo al invocar glTexImage2D(). Error de OpenGL: %s\n", gluErrorString(ret));
			fprintf(stderr, "Invocado con internalFormat %d, width %d, height %d, border %d, format %d (GL_RGBA es %d), type %d (GL_UNSIGNED_BYTE es %d), pixels es %p\n", 
							internalFormat, image->Width(), image->Height(), border, format, type, image->Pixels());
			Destroy();
			
			return 0;
		}
	}

	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
  // Calidad de la textura
  if (instance->hasMipmaps)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }
  else
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }

	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
  return instance->id;
}

// Crea la textura desde un fichero bmp, tga o jpg.
// Devuelve el id de textura.
u32 JGLTexture::Load(const char *fileName, bool withMipmaps /*= false*/, s32 internalFormat /*= 4*/, s32 border /*= 0*/, s32 format /*= GL_RGBA*/, s32 type /*= GL_UNSIGNED_BYTE*/)
{
	{s32 rettt = glGetError(); if (rettt) printf("%s (%d) Error OpenGL: %s\n", __FILE__, __LINE__, gluErrorString(rettt));};
	if (fileName == 0)
		return 0;

	JImage image;
	if (!image.Load(fileName, 0))
	{
		return 0;
	}

  return Create(&image, withMipmaps, internalFormat, border, format, type);
}

// Constructor copia, referencia la textura.
JGLTexture::JGLTexture(JGLTexture &other) : instance(0)
{
	Ref(other);
}

// Constructor copia, referencia la textura.
JGLTexture::JGLTexture(const JGLTexture &other) : instance(0)
{
	Ref(other);
}

// Referencia la textura dada.
void JGLTexture::Ref(JGLTexture &other)
{
	Destroy();

	if (other.instance != 0)
	{
		instance = other.instance;
		++(instance->refCount);
	}
}

// Referencia la textura dada.
void JGLTexture::Ref(const JGLTexture &other)
{
	Destroy();

	if (other.instance != 0)
	{
		instance = other.instance;
		++(instance->refCount);
	}
}

// Referencia la textura dada.
JGLTexture & JGLTexture::operator=(JGLTexture &other)
{
	Ref(other);
	return *this;
}

// Referencia la textura dada.
JGLTexture & JGLTexture::operator=(const JGLTexture &other)
{
	Ref(other);
	return *this;
}
