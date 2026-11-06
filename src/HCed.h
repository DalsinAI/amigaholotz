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

/** Level editor for Holotz's Castle.
 * @file    HCed.h
 * @author  Juan Carlos Seijo Pérez
 * @date    30/05/2004
 * @version 0.0.1 - 30/05/2004 - First version
 */

#ifndef _HCED_INCLUDED
#define _HCED_INCLUDED

#include <JLib/Util/JApp.h>
#include <JLib/Graphics/JFont.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Graphics/JTextMenu.h>
#include <JLib/Graphics/JImageMenu.h>
#include <HCUtil.h>
#include <HCLevel.h>
#include <HCEnemyBall.h>
#include <HCEnemyStatic.h>
#include <HCEnemyRandom.h>
#include <HCEnemyChaser.h>
#include <HCEnemyMaker.h>
#include <HCPreferences.h>
#include <list>

#define HCED_MARGIN             48      /**< Margin for menus. */
#define HCED_REFAPPWIDTH       800      /**< Referential app width, overridable with -f option of JApp. */
#define HCED_REFAPPHEIGHT      600      /**< Referential app height, overridable with -f option of JApp. */
#define HCED_INPUT_STORY       1        /**< Input new story name. */
#define HCED_INPUT_THEME       2        /**< Input theme. */

/** State of the application.
 */
enum HCedState
{
	HCEDSTATE_FLOOR = 0,
	HCEDSTATE_CONTFLOOR,
	HCEDSTATE_LADDER,
	HCEDSTATE_BAR,
	HCEDSTATE_BREAK,
	HCEDSTATE_OBJECT,
	HCEDSTATE_ROPE,
	HCEDSTATE_START,
	HCEDSTATE_EXIT,
	HCEDSTATE_ENEMY,
	HCEDSTATE_SAVE,
	HCEDSTATE_OPENSTORY,
	HCEDSTATE_APPEXIT,
	HCEDSTATE_COUNT,
};

/** Level editor for Holotz's Castle.
 */
class HCed : public JApp
{
	HCedState state;                        /**< State of the app. */
	
	JImage imgMenu[HCEDSTATE_COUNT];        /**< Main menu images. */
	JImage imgMenuOver[HCEDSTATE_COUNT];    /**< Main menu over images. */
	JImage imgEnemy[HCENEMYTYPE_COUNT];     /**< Enemy type menu images. */
	JImage imgEnemyOver[HCENEMYTYPE_COUNT]; /**< Enemy type menu images. */
	JImage imgCredits;                      /**< Credits image. */

	JImage * imgMouse;                      /**< Current state image. */

	// Application menus
	JImageMenu *menuMain;                   /**< Application menu. */
	JImageMenu *menuEnemyType;              /**< Enemy type menu. */
	JTextMenu *menuEnemyParam1;             /**< Enemy param 1 menu. */
	JTextMenu *menuEnemyParam2;             /**< Enemy param 2 menu. */
	JTextMenu *menuRopePeriod;              /**< Rope period menu. */
	JTextMenu *menuOpenStory;               /**< Open story menu. */
	JTextMenu *menuTheme;                   /**< Theme menu. */
	s32 inputNewStory;                      /**< New story input flag 0 no input, 1 story name, 2 theme select. */
	JString strNewStoryName;                /**< New story input string. */
	JImage *imgNewStory;                    /**< New story image. */
	JString storyDir;                       /**< Stories directory. */
	
	// These application menus depend on the level
	// 'Types' are applied in different ways depending on the action
	// 'Subtypes' define the representation (in dirs 1, 2, etc.)
	JImageMenu *menuMainSubtype;            /**< Main character subtype menu. */
	JImageMenu *menuBreakSubtype;           /**< Break subtype menu. */
	JImageMenu *menuObjectSubtype;          /**< Object subtype menu. */
	JImageMenu *menuEnemySubtype[HCENEMYTYPE_COUNT]; /**< Enemy subtype menus. */
	JImageMenu *menuFloorSubtype;           /**< Floor subtype menu. */
	JImageMenu *menuContFloorSubtype;       /**< Continuous floor subtype menu. */
	JImageMenu *menuBarSubtype;             /**< Bar subtype menu. */
	JImageMenu *menuLadderSubtype;          /**< Ladder subtype menu. */
	JImageMenu *menuRopeSubtype;            /**< Rope subtype menu. */
	u32 appBackColor;                       /**< Application back color. */

	JImage imgBackground;                   /**< Background image. */

