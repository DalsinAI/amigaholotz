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

/** Defines a chaser enemy.
 * @file    HCEnemyChaser.h
 * @author  Juan Carlos Seijo Pérez
 * @date    27/05/2004
 * @version 0.0.1 - 27/05/2004 - First version.
 */

#ifndef _HCENEMYCHASER_INCLUDED
#define _HCENEMYCHASER_INCLUDED

#include <HCEnemy.h>
#include <HCLevel.h>
#include <vector>

#define HCCHASECELL_CLOSED    HCCA_UP | HCCA_DOWN | HCCA_LEFT | HCCA_RIGHT

/** Chase path element.
 */
struct HCChasePath
{
	s32 row;                              /**< Row of the cell. */
	s32 col;                              /**< Column of the cell. */
	u32 action;                           /**< Action to perform in this cell. */
};

/** Cell for chase calculations.
 */
struct HCChaseCell
{
	HCChasePath path;                     /**< Path info. */
	u32 tried;                            /**< Tried actions. */
	bool visited;                         /**< Flag controlling whether it belongs to the current path or not. */
	s32 numJumpedRows;                    /**< Number of rows jumped. */
	s32 depth;                            /**< Search depth reached. */
	HCCell *cur;                          /**< This chase cell into the map. */
	HCChaseCell *prev;                    /**< Previous cell in the search path. */
};

/** Enemy with the logic of a chaser. Goes everywhere 
 * (even jumping or sliding) to reach Ybelle.
 */
class HCEnemyChaser : public HCEnemy
{
	HCCharacter *chased;                  /**< Chased character. */
	HCChaseCell **cells;                  /**< Matrix of chasing cells. */

	HCChasePath *chasePath;               /**< Chase path. */
	u32 chasePathNumCells;                /**< Number of cells in the chase path. */
	s32 chaseIndex;                       /**< Index within the search path. */

	HCChasePath *tmpPath;                 /**< Temporary path. */
	u32 tmpPathNumCells;                  /**< Number of cells in the temporary path. */

	s32 searchDepth;                      /**< Maximum number of chase steps. */
	s32 finalRow;                         /**< Final row to reach. */
	s32 finalCol;                         /**< Final column to reach. */
	s32 nextRow;                          /**< Next row to reach. */
	s32 nextCol;                          /**< Next column to reach. */

	/** Resets chase cells for a new search.
	 */
	void ResetChaseMap();

	/** Goes to the given cell. The cell must be in the same row or in the same column.
	 * @param  row Row to reach.
	 * @param  col Column to reach.
	 * @return 0 if it's reachable, 1 if reached, -1 if not reachable.
	 */
	s32 GoToSimple(s32 row, s32 col);

	/** Goes to the given cell using GoToSimple() sequences.
	 * @return 0 if going, 1 if reached, 2 before leaving it (at the edge), -1 if not reachable.
	 */
	s32 GoTo(s32 row, s32 col);
	
 public:
	/** Creates a chaser enemy. 
	 */
	HCEnemyChaser();

	/** Initializes this enemy.
	 * @param  sprites Sprites for this enemy.
	 * @param  _map Map for this enemy.
	 * @param  _ropes Ropes this enemy must check.
	 * @param  nRopes Number of ropes this enemy must check.
	 * @return <b>true</b> if everything goes well, <b>false</b> otherwise.
	 */
	virtual bool Init(JImageSprite *sprites, HCMap *_map, HCRope **_ropes = 0, s32 nRopes = 0);

	/** Updates the chase.
	 * @return Return value of the associated drawable's update.
	 */
	s32 UpdateChase();

	/** Updates the character.
	 * @return Return value of the associated drawable's update.
	 */
	virtual s32 Update();

	/** Makes this enemy chase the given character.
	 * @param  character Character to chase, 0 for none
	 */
	void Chase(HCCharacter *character) {chased = character;}
	
	/** Checks whether the temporary path is best than the actual path and updates it if so.
	 */
	void CheckBestPath();

	/** Destroys the object and allows scalar destruction.
	 */
	void Destroy();

	/** Destroys the object and allows scalar destruction.
	 */
	virtual ~HCEnemyChaser() {Destroy();}
};

#endif // _HCENEMYCHASER_INCLUDED
