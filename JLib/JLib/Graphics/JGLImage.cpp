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

/** Clase para la presentación de imágenes OpenGL.
 * @file    JGLImage.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    19/05/2004
 * @version 0.0.1 - 19/05/2004 - Primera versión.
 */

#include <JLib/Graphics/JGLImage.h>

bool JGLImage::Init(JImage *image, bool withMipmaps)
{
	if (image == 0)
	{
		return false;
	}

	s32 w = JGLTexture::Next2Power(image->Width());
	s32 h = JGLTexture::Next2Power(image->Height());
	JImage *img;
	
	// Si la anchura o la altura no son potencia de dos, tenemos que 
	// cambiar el tamaño de la imagen
	if (w != image->Width() || h != image->Height())
	{
		img = new JImage(w, h, image->BitsPP());
		if (!img || !img->Paste(image, 0, 0, image->Width(), image->Height(), 0, 0))
		{
			JDELETE(img);
			
			return false;
		}
	}
	else
	{
		img = image;
	}
	
	bool ok = texture.Create(img, withMipmaps);

	if (img != image)
	{
		delete img;
	}

	if (!ok)
	{
		return false;
	}
	
	// Crea la lista de visualización
	listIndex = glGenLists(1);

	if (listIndex == 0)
	{
		texture.Destroy();

		return false;
	}

	float tw, th, halfW, halfH;
	tw = float(image->Width())/float(w);
	th = float(image->Height())/float(h);
	halfW = float(image->Width())/2.0f;
	halfH = float(image->Height())/2.0f;

	//fprintf(stderr, "Imagen tw%f th%f iw%d ih%d w%d h%d\n", tw, th, image->Width(), image->Height(), w, h);
	
	glNewList(listIndex, GL_COMPILE);
	
	texture.GLBind();
	
	// 2-3-5-7
	// |\|\|\| CCW
	// 0-1-4-6
	glBegin(GL_TRIANGLE_STRIP);

	// Bottom left
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	texture.GLCoord2f(0.0f, th);
	glVertex3f(-halfW, -halfH, 0.0f);

	// Bottom right
	texture.GLCoord2f(tw, th);
	glVertex3f(halfW, -halfH, 0.0f);

	// Top left
	texture.GLCoord2f(0.0f, 0.0f);
	glVertex3f(-halfW, halfH, 0.0f);

	// Top right
	texture.GLCoord2f(tw, 0.0f);
	glVertex3f(halfW, halfH, 0.0f);

	glEnd();

	glEndList();

	return ok;
}
