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
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

/** OpenGL application base class.
 * @file    JGLApp.h
 * @author  Juan Carlos Seijo Pérez
 * @date    01/04/2003
 * @version 0.0.1 - 01/04/2003 - First version.
 */

#ifndef _JGLAPP_INCLUDED
#define _JGLAPP_INCLUDED

#ifdef WIN32
#include <windows.h>
#endif

#include <JLib/Util/JApp.h>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

class JGLApp : public JApp
{
public:
  /** Creates the app.
   * @param  strTitle Window title.
   * @param  w Window width.
   * @param  h Window height.
   * @param  fullScr Fullscreen mode flag.
	 * @param  _depth Color depth.
	 * @param  otherFlags SDL additional flags. By default SDL_OPENGL | SDL_OPENGLBLIT are used.
   */
  JGLApp(const JString &strTitle, s32 w = 640, s32 h = 480, bool fullScr = false, s32 _depth = 16, u32 otherFlags = 0);

  /** Ends the application. The base class (JApp) quits SDL.
   */
  virtual ~JGLApp();

  /** Inits the app.
   * @return <b>true</b> if succeeded, <b>false</b> if not.
   */
  virtual bool Init();

	/** Swaps front and back buffer if dowblu buffered, else simply updates the screen.
	 */
	void SwapBuffers() {SDL_GL_SwapBuffers();}
};

#endif  // JGLAPP_INCLUDED
