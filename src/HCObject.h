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

/** Object definition file.
 * @file    HCObject.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2004
 * @version 0.0.1 - 30/04/2004 - First version.
 */

#ifndef _HCOBJECT_INCLUDED
#define _HCOBJECT_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <HCTheme.h>
#include <HCCell.h>

/** Object state.
 */
typedef enum
{
  HCOBJECTSTATE_NONE = 0,               /**< Non visible state. */
  HCOBJECTSTATE_NORMAL,                 /**< Normal, visible state. */
  HCOBJECTSTATE_ACQUIRED,               /**< Acquired, visible state. */
} HCObjectState;

/** Object class. Defines an object of the game and its behaviour.
 */
class HCObject : public JDrawable
{
 protected:
  HCObjectState state;                  /**< Object state. */
  JImageSprite normal;                  /**< Normal sprite to show for the object. */
  JImageSprite acquired;                /**< Sprite to show when the object is obtained. */
	s32 subtype;                          /**< Subtype (appearance) of this object. */

 public:
  /** Creates a new object in the HCOBJECTSTATE_NORMAL state.
   */
  HCObject() : state(HCOBJECTSTATE_NORMAL), subtype(0)
  {}
  
	/** Initializes the object according to the given theme and the current subtype.
	 * @param  theme Theme to use.
	 * @return <b>true</b> if successfull, <b>false</b> otherwise.
	 */
	bool Init(HCTheme *theme);

  /** Updates the object.
   * @return 0 If the object is in HCOBJECTSTATE_NORMAL, 1 if in HCOBJECTSTATE_ACQUIRED state, 
   * -1 if in HCOBJECTSTATE_NONE state. Any of the two last mean that the object has been 
	 * acquired.
   */
  virtual s32 Update();

  /** Draws the object.
   */
  virtual void Draw();

	/** Places this object in the acquired state.
	 */
	void Acquire() {state = HCOBJECTSTATE_ACQUIRED;}

	/** Returns the state of this object.
	 * @return The state of this object.
	 */
	HCObjectState State() {return state;}

	/** Loads the object. 
	 * @param file File opened and positioned already.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Load(JRW &file);

	/** Saves the object. 
	 * @param file File opened and positioned already.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Save(JRW &file);

  /** Gets the subtype of this object.
   * @return Subtype of this object.
   */
  s32 Subtype() {return subtype;}

  /** Sets the subtype of this object.
   * @param newSubtype New type of this object.
   */
  void Subtype(s32 newSubtype) {subtype = newSubtype;}

	/** Positions this object.
	 * @param  xPos New x coordinate.
	 * @param  yPos New y coordinate.
	 */
	virtual void Pos(float xPos, float yPos) 
	{normal.Pos(xPos, yPos); acquired.Pos(xPos, yPos); pos.x = xPos; pos.y = yPos;}

	/** Gets this object's position.
	 * @return Object's position.
	 */
	virtual const JVector & Pos() {return pos;}

	/** Returns the normal sprite.
	 * @return The normal sprite.
	 */
	JImageSprite & Normal() {return normal;}

	/** Returns the acquired sprite.
	 * @return The acquired sprite.
	 */
	JImageSprite & Acquired() {return acquired;}

  /** Destroys the object.
   */
  virtual ~HCObject()
  {}
};

#endif // _HCOBJECT_INCLUDED
