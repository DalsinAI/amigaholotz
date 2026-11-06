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

/** Clase de aplicación OpenGL.
 * @file    JGLApp.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    01/04/2003
 * @version 0.0.1 - 01/04/2003 - Primera versión.
 */

#include <JLib/Util/JGLApp.h>

// Constructor
JGLApp::JGLApp(const JString &strTitle, s32 w, s32 h, bool fullScr, s32 _depth, u32 otherFlags)
 : JApp(strTitle, w, h, fullScr, _depth, otherFlags)
{
}

// Inicializa la aplicación
bool JGLApp::Init()
{
  if (-1 == SDL_Init(SDL_INIT_EVERYTHING))
  {
    return false;
  }

	// Establece los valores de profundidad por canal
	s32 rd = 5, gd = 6, bd = 5, ad = 0, db = 1;

	switch (depth)
	{
		case 24:
			rd = gd = bd = ad = 8;
			break;

		case 32:
			rd = gd = bd = ad = 8;
			break;

		default:
			break;
	}
	
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, rd );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, gd );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, bd );
  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, ad );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, depth );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, db );

  flags |= SDL_OPENGL | SDL_OPENGLBLIT;
  if (fullScreen)
    flags |= SDL_FULLSCREEN;

  if ( (screen = SDL_SetVideoMode(width, height, depth, flags)) == 0 ) 
  {
    fprintf(stderr, "No se pudo establecer el modo OpenGL: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

	SDL_GL_GetAttribute( SDL_GL_RED_SIZE, &rd );
  SDL_GL_GetAttribute( SDL_GL_GREEN_SIZE, &gd );
  SDL_GL_GetAttribute( SDL_GL_BLUE_SIZE, &bd );
  SDL_GL_GetAttribute( SDL_GL_ALPHA_SIZE, &ad );
  SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &db );

	fprintf(stderr, "Setvideomode(): \n");
	fprintf(stderr, "W=%d H=%d BPP=%d\n", screen->w, screen->h, screen->format->BitsPerPixel);
	fprintf(stderr, "OpenGL: %s\n", screen->flags & SDL_OPENGL ? "YES" : "NO");
	fprintf(stderr, "OpenGL blitting: %s\n", screen->flags & SDL_OPENGLBLIT ? "YES" : "NO");
	fprintf(stderr, "FullScreen: %s\n", screen->flags & SDL_FULLSCREEN ? "YES" : "NO");
	fprintf(stderr, "R:%d G:%d B:%d A:%d D-Buffer:%d\n", rd, gd, bd, ad, db);
	fprintf(stderr, "\n");

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 10000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  // Establecemos el título
  SDL_WM_SetCaption(title, 0);

  // Activamos la repetición de teclas
  SDL_EnableKeyRepeat(300, 30);

	// Inicializamos el array de teclas
	keys = SDL_GetKeyState(&numKeys);

  return true;	// Todo OK
}

// Destructor
JGLApp::~JGLApp()
{
  SDL_Quit();
}
