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

/** Timer.
 * @file    JTimer.h.
 * @author  Juan Carlos Seijo Pérez.
 * @date    21/04/2003.
 * @version 0.0.1 - 21/04/2003 - First version.
 */

#ifndef _JTIMER_INCLUDED
#define _JTIMER_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>
#include <SDL.h>
#include <time.h>

#ifdef WIN32
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif // WIN32

/** Generic timer. Includes functions to count fixed periods of milliseconds (ms), time between calls, etc.
 */
class JTimer
{
protected:
	u32 start;	  	    /**< First measure (cycle 0) */
	u32 pause;	  	    /**< Pause measure (millis) */
	u32 last;	  	      /**< Last measure (millis) */
	
  u32 cycleTime;	    /**< Cycle period (millis) */

  u32 cur;				    /**< Current measure (millis) */

  u32 lastQueryCycle; /**< Cycle of the last query */

public:
	/** Creates and initializes the timer.
   * @param  millis Milliseconds per cycle.
   */
	JTimer(u32 millis = 1L);
	
	/** Starts the timer.
   * @param  millis Milliseconds per cycle (0 to use the value given at creation time, by default is zero).
   */
	void Start(u32 millis = 0L);
	
	/** Pauses this timer.
   */
	void Pause();
	
	/** Resumes this timer.
   */
	void Continue();
	
	/** Queries the number of ms since the start of the last cycle.
   * @return Number of ms since the start of the last cycle.
   */
	u32 Lap();

	/** Queries the number of ms to end the current cycle.
   * @return Number of ms to end the current cycle.
   */
	u32 Rem();

	/** Queries the time in ms between calls to Tick().
   * @return Time in ms between calls to Tick().
   */
	u32 Tick();

  /** Queries the number of cycles since Start().
   * @return Number of cycles since Start().
   */
	u32 Cycles();

  /** Queries if the cycle has changed since the last call to Lap(), Rem() or Start()..
   * @return 0 if not, different if so.
   */
  u32 Changed();

  /** Queries the number of ms since Start() was called.
   * @return Number of ms since Start() was called.
   */
  u32 TotalLap();

  /** Waits for the cycle to complete.
   */
  void WaitCycle();

  /** Returns the number of seconds since the epoch (00:00h, 01/01/1970).
   * @return Number of seconds since the epoch (00:00h, 01/01/1970).
   */
  static time_t CurS();
  
  /** Returns the number of milliseconds since the current second started.
   * @return Number of milliseconds since the current second started.
   */
  static u16 CurMs();

  /** Returns the timestamp as a string.
   * @return Timestamp as a string.
   */
  static const s8 * StrTime();

	/** Returns the cycle period in ms.
	 * @return Cycle period in ms.
	 */
	u32 CycleTime() {return cycleTime;}
};

#endif // _JTIMER_INCLUDED
