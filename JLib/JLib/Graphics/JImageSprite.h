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

/** Sprite de imágenes.
 * @file    JImageSprite.h
 * @author  Juan Carlos Seijo Pérez.
 * @date    18/10/2003.
 * @version 0.0.1 - 18/10/2003 - Primera versión.
 * @version 0.0.2 - 01/06/2004 - Adición de método de copia.
 */

#ifndef _JIMAGESPRITE_INCLUDED
#define _JIMAGESPRITE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JLoadSave.h>
#include <JLib/Graphics/JSprite.h>

/** Sprite de imágenes.
 */
class JImageSprite : public JSprite
{
protected:
  s32 wMax;                     /**< Anchura del frame mayor. */
  s32 hMax;                     /**< Altura del frame mayor. */

public:
  /** Crea el objeto.
   */
  JImageSprite() : JSprite(), wMax(0), hMax(0) {}

  /** Crea un sprite de imágenes a partir de otro dado.
   */
  JImageSprite(JImageSprite &spr);

  /** Carga el sprite desde un fichero.
   * Si _numFrames == 0 toma las que tenga internamente (0 por defecto).
   * Si frameW != 0 y frameH == 0, considera que las frames están en horizontal.
   * Si frameW == 0 y frameH != 0, considera que las frames están en vertical.
   * Si frameW != 0 y frameH != 0, intenta encajarlas en H o V, si no intenta encajar los cuadrados.
   * Si frameW == 0 y frameH == 0 o numFrames == 0 o hay una incoherencia o un error, devuelve false.
   * @param  fileName Nombre del fichero.
   * @param  frameW Anchura de cuadro.
   * @param  frameH Altura de cuadro.
   * @param  _numFrames Número de frames.
   * @param  colorKey Color transparente.
   * @return Si todo va bien, devuelve <b>true</b>, si no, false.
   */
  bool Load(const JString &fileName, u32 frameW,  u32 frameH = 0,  u32 _numFrames = 0, u32 colorKey = 0x00000000);

  /** Carga el sprite desde varios ficheros de imágenes.
   * @param  imageFiles Array con los nombres de los ficheros.
   * @param _numFrames Número de ficheros (frames) del sprite.
   * @param colorKey Color transparente.
   * Si todo va bien, devuelve true.
   */
  bool Load(JString *imageFiles, u32 _numFrames, u32 colorKey = 0x00000000);

  /** Dibuja el sprite en pantalla.
   */
  virtual void Draw();

  /** Alinea arriba los frames del sprite.
   */
  void AlignUp();

  /** Alinear abajo los frames del sprite.
   */
  void AlignDown();

  /** Alinea a la izquierda los frames del sprite.
   */
  void AlignLeft();

  /** Alinea a la derecha los frames del sprite.
   */
  void AlignRight();

  /** Ajusta al máximo el borde del sprite.
   */
  void AdjustSize();

  /** Devuelve la anchura máxima.
   * @return Anchura máxima.
   */
  s32 MaxW() const {return wMax;}

  /** Devuelve la altura máxima.
   * @return Altura máxima.
   */
  s32 MaxH() const {return hMax;}

	/** Devuelve el frame actual en forma de Image.
	 * @return Frame actual en forma de image.
	 */
	JImage * CurImage() {return (JImage *)frames[curFrame];}

  /** Carga el sprite.
   * @param  f Fichero posicionado para cargar el sprite.
   * @return 0 si todo fue bien, 1 en caso de error de E/S, 2 en caso
   * de error en los datos.
   */
  u32 Load(JRW &f);

  /** Salva el sprite.
   * @param  f Fichero posicionado para salvar el sprite.
   * @return 0 si todo fue bien, 1 en caso de error de E/S, 2 en caso
   * de error en los datos.
   */
  u32 Save(JRW &f);

  /** Libera la memoria asociada.
   */
  virtual ~JImageSprite(){}

	/** Crea una referencia del sprite dado. Las imagenes se referencian, de forma que no
	 * se crea una copia de las mismas, ocupan la misma memoria. Es equivalente al
	 * constructor copia.
	 */
	void Ref(JImageSprite &);
};

#endif  // _JIMAGESPRITE_INCLUDED
