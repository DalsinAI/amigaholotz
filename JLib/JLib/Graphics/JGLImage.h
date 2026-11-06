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
 * @file    JGLImage.h
 * @author  Juan Carlos Seijo Pérez
 * @date    19/05/2004
 * @version 0.0.1 - 19/05/2004 - Primera versión.
 */

#ifndef _JGLIMAGE_INCLUDED
#define _JGLIMAGE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Graphics/JGLTexture.h>

/** Permite mostrar imágenes en OpenGL de forma simple.
 */
class JGLImage : public JDrawable
{
	JGLTexture texture;                   /**< Textura OpenGL asociada. */
	u32 listIndex;                        /**< Índice de lista de visualización. */

 public:
	/** Crea la imagen openGL vacía. Se debe llamar a Init antes de comenzar a usarla.
	 */
	JGLImage() : listIndex(0)
	{}
	
	/** Carga la imagen dada en una textura OpenGL y prepara la lista de visualización
	 * para mostrarla en pantalla.
	 * @param  image Imagen a cargar.
	 * @param  withMipmaps Determina si la debe crear con niveles de mipmaps o no.
	 * @return <b>true</b> si se cargó correctamente, <b>false</b> si no.
	 * @todo implementar malla.
	 */
	bool Init(JImage *image, bool withMipmaps = false);

	/** Devuelve la textura OpenGL.
	 * @return Textura asociada.
	 */
	const JGLTexture & Texture() {return texture;}

	/** Dibuja la imagen.
	 */
	void Draw() {glCallList(listIndex);}

	/** Devuelve la anchura de la imagen.
	 * @return Anchura de la imagen.
	 */
	s32 Width() {return texture.Width();}

	/** Devuelve la altura de la imagen.
	 * @return Altura de la imagen.
	 */
	s32 Height() {return texture.Height();}

	/** Destruye el objeto.
	 */
	virtual ~JGLImage()
	{}
};

#endif // _JGLIMAGE_INCLUDED