	HCMap map;                              /**< Holotz's castle map. */
	HCLevel level;                          /**< Holotz's castle level. */

	JString storyName;                      /**< Name of the story to load/save */
	JString filename;                       /**< Full-path name of the file to load/save */
	JString themeName;                      /**< Theme name */
	s32 defRows;                            /**< Default number of rows. */
	s32 defCols;                            /**< Default number of columns. */
	s32 levelNumber;                        /**< Level number. */

	HCEnemyType enemyType;                  /**< Current enemy type. */
	s32 enemySubtype;                       /**< Current enemy subtype. */
	s32 enemyParam1;                        /**< Current enemy param 1. */
	s32 enemyParam2;                        /**< Current enemy param 2. */
	s32 floorSubtype;                       /**< Current floor subtype. */
	s32 contFloorSubtype;                   /**< Current continuous floor subtype. */
	s32 breakSubtype;                       /**< Current break subtype. */
	s32 barSubtype;                         /**< Current bar subtype. */
	s32 ladderSubtype;                      /**< Current ladder subtype. */
	s32 objectSubtype;                      /**< Current object subtype. */
	s32 ropeSubtype;                        /**< Current rope subtype. */
	float ropePeriod;                       /**< Current rope period. */

	HCObject *curObject;                    /**< Current edited object. */
	HCRope *curRope;                        /**< Current edited rope. */
	HCEnemy *curEnemy;                      /**< Current edited enemy. */

	JFont fontSmall;                        /**< Small font. */
	JFont fontNormal;                       /**< Normal font. */
	JFont fontLarge;                        /**< Large font. */
	
	JImage *imgGravity;                     /**< Map's gravity image. */
	JImage *imgCharVx;                      /**< Main character's x veloccity. */
	JImage *imgCharVy;                      /**< Main character's y veloccity. */
	JImage *imgCharJumpRows;                /**< Main character's maximum jump rows. */
	JImage *imgMapSize;                     /**< Map size image. */

 public:
	/** Creates the editor. Init() must be called in order to use the object.
	 */
	HCed();

	/** Initializes the application.
	 * @param  argc Argument count from the command line, as in main().
	 * @param  argv Argument list from the command line, as in main().
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool Init(int argc, char **argv);

	/** Initializes a default level.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitDefaultLevel();

	/** Initializes a loaded level.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitLoadedLevel();

	/** Initializes the level dependant menus.
	 * @return 0 if initialization succeeded, -1 otherwise.
	 */
	s32 InitLevelMenus();

	/** Destroy the level dependant menus.
	 */
	void DestroyLevelMenus();

	/** Initializes the main menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitMainMenu();

	/** Initializes the main character subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitMainSubtypeMenu();

	/** Initializes the floor subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitFloorSubtypeMenu();
	
	/** Initializes the continuous floor subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitContFloorSubtypeMenu();
	
	/** Initializes the break subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitBreakSubtypeMenu();
	
	/** Initializes the bar subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitBarSubtypeMenu();
	
	/** Initializes the ladder subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitLadderSubtypeMenu();
	
	/** Initializes the object subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitObjectSubtypeMenu();
	
	/** Initializes the rope period menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitRopePeriodMenu();

	/** Initializes the rope subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitRopeSubtypeMenu();

	/** Initializes the enemy type menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitEnemyTypeMenu();

	/** Initializes the enemy subtype menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitEnemySubtypeMenu();

	/** Initializes the enemy param 1 menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitEnemyParam1Menu();

	/** Initializes the enemy param 2 menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitEnemyParam2Menu();

	/** Initializes the open story menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitOpenStoryMenu();


	/** Initializes the theme menu.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool InitThemeMenu();

	/** Destroys the main character subtype menu.
	 */
	void DestroyMainSubtypeMenu();

	/** Destroys the break subtype menu.
	 */
	void DestroyBreakSubtypeMenu();
	
	/** Destroys the object subtype menu.
	 */
	void DestroyObjectSubtypeMenu();
	
	/** Destroys the enemy subtype menu.
	 */
	void DestroyEnemySubtypeMenu();

