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

/** The game of Holotz's Castle.
 * @file    HolotzCastle.h
 * @author  Juan Carlos Seijo Pérez
 * @date    27/04/2004
 * @version 0.0.1 - 27/04/2004 - First version.
 */

#ifndef _HOLOTZ_INCLUDED
#define _HOLOTZ_INCLUDED

#include <JLib/Util/JApp.h>
#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JTextMenu.h>
#include <HCLevel.h>
#include <HCScript.h>
#include <HCEnemyChaser.h>
#include <HCPreferences.h>
#include <HCPlaylist.h>
#include <HCLoadSaveSlot.h>
#include <HCUtil.h>

class HCScript;
class HCScriptAction;
class HCEnemy;
class HCEnemyChaser;

#define ACKNOWLEDGEMENTS \
"Special thanks go to:\n"\
" \n"\
"Martinica (the best beta-tester mother),\n"\
"Bebo, Ampis & Roberto & Oscar, Padre, Cris &\n"\
"Rafa & Lucía & Paula & Rafael\n"\
"Quique & Marga & Jorge & Javier,\n"\
"Carlos & Julia & Jorge & Jose\n"\
"Ana & Nathan & María & Marco,\n"\
"Jorge & Amelia & Alejandro,\n"\
"Carlos & Vero & Álvaro & Irene, Edu & Vero,\n"\
"Alex, Chily, Santos & Rocío & África, Félix &\n"\
"Carmen, Ossuna's, Jose & Gloria, Ori &\n"\
"Loli, Diana & David, Luis,\n"\
"Ángel, Celia, Íñigo,\n"\
"Markus Rath/Ronny Standtke (German translation),\n"\
"Milan Babuškov (Add-on levels),\n"\
"Miriam Ruiz (Debian package),\n"\
"Nicolas Elie (French translation),\n"\
"Вячеслав Диконов \n(Vyacheslav Dikonov - Russian translation),\n"\
"3ARRANO Euskalgintza (Euskara translation),\n"\
"Andreas Jochens (AMD64 Patches),\n"\
"Maksim Mokriyev (Ukranian translation)\n"\
"Filip (Triple T) Bártek (Czech translation)\n"\
"Pierre-Paul Lavoie (FreeBSD porting)\n"\
"Filipe Silva (Portuguese translation)\n"\
"Ben Wong (English translation, patches)\n"\
"Federico 'hs1' Tolomei (Italian translation)\n"\
"Jarno van der Kolk (Dutch translation/Joystick)\n"\
"Jason Woofenden (PPC Patches)\n"\
"Syltbullen (Swedish translation)\n"\
"Nikola Smolenski (Serbian translation)\n"\
"...and you for playing!"

/** Possible states of the game.
 */
typedef enum
{
	HCS_MENU = 1,                         /**< Showing main menu. */
	HCS_PLAYING,                          /**< Playing. */
	HCS_PAUSED,                           /**< Paused. */
	HCS_INTRO,                            /**< Intro. */
	HCS_CREDITS,                          /**< Game Credits. */
	HCS_SCRIPT,                           /**< Scripted level. */
	HCS_ENDLEVEL,                         /**< Level ends. */
	HCS_GAMEOVER,                         /**< Game over. */
	HCS_HELP,                             /**< Help. */
} HCState;

typedef enum
{
	HCCREDITS_PROGRAMMING = 0,            /**< Programming image. */
	HCCREDITS_STORY,                      /**< Story image. */
	HCCREDITS_DRAWING0,                   /**< Drawing 1st image. */
	HCCREDITS_DRAWING1,                   /**< Drawing 2nd image. */
	HCCREDITS_DRAWING2,                   /**< Drawing 3rd image. */
	HCCREDITS_ANIMATION0,                 /**< Animation 1st image. */
	HCCREDITS_ANIMATION1,                 /**< Animation 2nd image. */
	HCCREDITS_ANIMATION2,                 /**< Animation 3rd image. */
	HCCREDITS_ANIMATION3,                 /**< Animation 4th image. */
	HCCREDITS_MUSIC,                      /**< Music image. */
	HCCREDITS_HOLOTZCASTLE,               /**< Holotz's castle image. */
	HCCREDITS_LOGO,                       /**< Mainrector image. */
	HCCREDITS_IMAGE_COUNT                 /**< Number of images. */
} HCCreditsImageType;

