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

/** Eje de coordenadas (base + origen)
 * @file    JCoordAxes.h
 * @author: Juan Carlos Seijo Pérez
 * @date    30/04/2003
 * @version 0.0.1 - 30/04/2003 - Primera versión.
 */

#ifndef _JCOORDAXES_INCLUDED
#define _JCOORDAXES_INCLUDED

#include <JLib/Util/JTypes.h>

/** Encapsula unos ejes coordenados consistentes en una base definida
 * por 3 vectores y un punto en el espacio, el origen.
 */
class JCoordAxes : public JBasis
{
public:
  JPoint O;     /**< Origen de coordenadas relativo al origen del padre. */

public:
  /** Crea unos ejes coordenados.
   */
  JCoordAxes(){}
  
  /** Crea unos ejes coordenados a partir de los vectores base y el origen.
   * @param  o Origen de coordenadas.
   * @param  v0 Vector base 0
   * @param  v1 Vector base 1
   * @param  v2 Vector base 2
   */
  JCoordAxes(const JPoint& o,
             const JVector& v0,
             const JVector& v1,
             const JVector& v2) : JBasis (v0, v1, v2), O(o){};
  
  /** Crea unos ejes coordenados a partir de la base y el origen.
   * @param  o Origen de coordenadas.
   * @param  b Base de vectores.
   */
  JCoordAxes(const JPoint& o,
             const JBasis& b) : JBasis (b), O(o) {};
  
  /** Devuelve el origen de coordenadas.
   * @return Origen de coordenadas.
   */ 
  const JPoint& Position() const { return O; }
  
  /** Establece el origen de coordenadas.
   * @param  p Nuevo origen de coordenadas.
   */
  void Position(const JPoint& p) { O = p; }
  
  /** Transforma las coordenadas de un punto a coordenadas locales.
   * Dado un punto en el sistema de referencia al que se refiere la
   * base de estos ejes coordenados, devuelve el punto correspondiente
   * respecto de esta base.
   * @param  p Punto a transformar.
   * @return Punto referido a la base de estos ejes coordenados.
   */
  const JPoint transformPointToLocal(const JPoint& p) const
  {
    // Traslada al origen local y luego proyecta en la base local
    return TransformVectorToLocal(p - O);
  }
  
  /** Transforma las coordenadas de un punto a coordenadas del padre.
   * Dado un punto en este sistema de referencia, devuelve el punto 
   * correspondiente respecto del sistema de referencia en el que están
   * expresados los vectores de esta base.
   * @param  p Punto a transformar.
   * @return Punto referido a la base padre de estos ejes coordenados.
   */
  const JPoint TransformPointToParent(const JPoint& p) const
  {
    // Transforma las coordenadas y traslada este origen
    return TransformVectorToParent(p) + O;
  }
  
  /** Traslada el origen por el vector dado.
   * @param  v Vector de desplazamiento.
   */
  void Translate(const JVector& v)
  {
    O += v;
  }
};

#endif  // _JCOORDAXES_INCLUDED
