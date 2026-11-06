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

/** Exit gadget for Holtz's castle game.
 * @file    HCExit.h
 * @author  Juan Carlos Seijo Pérez
 * @date    17/08/2004
 * @version 0.0.1 - 17/08/2004 - First version.
 */

#ifndef _HCEXIT_INCLUDED
#define _HCEXIT_INCLUDED

#include <JLib/Graphics/JDrawable.h>
#include <JLib/Util/JTypes.h>
#include <JLib/Util/JTimer.h>
#include <JLib/Util/JApp.h>
#include <HCCharacter.h>
#include <HCMap.h>

/** State of this exit object.
 */
typedef enum
{
	HCEXITSTATE_LOCKED = 0,               /**< The exit is closed. */
	HCEXITSTATE_UNLOCKED,                 /**< The exit is opened. */
	HCEXITSTATE_SWALLOWING,               /**< The exit is swallowing a character. */
	HCEXITSTATE_SWALLOWED                 /**< The exit has just swallowed the character. */
} HCExitState;

/** A spark.
 */
struct HCExitSpark
{
	float x0, y0, x, y;                   /**< Position of this spark. */
	float vx, vy;                         /**< Veloccity of this spark. */
	SDL_Color c;                          /**< Color of this spark. */
	u8 dc;                                /**< Color variation rate. */
};

/** Exit for a level. The character must be aligned with it to reach it.
 */
class HCExit : public JDrawable
{
 protected:
	HCMap *map;                           /**< Map for this exit. */
	HCExitState state;                    /**< Current state. */
	float x1;                             /**< X of lower right box for sparkles. */
	float y1;                             /**< Y of lower right box for sparkles. */
	JTimer timer;                         /**< Timer for FPS count. */

	HCExitSpark *sparks;                  /**< Sparks. */
	s32 numSparks;                        /**< Number of sparkles. */
	
	HCCharacter *character;               /**< Character to swallow. */
	JImage *imgCharacter;                 /**< Frame of the character being swallowed. */

 public:
	/** Creates the exit. Init must be called before starting to use it.
	 */
	HCExit();

	/** Initializes the exit.
	 * @param  _map Map for this exit.
	 * @return <b>true</b> if everything goes well, <b>false</b> otherwise.
	 */
	virtual bool Init(HCMap *_map, s32 nSparks, s32 w = 0);

	/** Draws the exit.
	 */
	virtual void Draw();

	/** Updates the exit.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 Update();

	/** Returns the state of the exit. Must use one of Lock(), Unlock() or Swallow() to change the state.
	 * @return the state of the exit.
	 */
	HCExitState State() {return state;}
	
	/** Returns the number of sparks.
	 * @return the number of sparks.
	 */
	s32 NumSparks() {return numSparks;}
	
	/** Returns the lower right x coordinate of the exit.
	 * @return the lower right x coordinate of the exit.
	 */
	float X1() {return x1;}
	
	/** Returns the lower right y coordinate of the exit.
	 * @return the lower right y coordinate of the exit.
	 */
	float Y1() {return y1;}
	
	/** Locks the exit.
	 */
	void Lock() {state = HCEXITSTATE_LOCKED;}
	
	/** Unlocks the exit.
	 */
	void Unlock();
	
	/** Swallows the character.
	 * @param  ch Character to swallow.
	 */
	void Swallow(HCCharacter *ch);
	
	/** Positions this object.
	 * @param  xPos New x coordinate.
	 * @param  yPos New y coordinate.
	 */
	virtual void Pos(float xPos, float yPos);

	
	/** Destroys this exit.
	 */
	void Destroy();
	
	/** Destroys this exit. Allows scalar destruction.
	 */
	virtual ~HCExit();
};

#endif // _HCEXIT_INCLUDED
