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

/** Clase para gestión de una ventana gráfica.
 * @file    JApp.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    01/04/2003
 * @version 0.0.1 - Primera versión - 01/04/2003
 */

#include <JLib/Util/JApp.h>

JApp * JApp::thisApp = 0;

// Constructor
JApp::JApp(const JString &strTitle, s32 w, s32 h, bool fullScr, s32 _depth, u32 otherFlags)
 : active(true), paused(false), endLoop(false), title(strTitle), width(w), height(h), depth(_depth), fullScreen(fullScr), 
   doInput(true), flags(otherFlags), appPauseTime(0), keys(0), keyMods(KMOD_NONE), numKeys(0), mouseX(0), mouseY(0), mouseBt(0), 
   mouseRelX(0), mouseRelY(0), joyAxisX(0), joyAxisY(0), joyButton(0), fps(25), dumpVideoInfo(false), soundEnabled(false), iconName(0), screen(0), joystick(0)
{
  __OnActive = 0;
  __OnKeyUp  = 0;
  __OnKeyDown = 0;
  __OnMouseMove = 0;
  __OnMouseDown = 0;
  __OnMouseUp = 0;
  __OnJoyMove = 0;
  __OnJoyBall = 0;
  __OnJoyHat = 0;
  __OnJoyUp = 0;
  __OnJoyDown = 0;
  __OnQuit = 0;
  __OnSysWM = 0;
  __OnResize = 0;
  __OnExpose = 0;
  __OnUser = 0;
	thisApp = this;
}

// Inicializa la aplicación
bool JApp::Init()
{
  if (-1 == SDL_Init(SDL_INIT_EVERYTHING))
  {
    return false;
  }

	if (dumpVideoInfo)
	{
		DumpVideoInfo();
	}

  if (fullScreen)
    flags |= SDL_FULLSCREEN;

	// Establecemos el icono
	if (iconName)
	{
		JImage imgIcon;
		if (!imgIcon.Load(iconName))
		{
			fprintf(stderr, "Couldn't load icon!: %s\n", iconName);
		}
		else
		{
			imgIcon.ColorKey(0, SDL_SRCCOLORKEY);
			SDL_WM_SetIcon(imgIcon.Surface(), 0);
		}
	}

  if ( (screen = SDL_SetVideoMode(width, height, depth, flags)) == 0 ) 
  {
    fprintf(stderr, "Error: Couldn't set video mode %s\n", SDL_GetError());
    SDL_Delay(2000);
    SDL_Quit();
    return false;
  }

  if(SDL_NumJoysticks() > 0)
  {
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    if(!joystick)
    {
      fprintf(stderr, "Warning: Joystick not initialized: %s\n", SDL_GetError());
    }
  }
  else
  {
    joystick = 0;
  }

	// Initializes the font subsystem
	if (!JFont::Init())
	{
		return false;
	}

  // Establecemos el título
  SDL_WM_SetCaption(title, 0);

  // Activamos la repetición de teclas
  SDL_EnableKeyRepeat(300, 30);

	// Inicializamos el array de teclas
	keys = SDL_GetKeyState(&numKeys);

	FPS(fps);

	return true;	// Todo OK
}