	/** Draws the application.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
	bool Draw();

	/** Draws the application in the FLOOR state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawFloor();

	/** Draws the application in the CONTFLOOR state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawContFloor();

	/** Draws the application in the LADDER state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawLadder();

	/** Draws the application in the BAR state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawBar();

	/** Draws the application in the BREAK state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawBreak();

	/** Draws the application in the OBJECT state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawObject();

	/** Draws the application in the ROPE state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawRope();

	/** Draws the application in the START state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawStart();

	/** Draws the application in the EXIT state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawExit();

	/** Draws the application in the ENEMY state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawEnemy();

	/** Draws the application in the SAVE state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawSave();

	/** Draws the application in the OPENSTORY state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawOpenStory();

	/** Draws the application in the APPEXIT state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> otherwise.
	 */
  bool DrawAppExit();

	/** Updates the application.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
	bool Update();
	
	/** Checks for floor erasing.
	 */
	void UpdateEraseFloor();

	/** Updates the application in the FLOOR state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateFloor();

	/** Updates the application in the CONTFLOOR state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateContFloor();

	/** Updates the application in the LADDER state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateLadder();

	/** Updates the application in the BAR state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateBar();

	/** Updates the application in the BREAK state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateBreak();

	/** Updates the application in the OBJECT state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateObject();

	/** Updates the application in the ROPE state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateRope();

	/** Updates the application in the START state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateStart();

	/** Updates the application in the EXIT state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateExit();

	/** Updates the application in the ENEMY state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateEnemy();

	/** Updates the application in the SAVE state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateSave();

	/** Updates the application in the OPENSTORY state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateOpenStory();

	/** Updates the application in the APPEXIT state.
	 * @return <b>true</b> if update succeeded, <b>false</b> otherwise.
	 */
  bool UpdateAppExit();

	/** Process key down events.
	 * @param  key Key released.
	 */
	static void OnKeyDown(SDL_keysym key);
 
	/** Process key up events.
	 * @param  key Key released.
	 */
	static void OnKeyUp(SDL_keysym key);
 
	/** Process mouse up events.
	 * @param  bt Released button mask.
	 * @param  x X position of the release.
	 * @param  y Y position of the release.
	 */
	static void OnMouseUp(s32 bt, s32 x, s32 y);

	/** Process mouse down events.
	 * @param  bt Pressed button mask.
	 * @param  x X position of the press.
	 * @param  y Y position of the press.
	 */
	static void OnMouseDown(s32 bt, s32 x, s32 y);

  /** Updates the window title.
   */
	void OnFilenameChange();

  /** Updates the gravity image.
   */
	static void OnGravityChange();

  /** Updates the character' Vx image.
   */
	static void OnCharVxChange();

  /** Updates the character' Vy image.
   */
	static void OnCharVyChange();

  /** Updates the character' jump rows image.
   */
	static void OnCharJumpRowsChange();

  /** Updates the map size image.
   */
	static void OnMapSizeChange();

  /** Floor action.
   * @param  data Additional data.
   */
	static void OnFloor(void *data);

  /** Continuous floor action.
   * @param  data Additional data.
   */
	static void OnContFloor(void *data);

  /** Ladder action.
   * @param  data Additional data.
   */
	static void OnLadder(void *data);

  /** Bar action.
   * @param  data Additional data.
   */
	static void OnBar(void *data);

  /** Break action.
   * @param  data Additional data.
   */
	static void OnBreak(void *data);

  /** Object action.
   * @param  data Additional data.
   */
	static void OnObject(void *data);

  /** Rope action.
   * @param  data Additional data.
   */
	static void OnRope(void *data);

  /** Start action.
   * @param  data Additional data.
   */
	static void OnStart(void *data);

  /** Exit action.
   * @param  data Additional data.
   */
	static void OnExit(void *data);

  /** Enemy action.
   * @param  data Additional data.
   */
	static void OnEnemy(void *data);

  /** Save action.
   * @param  data Additional data.
   */
	static void OnSave(void *data);

  /** OpenStory action.
   * @param  data Additional data.
   */
	static void OnOpenStory(void *data);

  /** Story selection action.
   * @param  data Additional data.
   */
	static void OnSelectStory(void *data);

  /** Theme selection action.
   * @param  data Additional data.
   */
	static void OnSelectTheme(void *data);

  /** AppExit action.
   * @param  data Additional data.
   */
	static void OnAppExit(void *data);

	/** Main subtype selection method.
	 * @param  data Main subtype casted.
	 */
	static void OnMainSubtype(void *data);

	/** Floor subtype selection method.
	 * @param  data Floor subtype casted.
	 */
	static void OnFloorSubtype(void *data);

	/** Continuous floor subtype selection method.
	 * @param  data Floor subtype casted.
	 */
	static void OnContFloorSubtype(void *data);

