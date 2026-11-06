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

/** Clase base para threads.
 * @file    JThread.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    18/04/2004
 * @version 0.0.1 - 18/04/2004 - Primera versión.
 */

#include <JLib/Util/JThread.h>

JThread::JThread(int (* func)(void *), void *data)
	: valid(false), paused(false), terminate(false), thread(0), id(0)
{
	if (func)
	{
		s32 ret;
		if (0 == (ret = SDL_WasInit(SDL_INIT_EVENTTHREAD)))
		{
			ret = SDL_Init(SDL_INIT_EVENTTHREAD);
		}

		if (ret == 0)
		{
			// Si no se dan datos asociados, se pasará este objeto a la función
			if (data == 0)
			{
				data = this;
			}
			
			thread = SDL_CreateThread(func, data);
			id = SDL_GetThreadID(thread);
			
			fprintf(stderr, "Nuevo thread: 0x%x", id);
		}
		else
		{
			fprintf(stderr, "No se pudo iniciar el sistema de threads.");
		}
	}
}