// Comprueba los eventos
bool JApp::UpdateEvents()
{
	keys = SDL_GetKeyState(&numKeys);
	keyMods = SDL_GetModState();

  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    switch(e.type)
    {
      case SDL_ACTIVEEVENT:
				// Pasamos a estado de pausa
				switch (e.active.state)
				{
				case SDL_APPMOUSEFOCUS:
					//printf("Mouse focus %s!\n", e.active.gain == 0 ? "LOST" : "GAINED");
					break;
				case SDL_APPINPUTFOCUS:
					//printf("Input focus %s!\n", e.active.gain == 0 ? "LOST" : "GAINED");
					break;
				case SDL_APPACTIVE:
					// Pausa o reanuda la aplicación
					e.active.gain == 0 ? Pause(true) : Pause(false);
					break;
				}
				
        if (__OnActive)
        {
          __OnActive(e.active.gain != 0, (s32)e.active.state);
        }
				else
				{
					/**< @todo Averiguar por qué da problemas. */
          //active = (e.active.gain != 1);
				}
        break;

      case SDL_KEYDOWN:
				
				// Actualiza el estado del teclado
				keys = SDL_GetKeyState(&numKeys);
				keyMods = SDL_GetModState();
        
				if (__OnKeyDown)
        {
          __OnKeyDown(e.key.keysym);
        }
        break;

      case SDL_KEYUP:

				// Actualiza el estado del teclado
				keys = SDL_GetKeyState(&numKeys);
				keyMods = SDL_GetModState();
        
        if (__OnKeyUp)
        {
          __OnKeyUp(e.key.keysym);
        }
        break;

      case SDL_MOUSEMOTION:

				// Actualiza el estado del ratón
				SDL_GetMouseState(&mouseX, &mouseY);
				SDL_GetRelativeMouseState(&mouseRelX, &mouseRelY);

        if (__OnMouseMove)
        {
          __OnMouseMove(e.motion.x, e.motion.y, e.motion.state, e.motion.xrel, e.motion.yrel);
        }
        break;

      case SDL_MOUSEBUTTONDOWN:
				
				// Actualiza el estado de los botones del ratón
				mouseBt = SDL_GetMouseState(&mouseX, &mouseY);

				if (__OnMouseDown)
        {
          __OnMouseDown(e.button.button, e.button.x, e.button.y);
        }
        break;

      case SDL_MOUSEBUTTONUP:

				// Actualiza el estado de los botones del ratón
				mouseBt = SDL_GetMouseState(&mouseX, &mouseY);

        if (__OnMouseUp)
        {
          __OnMouseUp(e.button.button, e.button.x, e.button.y);
        }
        break;

      case SDL_JOYAXISMOTION:
        switch(e.jaxis.axis)
        {
          case 0:
            joyAxisX = e.jaxis.value;
            break;

          case 1:
            joyAxisY = e.jaxis.value;
            break;

          default:
            break;
        }

        if (__OnJoyMove)
        {
          __OnJoyMove(e.jaxis.which, e.jaxis.axis, e.jaxis.value);
        }
        break;

      case SDL_JOYBALLMOTION:
        if (__OnJoyBall)
        {
          __OnJoyBall(e.jball.which, e.jball.ball, e.jball.xrel, e.jball.yrel);
        }
        break;

      case SDL_JOYHATMOTION:
        if (__OnJoyHat)
        {
          __OnJoyHat(e.jhat.which, e.jhat.hat, e.jhat.value);
        }
        break;

      case SDL_JOYBUTTONDOWN:
        joyButton = 1;

        if (__OnJoyDown)
        {
          __OnJoyDown(e.jbutton.which, e.jbutton.button);
        }
        break;

      case SDL_JOYBUTTONUP:
        joyButton = 0;

        if (__OnJoyUp)
        {
          __OnJoyUp(e.jbutton.which, e.jbutton.button);
        }
        break;

      case SDL_QUIT:
        if (__OnQuit)
        {
          __OnQuit();
        }
        Exit();
        break;

      case SDL_SYSWMEVENT:
        if (__OnSysWM)
        {
          __OnSysWM();
        }
        break;

      case SDL_VIDEORESIZE:

				if (__OnResize)
        {
          __OnResize(e.resize.w, e.resize.h);
        }
				else
				{
					Resize(e.resize.w, e.resize.h, fullScreen);
				}
        break;

      case SDL_VIDEOEXPOSE:
        if (__OnExpose)
        {
          __OnExpose();
        }
        break;

      case SDL_USEREVENT:
        if (__OnUser)
        {
          __OnUser(e.user.code, e.user.data1, e.user.data2);
        }
        break;

      default:
        break;
    }
  }

  return true;
}


// Bucle principal
s32 JApp::MainLoop()
{
	while (!endLoop)
	{
		UpdateEvents();
		
		if (active)
		{
			if (timer.Changed())
			{
				if (Update())
				{
					Draw();
				}
				else
				{
					printf("Error updating the app.\n");
				}
			}
		}
		else
		{
			sleep(1);
		}
	}

  return 0;
}

// Pausa la aplicación. Devuelve los ms que lleva en pausa (false) o
// los transcurridos desde el inicio de la aplicación (true).
u32 JApp::Pause(bool doPause)
{
  paused = doPause;
  if (paused)
  {
    appPauseTime = SDL_GetTicks();
  }
  else
  {
    appPauseTime = SDL_GetTicks() - appPauseTime;
  }

  return appPauseTime;
}

// Cambia el tamaño de la ventana
void JApp::Resize(s32 w, s32 h, bool _fullScreen)
{
  if ((fullScreen = _fullScreen))
    flags |= SDL_FULLSCREEN;
  else
    flags &= ~SDL_FULLSCREEN;

  SDL_Surface *surf;
  surf  = SDL_SetVideoMode(w, h, depth, flags);
  
  if (surf)
  {
    screen = surf;
    width = w;
    height = h;
  }
}

void JApp::FPS(s32 newFPS)
{
	fps = newFPS;
	
	if (fps < 0)
	{
		fps = 25;
	}

	timer.Start(1000/fps);
}

