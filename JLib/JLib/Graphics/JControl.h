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

/** Clase base de un control de interfaz de usuario.
 * @file    JControl.h.
 * @author  Juan Carlos Seijo Pérez.
 * @date    27/10/2003.
 * @version 0.0.1 - 27/10/2003 - Primera versión.
 */

#ifndef _JCONTROL_INCLUDED
#define _JCONTROL_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <string.h>

class JUI;

/** Tipo de control.
 */
typedef enum 
{
	JCONTROLTYPE_IMAGE = 0,                              /**< Imágenes estáticas. */
	JCONTROLTYPE_IMAGESPRITE,                            /**< Sprite de imágenes. */
	JCONTROLTYPE_MESH,                                   /**< Geometría. */
	JCONTROLTYPE_MESHSPRITE,                             /**< Sprite de geometría. */
} JControlType;

/** Clase base de un control de interfaz de usuario.
 */
class JControl : public JDrawable
{
	friend class JUI;

private:
  static u32 controlCount;                             /**< Lleva cuenta del nº de controles. */

protected:
	JControlType type;                                   /**< Tipo de control. */
	u32 id;                                              /**< Identificador de control. */
	u32 parentId;                                        /**< Identificador del control padre. */
	JControl * parent;                                   /**< Control padre. */
  u32 focusIndex;                                      /**< Índice de enfoque. */
	u32 state;                                           /**< Estado del control. */
  void (*focus)(JControl *c, void *data);              /**< Función de enfoque. */
  void (*unfocus)(JControl *c, void *data);            /**< Función de desenfoque. */
  void (*enable)(JControl *c, void *data);             /**< Función de habilitación. */
  void (*disable)(JControl *c, void *data);            /**< Función de deshabilitación. */
  void (*appear)(JControl *c, void *data);             /**< Función de aparición. */
  void (*disappear)(JControl *c, void *data);          /**< Función de desaparición. */
  void (*select)(JControl *c, void *data);             /**< Función de selección. */
  void (*unselect)(JControl *c, void *data);           /**< Función de deselección. */
  void *focusData;                                     /**< Datos adicionales de enfoque. */
  void *unfocusData;                                   /**< Datos adicionales de desenfoque. */
  void *enableData;                                    /**< Datos adicionales de habilitación. */
  void *disableData;                                   /**< Datos adicionales de deshabilitar. */
  void *appearData;                                    /**< Datos adicionales de aparición. */
  void *disappearData;                                 /**< Datos adicionales de desaparición. */
  void *selectData;                                    /**< Datos adicionales de selección. */
  void *unselectData;                                  /**< Datos adicionales de deselección. */
	
  /** Acción a realizar cuando recibe el enfoque. */
  void OnFocus() {if (focus) focus(this, focusData);};
	
  /** Acción a realizar cuando pierde el enfoque. */
  void OnUnfocus() {if (unfocus) unfocus(this, unfocusData);};
	
  /** Acción a realizar cuando es activado. */
  void OnEnable() {if (enable) enable(this, enableData);};
	
  /** Acción a realizar cuando es desactivado. */
  void OnDisable() {if (disable) disable(this, disableData);};
	
  /** Acción a realizar cuando aparece. */
  void OnAppear() {if (appear) appear(this, appearData);};
  
  /** Acción a realizar cuando desaparece. */
  void OnDisappear() {if (disappear) disappear(this, disappearData);};
	
  /** Acción a realizar cuando se selecciona. */
  void OnSelect() {if (select) select(this, selectData);};
  
  /** Acción a realizar cuando se deselecciona. */
  void OnUnselect() {if (unselect) unselect(this, unselectData);};
	
 public:
  static const u32 VISIBLE;    /**< Flag de visibilidad. */
  static const u32 FOCUSED;    /**< Flag de enfoque. */
  static const u32 ENABLED;    /**< Flag de habilitación. */
  static const u32 SELECTED;   /**< Flag de selección. */
	
  /** Crea un control vacío.
   */
  JControl(JControl *_parent = 0, u32 _state = VISIBLE & ENABLED);
  
	/** Devuelve el identificador de este control.
	 * @return Identificador del control.
	 */
	u32 Id() {return id;}
	
	/** Devuelve el control padre.
	 * @return Control padre de este, 0 si no tiene.
	 */
	JControl * Parent() {return parent;}
	
  /** Función de dibujo. */
  virtual void Draw();
	
  /** Función de actualización cuando está visible.
   * @return Dependiente de la implementación.
   */
  virtual s32 Update();
	
