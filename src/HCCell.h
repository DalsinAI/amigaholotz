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

/** Cell definition file.
 * @file    HCCell.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2004
 * @version 0.0.1 - 30/04/2004 - First version.
 */

#ifndef _HCCELL_INCLUDED
#define _HCCELL_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Graphics/JImageSprite.h>

/** Cell types.
 */
typedef enum
{
  HCCELLTYPE_BLANK = 0,                  /**< Blank cell. */
  HCCELLTYPE_FLOOR,                      /**< Floor cell. */
  HCCELLTYPE_CONTFLOOR,                  /**< Continuous floor cell. */
  HCCELLTYPE_LADDER,                     /**< Ladder cell. */
  HCCELLTYPE_BAR,                        /**< Firefighter bar cell. */
  HCCELLTYPE_BREAK,                      /**< Breakable floor cell. */
} HCCellType;

/** Actions allowed in a cell.
 */
typedef enum
{
  HCACTION_UP = 1,                       /**< Go up. */
  HCACTION_DOWN = 2,                     /**< Go down. */
  HCACTION_LEFT = 4,                     /**< Go left. */
  HCACTION_RIGHT = 8,                    /**< Go right. */
  HCACTION_FALL = 16,                    /**< Fall. */
  HCACTION_SLIDE = 32,                   /**< Slide. */
} HCAction;

/** Generic 2D blank cell.
 */
class HCCell : public JDrawable
{
 protected:
  HCCellType type;                       /**< Type of this cell. */
  s32 subtype;                           /**< Subtype of this cell. */
  u32 actionMask;                        /**< Mask of allowed actions within this cell. */

 public:
  /** Creates an empty cell of type HCCELLTYPE_BLANK by default and
   * movement to the sides, down and falling as actions allowed.
   */
  HCCell(HCCellType t = HCCELLTYPE_BLANK, u32 actions = HCACTION_FALL | HCACTION_LEFT | HCACTION_RIGHT) 
	: type(t), subtype(0), actionMask(actions)
  {}

  /** Returns the mask of allowed actions for this cell.
   * @return the mask of allowed actions for this cell.
   */
  u32 Actions()
  {
    return actionMask;
  }

  /** Enables the specified actions over this cell.
   * @param  actions OR'ed combination of the actions to allow for this cell.
   */
  void Enable(u32 actions)
  {
    actionMask |= actions;
  }

  /**Disables the specified actions over this cell.
   * @param  actions OR'ed combination of the actions to allow for this cell.
   */
  void Disable(u32 actions)
  {
    actionMask &= ~(actions);
  }

  /** Gets the type of this cell.
   * @return Type of this cell.
   */
  const HCCellType & Type() {return type;}

  /** Sets the type of this cell.
   * @param newType New type of this cell.
   */
  void Type(const HCCellType &newType) {type = newType;}

  /** Gets the subtype of this cell.
   * @return Subtype of this cell.
   */
  s32 Subtype() {return subtype;}

  /** Sets the subtype of this cell.
   * @param newSubtype New type of this cell.
   */
  void Subtype(s32 newSubtype) {subtype = newSubtype;}

  /** Destroys the object.
   */
  virtual ~HCCell()
  {}
};

class HCDrawableCell : public HCCell
{
 protected:
	JImage drawable;                      /**< Graphical representation of the cell. */

 public:
	/** Creates the cell.
	 */
	HCDrawableCell(HCCellType t, JImage *d, u32 actions = HCACTION_LEFT | HCACTION_RIGHT) : HCCell(t, actions)
	{
		if (d)
			drawable.Ref(*d);
	}

  /** Draws the cell.
   */
  virtual void Draw() {drawable.Draw();}
	
	/** Positions this cell.
	 * @param  xPos New x coordinate.
	 * @param  yPos New y coordinate.
	 */
	virtual void Pos(float xPos, float yPos) 
	{pos.x = xPos; pos.y = yPos; drawable.Pos(xPos, yPos);}

	/** Destroys the cell.
	 */
	virtual ~HCDrawableCell()
	{}
};

/** Represents a simple floor or wall
 */
class HCFloorCell : public HCDrawableCell
{
 public:
	HCFloorCell(JImage *d, u32 actions = 0) : HCDrawableCell(HCCELLTYPE_FLOOR, d, actions)
	{}

	virtual ~HCFloorCell() {}
};

/** Represents a bar
 */
class HCBarCell : public HCDrawableCell
{
 public:
	HCBarCell(JImage *d, u32 actions = HCACTION_SLIDE | HCACTION_DOWN | HCACTION_LEFT | HCACTION_RIGHT) : HCDrawableCell(HCCELLTYPE_BAR, d, actions)
	{}

	virtual ~HCBarCell() {}
};

/** Represents a ladder
 */
class HCLadderCell : public HCDrawableCell
{
 public:
	HCLadderCell(JImage *d, u32 actions = HCACTION_UP | HCACTION_DOWN | HCACTION_LEFT | HCACTION_RIGHT) : HCDrawableCell(HCCELLTYPE_LADDER, d, actions)
	{}

	virtual ~HCLadderCell() {}
};

#endif // _HCCELL_INCLUDED
