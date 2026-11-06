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

/** Clase de gestión de una interfaz de usuario.
 * @file    JUI.h
 * @author  Juan Carlos Seijo Pérez
 * @date    13/06/2004
 * @version 0.0.1 - 13/06/2004 - Primera versión.
 */

#ifndef _JUI_INCLUDED
#define _JUI_INCLUDED

#include <JLib/Util/JObject.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JControl.h>
#include <JLib/Graphics/JControlImage.h>

/** Clase de gestión de una interfaz de usuario.
 */
class JUI : public JDrawable
{
	s32 numControls;                      /**< Número de controles que contiene la interfaz. */
	JControl **controls;                  /**< Controles que conforman la interfaz. */

 public:
	/** Contruye el objeto vacio. Init debe ser llamado para comenzar a usarlo.
	 */
	JUI() : numControls(0), controls(0)
	{}

	/** Inicializa la interfaz.
	 */
	bool Init();

	/** Devuelve los controles de esta interfaz.
	 */
	JControl **& Controls() {return controls;}

	/** Dibuja la interfaz de usuario.
	 */
	void Draw();

	/** Actualiza la interfaz de usuario.
	 * @return Siempre 0.
	 */
	s32 Update();

	/** Actualiza la interfaz en función del ratón.
	 */
	s32 TrackMouse(s32 x, s32 y, s32 bt);

	/** Actualiza la interfaz en función del teclado.
	 */
	s32 TrackKeyboard(s32 x, s32 y, s32 bt);

	/** Carga la interfaz desde un fichero de definición de interfaz.
   * @param  f Fichero abierto y posicionado para cargar el objeto.
   * @return 0 si todo va bien, 1 en caso de error de E/S,
   * 2 en caso de incoherencia de los datos.
	 */
	u32 Load(JRW &f);

	/** Salva la interfaz a un fichero de definición de interfaz.
   * @param  f Fichero abierto y posicionado para salvar el objeto.
   * @return 0 si todo va bien, 1 en caso de error de E/S,
   * 2 en caso de incoherencia de los datos.
	 */
	u32 Save(JRW &f);

	/** Destruye los controles de la interfaz.
	 */
	void Destroy();

	/** Destruye el objeto.
	 */
	~JUI() {Destroy();};
};

#endif // _JUI_INCLUDED