int JApp::ParseArg(char *args[], int argc)
{
	if (args[0][0]!='-')
		return -1; // error

	switch (args[0][1])
	{
		// '-f' Fullscreen
		case 'f':
			flags |= SDL_FULLSCREEN;
			return 0; // no arguments used

		// '-w' En ventana
		case 'w':
			flags &= ~SDL_FULLSCREEN;
			return 0; // no arguments used

		// '-i' Muestra información del video
		case 'i':
			dumpVideoInfo = true;
			return 0; // no arguments used

		// '-m' Modo gráfico/tamaño de ventana
		case 'm':
			{
				s32 w, h, bpp;
				if (3 ==sscanf(args[0] + 2, "%d%*c%d%*c%d", &w, &h, &bpp))
				{
					if (w > 0 && h > 0 && bpp > 0)
					{
						width = w;
						height = h;
						depth = bpp;
					}
				}
			}
			return 0; // no arguments used
		case '-':
			{
				switch (*(args[0] + 2))
				{
					case 'f':
						if (strcmp((args[0] + 2), "fps") == 0)
						{
							if (argc<1) // I need at least 1 more argument
								return -2;
							fps = atoi(args[1]);
						}
						return 1; // 1 argument used
				}
			}
	}
	return -1; // error
}

void JApp::PrintUsage(char *program)
{
	fprintf(stderr, "JApp v1.0. (C) Juan Carlos Seijo Pérez / 2002-2004.\n\n");
	fprintf(stderr, "Usage: %s [-f]ullscreen [-w]indowed [--fps nnn] [-mWxHxBPP] [-i]nfo_of_video\n", program);
	fprintf(stderr, "\n");
	exit(0);
}

void JApp::ParseArgs(s32 argc, char **argv)
{
	s32 i = 0;

	while (++i < argc)
	{
		switch (*argv[i])
		{
		case '-':
			switch (*(argv[i] + 1))
			{
				// '-h' opción: ayuda
			case 'h':
				PrintUsage(argv[0]);
				break;

			default:
				// TODO: Preguntar a Miriam
				//				int n = ParseArg(argv+i, argc-2);
				int n = ParseArg(argv+i, argc-i);
				if (n>=0)
					i += n;
				else if (n==-1)
					fprintf(stderr, "Unknown option: %s. Use -h to get help.\n", argv[i]);
				else if (n==-2)
					fprintf(stderr, "Not enough arguments to option: %s. Use -h to get help.\n", argv[i]);
				else if (n<0)
					fprintf(stderr, "Error in option option %s. Use -h to get help.\n", argv[i]);
				break;
			}
			break;

		default:
			// treats it as the file name to load
			int n = ParseArg(argv+i, argc-i);
			if (n>=0)
				i += n;
			else if (n<0)
				fprintf(stderr, "Error in parameter: %s. Use -h to get help.\n", argv[i]);
			break;
		}
	}
}

void JApp::DumpVideoInfo()
{
	const SDL_VideoInfo *vi = VideoInfo();

	fprintf(stdout, "hw_available: %s\n", vi->hw_available ? "yes" : "no");
	fprintf(stdout, "wm_available: %s\n", vi->wm_available ? "yes" : "no");
	fprintf(stdout, "     blit_hw: %s\n", vi->blit_hw ? "yes" : "no");
	fprintf(stdout, "  blit_hw_CC: %s\n", vi->blit_hw_CC ? "yes" : "no");
	fprintf(stdout, "   blit_hw_A: %s\n", vi->blit_hw_A ? "yes" : "no");
	fprintf(stdout, "     blit_sw: %s\n", vi->blit_sw ? "yes" : "no");
	fprintf(stdout, "  blit_sw_CC: %s\n", vi->blit_sw_CC ? "yes" : "no");
	fprintf(stdout, "   blit_sw_A: %s\n", vi->blit_sw_A ? "yes" : "no");
	fprintf(stdout, "   blit_fill: %s\n", vi->blit_fill ? "yes" : "no");
	fprintf(stdout, "video memory: %d\n\n", vi->video_mem);

	fprintf(stdout, "Current pixel format:\n");
	fprintf(stdout, " BitsPerPixel: %d\n", vi->vfmt->BitsPerPixel);
	fprintf(stdout, "BytesPerPixel: %d\n", vi->vfmt->BytesPerPixel);
	fprintf(stdout, 
					"     RGBAmask: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", 
					vi->vfmt->Rmask, vi->vfmt->Gmask, vi->vfmt->Bmask, vi->vfmt->Amask);
	fprintf(stdout, 
					"    RGBAshift: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", 
					vi->vfmt->Rshift, vi->vfmt->Gshift, vi->vfmt->Bshift, vi->vfmt->Ashift);
	fprintf(stdout, 
					"     RGBAloss: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", 
					vi->vfmt->Rloss, vi->vfmt->Gloss, vi->vfmt->Bloss, vi->vfmt->Aloss);
	fprintf(stdout, "     ColorKey: 0x%08x\n", vi->vfmt->colorkey);
	fprintf(stdout, "        Alpha: 0x%02x\n", vi->vfmt->alpha);
}

void JApp::Icon(const char *filename)
{
	free(iconName);
	
	if (filename)
	{
		iconName = strdup(filename);
	}
}

void JApp::Destroy()
{
	free(iconName);
}

JApp::~JApp()
{
	Destroy();
	SDL_Quit();
}