	/** Bar subtype selection method.
	 * @param  data Bar subtype casted.
	 */
	static void OnBarSubtype(void *data);

	/** Ladder subtype selection method.
	 * @param  data Ladder subtype casted.
	 */
	static void OnLadderSubtype(void *data);

	/** Break subtype selection method.
	 * @param  data Break subtype casted.
	 */
	static void OnBreakSubtype(void *data);

	/** Rope subtype selection method.
	 * @param  data Rope type casted.
	 */
	static void OnRopeSubtype(void *data);

	/** Rope period selection method.
	 * @param  data Period casted.
	 */
	static void OnRopePeriod(void *data);

	/** Object subtype selection method.
	 * @param  data Subtype casted.
	 */
	static void OnObjectSubtype(void *data);

	/** Enemy subtype selection method.
	 * @param  data Enemy type casted.
	 */
	static void OnEnemySubtype(void *data);

	/** Enemy type selection method.
	 * @param  data Enemy type casted.
	 */
	static void OnEnemyType(void *data);

	/** Enemy param 1 selection method.
	 * @param  data Enemy param 1 casted.
	 */
	static void OnEnemyParam1(void *data);

	/** Enemy param 2 selection method.
	 * @param  data Enemy param 2 casted.
	 */
	static void OnEnemyParam2(void *data);

	/** Builds the continuous floor at the specified cell. Not performs
	 * update of the surrounding continuous floors.
	 * @param row Row of the continuous floor.
	 * @param col Col of the continuous floor.
	 */
	void BuildContFloorOnce(s32 row, s32 col);

	/** Builds the continuous floor at the specified cell and updates
	 * the surrounding continuous floors.
	 * @param row Row of the continuous floor.
	 * @param col Col of the continuous floor.
	 */
	void BuildContFloor(s32 row, s32 col);
	
	/** Determines whether the mouse is over the map or not.
	 * @return  <b>true</b> if the mouse is over, <b>false</b> if not.
	 */
	inline bool MouseOverMap();

	/** Adds a new object to the level at the given position.
	 * Sets it as the current edited object.
	 */
	void AddObject(s32 x, s32 y);
	
	/** Adds a new rope to the level hanging from the given position.
	 * Sets it as the current edited rope.
	 */
	void AddRope(s32 x, s32 y);
	
	/** Adds a new enemy to the level in the given position.
	 * Sets it as the current edited enemy.
	 */
	void AddEnemy(s32 x, s32 y);
	
	/** Deletes the current object.
	 */
	void DeleteObject();
	
	/** Deletes the current rope.
	 */
	void DeleteRope();
	
	/** Deletes the current enemy.
	 */
	void DeleteEnemy();

	/** Returns the enmy corresponding to the given type and subtype
	 * @param  t Type of enemy.
	 * @param  s Subtype of enemy.
	 * @return requested sprites or 0 if they don't exist.
	 */
	JImageSprite * GetEnemySprites(HCEnemyType t, s32 s);

	/** Returns the number of enemy subtypes corresponding to the given type.
	 * @param  t Type of enemy.
	 * @return The number of subtypes.
	 */
	s32 GetNumEnemySprites(HCEnemyType t);

	/** Parses argument.
	 * @param  args Command line arguments.
	 * @return Number of parameters used.
	 */
	virtual int ParseArg(char *args[], int argc);

	/** Parses the application arguments.
	 * @param  argc Command line argument count.
	 * @param  argv Command line arguments.
	 */
	void ParseArgs(s32 argc, char **argv);

	/** Shows the usage string.
	 */
	virtual void PrintUsage(char *program);

	/** Loads the level pointed by filename. If it doesn't exist, creates it with the default values.
	 * @return 0 if it was succesfully loaded, -1 if there was an error or 1 if the current level is the first.
	 */
	s32 LoadLevel(); 

	/** Loads the next level within the story. If it doesn't exist, creates it with the default values.
	 * @return 0 if it was succesfully loaded, -1 if there was an error.
	 */
	s32 NextLevel(); 

	/** Loads the previous level within the story. If it doesn't exist, creates it with the default values.
	 * @return 0 if it was succesfully loaded, -1 if there was an error or 1 if the current level is the first.
	 */
	s32 PrevLevel(); 

	/** Loads the level indicated by the member 'filename'.
	 * @return 0 if it was succesfully loaded, -1 if there was an error.
	 */
	s32 NewLevel();

	/** Destroys the app. Allows scalar destruction.
	 */
	virtual ~HCed();
};

#endif // _HCED_INCLUDED