typedef enum
{
	HCCREDITS_PROGRAMMING_TEXT = 0,       /**< Programming text. */
	HCCREDITS_STORY_TEXT,                 /**< Story text. */
	HCCREDITS_DRAWING_TEXT,               /**< Drawing 1st text. */
	HCCREDITS_ANIMATION_TEXT,             /**< Animation 1st text. */
	HCCREDITS_MUSIC_TEXT,                 /**< Music text. */
	HCCREDITS_HOLOTZCASTLE_TEXT,          /**< Holotz's castle text. */
	HCCREDITS_LOGO_TEXT,                  /**< Holotz's castle text. */
	HCCREDITS_ACK_TEXT,                   /**< Acknowledgements text. */
	HCCREDITS_TEXT_COUNT                  /**< Number of texts. */
} HCCreditsTextType;

#define HC_NUM_SLOTS            5       /**< Number of slots to save and load. */

/** Application class.
 */
class HCApp : public JApp
{
 protected:
	HCState lastState;                    /**< Last state of the game. */
	HCState state;                        /**< Current state of the game. */
	
	JImage *imgMenu;                      /**< Main title background. */
	JTextMenu *menu;                      /**< Application menu. */

	JImage *imgCredits[HCCREDITS_IMAGE_COUNT]; /**< Credits images. */
	JImage *textCredits[HCCREDITS_TEXT_COUNT]; /**< Credits texts. */
	s32 inDrawing;                             /**< Current drawing fading in. */
	s32 outDrawing;                            /**< Current drawing fading out. */
	s32 inAnimation;                           /**< Current animation fading in. */
	s32 outAnimation;                          /**< Current animation fading out. */

	HCPreferences preferences;            /**< Preferences for the game. */
	HCPlaylist playlist;                  /**< Playlist with the story order. */
	char *playlistName;                   /**< Name of the playlist. */

	JTimer timerGeneral;                  /**< Timer for general use. */
	JImage *imgIntro;                     /**< Intro image (MainReactor). */
	JImage *imgFatalFun;                  /**< Intro image (Fatalfun). */
	JImage *textIntro;                    /**< Intro text (Presents...). */

	HCLevel level;                        /**< Game level. */
	HCScript script;                      /**< Level script. */
	s32 levelNumber;                      /**< Current level within the current world. */

	JChunk musicMainTitle;                /**< Main title screen music. */
	JChunk musicCredits;                  /**< Credits screen music. */
	JChunk musicGameOver;                 /**< Game over screen music. */
	JChunk musicBeginLevel;               /**< Start-of-level music. */
	
	JFont fontSmall;                      /**< Small font. */
	JFont fontMedium;                     /**< Medium font. */
	JFont fontLarge;                      /**< Large font. */
	
	JImage *imgBack;                      /**< Image for background storing. */
	HCText *textBack;                     /**< General purpouse text message. */
	JImage *imgHelp;                      /**< Help text message. */
	
	HCLoadSaveSlot saveData[HC_NUM_SLOTS];/**< Slots for loading and saving. */

	bool stateChanged;                    /**< App state change flag */

	/** Initializes the menu state.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitMenu();
	
	/** Initializes the intro state.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitIntro();
	
	/** Initializes the paused state.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitPaused();
	
	/** Initializes the playing state.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitPlaying();
	
	/** Initializes the game over state.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitGameOver();
	
	/** Initializes the end level state.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitEndLevel();
	
	/** Initializes the credits state.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitCredits();
	
	/** Initializes the help text.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitHelp();
	
	/** Destroys the help text.
	 */
	void DestroyHelp();
	
	/** Initializes the data slot for loading and saving.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitSlots();
	
	/** Initializes the sound.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitSound();
	
	/** (Re)initializes the fonts.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> if not.
	 */
	bool InitFonts();
	
	/** Callback: App exit.
	 * @param  data App.
	 */
	static void OnExit(void *data);
	
	/** Callback: Credits.
	 * @param  data App.
	 */
	static void OnCredits(void *data);
	
	/** Callback: Difficulty level select.
	 * @param  data Value casted to s32, HCPREFERENCES_EASY or HCPREFERENCES_NORMAL or HCPREFERENCES_HARD.
	 */
	static void OnDifficulty(void *data);

	/** Callback: Sound select.
	 * @param  data Value casted to s32, 0 to turn off and 1 to turn on.
	 */
	static void OnSound(void *data);

	/** Callback: New game.
	 */
	static void OnNew(void *data);

	/** Callback: Continue game.
	 */
	static void OnContinue(void *data);

	/** Callback: Help.
	 */
	static void OnHelp(void *data);

	/** Callback: video mode select.
	 * @param  data Mode number according to preferences casted to a s32.
	 */
	static void OnVideoMode(void *data);

	/** Callback: Color depth select.
	 * @param  data BPP casted to a s32.
	 */
	static void OnBPP(void *data);

	/** Callback: Window mode select.
	 * @param  data Window mode casted to s32.
	 */
	static void OnWindowMode(void *data);

