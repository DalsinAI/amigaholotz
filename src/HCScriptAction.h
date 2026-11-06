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

/** Definition of script actions.
 * @file    HCScriptAction.h
 * @author  Juan Carlos Seijo Pérez
 * @date    03/07/2004
 * @version 0.0.1 - 03/07/2004 - Primera versión.
 */

#ifndef _HCSCRIPTACTION_INCLUDED
#define _HCSCRIPTACTION_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JApp.h>
#include <JLib/Util/JTextFile.h>
#include <JLib/Util/JUtil.h>
#include <JLib/Graphics/JFont.h>
#include <HCCharacter.h>
#include <HCText.h>

class HCApp;

/** Types of action.
 */
enum HCScriptActionType
{
	HCSAT_NOP = 0,                        /**< No operation. */
	HCSAT_MOVE,                           /**< Moves a character. */
	HCSAT_DIALOG,                         /**< Makes a character to talk. */
	HCSAT_NARRATIVE,                      /**< Shows a narrative text. */
	HCSAT_SOUND,                          /**< Plays a sound. */
	HCSAT_WAIT,                           /**< Waits. */
};

/** Basic action.
 */
class HCScriptAction
{
 public:
	HCScriptActionType type;              /**< Type of this action. */
	bool finished;                        /**< Indicates whether it has finished or not. */

	/** Creates a script action.
	 */
	HCScriptAction(HCScriptActionType _type = HCSAT_NOP) : type(_type), finished(false)
	{}
	
	/** Checks if it has finished.
	 * @return <b>true<b> if it has finished, <b>false</b> otherwise.
	 */
	bool Finished() {return finished;}

	/** Loads the action from the file. If the method encounters '[ MOVE ball 2 4 5 ]', it will
	 * create and return a new HCScriptActionMove* (MOVE) casted to a HCScriptAction* of 
	 * the third (2) ball enemy of the level to move left (4) five cells (5).
	 * @param  f File Opened an positioned in the begin of the action (just after character '[').
	 * @return A new action corresponding to the one found or 0 if an error occurred.
	 */
	static HCScriptAction * Load(JTextFile &f);

	/** Returns the appropiate character based upon its name and index.
	 * @param  name Character name. Must be one of "main", "enemy" or "guest".
	 * @index  index Index of the character in the array.
	 */
	HCCharacter * Character(const char *name, s32 index);

	/** Skips this action. Must be implemented in the children classes, if possible.
	 */
  virtual void Skip() {}
	
	/** Updates the action.
	 * @return 0 if it didn't need update, 1 otherwise.
	 */
	virtual s32 Update();

	/** Prepares the action for execution. The block calls this method when its time 
	 * to execute the action. The action prepares its data to be executed (init timers, etc.).
	 */
	virtual void Current() {}
	
	/** Destroys the object.
	 */
	virtual ~HCScriptAction() {}
};

/** For unifying the actions' hierarchy.
 */
typedef HCScriptAction HCScriptActionNop;

class HCScriptActionMove : public HCScriptAction
{
 protected:
	HCCharacter *character;               /**< Character to move. */
	s32 direction;                        /**< Direction to follow. */
	s32 totalAmount;                      /**< Amount of cells to displace. */
	s32 curAmount;                        /**< Current amount of cells. */
	s32 orgRow;                           /**< Original row. */
	s32 orgCol;                           /**< Original column. */
	s32 lastRow;                          /**< Last visited row. */
	s32 lastCol;                          /**< Last visited column. */

 public:
	/** Creates an empty Move Action. Init must be called in order to use it.
	 */
	HCScriptActionMove() : HCScriptAction(HCSAT_MOVE), character(0)
	{}

	/** Initializes this action.
	 * @param  _character Character to move.
	 * @param  dir Movement direction (one of 2, 4, 6 or 8).
	 * @param  amount Movement amount in cells.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool Init(HCCharacter *_character, s32 dir, s32 amount);

	/** Loads the action from the file.
	 * @param  f File Opened an positioned in the begin of the action (just after character '[').
	 * @return <b>true</b> if loading succeeded, <b>false</b> otherwise.
	 */
	bool Load(JTextFile &f);

	/** Updates the action.
	 * @return 0 if it didn't need update, 1 otherwise.
	 */
	virtual s32 Update();

	/** Sets the character state.
	 */
	virtual void Current();
	
	/** Destroys the object.
	 */
	virtual ~HCScriptActionMove() {}
};

class HCScriptActionDialog : public HCScriptAction
{
 protected:
	HCCharacter *character;               /**< Character who talks. */
	HCText dialog;                        /**< Dialog to show. */

 public:
	/** Creates an empty Dialog Action. Init must be called in order to use it.
	 */
	HCScriptActionDialog() : HCScriptAction(HCSAT_DIALOG), character(0)
	{}

