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

/** Definiciones para controlar el estado de un sólido rígido.
 * @file    JRigidBodyState.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2003
 * @version 0.0.1 - 30/04/2003 - Primera versión.
 */

#ifndef _JRIGIDBODYSTATE_INCLUDED
#define _JRIGIDBODYSTATE_INCLUDED

/** Encapsula métodos de cálculo de dinámica de sólido rígido.
 */
class JRigidBodyState : public JCoordAxes
{
public: 
  JVector V;        /**< Velocidad lineal en m/s */
  JVector W;        /**< Velocidad angular en rad/s */
  JMatrix I;        /**< Tensor de inercia en coordenadas de mundo, kg m m */
  JMatrix I_inv;    /**< Inverso del tensor de inercia en coordenadas de mundo */

public: 
  /** Crea un nuevo controlador de estado de sólido rígido. */
  JRigidBodyState() {}
  
  /** Crea un nuevo controlador de estado de sólido rígido 
   * con la velocidad lineal y angular.
   * @param  v Velocidad lineal.
   * @param  w Velocidad angular.
   */
  JRigidBodyState(const JVector& v, const JVector& w) : V (v), W (w) {}
  
  /** Devuelve la velocidad lineal
   * @return Velocidad lineal del sólido.
   */
  const JVector& Velocity() const { return V; }
  
  /** Establece la velocidad lineal.
   * @param  v Velocidad lineal.
   */
  void Velocity(const JVector& v) { V = v; }
  
  /** Devuelve la velocidad angular
   * @return Velocidad angular del sólido.
   */
  const JVector& AngularVelocity() const { return W; }
  
  /** Establece la velocidad angular.
   * @param  v Velocidad angular.
   */
  void AngularVelocity(const JVector& v) {W = v;}
  
  /** Devuelve el tensor de inercia.
   * @return Tensor de inercia del sólido.
   */
  const JMatrix& inertiaTensor() const { return I; }
  
  /** Devuelve el inverso del tensor de inercia.
   * @return Inverso del tensor de inercia del sólido.
   */
  const JMatrix& InverseInertiaTensor() const { return I_inv; }
  
  /** Calcula el tensor de inercia y su inverso a partir
   * de la orientacion actual y los momentos de inercia principales.
   * @param  ip Orientación actual.
   * @todo   Implementar.
   */
  void CalculateInertiaTensor(const JVector& ip);
}; 

#endif  // _JRIGIDBODYSTATE_INCLUDED
