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

/** Vector en 3D.
 * @file    JVector.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2003
 * @version 0.0.1 - 30/04/2003 - Primera versión.
 */

#ifndef _JVECTOR_INCLUDED
#define _JVECTOR_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>
#include <math.h>

typedef float JScalar;              /**< Escalar decimal */

/** Encapsula un vector en el espacio de coordenadas (x, y, z) y
 * las operaciones más comunes entre vectores.
 */
class JVector
{
public:
  JScalar x;                    // Coordenada x
  JScalar y;                    // Coordenada y
  JScalar z;                    // Coordenada z

public:
  /** Crea un vector.
   */
  JVector() : x(0), y(0), z(0) {};
  
  /** Crea un vector con los valores dados.
   * @param  _x Valor de la componente x
   * @param  _y Valor de la componente y
   * @param  _z Valor de la componente z
   */
  JVector(const JScalar& _x, const JScalar& _y, const JScalar& _z)
    : x(_x), y(_y), z(_z) {};
  
  /** Crea un vector a partir del array de valores.
   * El array debe contener al menos 3 elementos que se asignarán,
   * respectivamente, a las componentes 'x', 'y' y 'z'.
   */
  JVector(const JScalar *arr)
    : x(arr[0]), y(arr[1]), z(arr[2]) {};
  
  /** Acceso indexado.
   * @param  i Índice de la componente a recuperar (0 para x, 1 para y,
   * 2 para z).
   * @return Coordenada pedida.
   */
  JScalar& operator [] (const s32 i)
  {
    return *((&x) + i);
  }
  
  /** Determina si el vector dado es igual a este.
   * Dos vectores son iguales si todas sus componentes son iguales.
   * El resultado podría no ser el esperado debido a errores de precisión.
   * En estos casos se debe usar el método NearlyEquals() para tener en
   * cuenta esta imprecisión.
   * @param  v Vector a comparar con este.
   * @return <b>true</b> si son iguales, <b>false</b> si no.
   */
  const bool operator == (const JVector& v) const
  {
    return (v.x==x && v.y==y && v.z==z);
  }
  
  /** Determina si el vector dado es diferente a este.
   * El resultado podría no ser el esperado debido a errores de precisión.
   * En estos casos se debe usar el método NearlyEquals() para tener en
   * cuenta esta imprecisión.
   * @param  v Vector a comparar con este.
   * @return <b>true</b> si son diferentes, <b>false</b> si no.
   */
  const bool operator != (const JVector& v) const
  {
    return !(v == *this);
  }
  
  /** Devuelve un vector que es la negación de este, es decir,
   * con todas las coordenadas cambiadas de signo.
   */
  const JVector operator - () const
  {
    return JVector(-x, -y, -z);
  }
  
  /** Asigna otro vector a este.
   * @param  v Vector que se quiere asignar a este.
   * @return Este objeto.
   */
  const JVector& operator = (const JVector& v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
    
    return *this;
  }
  
  /** Suma un vector dado a este vector. A diferencia del operador
   * '+', este operador no crea un objeto nuevo.
   * @param  v Vector a sumar a este.
   * @return Este objeto.
   */
  const JVector& operator += (const JVector& v) 
  {
    x += v.x;
    y += v.y;
    z += v.z;
  
    return *this;
  } 
  
