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

/** Matriz 3x3.
 * @file    JMatrix.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2003
 * @version 0.0.1 - 30/04/2003 - Primera versión.
 */

#ifndef _JMATRIX_INCLUDED
#define _JMATRIX_INCLUDED

#include <JLib/Util/JTypes.h>

/** Representa una matriz tridimensional mediante 3 vectores en columna.
 */
class JMatrix
{
public:
  JVector C[3];             /**< Vectores columna */

public:
  
  /** Crea una nueva matriz. La matriz es la matriz identidad.
   */
  JMatrix()
  {
    // Matriz identidad
    C[0].x = 1;
    C[1].y = 1;
    C[2].z = 1;
  }
  
  /** Crea la matriz a partir de tres vectores columna.
   * @param  c0 Vector columna 0.
   * @param  c1 Vector columna 1.
   * @param  c2 Vector columna 2.
   */
  JMatrix(const JVector& c0, const JVector& c1, const JVector& c2)
  {
    C[0] = c0;
    C[1] = c1;
    C[2] = c2;
  }
  
  /** Acceso indexado. 
   * @note   Si se usa con el del vector el acceso es <b>M[columna][fila]</b>, 
   * (y NO el estándar M[fila][columna]).
   * @param  i Índice del vector columna a recuperar (0, 1 ó 2).
   */
  JVector& operator [] (s32 i)
  {
    return C[i];
  }
  
  /** Comparación de matrices.
   * @param  m Matriz a comparar con esta.
   * @return <b>true</b> en caso de ser iguales, <b>false</b> si no.
   */
  const bool operator == (const JMatrix& m) const
  {
    return C[0]==m.C[0] && C[1]==m.C[1] && C[2]==m.C[2];
  }
  
  /** Comparación de matrices.
   * @param  m Matriz a comparar con esta.
   * @return <b>true</b> en caso de ser diferentes, <b>false</b> si no.
   */
  const bool operator != (const JMatrix& m) const
  {
    return !(m == *this);
  }
  
  /** Asigna una matriz a esta. 
   * @param  m Matriz a asignar a esta.
   * @return Este objeto matriz.
   */
  const JMatrix& operator = (const JMatrix& m)
  {
    C[0] = m.C[0];
    C[1] = m.C[1];
    C[2] = m.C[2];
  
    return *this;
  }

  /** Incremento. Suma cada elemento de esta matriz con el
   * correspondiente de la matriz dada.
   * @param  m Matriz a sumar a esta.
   * @return Este objeto matriz.
   */
  const JMatrix& operator +=(const JMatrix& m)
  {
    C[0] += m.C[0];
    C[1] += m.C[1];
    C[2] += m.C[2];
    
    return *this;
  }
  
  /** Decremento. Resta cada elemento de la matriz dada del
   * correspondiente de esta matriz.
   * @param  m Matriz a restar de esta.
   * @return Este objeto matriz.
   */
  const JMatrix& operator -=(const JMatrix& m) 
  {
    C[0] -= m.C[0];
    C[1] -= m.C[1];
    C[2] -= m.C[2];
    
    return *this;
  }
  
  /** Multiplica cada elemento de esta matriz por el escalar dado.
   * Esta operación afecta al objeto matriz actual.
   * @param  s Escalar por el que multiplicar.
   * @return Este objeto matriz.
   */
  const JMatrix& operator *= (const JScalar& s)
  {
    C[0] *= s;
    C[1] *= s;
    C[2] *= s;
  
    return *this;
  }

  /** Multiplica esta matriz por la dada, en ese orden.
   * Esta operación afecta al objeto matriz actual.
   * @param  m Matriz por la que multiplicar.
   * @return Este objeto matriz.
   */
  const JMatrix& operator *= (const JMatrix& m)
  {
    // NOTA: No cambiar las columnas en medio de la operación
    JMatrix temp = (*this);
    C[0] = temp * m.C[0];
    C[1] = temp * m.C[1];
    C[2] = temp * m.C[2];
    
    return *this;
  }
  
  /** Suma la matriz dada a esta.
   * Esta operación devuelve un nuevo objeto matriz.
   * @param  m Matriz a sumar a esta.
   * @return Nuevo objeto matriz resultado de la operación.
   */
  const JMatrix operator + (const JMatrix& m) const
  {
    return JMatrix(C[0] + m.C[0], C[1] + m.C[1], C[2] + m.C[2]);
  }
  
