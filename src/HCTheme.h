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

/** Theme for Holotz's castle. Loads several resources as floors
 * breaks, ladders, bars and characters.
 * @file    HCTheme.h
 * @author  Juan Carlos Seijo Pérez
 * @date    01/06/2004
 * @version 0.0.1 - 01/06/2004 - First version.
 */

#ifndef _HCTHEME_INCLUDED
#define _HCTHEME_INCLUDED

#include <unistd.h>
#include <JLib/Util/JFile.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Graphics/JImageSprite.h>
#include <HCUtil.h>

/** Type of floor image. Used to build dinamically the map.
 */
enum HCFloorDrawType
{
	HCFDT_C = 0,                 /**< Corner with 3 neighbours. */
	HCFDT_C1,                    /**< DL corner. */
	HCFDT_C1DL,                  /**< DL corner with DL neighbours. */
	HCFDT_C3,                    /**< DR corner. */
	HCFDT_C3DR,                  /**< DR corner with DL neighbours. */
	HCFDT_C7,                    /**< UL corner. */
	HCFDT_C7UL,                  /**< UL corner with UR neighbours. */
	HCFDT_C9,                    /**< UR corner. */
	HCFDT_C9UR,                  /**< UR corner with UR neighbours. */
	HCFDT_CU,                    /**< C7 or C9 with U neighbour. */
	HCFDT_CD,                    /**< C1 or C3 with D neighbour. */
	HCFDT_CL,                    /**< C1 or C7 with L neighbour. */
	HCFDT_CR,                    /**< C3 or C9 with R neighbour. */
	HCFDT_S2,                    /**< D side. */
	HCFDT_S4,                    /**< L side. */
	HCFDT_S6,                    /**< R side. */
	HCFDT_S8,                    /**< U side. */
	HCFDT_SH,                    /**< U or D sides with U or D neighbour, respectively*/
	HCFDT_SV,                    /**< L or R sides with L or R neighbour, respectively*/
	HCFDT_I,                     /**< Inner part of the cell. */
	HCFDT_COUNT,                 /**< Number of floor part types. */
};

/** Break sprite types.
 */
enum HCBreakDrawType
{
	HCBDT_NORMAL = 0,
	HCBDT_BREAKING,
	HCBDT_BROKEN,
	HCBDT_COUNT,
};

/** Object sprite types.
 */
enum HCObjectDrawType
{
	HCODT_NORMAL = 0,
	HCODT_ACQUIRED,
	HCODT_COUNT,
};

/** Rope image types.
 */
enum HCRopeDrawType
{
	HCRDT_TOP = 0,
	HCRDT_MIDDLE,
	HCRDT_EDGE,
	HCRDT_COUNT,
};

/** Character sprite types. Must be the same as
 * HCCharacterState but is placed here to avoid circular references between
 * the files.
 */
enum HCCharacterDrawType
{
	HCCDT_STOP = 0,                       /**< Stopped state. */
	HCCDT_RIGHT,                          /**< Walk-right state. */
	HCCDT_LEFT,                           /**< Walk-left state. */
	HCCDT_UP,                             /**< Up state. */
	HCCDT_DOWN,                           /**< Down state. */
	HCCDT_SLIDE,                          /**< Slide state. */
	HCCDT_JUMP,                           /**< Jump left state. */
	HCCDT_JUMPLEFT,                       /**< Jump left state. */
	HCCDT_JUMPRIGHT,                      /**< Jump right state. */
	HCCDT_FALL,                           /**< Fall state. */
	HCCDT_DIE,                            /**< Die state. */
	HCCDT_HANG,                           /**< Hang state. */
	HCCDT_COUNT,                          /**< State count. */
};

/** Text image types.
 */
enum HCTextDrawType
{
	HCTDT_1 = 0,                          /**< Lower left corner */
	HCTDT_2,                              /**< Lower side */
	HCTDT_3,                              /**< Lower right corner */
	HCTDT_4,                              /**< Left side */
	HCTDT_5,                              /**< Center */
	HCTDT_6,                              /**< Right corner */
	HCTDT_7,                              /**< Upper left corner */
	HCTDT_8,                              /**< Up side */
	HCTDT_9,                              /**< Upper right corner */
	HCTDT_LEFT,                           /**< Lower left peak (only in dialogs) */
	HCTDT_RIGHT,                          /**< Lower right peak (only in dialogs) */
};

/** Theme for Holotz's Castle.
 */
