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

/** Base ortonormal respecto a un padre.
 * @file    JBasis.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2003
 * @version 0.0.1 - 30/04/2003 - Primera versión.
 */

#ifndef _JBASIS_INCLUDED
#define _JBASIS_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>

/** Base de vectores ortonormales, respecto a un padre.
 */
class JBasis
{
public:
  JMatrix R;            /**< Vectores base en columnas */

public:
  /** Crea una base.
   */
  JBasis(){};
  
  /** Crea una base a partir de sus vectores columna.
   */
  JBasis(const JVector& v0,
         const JVector& v1,
         const JVector& v2) : R(v0, v1, v2){};
  
  /** Crea una base a partir de una matriz. Las columnas son los vectores
   * base.
   */
  JBasis(const JMatrix& m) : R(m) {}

  /** Acceso indexado a los vectores columna.
   * @param  i Índice del vector columna a ser recuperado (0, 1 ó 2).
   * @return Vector columna pedido.
   */
  const JVector& operator [] (s32 i) const { return R.C[i]; }
  
  /** Función de acceso al primer vector columna.
   * Es equivalente a usar operator [0].
   * @return Primer vector columna.
   */
  const JVector& X() const { return R.C[0]; }

  /** Función de acceso al segundo vector columna.
   * Es equivalente a usar operator [1].
   * @return Segundo vector columna.
   */
  const JVector& Y() const { return R.C[1]; }

  /** Función de acceso al tercer vector columna.
   * Es equivalente a usar operator [2].
   * @return Tercer vector columna.
   */
  const JVector& Z() const { return R.C[2]; }
  
  /** Devuelve la matriz de base. Sus columnas son los vectores que
   * forman la base.
   * @return Matriz de base.
   */
  const JMatrix& Basis() const { return R; }
  
  /** Establece la base a partir de sus vectores.
   * @param v0 Primer vector base.
   * @param v1 Segundo vector base.
   * @param v2 Tercer vector base.
   */
  void Basis(const JVector& v0, const JVector& v1, const JVector& v2)
  {
    this->R[0] = v0;
    this->R[1] = v1;
    this->R[2] = v2;
  }
  
  /** Rotación de mano derecha en torno al primer vector base (X).
   * @param a Angulo de rotación en radianes.
   */
  void RotateAboutX(const JScalar& a)
  {
    if(0 != a)  // No se rota 0
    {
      JVector b1 = this->Y()*cosf(a) + this->Z()*sinf(a);   // Rota X
      JVector b2 = -this->Y()*sinf(a) + this->Z()*cosf(a);  // Rota Z
      
      // Establece la base
      this->R[1] = b1;
      this->R[2] = b2;
      // X no camia
    }
  }
  
  /** Rotación de mano derecha en torno al segundo vector base (Y).
   * @param a Angulo de rotación en radianes.
   */
  void RotateAboutY(const JScalar& a)
  {
    if(0 != a)  // No se rota 0
    {
      JVector b2 = this->Z()*cosf(a) + this->X()*sinf(a);   // Rota Z
      JVector b0 = -this->Z()*sinf(a) + this->X()*cosf(a);  // Rota Y

      // Establece la base
      this->R[2] = b2;
      this->R[0] = b0;
      // Y no camia
    }
  }
  
  /** Rotación de mano derecha en torno al tercer vector base (Z).
   * @param a Angulo de rotación en radianes.
   */
  void RotateAboutZ(const JScalar& a)
  {
    if(0 != a)  // No se rota 0
    {
      // No cambiar la base mientras se hace la operación
      JVector b0 = this->X()*cosf(a) + this->Y()*sinf(a);   // Rota X
      JVector b1 = -this->X()*sinf(a) + this->Y()*cosf(a);  // Rota Y

      // Establece la base
      this->R[0] = b0;
      this->R[1] = b1;
      // Z no cambia
    }
  }
  
  /** Rotación de mano derecha en torno al eje dado.
   * @param theta Angulo de rotación en radianes.
   * @param u Vector director del eje de rotación.
   */
  void Rotate(const JScalar& theta, const JVector& u)
  {
    JBasis ZInv, ZZ, YInv, YY, U;
    JVector v;
    
    // Giro hasta el plano YZ
    v = u;
    v.x = 0.0f;
    ZZ.RotateAboutZ(acosf(1.0f - v.Length()));
    ZInv = ZZ.R.Inverse();
    
    // Giro hasta el plano XY
    v = u;
    v.z = 0.0f;
    YY.RotateAboutY(acosf(1.0f - v.Length()));
    YInv = YY.R.Inverse();

    // Giro en eje Z
    U.R = this->R;
    U.RotateAboutZ(theta);
    this->R = ZInv.R * YInv.R * U.R * YY.R * ZZ.R;
  }
  
  // Rotación, la longitud 'da' es theta, la dirección unitaria de 'da' es u
  void Rotate(const JVector& da);

  /** Espresa en coordenadas de esta base el vector dado en coordenadas
   * de la base padre.
   * @param  v Vector expresado en coordenadas de la base padre.
   * @return Vector expresado en coordenadas de esta base.
   */
  const JVector TransformVectorToLocal(const JVector& v) const
  {
    return JVector(R.C[0].Dot(v), R.C[1].Dot(v), R.C[2].Dot(v));
  }
  
  /** Espresa en coordenadas de la base padre el vector dado en coordenadas
   * de esta base.
   * @param  v Vector expresado en coordenadas de esta base.
   * @return Vector expresado en coordenadas de la base padre.
   */
  const JPoint TransformVectorToParent(const JVector& v) const
  {
    return R.C[0] * v.x + R.C[1] * v.y + R.C[2] * v.z;
  }
};

#endif  // _JBASIS_INCLUDED
