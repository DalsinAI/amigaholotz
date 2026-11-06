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

/** Break Floor floor definition file.
 * @file    HCBreak.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2004
 * @version 0.0.1 - 30/04/2004 - First version.
 */

#ifndef _HCBREAK_INCLUDED
#define _HCBREAK_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JImageSprite.h>
#include <HCCell.h>

/** Object state.
 */
typedef enum
{
  HCBREAKSTATE_NORMAL = 0,              /**< Normal, visible state. */
  HCBREAKSTATE_BREAKING,                /**< Breaking, visible state. */
  HCBREAKSTATE_BROKEN,                  /**< Non visible state. */
} HCBreakState;

/** Break Floor class. Defines an break floor of the game and its behaviour.
 */
class HCBreak : public HCCell
{
 protected:
  HCBreakState state;                   /**< Break Floor state. */
	JImageSprite *cur;                    /**< Current sprite for this break floor. */
  JImageSprite normal;                  /**< Normal sprite to show for the break. */
  JImageSprite breaking;                /**< Sprite to show when the floor is breaking. */
  JImageSprite broken;                  /**< Sprite to show when the floor is broken. */
	HCBreak *prev;                        /**< Previous break floor, 0 if no more. */
	HCBreak *next;                        /**< Next break floor, 0 if no more. */

 public:
  /** Creates a new break floor in the HCBREAKSTATE_NORMAL state.
   */
  HCBreak(JImageSprite *states, HCBreak *_prev = 0)
	: 
	HCCell(HCCELLTYPE_BREAK, 0), state(HCBREAKSTATE_NORMAL), cur(0), prev(_prev), next(0)
  {
		normal.Ref(states[HCBREAKSTATE_NORMAL]);
		breaking.Ref(states[HCBREAKSTATE_BREAKING]);
		broken.Ref(states[HCBREAKSTATE_BROKEN]);

		if (prev)
		{
			prev->next = this;
		}
    
    cur = &normal;
	}
  
  /** Updates the break floor.
   * @return 0 If it's in HCBREAKSTATE_NORMAL, 1 if in HCBREAKSTATE_BREAKING, 
   * -1 if in HCBREAKSTATE_NONE state, which means that the break floor has been broken.
   */
  virtual s32 Update();

  /** Draws the break floor.
   */
  virtual void Draw();

	/** Places this break floor in the breaking state.
	 */
	void Break();

	/** Positions this cell.
	 * @param  xPos New x coordinate.
	 * @param  yPos New y coordinate.
	 */
	virtual void Pos(float xPos, float yPos);

	/** Returns the state of this break floor.
	 * @return The state of this break floor.
	 */
	HCBreakState State() {return state;}

  /** Destroys the break.
   */
  virtual ~HCBreak()
  {}
};

#endif // _HCBREAK_INCLUDED
