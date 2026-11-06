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

/** Holotz's Castle character definition file.
 * @file    HCCharacter.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2004
 * @version 0.0.1 - 30/04/2004 - First version.
 */

#ifndef _HCCHARACTER_INCLUDED
#define _HCCHARACTER_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JFile.h>
#include <JLib/Graphics/JImageSprite.h>
#include <HCTheme.h>
#include <HCMap.h>
#include <HCRope.h>
#include <HCText.h>
#include <HCPreferences.h>

class HCed;

enum HCCharacterState
{
	HCCS_STOP = 0,                        /**< Stopped state. */
	HCCS_RIGHT,                           /**< Walk-right state. */
	HCCS_LEFT,                            /**< Walk-left state. */
	HCCS_UP,                              /**< Up state. */
	HCCS_DOWN,                            /**< Down state. */
	HCCS_SLIDE,                           /**< Slide state. */
	HCCS_JUMP,                            /**< Jump left state. */
	HCCS_JUMPLEFT,                        /**< Jump left state. */
	HCCS_JUMPRIGHT,                       /**< Jump right state. */
	HCCS_FALL,                            /**< Fall state. */
	HCCS_DIE,                             /**< Die state. */
	HCCS_HANG,                            /**< Hang state. */
	HCCS_COUNT,                           /**< State count. */
};

// Requested actions for the character
#define HCCA_STOP   0x00
#define HCCA_UP     0x01
#define HCCA_DOWN   0x02
#define HCCA_LEFT   0x04
#define HCCA_RIGHT  0x08
#define HCCA_JUMP   0x10

class HCCharacter : public JDrawable
{
	friend class HCed;

 protected:
	JImageSprite states[HCCS_COUNT];      /**< Table of state drawables. */
	HCCharacterState state;               /**< Current state of the character. */
	u32 lastAction;                       /**< Last action of the character. */
	u32 actions;                          /**< Actions requested for the character. */
	HCMap *map;                           /**< Map for this character. */
	JVector v;                            /**< Veloccity. */
	JVector a;                            /**< Acceleration. */
	JVector vMax;                         /**< Maximum veloccity. */
	JVector aMax;                         /**< Maximum acceleration. */
	JVector vJumpMax;                     /**< Maximum veloccity for jumps. */
	JTimer timer;                         /**< Timer for jumps. */

	s32 startJumpRow;                     /**< Row at where jump took place. */
	s32 maxFallRows;                      /**< Max. number of cells to free fall if not reached floor or a rope yet. */
	s32 maxJumpRows;                      /**< Max. number of cells in vertical it can jump. */
	s32 maxJumpCols;                      /**< Max. number of cells in horizontal it can jump. */
	s32 breakRow;                         /**< Row starting break floor, -1 if none. */
	s32 breakCol;                         /**< Column starting break floor. */
	s32 hangCheckYOffset;                 /**< Offset within character for hanging cheks. */
	s32 hangCheckSize;                    /**< Size of the square side centered at the offset for hanging cheks. */

	JString name;                         /**< Name of the character. */
	s32 subtype;                          /**< Subtype within the theme. */

	HCRope **ropes;                       /**< Ropes to check. */
	s32 numRopes;                         /**< Number of ropes to check. */
	HCRope *trackRope;                    /**< Rope to track when hanging. */

	HCText *dialog;                       /**< Dialog to say. */

 public:
	
	/** Creates the character. Init must be called before starting to use it.
	 */
	HCCharacter();

	/** Initializes the character.
	 * @param  sprites Sprites for this character.
	 * @param  _map Map for this character.
	 * @param  _ropes Ropes this character must check.
	 * @param  nRopes Number of ropes this character must check.
	 * @return <b>true</b> if everything goes well, <b>false</b> otherwise.
	 */
	virtual bool Init(JImageSprite *sprites, HCMap *_map, HCRope **_ropes = 0, s32 nRopes = 0);

	/** Draws the character.
	 */
	virtual void Draw();

	/** Updates the character. The general character's update pipeline is:
	 * <pre>
	 * 1. Update sprite
	 * 2. If hanging, do nothing (return)
	 * 3. Update vellocity.
	 * 4. Update collisions with the map
	 * 6. Update their dialog
	 * </pre>
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 Update();

	/** Updates the veloccity of the character based upon the character's acceleration and the map's gravity.
	 */
	virtual void UpdateVeloccity();

	/** Updates collisions with the map and modifies the veloccity according to the result of the collision tests.
	 */ 
	virtual void UpdateCollisions();

	/** Updates this character's speech dialog.
	 */
	virtual void UpdateDialog();