	/** Callback: Language select.
	 * @param  data Language index according to preferences casted to a s32.
	 */
	static void OnLanguage(void *data);

	/** Callback: Restore defaults.
	 * @param  data Null.
	 */
	static void OnDefaults(void *data);

	/** Callback: Load game.
	 * @param  data Slot number to load from.
	 */
	static void OnLoad(void *data);

	/** Callback: Save game.
	 * @param  data Slot number to save to.
	 */
	static void OnSave(void *data);

	/** Draws the menu.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawMenu();

	/** Draws the game.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawPlaying();

	/** Draws the game paused.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawPaused();

	/** Draws the game in the ending level state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawEndLevel();

	/** Draws the game in the game over state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawGameOver();

	/** Draws the hall of fame.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawIntro();

	/** Draws the credits.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawCredits();

	/** Draws the load state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawLoad();

	/** Draws the save state.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawSave();

	/** Draws the scripted level.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool DrawScript();

	/** Updates the menu.
	 * @return <b>true</b> if updating succeeded, <b>false</b> if not.
	 */
	bool UpdateMenu();

	/** Updates the game.
	 * @return <b>true</b> if updating succeeded, <b>false</b> if not.
	 */
	bool UpdatePlaying();

	/** Updates the game paused.
	 * @return <b>true</b> if updating succeeded, <b>false</b> if not.
	 */
	bool UpdatePaused();

	/** Updates the game in the ending level state.
	 * @return <b>true</b> if updateing succeeded, <b>false</b> if not.
	 */
	bool UpdateEndLevel();

	/** Updates the game in the game over state.
	 * @return <b>true</b> if updateing succeeded, <b>false</b> if not.
	 */
	bool UpdateGameOver();

	/** Updates the hall of fame.
	 * @return <b>true</b> if updating succeeded, <b>false</b> if not.
	 */
	bool UpdateIntro();

	/** Updates the credits.
	 * @return <b>true</b> if updating succeeded, <b>false</b> if not.
	 */
	bool UpdateCredits();

	/** Updates the load state.
	 * @return <b>true</b> if updating succeeded, <b>false</b> if not.
	 */
	bool UpdateLoad();

	/** Updates the save state.
	 * @return <b>true</b> if updating succeeded, <b>false</b> if not.
	 */
	bool UpdateSave();

	/** Updates the script.
	 * @return <b>true</b> if updating succeeded, <b>false</b> if not.
	 */
	bool UpdateScript();

	/** Loads the current world/level.
	 * @return 1 if loading succeeded, 0 if not, -1 if game ended.
	 */
	s32 LoadWorld();

	/** Process key up events.
	 */
	static void OnKeyUp(SDL_keysym key);

	/** Process key down events.
	 */
	static void OnKeyDown(SDL_keysym key);

	/** Process the state change of the app.
	 */
	void ProcessStateChange();

 public:
	/** Creates the application object.
	 */
	HCApp();

	/** Initializes the application.
	 * @param  argc Argument count from the command line, as in main().
	 * @param  argv Argument list from the command line, as in main().
	 * @return <b>true</b> if initialization is ok, <b>false</b> if not.
	 */
	bool Init(s32 argc = 0, char **argv = 0);

	/** Draws the application.
	 * @return <b>true</b> if drawing succeeded, <b>false</b> if not.
	 */
	bool Draw();

	/** Updates the application.
	 * @return <b>true</b> if update succeeded, <b>false</b> if not.
	 */
	bool Update();

	/** Returns the state of the app.
	 * @return State of the app.
	 */
	inline HCState State();

	/** Changes the state of the app.
	 * @param  newState New state of the app.
	 */
	void State(HCState newState);

	/** Process user input.
	 */
	void ProcessInput();

	/** Returns the current level.
	 * @return Current level.
	 */
	HCLevel *Level() {return &level;}

	/** Returns the small font.
	 * @return Small font.
	 */
	JFont *FontSmall() {return &fontSmall;}

	/** Returns the medium font.
	 * @return Medium font.
	 */
	JFont *FontMedium() {return &fontMedium;}

	/** Returns the large font.
	 * @return Large font.
	 */
	JFont *FontLarge() {return &fontLarge;}
	
	/** Parses argument.
	 * @param  args Command line arguments.
	 * @param  argc Arguments left.
	 * @return Number of parameters used.
	 */
	virtual int ParseArg(char *args[], int argc);

	/** Shows the usage string.
	 */
	virtual void PrintUsage(char *program);

	/** Frees the allocated resources.
	 */
	void Destroy();

	/** Destroys the application object.
	 */
	virtual ~HCApp() {Destroy();}

	friend void OnKeyUp(SDL_keysym key);
};

#endif // _HOLOTZ_INCLUDED
