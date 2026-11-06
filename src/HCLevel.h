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

/** Game level. Includes a map and its enemies.
 * @file    HCLevel.h
 * @author  Juan Carlos Seijo Pérez
 * @date    29/05/2004
 * @version 0.0.1 - 29/05/2004 - First version
 */

#ifndef _HCLEVEL_INCLUDED
#define _HCLEVEL_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Sound/JChunk.h>
#include <JLib/Graphics/JDrawable.h>
#include <HCMap.h>
#include <HCRope.h>
#include <HCObject.h>
#include <HCCharacter.h>
#include <HCEnemyBall.h>
#include <HCEnemyStatic.h>
#include <HCEnemyRandom.h>
#include <HCEnemyMaker.h>
#include <HCEnemyChaser.h>
#include <HCText.h>
#include <HCExit.h>
#include <HCTimer.h>
#include <HCPreferences.h>

class HCLevel : public JDrawable
{
	// Allows the editor to modify it at pleasure.
	friend class HCed;
	
 protected:
	HCMap map;                                      /**< Level map. */
	HCTheme theme;                                  /**< Level theme. */
	HCExit levelExit;                               /**< Level exit. */
	JImage bg;                                      /**< Level background. */
	
	HCCharacter character;                          /**< Main character. */
	
	s32 numEnemies;                                 /**< Number of enemies. */
	HCEnemy **enemies;                              /**< Array of enemies. */
	
	s32 numRopes;                                   /**< Number of ropes. */
	HCRope **ropes;                                 /**< Array of ropes. */
	
	s32 remainingObjects;                           /**< Number of objects remaining to unlock the exit. */
	s32 numObjects;                                 /**< Number of objects. */
	HCObject **objects;                             /**< Array of objects. */
	
	bool scripted;                                  /**< Determines whether it has a script or not. */
	HCText *narrative;                              /**< Narrative to be shown. */

	bool wideMap;                                   /**< Indicates wherther the map is wider than the movement window or not. */
	bool highMap;                                   /**< Indicates wherther the map is higher than the movement window or not. */
	
	JVector spot;                                   /**< Viewing spot. The level tries always to make it visible. */
	s32 viewRadius;                                 /**< Radius of the viewing spot. */
	
	s32 maxTime;                                    /**< Maximum time to complete the level. */
	HCTimer levelTimer;                             /**< Level timer. */
	JFont *timerFont;                               /**< Level timer font. */
	
	JChunk soundObjectAcquired;                     /**< Sound for object acquired. */
	JChunk musicEndLevel;                           /**< End level screen music. */
	JChunk soundExitUnlocked;                       /**< Exit unlocked sound. */

 public:
	/** Creates an empty level. One must Load() or call Init and manipulate 
	 * directly it (if editing).
	 */
	HCLevel();

	/** Initializes the level. Creates an empty background for it.
	 * @return <b>true</b> if successful, <b>false</b> otherwise.
	 */
	bool Init();

	/** Updates the level.
	 * @return 0 if no important event took place, 1 if player died, 2 if player went through the exit.
	 */
	s32 Update();

	/** Draws the level.
	 */
	void Draw();

	/** Process game input.
	 * @param  input Action mask.
	 */
	void ProcessInput(u32 input) {character.Act(input);}

	/** Loads the level. 
	 * @param file File opened and positioned already.
   * @param filename name of the file opened.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Load(JRW &file, const char *filename);

	/** Saves the level. The file must be opened and positioned already.
	 * @param file File opened and positioned already.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Save(JRW &file);

	/** Returns this level's map.
	 * @return Map.
	 */
	HCMap & Map() {return map;}

	/** Returns the number of enemies.
	 * @return Number of enemies.
	 */
	s32 NumEnemies() {return numEnemies;}

	/** Returns the number of ropes.
	 * @return Number of ropes.
	 */
	s32 NumRopes() {return numRopes;}

	/** Returns the number of objects.
	 * @return Number of objects.
	 */
	s32 NumObjects() {return numObjects;}

	/** Returns the main character.
	 * @return Main character.
	 */
	HCCharacter *Character() {return &character;}

	/** Returns the enemies.
	 * @return enemies.
	 */
	HCEnemy ** Enemies() {return enemies;}

	/** Returns the ropes.
	 * @return ropes.
	 */
	HCRope ** Ropes() {return ropes;}

	/** Returns the objects.
	 * @return objects.
	 */
	HCObject ** Objects() {return objects;}

	/** Returns the current narrative object.
	 * @return current narrative object.
	 */
	HCText *Narrative() {return narrative;}
	
	/** Sets the current narrative object.
	 * @param Current narrative object to be shown.
	 */
	void Narrative(HCText *newNarrative) {narrative = newNarrative;}
	
	/** Returns the exit object.
	 * @return exit object.
	 */
	HCExit *LevelExit() {return &levelExit;}
	
	/** Returns the theme.
	 * @return This level's theme.
	 */
	HCTheme & Theme() {return theme;}

	/** Positions this level. The background is centered respect to the map.
	 * @param  xPos New x coordinate of either the background or the map, the widest.
	 * @param  yPos New y coordinate of either the background or the map, the highest.
	 */
	virtual void Pos(float xPos, float yPos);

	/** Checks if the character is over any object.
	 */
	void CheckObjects();
	
	/** Updates the number of objects to be acquired to end the level.
	 */
	void ObjectAcquired(HCObject *object);

	/** Returns whether exit is unlocked or not. Unlocked, means that all
	 * the objects had been collected from the map.
	 * @return <b>true</b> if the exit is unlocked and the character can go
	 * to the next level, <b>false</b> if not.
	 */
	bool IsExitUnlocked() {return remainingObjects == 0;}

	/** Sets the scripted flag.
	 * @param  newState New value for the flag.
	 */
	void Scripted(bool newState) {scripted = newState;}

	/** Gets the scripted flag.
	 * @return Whether it has an script or not.
	 */
	bool Scripted() {return scripted;}

	/** Tests if the given characters overlap or not.
	 * @param  c1 First character to test.
	 * @param  c2 Second character to test.
	 * @return Whether they overlap or not.
	 */
	bool Collide(HCCharacter *c1, HCCharacter *c2);

	/** Sets the font to render the timer.
	 * @param  f Font to use to render the timer.
	 */
	void SetTimerFont(JFont *f) {timerFont = f;}

	/** Destroys the level, frees resources.
	 */
	void Destroy();

	/** Pauses or continues the level timer.
	 */
	void Pause(bool pause) {pause ? levelTimer.Pause() : levelTimer.Continue();}

	/** Starts the timer.
	 */
	void Start() {levelTimer.Start();}

	/** Destroys the level, frees resources, allows scalar destruction.
	 */
	virtual ~HCLevel() {Destroy();}
};

#endif // _HCLEVEL_INCLUDED
