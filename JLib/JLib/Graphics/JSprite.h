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

/** Sprite genérico.
 * @file    JSprite.h
 * @author  Juan Carlos Seijo Pérez
 * @date    18/10/2003
 * @version 0.0.1 - 18/10/2003 - Primera versión.
 * @note
 * <pre>
 * Combinaciones de loop, goBack y backwards (L,G,B)
 *
 * Tiempo  0   1   2   3   4  ...
 * Frame  123 123 123
 *
 * Loop(false), GoBack(false), Backwards(false)
 * Sprite o-- -o- --o --o --o ...
 *
 * Loop(false), GoBack(false), Backwards(true)
 *        --o -o- o-- o-- o-- ...
 *
 * Loop(true), GoBack(false), Backwards(false)
 *        o-- -o- --o o-- -o- ...
 *
 * Loop(true), GoBack(true), Backwards(false)
 *        o-- -o- --o -o- o-- -o- --o -o- ...
 *
 * Loop(true), GoBack(false), Backwards(true)
 *        --o -o- o-- --o -o- o-- --o ...
 *
 * Loop(true), GoBack(true), Backwards(true)
 *        --o -o- o-- -o- --o -o- o-- ...
 *
 * (No tiene efecto activar GoBack sin Loop.)
 * </pre>
 */

#ifndef _JSPRITE_INCLUDED
#define _JSPRITE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JUtil.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Util/JTimer.h>
#include <JLib/Util/JFile.h>
#include <JLib/Util/JLoadSave.h>

/** Sprite genérico.
 */
class JSprite : public JDrawable
{
protected:
  JTimer timer;                   /**< Cronómetro del sprite */
  u32 fps;                        /**< Cuadros por segundo */
  u32 numFrames;                  /**< Número de cuadros */
  JDrawable **frames;             /**< Cuadros */
  u32 lastFrame;                  /**< Último cuadro */
  u32 firstFrame;                 /**< Primer cuadro */
  s32 frameAdvance;               /**< Incremento de frame */
  u32 curFrame;                   /**< Índice del cuadro actual */
  bool loop;                      /**< ¿Debe iniciar desde donde empezó? */
  bool goBack;                    /**< ¿Debe volver hacia atrás cuando llegue al final? */
  bool backwards;                 /**< ¿Debe ir siempre hacia atrás? */
  bool paused;                    /**< ¿Debe pausar? */

public:
  /** Crea un sprite vacío. Se debe usar init para inicializarlo.
   */
  JSprite() : fps(1), numFrames(0), frames(0), lastFrame(0), firstFrame(0), 
  frameAdvance(0), curFrame(0), loop(false), goBack(false),	backwards(false), paused(false)
	{}
  
  /** Inicializa el sprite con los parámetros dados. No crea el array de frames,
   * puesto que lo debe hacer la clase hija.
   * @param  nFrames Número de frames del sprite.
   * @param  _FPS Velocidad en cuadros por segundo.
   * @param  mustLoop Indicador de repetición.
   * @param  mustGoBack Indicador de sentido invertido.
   * @param  mustGoBackwards Indicador de vuelta al comienzo.
   */
  void Init(u32 nFrames, u32 _FPS, bool mustLoop = false, bool mustGoBack = false, bool mustGoBackwards = false);

  /** Pasa al siguiente frame si han pasado 1/fps segundos.
	 * @return El nº de frame actual si ha cambiado, -1 si no cambió 
   * y -2 si está en pausa.
	 */
  s32 Update();

  /** Dibuja el sprite. Las x, y del sprite dan la posición global; las x, y de cada frame, 
   * el offset rpto. a la posición global
   */
  virtual void Draw() {frames[curFrame]->Draw();}

  /** Destruye el sprite, liberando la memoria asociada.
   */
  void Destroy();

  /** Destruye el sprite, liberando la memoria asociada.
   */
  virtual ~JSprite() {Destroy();}
  
  /** Devuelve el número de cuadros por segundo.
   * @return Número de cuadros por segundo.
   */
  const u32 FPS() {return fps;}

  /** Establece el número de cuadros por segundo.
   * @param  newFPS Número de cuadros por segundo.
   */
  void FPS(const u32 newFPS);

  /** Devuelve el número de cuadros totales en el sprite.
   @return Número de cuadros totales en el sprite.
   */
  const u32 NumFrames() {return numFrames;}

  /** Devuelve el índice del cuadro actual.
   * @return Índice del cuadro actual.
   */
  const u32 CurFrame() {return curFrame;}

  /** Establece el índice del cuadro actual.
   * @param  newFrame Índice del cuadro actual.
   */
  void CurFrame(const u32 newFrame) 
  {
    if (newFrame < numFrames) curFrame = newFrame;
  }

  /** Devuelve el frame en la posición indicada.
   * @param  i Índice del frame a recuperar.
   * @return Frame en la posición i.
   */
  JDrawable * Frame(u32 i) {return frames[i];}

  /** Determina si debe volver al comienzo.
   * @return <b>true</b> si al llegar al final el sprite vuelve al comienzo, 
   * <b>false</b> si no.
   */
  bool Loop() {return loop;}

  /** Establece si al llegar al final, el sprite vuelve al comienzo
   * @param  mustLoop Nuevo valor de la propiedad.
   */
  void Loop(bool mustLoop) {loop = mustLoop;}

  /** Determina si debe volver al comienzo.
   * @return <b>true</b> si debe volver hacia atrás cuando llegue al final, <b>false</b> si no.
   */
  bool GoBack() {return goBack;}

  /** Establece si debe volver al comienzo.
   * @param  mustGoBack <b>true</b> si debe volver hacia atrás cuando llegue al final, <b>false</b> si no.
   */
  void GoBack(bool mustGoBack) {goBack = mustGoBack;}

  /** Determina si debe ir hacia atrás.
   * @return <b>true</b> si debe ir siempre hacia atrás, <b>false</b> si no.
   */
  bool Backwards() {return backwards;}

  /** Establece si debe ir siempre hacia atrás.
   * @param  mustGoBackwards <b>true</b> si debe ir hacia atrás, <b>false</b> si no.
   */
  void Backwards(bool mustGoBackwards) {backwards = mustGoBackwards;}

  /** Pausa o reanuda la animación.
   * @param pause <b>true</b> para pausarla, <b>false</b> para reanudarla.
   */
  void Paused(bool pause) {paused = pause;}

  /** Determina si están en pausa la animación.
   * @return <b>true</b> si está en pausa, <b>false</b> si no.
   */
  bool Paused() {return paused;}

  /** Va al frame anterior.
   * @return <b>true</b> si había frame anterior, <b>false</b> si no.
   */
  bool PrevFrame();

  /** Va al frame siguiente
   * @return <b>true</b> si había frame posterior, <b>false</b> si no.
   */
  bool NextFrame();

  /** Va al primer frame.
   */
  void FirstFrame() {curFrame = firstFrame;}

  /** Va al último frame
   */
  void LastFrame() {curFrame = lastFrame;}
};

#endif  // _JSPRITE_INCLUDED