class HCTheme
{
 protected:
	JImage *imgFloor;                     /**< Floor images. */
	s32 numFloors;                        /**< Number of Floor variants. */
	JImage **imgContFloor;                /**< Continuous floor images. */
	s32 numContFloors;                    /**< Number of Continuous Floor variants. */
	JImageSprite **sprBreak;              /**< Break sprites. */
	s32 numBreaks;                        /**< Number of Break variants. */
	JImageSprite **sprObject;             /**< Object sprites. */
	s32 numObjects;                       /**< Number of Object variants. */
	JImage *imgBar;                       /**< Bar images. */
	s32 numBars;                          /**< Number of Bar variants. */
	JImage *imgLadder;                    /**< Ladder images. */
	s32 numLadders;                       /**< Number of Ladder variants. */
	JImage **imgRope;                     /**< Rope images. */
	s32 numRopes;                         /**< Number of Rope variants. */
	JImageSprite **sprMain;               /**< Main character sprites. */
	s32 numMains;                         /**< Number of Main character variants. */
	JImageSprite **sprGuest;              /**< Guest sprites. */
	s32 numGuests;                        /**< Number of guest variants. */
	JImageSprite **sprBall;               /**< Ball enemy sprites. */
	s32 numBalls;                         /**< Number of Ball variants. */
	JImageSprite **sprRandom;             /**< Random enemy sprites. */
	s32 numRandoms;                       /**< Number of Random variants. */
	JImageSprite **sprStatic;             /**< Static enemy sprites. */
	s32 numStatics;                       /**< Number of Static variants. */
	JImageSprite **sprMaker;              /**< Maker enemy sprites. */
	s32 numMakers;                        /**< Number of Maker variants. */
	JImageSprite **sprChaser;             /**< Chaser enemy sprites. */
	s32 numChasers;                       /**< Number of Chaser variants. */
	JImage **imgDialog;                   /**< Dialog images. */
	s32 numDialogs;                       /**< Number of Dialog variants. */
	JImage **imgNarrative;                /**< Narrative images. */
	s32 numNarratives;                    /**< Number of Narrative variants. */
	
	char name[256];                       /**< Theme's name. */
 
	/** Count the number of correlative dirs starting from 1 in the cwd.
	 * @return Number of correlative dirs starting from 1 in the cwd.
	 */
	s32 CountDirs();

	/** Load floor resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadFloors();

	/** Load continuous floor resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadContFloors();

	/** Load break resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadBreaks();

	/** Load bar resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadBars();

	/** Load ladder resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadLadders();

	/** Load decorative resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadDecos();

	/** Load object resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadObjects();

	/** Load rope resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadRopes();

	/** Load main character's resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadChar(const char *directory, JImageSprite ** &sprArr, s32 &num);

	/** Load character resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadCharacters();

	/** Load dialog resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadDialogs();

	/** Load narrative resources.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool LoadNarratives();

 public:
	/** Creates an empty theme. Load() must be called in order to use it.
	 */
	HCTheme();
	
	/** Returns the Floor image.
	 * @param  index Element to retrieve.
	 * @return &The Floor image. 
	 */
	JImage & Floor(s32 index) {return imgFloor[index];}

	/** Returns the continuous Floor images.
	 * @param  index Element to retrieve.
	 * @return &The continuous Floor images. 
	 */
	JImage * ContFloor(s32 index) {return imgContFloor[index];}

	/** Returns the Break sprites.
	 * @param  index Element to retrieve.
	 * @return &The Break sprites. 
	 */
	JImageSprite * Break(s32 index) {return sprBreak[index];}

	/** Returns the Object sprites. index 0 is the key.
	 * @param  index Element to retrieve.
	 * @return &The Object sprites. 
	 */
	JImageSprite * Object(s32 index) {return sprObject[index];}

	/** Returns the Bar image.
	 * @param  index Element to retrieve.
	 * @return &The Bar image. 
	 */
	JImage & Bar(s32 index) {return imgBar[index];}

	/** Returns the Ladder image.
	 * @param  index Element to retrieve.
	 * @return &The Ladder image. 
	 */
	JImage & Ladder(s32 index) {return imgLadder[index];}

	/** Returns the Rope images.
	 * @param  index Element to retrieve.
	 * @return &The Rope images. 
	 */
	JImage * Rope(s32 index) {return imgRope[index];}

	/** Returns the Main character sprites.
	 * @param  index Element to retrieve.
	 * @return The Main character sprites. 
	 */
	JImageSprite * MainChar(s32 index) {return sprMain[index];}

	/** Returns the Guest sprites.
	 * @param  index Element to retrieve.
	 * @return The Guest sprites. 
	 */
	JImageSprite * Guest(s32 index) {return sprGuest[index];}

	/** Returns the Ball enemy sprites.
	 * @param  index Element to retrieve.
	 * @return The Ball enemy sprites. 
	 */
	JImageSprite * Ball(s32 index) {return sprBall[index];}

	/** Returns the Random enemy sprites.
	 * @param  index Element to retrieve.
	 * @return The Random enemy sprites. 
	 */
	JImageSprite * Random(s32 index) {return sprRandom[index];}

	/** Returns the Static enemy sprites.
	 * @param  index Element to retrieve.
	 * @return The Static enemy sprites. 
	 */
	JImageSprite * Static(s32 index) {return sprStatic[index];}

	/** Returns the Maker enemy sprites.
	 * @param  index Element to retrieve.
	 * @return The Maker enemy sprites. 
	 */
	JImageSprite * Maker(s32 index) {return sprMaker[index];}

