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
 * @file    JGLVector.cpp.
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2003
 * @version 0.0.1 - 30/04/2003 - Primera versión.
 */

#include <JLib/Graphics/JGLVector.h>

void JGLVector::Draw(JVector &pos)
{
	glLineWidth(3.0f);
  glBegin(GL_LINES);
    glVertex3f(pos.x, pos.y, pos.z);
    glVertex3f(x, y, z);
  glEnd();
	glLineWidth(1.0f);

  JVector v(*this + pos);
  JVector vr(v.Unit());
  float len = v.Length();

  if (len > 0.01)
  {
    JVector w(len, len, 0);
    w = w.Cross(v).Unit();

		glPointSize(5.0f);
    glBegin(GL_POINTS);
      glVertex3f(v.x, v.y, v.z);
      //glVertex3f(v.x - vr.x + w.x, v.y - vr.y - w.y, v.z - vr.z - w.z);
      //glVertex3f(v.x - vr.x + w.x, v.y - vr.y + w.y, v.z - vr.z + w.z);
    glEnd();
		glPointSize(1.0f);
  }
}
