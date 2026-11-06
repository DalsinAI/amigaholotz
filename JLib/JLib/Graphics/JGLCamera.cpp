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
//   | up                              ^ z (yaw)
//   |        __________>              |
// /---\      eye-target         *     |
// | x |O---------------------->***    | x (pitch)
// \---/ eye                   target  o-------->
//                                              y (roll)
///////////////////////////////////////////////////////////////////////////////

#include <JLib/Graphics/JGLCamera.h>

// Crea una cámara
JGLCamera::JGLCamera(JPoint _eye,
                     JPoint _target,
                     JVector _up)
{
  orgEye = _eye;
  orgTarget = _target;
  orgUp = _up;

  Reset();
}

// Calcula los vectores base en función del ojo y el objetivo
void JGLCamera::ComputeBasis()
{
  // Usa el eje z de esta cámara para apuntar al objetivo
  camera.R[2] = (camera.O - target).Unit();
  
  // El eje x será perpendicular al plano formado por up y eye-target
  camera.R[0] = camera.R[1].Cross(camera.R[2]).Unit();

  // El eje y es ortogonal a los anteriores
  camera.R[1] = camera.R[2].Cross(camera.R[0]).Unit();
}

// Modifica la posición de la cámara (eye) la cantidad indicada
void JGLCamera::Translate(JVector deltaPos)
{
  deltaPos = camera.TransformVectorToParent(deltaPos);
  camera.O += deltaPos;

  ComputeBasis();
}

// Mueve la cámara (eye) a la posición indicada
void JGLCamera::TranslateTo(JVector pos)
{
  camera.O = pos;

  ComputeBasis();
}

// Rota la cámara en torno al eje x pasando por el objetivo
void JGLCamera::Rotate(float x, float y, float z)
{
  JScalar tLen = (camera.O - target).Length();
  camera.O = target;

  camera.RotateAboutX(x);
  camera.RotateAboutY(y);
  camera.RotateAboutZ(z);
  
  camera.O += camera.R[2] * tLen;
}

// Modifica la posición del objetivo la cantidad indicada
void JGLCamera::TargetTranslate(JVector deltaTarget)
{
  deltaTarget = camera.TransformVectorToParent(deltaTarget);
  target += deltaTarget;

  ComputeBasis();
}

// Mueve la posición de objetivo a la posición indicada
void JGLCamera::TargetTranslateTo(JVector pos)
{
  target = camera.TransformVectorToParent(pos);

  ComputeBasis();
}

// Rota la cámara en torno:
// - al eje x (Pitch, dirección perpendicular al plano eye-target/up)
// - al eje y (Yaw, dirección up)
// - al eje z (Roll, dirección eye-target)
void JGLCamera::TargetRotate(float x, float y, float z)
{
  float tLen = (camera.O - target).Length();
  target = camera.O;

  camera.RotateAboutX(x);                           // Giramos la cantidad indicada
  camera.RotateAboutY(y);
  camera.RotateAboutZ(z);
  
  target -= camera.R[2] * tLen;    // Desplazamos la longitud original en la nueva dirección
}

// Zoom. Especifica la distancia eye-target
void JGLCamera::Zoom(float distance)
{
  if (distance <= 0)
    return;

  camera.O = target;
  camera.O -= (camera.R[2] * distance);

  ComputeBasis();
}

// Ajusta la perspectiva (más o menos distorsión)
void JGLCamera::Perspective(float distortAmount)
{
}

// Establece la proyección de cámara
void JGLCamera::Set()
{
  glLoadIdentity();
  gluLookAt(camera.O.x, camera.O.y, camera.O.z, 
            target.x, target.y, target.z, 
            camera.Y().x, camera.Y().y, camera.Y().z);
}

// Reset de cámara
void JGLCamera::Reset()
{
  camera.O = orgEye;
  target = orgTarget;
  
  // Inicializamos el eje Y de la base al vector up que nos dan
  camera.R[1] = orgUp;

  ComputeBasis();
}
