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

/** Definition of a generic enemy.
 * @file    HCEnemy.h
 * @author  Juan Carlos Seijo Pérez
 * @date    27/05/2004
 * @version 0.0.1 - 27/05/2004 - First version.
 */

#ifndef _HCENEMY_INCLUDED
#define _HCENEMY_INCLUDED

#include <HCTheme.h>
#include <HCCharacter.h>

#define HCENEMY_MIN_MAX_V               1.0f

/** Type of enemy.
 */
enum HCEnemyType
{
	HCENEMYTYPE_BALL = 0,                 /**< Rebounding ball. */
	HCENEMYTYPE_RANDOM,                   /**< Random movement enemy. */
	HCENEMYTYPE_STATIC,                   /**< Static enemy. */
	HCENEMYTYPE_MAKER,                    /**< Enemy maker. */
	HCENEMYTYPE_CHASER,                   /**< Chaser enemy. */
	HCENEMYTYPE_COUNT,                    /**< Number of different enemy types. */
};

/** Encapsulates a generic enemy. An enemy is a character
 * that moves by its own.
 */
class HCEnemy : public HCCharacter
{
 protected:
	HCEnemyType type;                     /**< Type of enemy. */
	
	s32 param1;                           /**< Behaviour param 1. */
	s32 param2;                           /**< Behaviour param 2. */
	
	static float maxXVeloccity;           /**< Maximum veloccity in x direction. */
	static float maxYVeloccity;           /**< Maximum veloccity in y direction. */

 public:
	/** Creates an empty enemy.
	 */
	HCEnemy(HCEnemyType _type) : HCCharacter(), type(_type), param1(0), param2(0)
	{}

	/** Initializes this enemy.
	 * @param  sprites Sprites for this enemy.
	 * @param  _map Map for this enemy.
	 * @param  _ropes Ropes this enemy must check.
	 * @param  nRopes Number of ropes this enemy must check.
	 * @return <b>true</b> if everything goes well, <b>false</b> otherwise.
	 */
	virtual bool Init(JImageSprite *sprites, HCMap *_map, HCRope **_ropes = 0, s32 nRopes = 0);

	/** Returns the type of enemy.
	 * @return Type of enemy.
	 */
	HCEnemyType Type() {return type;}

	/** Returns the behaviour param 1.
	 * @return Behaviour param 1.
	 */
	s32 Param1() {return param1;}

	/** Returns the behaviour param 2.
	 * @return Behaviour param 2.
	 */
	s32 Param2() {return param2;}

	/** Sets the behaviour param 1.
	 * @param  newParam New behaviour param 1.
	 */
	void Param1(s32 newParam) {param1 = newParam;}

	/** Sets the behaviour param 2.
	 * @param  newParam New behaviour param 2.
	 */
	void Param2(s32 newParam) {param2 = newParam;}

	/** Updates the enemy.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 Update();
	
	/** Loads the enemy. 
	 * @param  file File opened and positioned already.
	 * @param  theme The theme to use.
	 * @param  _map The level to use.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Load(JRW &file, HCTheme &theme, HCMap *_map);

	/** Stablishes the maximum global X veloccity for an enemy.
	 * @param  mvx Maximum X veloccity for any enemy.
	 */
	static void MaxXVeloccity(float mvx) {maxXVeloccity = JMax(1.0f, mvx);}

	/** Stablishes the maximum global Y veloccity for an enemy.
	 * @param  mvy Maximum Y veloccity for any enemy.
	 */
	static void MaxYVeloccity(float mvy) {maxYVeloccity = JMax(0.1f, mvy);}

	/** Saves the enemy. The file must be opened and positioned already.
	 * @param file File opened and positioned already.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Save(JRW &file);

	/** Destroys this enemy and allows scalar destruction.
	 */
	virtual ~HCEnemy() {}
};

#endif // _HCENEMY_INCLUDED