	/** Returns the Chaser enemy sprites.
	 * @param  index Element to retrieve.
	 * @return The Chaser enemy sprites. 
	 */
	JImageSprite * Chaser(s32 index) {return sprChaser[index];}

	/** Returns the Dialog image.
	 * @param  index Element to retrieve.
	 * @return &The Dialog image. 
	 */
	JImage * Dialog(s32 index) {return imgDialog[index];}

	/** Returns the Narrative image.
	 * @param  index Element to retrieve.
	 * @return &The Narrative image. 
	 */
	JImage * Narrative(s32 index) {return imgNarrative[index];}

	/** Loads the theme. The name of the theme is the name of the directory
	 * containing it. The structure is:
	 * <pre>
	 * themeName1
	 *  `-  1
	 *    `- contfloor - Floor cell parts (corner, sides, interiors)
   *      `  1       
   *      ` [2]
	 *      ` ...
	 *    `- break     - Break cell sprites
   *      `  1
   *      ` [2]
	 *      ` ...
	 *    `- ladder    - Ladder cell image
   *      `  1
   *      ` [2]
	 *      ` ...
	 *    `- bar       - Bar cell image
   *      `  1
   *      ` [2]
	 *      ` ...
	 *    `- object    - Objects (keys, powerups, etc.)
   *      `  1
   *      ` [2]
	 *      ` ...
	 *    `- deco      - Decorative images/effects
   *      `  1
   *      ` [2]
	 *      ` ...
	 *    `- rope      - Rope images (edge, top, middle)
   *      `  1
   *      ` [2]
	 *      ` ...
	 *    `- char      - Sprites for characters
	 *       `- main     - Sprites of main character
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- ball     - Sprites of ball-like enemies
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- random   - Sprites of random movement enemies
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- static   - Sprites of static enemies
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- maker    - Sprites of maker enemies
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- chaser   - Sprites of chaser enemies
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- char1    - Sprites of spare character 1
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- char2    - Sprites of spare character 2
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- char3    - Sprites of spare character 3
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- char4    - Sprites of spare character 4
   *         `  1
   *         ` [2]
	 *         ` ...
	 *       `- char5    - Sprites of spare character 5
   *         `  1
   *         ` [2]
	 *         ` ...
	 *  `- [themeName2]
	 *  `- ...
	 *
	 * </pre>
	 * @param themeName This theme's name.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool Load(const char *themeName);

	/** Returns the theme name (directory containing it).
	 * @return Theme name (directory containing it).
	 */
	const char * Name() {return name;}

	/** Destroys the theme. Frees resources.
	 */
	void Destroy();

	/** Number of Floor variants. 
	 * @return The requested quantity.
	 */
	s32 NumFloors() {return numFloors;}

	/** Number of Continuous Floor variants. 
	 * @return The requested quantity.
	 */
	s32 NumContFloors() {return numContFloors;}

	/** Number of Break variants. 
	 * @return The requested quantity.
	 */
	s32 NumBreaks() {return numBreaks;}

	/** Number of Object variants. 
	 * @return The requested quantity.
	 */
	s32 NumObjects() {return numObjects;}

	/** Number of Bar variants. 
	 * @return The requested quantity.
	 */
	s32 NumBars() {return numBars;}

	/** Number of Ladder variants. 
	 * @return The requested quantity.
	 */
	s32 NumLadders() {return numLadders;}

	/** Number of Rope variants. 
	 * @return The requested quantity.
	 */
	s32 NumRopes() {return numRopes;}

	/** Number of MainChar variants. 
	 * @return Number of MainChar variants. 
	 */
	s32 NumMainChars() {return numMains;}

	/** Number of Guest variants. 
	 * @return number of Guest variants. 
	 */
	s32 NumGuests() {return numGuests;}

	/** Number of Ball variants. 
	 * @return number of Ball variants. 
	 */
	s32 NumBalls() {return numBalls;}

	/** Number of Random variants. 
	 * @return number of Random variants. 
	 */
	s32 NumRandoms() {return numRandoms;}

	/** Number of Static variants. 
	 * @return number of Static variants. 
	 */
	s32 NumStatics() {return numStatics;}

	/** Number of Maker variants. 
	 * @return number of Maker variants. 
	 */
	s32 NumMakers() {return numMakers;}

	/** Number of Chaser variants. 
	 * @return number of Chaser variants. 
	 */
	s32 NumChasers() {return numChasers;}

	/** Number of Dialog variants. 
	 * @return number of Dialog variants. 
	 */
	s32 NumDialogs() {return numDialogs;}

	/** Number of Narrative variants. 
	 * @return number of Narrative variants. 
	 */
	s32 NumNarratives() {return numNarratives;}

	/** Destroys the theme. Frees resources. Allows scalar destruction.
	 */
	virtual ~HCTheme() {Destroy();}
};

#endif // _HCTHEME_INCLUDED