  /** Función de dibujo cuando está únicamente visible. */
  virtual void DrawVisible() = 0;
	
  /** Función de dibujo cuando está enfocado. */
  virtual void DrawFocus() = 0;
	
  /** Función de dibujo cuando está deshabilitado. */
  virtual void DrawDisabled() = 0;

  /** Función de dibujo cuando está seleccionado. */
  virtual void DrawSelected() = 0;

  /** Función de actualización cuando está únicamente visible. 
   * @return Máscara de estado del control.
	 */
  virtual s32 UpdateVisible() = 0;

  /** Función de actualización cuando está enfocado. 
   * @return Dependiente de la implementación.
   */
  virtual s32 UpdateFocus() = 0;

  /** Función de actualización cuando está deshabilitado. 
   * @return Dependiente de la implementación.
   */
  virtual s32 UpdateDisabled() = 0;

  /** Función de actualización cuando está seleccionado. 
   * @return Dependiente de la implementación.
   */
  virtual s32 UpdateSelected() = 0;

  /** Muestra el control.
   */
  virtual void Appear();

  /** Oculta el control.
   */
  virtual void Disappear();
  
  /** Habilita el control.
   */
  virtual void Enable();

  /** Deshabilita el control.
   */
  virtual void Disable();

  /** Enfoca el control.
   */
  virtual void Focus();

  /** Desenfoca el control.
   */
  virtual void Unfocus();

  /** Selecciona el control.
   */
  virtual void Select();

  /** Deselecciona el control.
   */
  virtual void Unselect();

  /** Establece la función para cuando recibe el enfoque.
   * @param  func Función callback a llamara cuando se produce el evento.
   * @param  data Datos adicionales.
   */
  void SetOnFocus(void (*func)(JControl*, void *), void *data = 0)     {focus = func; focusData = data;};

  /** Establece la función para cuando pierde el enfoque.
   * @param  func Función callback a llamara cuando se produce el evento.
   * @param  data Datos adicionales.
   */
  void SetOnUnfocus(void (*func)(JControl*, void *), void *data = 0)   {unfocus = func; unfocusData = data;};
  
  /** Establece la función para cuando es activado.
   * @param  func Función callback a llamara cuando se produce el evento.
   * @param  data Datos adicionales.
   */
  void SetOnEnable(void (*func)(JControl*, void *), void *data = 0)    {enable = func; enableData = data;};

  /** Establece la función para cuando es desactivado.
   * @param  func Función callback a llamara cuando se produce el evento.
   * @param  data Datos adicionales.
   */
  void SetOnDisable(void (*func)(JControl*, void *), void *data = 0)   {disable = func; disableData = data;};

  /** Establece la función para cuando aparece.
   * @param  func Función callback a llamara cuando se produce el evento.
   * @param  data Datos adicionales.
   */
  void SetOnAppear(void (*func)(JControl*, void *), void *data = 0)    {appear = func; appearData = data;};
  
  /** Establece la función para cuando desaparece.
   * @param  func Función callback a llamara cuando se produce el evento.
   * @param  data Datos adicionales.
   */
  void SetOnDisappear(void (*func)(JControl*, void *), void *data = 0) {disappear = func; disappearData = data;};

  /** Establece la función para cuando se selecciona.
   * @param  func Función callback a llamara cuando se produce el evento.
   * @param  data Datos adicionales.
   */
  void SetOnSelect(void (*func)(JControl*, void *), void *data = 0)    {select = func; selectData = data;};
  
  /** Establece la función para cuando se deselecciona.
   * @param  func Función callback a llamara cuando se produce el evento.
   * @param  data Datos adicionales.
   */
  void SetOnUnselect(void (*func)(JControl*, void *), void *data = 0)  {unselect = func; unselectData = data;};

	/** Determina si el control está visible.
   * @return <b>true</b> si lo está, <b>false</b> si no.
   */
	bool Visible() {return 0 != (state & VISIBLE);}

	/** Determina si el control está enfocado.
   * @return <b>true</b> si lo está, <b>false</b> si no.
   */
	bool Focused() {return 0 != (state & FOCUSED);}

	/** Determina si el control está seleccionado.
   * @return <b>true</b> si lo está, <b>false</b> si no.
   */
	bool Selected() {return 0 != (state & SELECTED);}

	/** Determina si el control está activado.
   * @return <b>true</b> si lo está, <b>false</b> si no.
   */
	bool Enabled() {return 0 != (state & ENABLED);}

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

  /** Libera la memoria asociada.
   */
  virtual ~JControl();
};

#endif  // _JCONTROL_INCLUDED
