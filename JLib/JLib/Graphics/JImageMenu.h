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

/** Menu en pantalla compuesto de imágenes.
 * @file    JImageMenu.h
 * @author  Juan Carlos Seijo Pérez
 * @date    28/04/2004
 * @version 0.0.1 - 28/04/2004 - Primera versión.
 * @version 0.0.2 - 25/09/2004 - Modificación del método 2D de Pos() para aceptar floats (quita libertad sino).
 */

#ifndef _JIMAGEMENU_INCLUDED
#define _JIMAGEMENU_INCLUDED

#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Util/JApp.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JTree.h>
#include <JLib/Util/JTimer.h>

/** Distribución del menú.
 */
typedef enum 
{
	JIMAGEMENU_LEFT = 1,                   /**< Alineado a la izquierda. */
	JIMAGEMENU_RIGHT,                      /**< Alineado a la derecha. */
	JIMAGEMENU_CENTER,                     /**< Centrado. */
	JIMAGEMENU_SAMELINE,                   /**< Todas las opciones en la misma línea. */
	JIMAGEMENU_FREE,                       /**< Opciones en cualquier posición */
} JImageMenuLayout;

/** Estructura de configuración del menú.
 */
struct JImageMenuConfig
{
	JImageMenuLayout layout;              /**< Distribución del menú. */
	bool trackMouse;                      /**< Indica si puede manejarse con ratón. */
	bool trackKeyboard;                   /**< Indica si puede manejarse con teclado. */
	bool trackJoystick;                   /**< Indica si puede manejarse con joystick/pad. */
	bool autoEnter;                       /**< Indica si desciende si no hay acción asociada. */
};

/** Opción de menú básica.
 */
class JImageMenuEntry
{
 protected:
	JImage *image;                        /**< Imagen asociada a la opción. */
	JImage *hiImage;                      /**< Imagen asociada a la opción resaltada. */

	void (*Action)(void *data);           /**< Acción asociada a la opción de menú. */
	void *data;                           /**< Datos asociados a la opción de menú. */

 public:
  /** Creates a menu entry.
   * @param  img Image for the item unhighlighted.
   * @param  hiImg Image for the item highlighted.
   * @param  pAct Pointer to function to call when the item is selected.
   * @param  actionData Pointer to additional data to be passed to the action function. In case you want to pass
   * a value (not a pointer to the value) to store it into the pointer itself, you can use the macros
   * JCAST_TSS_TO_VOIDPTR in JLib/Util/JTypes.h, where T in TSS is the signedness of the type (S or U)
   * and SS is the size in bits (8, 16 or 32). Read carefully the comments in that file before using them.
   */
	JImageMenuEntry(JImage *img, JImage *hiImg, void (*pAct)(void *) = 0, void *actionData = 0)
	: image(img), hiImage(hiImg), Action(pAct), data(actionData)
	{}

	/** Selecciona la opción de menú, llamando a la callback asociada, si existe.
	 * @return <b>true</b> en caso de haber acción asociada, <b>false</b> si no.
	 */
	bool Select();

	/** Devuelve la imagen asociada.
	 * @return La imagen asociada.
	 */
	JImage * Image() {return image;}

	/** Devuelve la imagen asociada al estado resaltado.
	 * @return La imagen asociada al estado resaltado.
	 */
	JImage * HiImage() {return hiImage;}

	/** Destruye el objeto.
	 */
	~JImageMenuEntry()
	{}
};

/** Encapsula un menú de texto en pantalla.
 */
class JImageMenu : public JDrawable
{
	JTree<JImageMenuEntry *> options;       /**< Árbol de opciones de menú. */
	JTree<JImageMenuEntry *>::Iterator *curOption; /**< Opción actual. */
	JImageMenuConfig config;              /**< Parámetros de configuración del menú. */
	
	/** Aplica la distribución de menú.
	 */
	void ApplyLayout(JTree<JImageMenuEntry *>::Iterator *it);

 public:
	/** Crea un menú vacío.
	 */
	JImageMenu();

	/** Inicializa el menú con la configuración dada. Esta función debe
	 * ser llamada después de haber rellenado el árbol del menú.
	 * @param  cfg Configuración del menú.
	 * @return <b>true</b> Si todo fue bien, <b>false</b> si no se encontró la
	 * fuente dada.
	 */
	bool Init(JImageMenuConfig &cfg);

	/** Devuelve el iterador de opción actual.
	 * @return Iterador de opción actual.
	 */
	JTree<JImageMenuEntry *>::Iterator * Menu() {return curOption;}

	/** Establece la posición del menú. La alineación se hace respecto de
	 * esta posición.
	 * @param  x Posición x.
	 * @param  y Posición y.
	 */
	virtual void Pos(float x, float y);

	/** Función de dibujo del menú.
	 */
	virtual void Draw();

	/** Procesa tecla arriba
	 */
	virtual void TrackKeyboard(SDL_keysym key);

	/** Procesa movimiento de mouse
	 */
	virtual void TrackMouse(s32 bt, s32 x, s32 y);

	/** Destruye el objeto y libera la memoria asociada.
	 */
	virtual ~JImageMenu()
	{
		for (options.Begin(); options.End(); options.Next())
			JDELETE(options.Cur());
		
		options.Clear();
	}
};

#endif // _JIMAGEMENU_INCLUDED
