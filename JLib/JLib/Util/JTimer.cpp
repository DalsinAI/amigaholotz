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

/** Timer.
 * @file    JTimer.cpp.
 * @author  Juan Carlos Seijo Pérez.
 * @date    21/04/2003.
 * @version 0.0.1 - 21/04/2003 - Primera versión.
 */

#include <JLib/Util/JTimer.h>

// Construye e inicializa un nuevo timer de baja resolución
JTimer::JTimer(u32 millis) : pause(0), last(0), cur(0), lastQueryCycle(0)
{
	if (millis == 0L)
    millis = 1L;

	cycleTime = millis;
  start = SDL_GetTicks();
}

// Comenzar
void JTimer::Start(u32 millis)
{
	if (millis != 0L)
		cycleTime = millis;

  start = SDL_GetTicks();
	last = 0;
	cur = 0;
	lastQueryCycle = 0;
	pause = 0;
}
	
// Pausar
void JTimer::Pause()
{
  if (!pause)
  {
    pause = SDL_GetTicks();
    cur = pause;            // Para la consulta durante la pausa
  }
}

// Continuar tras pausa
void JTimer::Continue()
{
  if (pause)
  {
    start += SDL_GetTicks() - pause;
    pause = 0;
  }
}

// Consulta el número de ms ocurridos desde el comienzo del último ciclo
u32 JTimer::Lap()
{
	if (!pause)
    cur = SDL_GetTicks();

  return(cur - start);
}

// Consulta el número de ms que faltan para terminar el ciclo actual
u32 JTimer::Rem()
{
  return(SDL_GetTicks() - start)%cycleTime;
}

// Consulta el tiempo transcurrido entre llamadas a Tick()
u32 JTimer::Tick()
{
  u32 ret = last;
	
  if (!pause)
    cur = SDL_GetTicks();

  ret = cur - last;
  last = cur;

  return ret;
}

// Consulta el número de ciclos desde el comienzo
u32 JTimer::Cycles()
{
	if (!pause)
    cur = SDL_GetTicks();

  return ((cur - start) / cycleTime);
}

// Consulta si hemos pasado a otro ciclo desde la última consulta
// y devuelve el número de ciclos transcurridos
u32 JTimer::Changed()
{
  if (!pause)
    cur = SDL_GetTicks();

  u32 ret;
  ret = ((cur - start) / cycleTime) - lastQueryCycle;  // Ciclo actual - último consultado
  lastQueryCycle += ret;
  
  return (ret);
}

// Consulta el número de ms ocurridos desde el comienzo
u32 JTimer::TotalLap()
{
  if (!pause)
    cur = SDL_GetTicks();
  
  return (cur - start);
}

// Espera a que se complete el ciclo
void JTimer::WaitCycle()
{
  u32 val;

  if (!pause)
  {
    do
    {
      cur = SDL_GetTicks();
      val = (cur - start) / cycleTime;
    }
    while (!(val - lastQueryCycle));

    lastQueryCycle = val;
  }
}

/** Devuelve el nº de segundos de la época (00:00h del 01/01/1970). */
time_t JTimer::CurS()
{
  return time(0);
}

/** Devuelve el nº de milisegundos del segundo actual. */
u16 JTimer::CurMs()
{
#ifdef WIN32
  timeb t;
  ftime(&t);
  return t.millitm;
#else
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_usec/1000;
#endif // WIN32
}

/** Devuelve el timestamp como cadena de texto. */
const s8 * JTimer::StrTime()
{
  time_t t;
  time(&t);
  return (ctime(&t));
}