  /** Resta un vector dado a este vector. A diferencia del operador
   * '-', este operador no crea un objeto nuevo.
   * @param  v Vector a restar a este.
   * @return Este objeto.
   */
  const JVector& operator -= (const JVector& v) 
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    
    return *this;
  } 
  
  /** Multiplica cada componente por el escalar dado.
   * @param  s Escalar por el que multiplicar.
   * @return Este objeto.
   */
  const JVector& operator *= (const JScalar& s)
  {
    x *= s;
    y *= s;
    z *= s;
    
    return *this;
  }
  
  /** Divide cada componente por un escalar.
   * A diferencia del operador '/' este no crea un nuevo objeto.
   * @param  s Escalar por el que dividir.
   * @return Este objeto.
   */
  const JVector& operator /= (const JScalar& s)
  {
    const JScalar r = 1/s;
    x *= r;
    y *= r;
    z *= r;
  
    return *this;
  }
  
  /** Devuelve la suma de este vector y el vector dado.
   * Este método no modifica el objeto vector actual, devuelve
   * un nuevo objeto.
   * @param  v Vector a sumar a este.
   * @return Vector resultado de la suma de este y el vector dado.
   */
  const JVector operator + (const JVector& v) const
  {
    return JVector(x + v.x, y + v.y, z + v.z);
  }
  
  /** Devuelve la resta de este vector y el vector dado.
   * Este método no modifica el objeto vector actual, devuelve
   * un nuevo objeto.
   * @param  v Vector a restar a este.
   * @return Vector resultado de la resta de este y el vector dado.
   */
  const JVector operator - (const JVector& v) const
  {
    return JVector(x - v.x, y - v.y, z - v.z);
  }
  
  /** Post-multiplicación por escalar.
   * Es el resultado de hacer v * s, en ese orden.
   * @param  s Escalar por el que multiplicar.
   * @return Nuevo objeto vector resultado de la multiplicación.
   */
  const JVector operator * (const JScalar& s) const
  {
    return JVector( x*s, y*s, z*s);
  }
  
  /** Pre-multiplicación por escalar.
   * Es el resultado de hacer s * v, en ese orden.
   * @param  s Escalar por el que multiplicar.
   * @param  v Vector.
   * @return Nuevo objeto vector resultado de la multiplicación.
   */
  friend inline const JVector operator * (const JScalar& s, const JVector& v)
  {
    return v*s;
  }
  
  /** Divide todas las componentes.
   * Este método no modifica al objeto vector actual.
   * @param  s Escalar por el que dividir.
   * @return Nuevo objeto vector con el resultado.
   */
  const JVector operator / (JScalar s) const
  {
    s = 1/s;
    return JVector(s*x, s*y, s*z);
  }
  
  /** Realiza el producto vectorial de este vector por el vector dado,
   * en ese orden.
   * @param  v Vector con el que realizar el producto vectorial.
   * @return Nuevo objeto vector con el resultado.
   */
  const JVector Cross(const JVector& v) const
  {
    return JVector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
  }
  
  /** Realiza el producto escalar por el vector dado.
   * El producto escalar de dos vectores v1 y v2 se define como
   * v1 · v2 = (x1*x2) + (y1*y2) + (z1*z2)
   * @param  v Vector con el que calcular el producto escalar.
   * @return Valor del producto escalar.
   */
  const JScalar Dot(const JVector& v) const
  {
    return x*v.x + y*v.y + z*v.z;
  }
  
  /** Determina la longitud (norma) del vector.
   * @return Longitud del vector.
   */
  const JScalar Length() const
  {
    return (JScalar)sqrt((double)this->Dot(*this));
  }
  
  /** Devuelve un vector unitario en el sentido de este.
   * Esta operación no modifica a este vector actual.
   * @return Nuevo vector unitario.
   */
  const JVector Unit() const
  {
    return (*this) / Length();
  }
  
  /** Convierte este vector en vector unitario (longitud 1).
   */
  void Normalize()
  {
    (*this) /= Length();
  }
  
  /** Determina si dos vectores son iguales con un margen de error 'e'.
   * Esta función se usa en los casos en que a precisión del redondeo decimal
   * se deba tener en cuenta.
   * @param  v Vector a comparar con este.
   * @param  e Margen de error positivo/negativo en cada componente.
   * @return <b>true</b> si son iguales dentro del margen dado <b>false</b> si no.
   */
  const bool NearlyEquals(const JVector& v, const JScalar e) const
  {
    return fabs(x - v.x) < e && fabs(y - v.y) < e && fabs(z - v.z) < e;
  }
};

typedef JVector JPoint;           /**< Posición 3D */

#endif  // _JVECTOR_INCLUDED
