/*
 * Holotz's Castle
 * Copyright (C) 2004 Juan Carlos Seijo Pérez
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 59 
 * Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * Juan Carlos Seijo Pérez
 * jacob@mainreactor.net
 */

/** Level timer for holotz's castle.
 * @file    HCTimer.h
 * @author  Juan Carlos Seijo Pérez
 * @date    04/10/2004
 * @version 0.0.1 -  4/10/2004 - First version.
 */

#ifndef _HCTIMER_INCLUDED
#define _HCTIMER_INCLUDED

#include <JLib/Util/JTimer.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Graphics/JFont.h>

/** Tracks the time elapsed within the level and updates the marker.
 */
class HCTimer : public JDrawable, public JTimer
{
 protected:
	JImage *img;                /**< Image to draw the time remaining to. */
	s32 maxTime;                /**< Maximum time. */
	JFont *font;                /**< Font to use. */

	/** Build the given time's image.
	 * @param  t Seconds to show.
	 * @return <b>true</b> if succedded, <b>false</b> otherwise.
	 */
	bool Build(u16 t, u8 r, u8 g, u8 b);
	
 public:
	/** Creates an empty timer.
	 */
	HCTimer() : img(0), maxTime(0), font(0) 
	{}
	
	/** Initializes the timer.
	 * @param  t Seconds to count.
	 * @param  f Font to use to render the text.
	 * @return <b>true</b> if succedded, <b>false</b> otherwise.
	 */
	bool Init(u16 t, JFont *f);
	
	/** Checks whether the time has elapsed or not.
	 * @return  Milliseconds remaining to end (note: <= 0 if finished).
	 */
	s32 TimeRemaining() {return maxTime - TotalLap();}

	/** Draws the timer.
	 */
	virtual void Draw();

	/** Updates the timer.
	 * @return 0 if not finished, -1 if so.
	 */
	virtual s32 Update();
	
	/** Destroys the associated image.
	 */
	void Destroy() {JDELETE(img);}
	
	/** Destroys the object.
	 */
	virtual ~HCTimer() {Destroy();}
};

#endif // _HCTIMER_INCLUDED