  /** Resta la matriz dada de esta.
   * Esta operación devuelve un nuevo objeto matriz.
   * @param  m Matriz a restar a esta.
   * @return Nuevo objeto matriz resultado de la operación.
   */
  const JMatrix operator - (const JMatrix& m) const
  {
    return JMatrix(C[0] - m.C[0], C[1] - m.C[1], C[2] - m.C[2]);
  }
  
  /** Multiplicación copia por un escalar.
   * Esta operación crea un nuevo objeto matriz.
   * @param  s Escalar por el que multiplicar cada elemento.
   * @return Nuevo objeto matriz resultado de la operación.
   */
  const JMatrix operator * (const JScalar& s) const
  {
    return JMatrix(C[0]*s, C[1]*s, C[2]*s);
  }

  /** Multiplicación propia por un escalar.
   * Esta operación actúa sobre este objeto.
   * @param  s Escalar por el que multiplicar cada elemento de la matriz.
   * @param  m Matriz a multiplicar.
   * @return Este objeto matriz resultado de la operación.
   */
  friend inline const JMatrix operator * (const JScalar& s, const JMatrix& m)
  {
    return m * s;
  }

  /** Post-Multiplicación copia por un vector (m * v).
   * @param  v Vector por el que multiplicar.
   * @return Vector resultado de la operación.
   */
  const JVector operator * (const JVector& v) const
  {
    return(C[0]*v.x + C[1]*v.y + C[2]*v.z);
  }
  
  /** Pre-Multiplicación copia por un vector (v * m).
   * @param  v Vector por el que multiplicar.
   * @param  m Matriz a multiplicar.
   * @return Vector resultado de la operación.
   */
  inline friend const JVector operator * (const JVector& v, const JMatrix& m)
  {
    return JVector(m.C[0].Dot(v), m.C[1].Dot(v), m.C[2].Dot(v));
  }
  
  /** Multiplica esta matriz por la dada en ese orden.
   * Esta operación crea una nueva matriz.
   * @param  m Matriz a multiplicar.
   * @return Nuevo objeto matriz resultado de la operación.
   */
  const JMatrix operator * (const JMatrix& m) const
  {
    return JMatrix((*this) * m.C[0], (*this) * m.C[1], (*this) * m.C[2]);
  }
  
  /** Devuelve la matriz traspuesta a esta.
   * @return Nueva matriz resultado de la trasposición.
   */
  JMatrix Transpose() const
  {
    return JMatrix(JVector(C[0].x, C[1].x, C[2].x),
                   JVector(C[0].y, C[1].y, C[2].y),
                   JVector(C[0].z, C[1].z, C[2].z));
  }
  
  /** Devuelve el determinante de esta matriz.
   * @return Valor del determinante de esta matriz.
   */
  const JScalar Determinant() const
  {
    return C[0].Dot(C[1].Cross(C[2]));
  }

  /** Devuelve una nueva matriz adjunta a esta.
   * @return Matriz adjunta.
   */
  JMatrix Adjoint() const
  {
    JMatrix m;
    m[0][0] =  (C[1].y * C[2].z - C[2].y * C[1].z);
    m[1][0] = -(C[0].y * C[2].z - C[2].y * C[0].z);
    m[2][0] =  (C[0].y * C[1].z - C[1].y * C[0].z);
    
    m[0][1] = -(C[1].x * C[2].z - C[2].x * C[1].z);
    m[1][1] =  (C[0].x * C[2].z - C[2].x * C[0].z);
    m[2][1] = -(C[0].x * C[1].z - C[1].x * C[0].z);
    
    m[0][2] =  (C[1].x * C[2].y - C[2].x * C[1].y);
    m[1][2] = -(C[0].x * C[2].y - C[2].x * C[0].y);
    m[2][2] =  (C[0].x * C[1].y - C[1].x * C[0].y);

    return m;
  }

  /** Devuelve una nueva matriz, inversa de esta.
   * @return Matriz inversa a esta.
   */
  const JMatrix Inverse() const
  {
    JMatrix m;
    JScalar j = Determinant();
    if (j != 0)
    {
      // La inversa es la matriz adjunta traspuesta dividida por el determinante
      m = Adjoint();
      m = m.Transpose();
      m *= 1.0f/Determinant();
    }

    return m;
  }
};

#endif  // _JMATRIX_INCLUDED
