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

/** Defines a random enemy.
 * @file    HCEnemyRandom.h
 * @author  Juan Carlos Seijo Pérez
 * @date    27/05/2004
 * @version 0.0.1 - 27/05/2004 - First version.
 */

#ifndef _HCENEMYRANDOM_INCLUDED
#define _HCENEMYRANDOM_INCLUDED

#include <HCLevel.h>
#include <HCEnemy.h>

/** Enemy that moves pseudo-randomly. Behaves like a ball but can go up and down too.
 */
class HCEnemyRandom : public HCEnemy
{
	s32 lastRow;                          /**< Last visited row. */
	s32 lastCol;                          /**< Last visited col. */

 public:
	/** Creates an enemy random. 
	 */
	HCEnemyRandom();

	/** Updates the character.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 Update();

	/** Updates collisions with the map and modifies the state and veloccity according 
	 * to the result of the collision tests.
	 */ 
	virtual void UpdateCollisions();

	/** Destroys the object and allows scalar destruction.
	 */
	virtual ~HCEnemyRandom() {}
};

#endif // _HCENEMYRANDOM_INCLUDED
