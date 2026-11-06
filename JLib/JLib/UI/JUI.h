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

/** Definiciones de una interfaz de usuario.
 * @file    JUI.h
 * @author  Juan Carlos Seijo Pérez
 * @date    28/07/2004
 * @version 0.0.1 - 28/07/2004 - Primera versión.
 */

#ifndef _JUI_INCLUDED
#define _JUI_INCLUDED

#include <vector>
#include <JLib/Util/JTypes.h>
#include <JLib/UI/JUIElement.h>
#include <JLib/Graphics/JDrawable.h>

/** Clase de gestión de una interfaz de usuario.
 * Gestiona los elementos de la interfaz y se encarga de gestionar el foco
 * dentro de cada uno de ellos en base al estado del teclado y del ratón.
 * Esta clase se encarga de dibujar cada uno de los elementos visibles de la
 * interfaz así como de su actualización.
 */
class JUI : public JDrawable
{
	vector<JUIElement> elements;          /**< Elementos de esta interfaz. */
	vector<JUIElement*> visibles;         /**< Elementos visibles. */
	vector<JUIElement*> invisibles;       /**< Elementos ocultos. */
	vector<JUIElement*> focusables;       /**< Elementos enfocables. */
	
	/** Crea una interfaz vacía. Se deben añadir elementos mediante Add() o bien
	 * cargar una interfaz desde fichero con Load().
	 */
	JUI();

	/** Actualiza la interfaz.
	 */
	s32 Update();

	/** Dibuja la interfaz.
	 */
	void Draw();

	/** Destruye la interfaz. Destruye cada elemento del vector de elementos.
	 */
	void Destroy();

	/** Destructor de la clase.
	 */
	~JUI() {Destroy();}
};

#endif // _JUI_INCLUDED
