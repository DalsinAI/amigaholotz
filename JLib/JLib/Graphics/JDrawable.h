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

/** Clase base de todos los objetos dibujables.
 * @file    JDrawable.h.
 * @author  Juan Carlos Seijo Pérez.
 * @date    15/11/2003.
 * @version 0.0.1 - 15/11/2003 - Primera versión.
 * @version 0.0.2 - 25/09/2004 - Modificación del método 2D de Pos() para aceptar floats (quita libertad sino).
 */

#ifndef _JDRAWABLE_INCLUDED
#define _JDRAWABLE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <stdio.h>
#include <JLib/Util/JObject.h>
#include <JLib/Util/JLoadSave.h>
#include <JLib/Math/JVector.h>

/** Clase base de todos los objetos dibujables.
 */
class JDrawable : public JObject
{
protected:
  JVector pos;                /**< Posición del objeto. */

public:
  /** Libera memoria.
   */
  virtual ~JDrawable() {}

  /** Dibuja el objeto. Debe ser implementada en la clase hija.
   */
  virtual void Draw() {return;}

  /** Actualiza el objeto.
   */
  virtual s32 Update() {return 0;}

  /** Recupera la posición de este objeto.
   * @return Posición del objeto.
   */
  virtual const JVector & Pos() const {return pos;}

  /** Establece la posición de este objeto.
   * @param  newPos Nueva posición del objeto.
   */
  virtual void Pos(const JVector &newPos) {pos = newPos;}

  /** Establece la posición de este objeto.
   * @param  x Nueva coordenada x.
   * @param  y Nueva coordenada y.
   * @param  z Nueva coordenada z.
   */
  virtual void Pos(float x, float y, float z) {pos.x = x; pos.y = y; pos.z = z;}

  /* Establece la posición de este objeto. Útil para 2D.
   * @param  x Nueva coordenada x.
   * @param  y Nueva coordenada y.
   */
  virtual void Pos(float x, float y) {pos.x = x; pos.y = y;}

  /** Establece la posición x de este objeto.
   * @param  x Nueva coordenada x.
   */
  virtual void X(float x) {pos.x = x;}

  /** Devuelve la posición x de este objeto.
   * @return Coordenada x del objeto.
   */
  virtual float X() const {return pos.x;}

  /** Establece la posición y de este objeto.
   * @param  y Nueva coordenada y.
   */
  virtual void Y(float y) {pos.y = y;}

  /** Devuelve la posición y de este objeto.
   * @return Coordenada y del objeto.
   */
  virtual float Y() const {return pos.y;}

  /** Establece la posición z de este objeto.
   * @param  z Nueva coordenada z.
   */
  virtual void Z(float z) {pos.z = z;}

  /** Devuelve la posición z de este objeto.
   * @return Coordenada z del objeto.
   */
  virtual float Z() const {return pos.z;}
};

#endif  // _JDRAWABLE_INCLUDED
