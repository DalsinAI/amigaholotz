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

/** Definición de vector en 2D
 * @file    J2DVector.h
 * @author  Juan Carlos Seijo Pérez
 * @date    05/01/2004
 * @version 0.0.1 - 05/01/2004 - Primera versión.
 */

#ifndef _J2DVECTOR_INCLUDED
#define _J2DVECTOR_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JLoadSave.h>
#include <math.h>

typedef float J2DScalar;              /**< Escalar en 2D. */

/** Encapsula un vector en el plano y las operaciones más comunes.
 */
class J2DVector : public JLoadSave
{
public:
  J2DScalar x;                        /**< Coordenada x. */
  J2DScalar y;                        /**< Coordenada y. */

public:
  /** Crea un vector en el plano de coordenadas (0,0).
   */
  J2DVector() : x(0), y(0) {};
  
  /** Crea un vector en el plano con las componentes dadas.
   * @param  _x Componente x del vector.
   * @param  _y Componente y del vector.
   */
  J2DVector(const J2DScalar& _x, const J2DScalar& _y)
    : x(_x), y(_y) {};
  
  /** Crea un vector en el plano apartir del array dado.
   * @param  arr Array con los dos valores x, y, en ese orden.
   */
  J2DVector(const J2DScalar *arr)
    : x(arr[0]), y(arr[1]) {};
  
  /** Devuelve la coordenada x.
   * @return  Coordenada x.
   */
  const J2DScalar& X()
  {
    return x;
  }
  
  /** Devuelve la coordenada y.
   * @return  Coordenada y.
   */
  const J2DScalar& Y()
  {
    return y;
  }
  
  /** Establece la coordenada x.
   * @param  _x Coordenada x.
   */
  void X(const J2DScalar& _x)
  {
    x = _x;
  }
  
  /** Establece la coordenada y.
   * @param  _y Coordenada y.
   */
  void Y(const J2DScalar& _y)
  {
    y = _y;
  }
  
  /** Comparación de vectores.
   * @param  v vector a comparar.
   * @return <b>true</b> si son iguales, <b>false</b> si no.
   */
  const bool operator == (const J2DVector& v) const
  {
    return (v.x==x && v.y==y);
  }
  
  /** Comparación de vectores.
   * @param  v vector a comparar.
   * @return <b>true</b> si son diferentes, <b>false</b> si no.
   */
  const bool operator != (const J2DVector& v) const
  {
    return (v.x!=x || v.y!=y);
  }
  
  /** Devuelve un vector que es la negación de este.
   * @return Vector negado.
   */
  const J2DVector operator - () const
  {
    return J2DVector(-x, -y);
  }
  
  /** Asignación.
   * @param  v Vector a asignar a este.
   * @return Este vector.
   */
  const J2DVector& operator = (const J2DVector& v)
  {
    x = v.x;
    y = v.y;
    
    return *this;
  }
  
  /** Asignación.
   * @param  xx Nuevo valor de x.
   * @param  yy Nuevo valor de y.
   */
  void Pos(J2DScalar xx, J2DScalar yy)
  {
    x = xx;
    y = yy;
  }
  
  /** Incremento.
   * @param  v Vector por el que se incrementará el valor de este.
   * @return Este vector.
   */
  const J2DVector& operator += (const J2DVector& v) 
  {
    x += v.x;
    y += v.y;
  
    return *this;
  } 
  
  /** Decremento.
   * @param  v Vector por el que se decrementará el valor de este.
   * @return Este vector.
   */
  const J2DVector& operator -= (const J2DVector& v) 
  {
    x -= v.x;
    y -= v.y;
    
    return *this;
  } 
  
  /** Multiplicación por escalar. Multiplica cada componente por el escalar
   * dado.
   * @param s Escalar por el que multiplicar.
   * @return Este vector.
   */
  const J2DVector& operator *= (const J2DScalar& s)
  {
    x *= s;
    y *= s;
    
    return *this;
  }
  
  /** División por escalar.Divide cada componente por el escalar dado.
   * @param  s Escalar por el que dividir.
   * @return Este vector.
   */
  const J2DVector& operator /= (const J2DScalar& s)
  {
    const J2DScalar r = 1/s;
    x *= r;
    y *= r;
  
    return *this;
  }
  
  /** Suma. Devuelve un nuevo vector que es la suma de este y el dado.
   * @param  v Vector a sumar con este.
   * @return Vector resultado de la suma.
   */
  const J2DVector operator + (const J2DVector& v) const
  {
    return J2DVector(x + v.x, y + v.y);
  }
  
  /** Resta. Devuelve un nuevo vector que es la resta de este y el dado.
   * @param  v Vector a restar de este.
   * @return Vector resultado de la resta.
   */
  const J2DVector operator - (const J2DVector& v) const
  {
    return J2DVector(x - v.x, y - v.y);
  }
  
  /** Post-multiplicación por escalar.
   * Es el resultado de hacer v * s, en ese orden.
   * @param  s Escalar por el que multiplicar.
   * @return Nuevo objeto vector resultado de la multiplicación.
   */
  const J2DVector operator * (const J2DScalar& s) const
  {
    return J2DVector( x*s, y*s);
  }
  
  /** Pre-multiplicación por escalar.
   * Es el resultado de hacer s * v, en ese orden.
   * @param  s Escalar por el que multiplicar.
   * @param  v Vector.
   * @return Nuevo objeto vector resultado de la multiplicación.
   */
  friend inline const J2DVector operator * (const J2DScalar& s, const J2DVector& v)
  {
    return v*s;
  }
  
  /** Divide todas las componentes.
   * Este método no modifica al objeto vector actual.
   * @param  s Escalar por el que dividir.
   * @return Nuevo objeto vector con el resultado.
   */
  const J2DVector operator / (J2DScalar s) const
  {
    s = 1/s;
    return J2DVector(s*x, s*y);
  }

  /** Realiza el producto escalar por el vector dado.
   * El producto escalar de dos vectores v1 y v2 se define como
   * v1 · v2 = (x1*x2) + (y1*y2) + (z1*z2)
   * @param  v Vector con el que calcular el producto escalar.
   * @return Valor del producto escalar.
   */
  const J2DScalar Dot(const J2DVector& v) const
  {
    return x*v.x + y*v.y;
  }
  
  /** Calcula la longitud (norma)
	 * @return La longitud del vector.
	 */
  const float Length() const
  {
    return (J2DScalar)sqrt((double)this->Dot(*this));
  }
	
	/** Calcula y devuelve un vector perpendicular a este.
	 * El vector normal resultado es N=(y, -x), con x e y las componentes de este vector.
	 * @return Vector normal 
	 */
	const J2DVector Normal() const
	{
		return J2DVector(y, -x);
	}
	
  /** Carga el objeto
	 * @param f Fichero posicionado sobre este objeto
	 */
	u32 Load(JFile &f)
	{
		// Carga las coordenadas de los puntos
		if (0 != f.Read(&x, sizeof(x)) &&
				0 != f.Read(&y, sizeof(y)))
		{
			return 0;
		}

		return 1;
	}

  /** Salva el objeto
	 * @param f Fichero posicionado sobre este objeto
	 */
	u32 Save(JFile &f)
	{
		// Salva las coordenadas de los puntos
		if (0 != f.Write(&x, sizeof(x)) &&
				0 != f.Write(&y, sizeof(y)))
		{
			return 0;
		}
	
		return 1;
	}
};

typedef J2DVector J2DPoint;           // Posición 2D

#endif  // _J2DVECTOR_INCLUDED
