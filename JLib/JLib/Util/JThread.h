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

/** Base class for threads.
 * @file    JThread.h
 * @author  Juan Carlos Seijo Pérez
 * @date    18/04/2004
 * @version 0.0.1 - 18/04/2004 - First version.
 */

#ifndef _JTHREAD_INCLUDED
#define _JTHREAD_INCLUDED

#include <JLib/Util/JTypes.h>
#include <SDL.h>
#include <SDL_thread.h>

/** Base class for threads.
 */
class JThread
{
 protected:
	bool valid;                 /**< Validity flag for this thread */
	bool paused;                /**< Pause flag of this thread */
	bool terminate;             /**< Termination flag of this thread */
	SDL_Thread *thread;         /**< SDL thread id for this thread */
	u32 id;                     /**< Thread id */

 public:
	/** Creates a new thread that will execute the given function.
	 * A method can be implemented in a child class and the be passed as the execution function
	 * if it is defined as 'static int Func(void *data)' or any other name instead of Func
	 * and using the constructor as JThread(&Func, data).
	 * A sample of the way Func could be implemented is:
	 * <pre>
	 * while (!terminate)
	 * {
	 *   while (paused) 
	 *     sleep(0);
	 *
	 *   // Code to execute
	 * }
	 * </pre>
	 * @param  func Function to execute. If zero, the thread is not launched.
	 * @param  data Additional data to pass to the function.
	 */
	JThread(int (* func)(void *), void *data = 0);

	/** Returns the thread id for this thread.
	 * @return Thread id of this thread.
	 */
	u32 GetId() {return id;}

	/** Checks if this thread must terminate.
	 * @return <b>true</b> if so, <b>false</b> if not.
	 */
	bool MustTerminate() {return terminate;}

	/** Signals this thread to end. This does not guarantee that it ends,
	 * because it depends on the implementation to use MustTerminate()
	 * as end condition.
	 */
	void Terminate() {terminate = true; paused = false; valid = false; SDL_WaitThread(thread, 0);}

	/** Checks if this thread is paused.
	 * @return <b>true</b> if so, <b>false</b> if not.
	 */
	bool Paused() {return paused;}

	/** Pauses this thread.
	 */
	void Pause() {paused = true;}

	/** Resumes this thread.
	 */
	void Continue() {paused = false;}

	/** Kills this thread. This aproximation must be avoided, is better to implement
	 * an exit mechanism in the child class as:
	 * <pre>
	 * while (!terminate)
	 * {
	 *   while (paused) 
	 *     sleep(0);
	 *
	 *   // Code to execute
	 * }
	 * </pre>
	 */
	void Kill() {SDL_KillThread(thread);}

	/** Determines if this thread is valid..
	 * @return <b>true</b> if so, <b>false</b> if not.
	 */
	bool Valid() {return valid;}
};

#endif // _JTHREAD_INCLUDED