	/** Check if the character must fall upon a broken floor and changes its state if so.
	 */
	virtual void CheckBrokenFloor();

	/** Check if the character must hang of a rope.
	 */
	virtual void CheckHang();

	/** Updates the character in the Stop state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateStop();

	/** Updates the character in the Right state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateRight();

	/** Updates the character in the Left state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateLeft();

	/** Updates the character in the Up state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateUp();

	/** Updates the character in the Down state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateDown();

	/** Updates the character in the Slide state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateSlide();

	/** Updates the character in the Jump state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateJump();

	/** Updates the character in the JumpLeft state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateJumpLeft();

	/** Updates the character in the JumpRight state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateJumpRight();

	/** Updates the character in the Fall state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateFall();

	/** Updates the character in the Die state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateDie();

	/** Updates the character in the Hang state.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 UpdateHang();

	/** Set this character's position.
	 * @param  x New X coordinate.
	 * @param  y New Y coordinate.
	 */
	virtual void Pos(float x, float y);

	/** Gets this character's position.
	 * @return Character's position.
	 */
	virtual const JVector & Pos() {return pos;}

	/** Gets the current row.
	 * @return The row this character is at.
	 */
	s32 Row();

	/** Gets the current column.
	 * @return The column this character is at.
	 */
	s32 Col();

	/** Fix the character position to the actual row and col.
	 * @param  col Column to be fixed at.
	 * @param  row Row to be fixed at.
	 */
	void FixPos(s32 col, s32 row);

	/** Fix the character's x position to the actual col.
	 * @param  col Column to be fixed at.
	 */
	void FixX(s32 col);

	/** Fix the character's y position to the actual row.
	 * @param  row Row to be fixed at.
	 */
	void FixY(s32 row);

	/** Changes the state of the character.
	 * @param  newState New state for this character.
	 */
	void State(HCCharacterState newState);

	/** Returns the state of the character.
	 * @return the state of the character.
	 */
	HCCharacterState State() {return state;}

	/** Returns the name of this character.
	 */
	const JString& Name() {return name;}

	/** Sets the name of this character. The name will be the base directory
	 * from where to load its resources.
	 */
	void Name(const JString &_name) {name = _name;}

  /** Gets the subtype of this character.
   * @return Subtype of this character.
   */
  s32 Subtype() {return subtype;}

  /** Sets the subtype of this character.
   * @param newSubtype New type of this character.
   */
  void Subtype(s32 newSubtype) {subtype = newSubtype;}

  /** Gets the dialog of this character.
   * @return Dialog of this character.
   */
  HCText * Dialog() {return dialog;}

  /** Sets the dialog for this character.
   * @param newDialog New type of this character.
   */
  void Dialog(HCText *newDialog) {dialog = newDialog;}

	/** Returns the current image sprite used.
	 * @return Current image sprite used.
	 */
	JImageSprite *CurSprite() {return &states[state];}

	/** Tries to put the character in the given state.
	 * @param  st State to try to reach.
	 * @return <b>true</b> if the state could be changed, <b>false</b> otherwise.
	 */
	bool Try(HCCharacterState st);

	/** Sets the actions this character must perform.
	 * @param  act Actions requested as a bitmask.
	 * @return <b>true</b> if the state could be changed, <b>false</b> otherwise.
	 */
	bool Act(u32 act);

	/** Returns the maximum veloccity vector for this character.
	 */
	JVector & MaxVeloccity() {return vMax;}

	/** Returns the maximum jump veloccity vector for this character.
	 */
	JVector & JumpMaxVeloccity() {return vJumpMax;}

	/** Returns the veloccity vector for this character.
	 */
	JVector & Veloccity() {return v;}

	/** Returns the acceleration vector for this character.
	 */
	JVector & Acceleration() {return a;}

	/** Returns the maximum jump rows parameter for this character.
	 */
	s32 MaxJumpRows() {return maxJumpRows;}

	/** Sets the maximum jump rows parameter for this character. Computes the
	 * maximum jump y veloccity of the character, also.
	 */
	void MaxJumpRows(s32 newMaxJumpRows);

	/** Loads the character. 
	 * @param file File opened and positioned already.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Load(JRW &file);

	/** Saves the character. The file must be opened and positioned already.
	 * @param file File opened and positioned already.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Save(JRW &file);

	/** Destroys the character.
	 */
	void Destroy();

	/** Destroys the character.
	 */
	virtual ~HCCharacter() {Destroy();}
};

#endif // _HCCHARACTER_INCLUDED