	/** Initializes this action.
	 * @param  _character Character to talk.
	 * @param  text Text of the dialog.
	 * @param  theme Theme to use.
	 * @param  font Font to use.
	 * @param  align Alignment of text.
	 * @param  left Whether the peak must face to the left or to the right.
	 * @param  subtype Subtype within the theme.
	 * @param  r Color red component.
	 * @param  g Color green component.
	 * @param  b Color blue component.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool Init(HCCharacter *_character, 
						const char *text, 
						HCTheme *theme, 
						JFont *font,
						JFontAlign align,
						bool left,
						s32 subtype,
						u8 r, u8 g, u8 b);

	/** Loads the action from the file.
	 * @param  f File Opened an positioned in the begin of the action (just after character '[').
	 * @return <b>true</b> if loading succeeded, <b>false</b> otherwise.
	 */
	bool Load(JTextFile &f);
	
	/** Updates the action.
	 * @return 0 if it didn't need update, 1 otherwise.
	 */
	virtual s32 Update();

	/** Skips this action.
	 */
  virtual void Skip() {dialog.Skip();}
	
	/** Sets the character's dialog and inits the timer.
	 */
	virtual void Current();
	
	/** Destroys the object.
	 */
	virtual ~HCScriptActionDialog() {}
};

class HCScriptActionNarrative : public HCScriptAction
{
 protected:
	HCText narrative;                        /**< Narrative to show. */

 public:
	/** Creates an empty Narrative Action. Init must be called in order to use it.
	 */
	HCScriptActionNarrative() : HCScriptAction(HCSAT_NARRATIVE)
	{}

	/** Initializes this action.
	 * @param  s32 Frame alignment within screen.
	 * @param  text Text of the narrative.
	 * @param  theme Theme to use.
	 * @param  font Font to use.
	 * @param  align Alignment of text.
	 * @param  left Whether the peak must face to the left or to the right.
	 * @param  subtype Subtype within the theme.
	 * @param  r Color red component.
	 * @param  g Color green component.
	 * @param  b Color blue component.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool Init(s32 alignment, 
						const char *text, 
						HCTheme *theme, 
						JFont *font,
						JFontAlign align,
						s32 subtype,
						u8 r, u8 g, u8 b);

	/** Loads the action from the file.
	 * @param  f File Opened an positioned in the begin of the action (just after character '[').
	 * @return <b>true</b> if loading succeeded, <b>false</b> otherwise.
	 */
	bool Load(JTextFile &f);

	/** Updates the action.
	 * @return 0 if it didn't need update, 1 otherwise.
	 */
	virtual s32 Update();

	/** Prepares for execution.
	 */
	virtual void Current();
	
	/** Destroys the object.
	 */
	virtual ~HCScriptActionNarrative() {}
};

class HCScriptActionSound : public HCScriptAction
{
 protected:
	JChunk sound;                         /**< Sound to play. */
	s32 numLoops;                         /**< Number of loops, -1 means play forever, 0 means once, 1 means 2 times, and so on. */
	
 public:
	/** Creates an empty Sound Action. Init must be called in order to use it.
	 */
	HCScriptActionSound() : HCScriptAction(HCSAT_SOUND)
	{}

	/** Initializes this action.
	 * @param  filename File name of the sound.
	 * @param  loops Number of loops, -1 means forever.
	 * @param  waitToEnd Must end to considere it is finished?
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool Init(const char *filename, s32 loops, bool waitToEnd);

	/** Loads the action from the file.
	 * @param  f File Opened an positioned in the begin of the action (just after character '[').
	 * @return <b>true</b> if loading succeeded, <b>false</b> otherwise.
	 */
	bool Load(JTextFile &f);

	/** Updates the action.
	 * @return 0 if it didn't need update, 1 otherwise.
	 */
	virtual s32 Update();
	
	/** Prepares the action for execution. The block calls this method when its time 
	 * to execute the action. The action prepares its data to be executed (init timers, etc.).
	 */
	virtual void Current();

	/** Destroys the object.
	 */
	virtual ~HCScriptActionSound() {}
};

class HCScriptActionWait : public HCScriptAction
{
 protected:
	JTimer timer;                         /**< Timer. */
	s32 ms;                               /**< Millisecond to wait. */
	
 public:
	/** Creates an empty Wait Action. Init must be called in order to use it.
	 */
	HCScriptActionWait() : HCScriptAction(HCSAT_WAIT)
	{}

	/** Initializes this action.
	 * @param  loops Number of milliseconds to wait.
	 * @return <b>true</b> if initialization succeeded, <b>false</b> otherwise.
	 */
	bool Init(s32 millis = 1000);

	/** Loads the action from the file.
	 * @param  f File Opened an positioned in the begin of the action (just after character '[').
	 * @return <b>true</b> if loading succeeded, <b>false</b> otherwise.
	 */
	bool Load(JTextFile &f);

	/** Updates the action.
	 * @return 0 if it didn't need update, 1 otherwise.
	 */
	virtual s32 Update();
	
	/** Prepares the action for execution. The block calls this method when its time 
	 * to execute the action. The action prepares its data to be executed (init timers, etc.).
	 */
	virtual void Current();

	/** Destroys the object.
	 */
	virtual ~HCScriptActionWait() {}
};

#endif // _HCSCRIPTACTION_INCLUDED
