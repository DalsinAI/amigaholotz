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

/** Preferences file for Holotz's Castle.
 * @file    HCPreferences.h
 * @author  Juan Carlos Seijo Pérez
 * @date    24/08/2004
 * @version 0.0.1 - 24/08/2004 - First version.
 */

#ifndef _HCPREFERENCES_INCLUDED
#define _HCPREFERENCES_INCLUDED

#include <vector>
#include <JLib/Util/JTextFile.h>
#include <JLib/Util/JUtil.h>
#include <JLib/Util/JTypes.h>

#define HCPREFERENCES_TOY               5           // Toy difficulty level
#define HCPREFERENCES_EASY              4           // Easy difficulty level
#define HCPREFERENCES_NORMAL            2           // Normal difficulty level
#define HCPREFERENCES_HARD              1           // Hard difficulty level

#define HCPREFERENCES_DEFFILENAME       "preferences.txt"
#define HCPREFERENCES_DEFLANGUAGE       0
#define HCPREFERENCES_DEFVIDEOMODE      0
#define HCPREFERENCES_DEFBPP            16
#define HCPREFERENCES_DEFFULLSCREEN     false
#define HCPREFERENCES_DEFSOUND          true
#define HCPREFERENCES_LANGFILE          "languages.txt"
#define HCPREFERENCES_DEFDIFFICULTY     HCPREFERENCES_NORMAL

class HCPreferences
{
 protected:
	char **langs;                         /**< Available languages strings. */
	char **langCodes;                     /**< Country codes strings for available languages (es, en, de, etc.). */
	s32 numLangs;                         /**< Number of different languages. */
	s32 curLang;                          /**< Current language index/country code. */
	s32 videoMode;                        /**< Video mode from the list of video modes supported. */
	JVideoMode *videoModes;               /**< Available video modes. */
	s32 numVideoModes;                    /**< Number of available video modes. */
	s32 bpp;                              /**< Color depth in bits per pixel. */
	bool fullscreen;                      /**< Fullscreen/windowed flag. */
	bool sound;                           /**< Sound on/off flag. */
	s32 difficulty;                       /**< Level of difficulty (Easy/Medium/Hard). */
	static HCPreferences *instance;       /**< Unique instance of the object. */

 public:
	/** Creates an empty preferences object.
	 */
	HCPreferences();

	/** Returns the instance of this preference object.
	 * @return Instance ob this object.
	 */
	static HCPreferences * Prefs() {return instance;}

	/** Sets the language.
	 * @param  lang Index of the available languages to use.
	 */
	void CurLang(s32 curLanguage) {curLang = curLanguage; JClamp(curLang, 0, numLangs);}
	
	/** Gets the current language index.
	 * @return Current language index.
	 */
	s32 CurLang() {return curLang;}
	
	/** Gets the available languages strings.
	 * @return Available languages strings.
	 */
	char **Langs() {return langs;}
	
	/** Gets the available language country codes.
	 * @return Available language country codes.
	 */
	char **LangCodes() {return langCodes;}
	
	/** Gets the number of available languages.
	 * @return Number of available languages.
	 */
	s32 NumLangs() {return numLangs;}
	
	/** Gets the number of available video modes.
	 * @return Number of available video modes.
	 */
	s32 NumVideoModes() {return numVideoModes;}
	
	/** Gets the available video modes.
	 * @return Available video modes or 0 if none exist.
	 */
	JVideoMode * VideoModes() {return videoModes;}
	
	/** Sets the video mode number.
	 * @param  Ordinal of the video mode to use, based upon the available modes.
	 */
	void VideoMode(s32 mode) {videoMode = mode;}
	
	/** Gets the video mode number.
	 * @return Video mode number.
	 */
	s32 VideoMode() {return videoMode;}

	/** Sets color depth.
	 * @param  New color depth. Supports 8, 16, 24 and 32 bits.
	 */
	void BPP(s32 newBPP);

	/** Gets the color depth.
	 * @return Color depth.
	 */
	s32 BPP() {return bpp;}

	/** Sets windowed or fullscreen mode.
	 * @param  <b>true</b> to use fullscreen mode, <b>false</b> for windowed mode.
	 */
	void Fullscreen(bool fs) {fullscreen = fs;}

	/** Gets the playing mode (windowed or full screen).
	 * @return <b>true</b> if fullscreen mode, <b>false</b> if windowed mode.
	 */
	bool Fullscreen() {return fullscreen;}

	/** Sets sound mode.
	 * @param  <b>true</b> activates sound, <b>false</b> deactivates it.
	 */
	void Sound(bool fs) {sound = fs;}

	/** Gets the sound mode.
	 * @return <b>true</b> if sound is active, <b>false</b> if not.
	 */
	bool Sound() {return sound;}
	
	/** Sets the level of difficulty.
	 * @param  Level of difficulty, one of HCPREFERENCES_EASY, HCPREFERENCES_NORMAL, HCPREFERENCES_HARD.
	 */
	void Difficulty(s32 newDiff) {difficulty = newDiff; JClamp(difficulty, HCPREFERENCES_HARD, HCPREFERENCES_TOY);}

	/** Gets the level of difficulty.
	 * @return Current level of difficulty, one of HCPREFERENCES_EASY, HCPREFERENCES_NORMAL, HCPREFERENCES_HARD.
	 */
	s32 Difficulty() {return difficulty;}

	/** Resets to defaults.
	 */
	void Reset();
	
	/** Load the preferences file.
	 * @param  filename Name of the file to use or 0 (the default) to use the default file name.
	 * @return 0 if loading succeeded, 1 if I/O error, 2 if integrity error.
	 */
	s32 Load(const char *filename = 0);

	/** Saves the preferences file.
	 * @param  filename Name of the file to use or 0 (the default) to use the default file name.
	 * @return 0 if saving succeeded, 1 if I/O error, 2 if integrity error.
	 */
	s32 Save(const char *filename = 0);

	/** Destroys this object.
	 */
	void Destroy();

	/** Destroys this object.
	 */
	virtual ~HCPreferences() {Destroy();}
};

#endif // _HCPREFERENCES_INCLUDED
