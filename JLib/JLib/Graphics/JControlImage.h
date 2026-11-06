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

/** Control de interfaz de usuario compuesto por imágenes.
 * @file    JControlImage.h.
 * @author  Juan Carlos Seijo Pérez.
 * @date    27/10/2003.
 * @version 0.0.1 - 27/10/2003 - Primera versión.
 */

#ifndef _JIMAGECONTROL_INCLUDED
#define _JIMAGECONTROL_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JControl.h>
#include <JLib/Graphics/JImage.h>
#include <SDL.h>

/** Control de interfaz de usuario compuesto por imágenes.
 */
class JControlImage : public JControl
{
protected:
  JImage imgNormal;                    /**< Imagen en estado normal */
  JImage imgDisabled;                  /**< Imagen en estado deshabilitado */
  JImage imgFocused;                   /**< Imagen en estado enfocado */
  JImage imgSelected;                  /**< Imagen en estado seleccionado */

  /** Destructor
   */
  void Destroy();
  
public:
  /** Crea un control de imágenes vacío. Init o Load deben ser llamados antes de usarlo.
   */
  JControlImage() : JControl(), imgNormal(0), imgDisabled(0), imgFocused(0), imgSelected(0) 
	{}

	/** Inicializa el control a partir de las imágenes dadas.
	 * @return <b>true</b> si se inicializó correctamente, <b>false</b> si no.
	 */
	bool Init(JImage &normal, JImage &disabled, JImage &focused, JImage &selected);

  /** Función de dibujo cuando está visible */
  virtual void DrawVisible();

  /** Función de dibujo cuando está enfocado */
  virtual void DrawFocus();

  /** Función de dibujo cuando está deshabilitado */
  virtual void DrawDisabled();

  /** Función de dibujo cuando está seleccionado */
  virtual void DrawSelected();
  
  /** Función de actualización cuando está visible */
  virtual s32 UpdateVisible();

  /** Función de actualización cuando está enfocado */
  virtual s32 UpdateFocus();

  /** Función de actualización cuando está deshabilitado */
  virtual s32 UpdateDisabled();

  /** Función de actualización cuando está seleccionado */
  virtual s32 UpdateSelected();

	/** Carga el control desde un fichero.
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
};

#endif  // _JIMAGECONTROL_INCLUDED
