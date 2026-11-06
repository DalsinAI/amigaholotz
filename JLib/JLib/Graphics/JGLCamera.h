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

///////////////////////////////////////////////////////////////////////////////
// @author: Juan Carlos Seijo Pérez
// @date: 29/04/2003
// @description: Cámara.
//
//   ^ __>                            Vista (ejes locales de cámara):
//   | up                              ^ y (yaw)
//   |        __________>              |
// /---\      eye-target         *     |
// | x |O---------------------->***    | x (pitch)
// \---/ eye                   target  o-------->
//                                              z (roll)
///////////////////////////////////////////////////////////////////////////////

#ifndef _JGLCAMERA_INCLUDED
#define _JGLCAMERA_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>
#include <JLib/Math/JMath.h>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

class JGLCamera : public JObject
{
public:
  float humanize;                     // Factor de irregularidad de movimiento
  float spring;                       // Factor de suavizado de movimiento
  float zoom;                         // Factor de zoom
  JPoint orgTarget;                   // Objetivo original 
  JPoint orgEye;                      // Punto de vista original
  JVector orgUp;                      // Sentido vertical original
  JPoint target;                      // Objetivo
  JCoordAxes camera;                  // Base de la cámara

  // Crea una cámara
  JGLCamera(JPoint _eye,
            JPoint _target,
            JVector _up);

  // Calcula los vectores base en función del ojo y el objetivo
  void ComputeBasis();

  // Modifica la posición de la cámara (eye) la cantidad indicada
  void Translate(JVector deltaPos);

  // Mueve la cámara (eye) a la posición indicada
  void TranslateTo(JVector pos);

  // Rota la cámara en torno:
  // - al eje x (Pitch, dirección perpendicular al plano eye-target/up)
  // - al eje y (Roll, dirección eye-target)
  // - al eje z (Yaw, dirección up)
  void Rotate(float x, float y, float z);

  // Modifica la posición del objetivo la cantidad indicada
  void TargetTranslate(JVector deltaTarget);
  
  // Mueve la posición de objetivo a la posición indicada
  void TargetTranslateTo(JVector pos);

  // Rota la cámara en torno a un eje pasando por el objetivo
  void TargetRotate(float x, float y, float z);

  // Zoom. Especifica la cantidad de zoom
  void Zoom(float amount);

  // Ajusta la perspectiva (más o menos distorsión)
  void Perspective(float distortAmount);

  // Establece la proyección de cámara
  void Set();

  // Reset de cámara
  void Reset();
};

#endif  // _JGLCAMERA_INCLUDED
