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

/** Menu en pantalla compuesto de cadenas de texto.
 * @file    JTextMenu.h
 * @author  Juan Carlos Seijo Pérez
 * @date    28/03/2004
 * @version 0.0.1 - 28/03/2004 - Primera versión.
 */

#ifndef _JTEXTMENU_INCLUDED
#define _JTEXTMENU_INCLUDED

#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Graphics/JFont.h>
#include <JLib/Util/JApp.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JTree.h>
#include <JLib/Util/JTimer.h>

/** Modo de renderizado de la fuente del menú.
 */
typedef enum 
{
	JTEXTMENU_SOLID = 1,                  /**< Sólido con colorkey. */
	JTEXTMENU_SHADED,                     /**< Antialiasing con fondo sólido. */
	JTEXTMENU_BLENDED,                    /**< Antialiasing con fondo transparente. */
} JTextMenuRenderStyle;

/** Distribución del menú.
 */
typedef enum 
{
	JTEXTMENU_LEFT = 1,                   /**< Alineado a la izquierda. */
	JTEXTMENU_RIGHT,                      /**< Alineado a la derecha. */
	JTEXTMENU_CENTER,                     /**< Centrado. */
	JTEXTMENU_SAMELINE,                   /**< Todas las opciones en la misma línea. */
	JTEXTMENU_UP,                         /**< Alineado arriba. */
	JTEXTMENU_DOWN,                       /**< Alineado abajo. */
} JTextMenuLayout;

/** Estructura de configuración del menú.
 */
struct JTextMenuConfig
{
	JFont * font;                         /**< Fuente a usar. */
	SDL_Color color;                      /**< Color de fuente. */
	SDL_Color backColor;                  /**< Color de fondo (JTEXTMENU_SHADED). */
	SDL_Color hiColor;                    /**< Color resaltado. */
	SDL_Color hiBackColor;                /**< Color de fondo resaltado (JTEXTMENU_SHADED). */
	s32 lineDistance;                     /**< Separación entre líneas (-1 para la predeterminada de la fuente). */
	JTextMenuLayout layout;               /**< Distribución del menú en horizontal. */
	JTextMenuLayout layoutV;              /**< Distribución del menú en vertical. */
	JTextMenuRenderStyle renderMode;      /**< Modo de renderizado. */
	bool trackMouse;                      /**< Indica si puede manejarse con ratón. */
	bool trackKeyboard;                   /**< Indica si puede manejarse con teclado. */
	bool trackJoystick;                   /**< Indica si puede manejarse con joystick/pad. */
	bool autoEnter;                       /**< Indica si desciende si no hay acción asociada. */
};

/** Opción de menú básica.
 */
class JTextMenuEntry
{
 protected:
 public:
	JImage *image;                        /**< Imagen asociada a la opción. */
	JImage *hiImage;                      /**< Imagen asociada a la opción resaltada. */

	JString text;                         /**< Texto de la opción de menú. */
	void (*Action)(void *data);           /**< Acción asociada a la opción de menú. */
	void *data;                           /**< Datos asociados a la opción de menú. */

 public:
	JTextMenuEntry(const JString &str, void (*pAct)(void *) = 0, void *actionData = 0)
	: image(0), hiImage(0), text(str), Action(pAct), data(actionData)
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

	/** Renderiza la opción de menú con la configuración dada.
	 * @param  cfg Configuración de menú.
	 * @return <b>true</b> Si todo fue bien, <b>false</b> si no se pudo renderizar.
	 */
	bool Init(JTextMenuConfig &cfg);

	/** Devuelve el texto de esta opción.
	 */
	const JString& Text() {return text;}

	/** Destruye el objeto y libera las imagenes asociadas.
	 */
	void Destroy()
	{
		JDELETE(image);
		JDELETE(hiImage);
	}

	/** Destruye el objeto y libera las imagenes asociadas.
	 */
	virtual ~JTextMenuEntry()
	{
		Destroy();
	}
};

/** Encapsula un menú de texto en pantalla.
 */
class JTextMenu : public JDrawable
{
	JTree<JTextMenuEntry *> options;      /**< Árbol de opciones de menú. */
	JTree<JTextMenuEntry *>::Iterator *curOption; /**< Opción actual. */
	JTextMenuConfig config;               /**< Parámetros de configuración del menú. */
	s32 maxW;                             /**< Anchura máxima. */
	s32 maxH;                             /**< Altura máxima. */
	
	/** Método recursivo de renderización de las opciones del menú.
	 * @param it Iterador de la opción actual.
	 * @return <b>true</b> Si todo fue bien, <b>false</b> si no hay memoria.
	 */
	bool RenderBranch(JTree<JTextMenuEntry *>::Iterator *it);

	/** Aplica la distribución de menú.
	 */
	void ApplyLayout(JTree<JTextMenuEntry *>::Iterator *it);

 public:
	/** Crea un menú vacío. Init debe ser llamada antes de utilizar el objeto.
	 */
	JTextMenu();

	/** Inicializa el menú con la configuración dada. Esta función debe
	 * ser llamada después de haber rellenado el árbol del menú.
	 * @param  cfg Configuración del menú.
	 * @return <b>true</b> Si todo fue bien, <b>false</b> si no se encontró la
	 * fuente dada.
	 */
	bool Init(JTextMenuConfig &cfg);

	/** Devuelve el iterador de opción actual.
	 * @return Iterador de opción actual.
	 */
	JTree<JTextMenuEntry *>::Iterator * Menu() {return curOption;}

	/** Función de dibujo del menú.
	 */
	virtual void Draw();

	/** Procesa tecla arriba
	 */
	virtual void TrackKeyboard(SDL_keysym key);

	/** Procesa movimiento de mouse
	 */
	virtual void TrackMouse(s32 bt, s32 x, s32 y);

	/** Devuelve la anchura máxima del menú.
	 * @return Anchura máxima del menú.
	 */
	s32 MaxW() {return maxW;}

	/** Devuelve la altura máxima del menú.
	 * @return Altura máxima del menú.
	 */
	s32 MaxH() {return maxH;}

	/** Destruye el objeto y libera la memoria asociada.
	 */
	virtual ~JTextMenu()
	{
		for (options.Begin(); !options.End(); options.Next())
		{
			JDELETE(options.Cur());
		}
		
		options.Clear();
		JDELETE(curOption);
	}
};

#endif // _JTEXTMENU_INCLUDED
