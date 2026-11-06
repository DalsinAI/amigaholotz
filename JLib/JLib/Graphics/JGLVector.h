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

/** Vector en pantalla (OpenGL).
 * @file    JGLVector.h.
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2003
 * @version 0.0.1 - 30/04/2003 - Primera versión.
 */

#ifndef _JGLVECTOR_INCLUDED
#define _JGLVECTOR_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Math/JVector.h>
#include <SDL.h>
#include <GL/gl.h>

/** Vector en pantalla para OpenGL.
 */
class JGLVector : public JVector
{
public:
  /** Dibuja el vector en pantalla.
   * @param  pos Posición de origen del vector. 
   */
  void Draw(JVector &pos);
};

#endif  // _JGLVECTOR_INCLUDED
