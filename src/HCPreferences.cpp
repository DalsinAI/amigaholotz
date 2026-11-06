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
 * @file    HCPreferences.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    24/08/2004
 * @version 0.0.1 - 24/08/2004 - First version.
 */

#include <HCPreferences.h>

#ifndef HC_DATA_DIR
#define HC_DATA_DIR "res/"
#endif

#if !defined(_WIN32) && !defined(__amigaos__) /* Amiga: in the drawer, as on Windows */
#include <string.h>
#include <unistd.h>
#endif

HCPreferences * HCPreferences::instance;

HCPreferences::HCPreferences()
{
	langs = 0;
	langCodes = 0;
	numLangs = 0;
	numVideoModes = 0;
	videoModes = 0;
	sound = 0;
	difficulty = 0;
	instance = this;
	Reset();
}

void HCPreferences::Reset()
{
	curLang = HCPREFERENCES_DEFLANGUAGE;
	videoMode = HCPREFERENCES_DEFVIDEOMODE;
	bpp = HCPREFERENCES_DEFBPP;
	fullscreen = HCPREFERENCES_DEFFULLSCREEN;
	sound = HCPREFERENCES_DEFSOUND;
	difficulty = HCPREFERENCES_DEFDIFFICULTY;
}

void HCPreferences::BPP(s32 newBPP)
{
	bpp = newBPP;

	if (bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32)
		bpp = HCPREFERENCES_DEFBPP;
}

s32 HCPreferences::Load(const char *filename)
{
	Destroy();

	s32 ret = 0;

	// Loads the available video modes
	videoModes = JListVideoModes(&numVideoModes, bpp);
	if (!videoModes)
	{
		fprintf(stderr, "No fullscreen video modes found, forcing to windowed.\n");
		ret |= 1;
	}
	
	// Loads the available languages
	JTextFile fLang;
	if (fLang.Load(HC_DATA_DIR HCPREFERENCES_LANGFILE, "rb"))
	{
		std::vector<char *> vL, vC;
		char strLang[64];
		char strCode[8];
		
		while (2 == sscanf(fLang.GetPos(), "%[^;];%[a-z]", strLang, strCode))
		{
			vL.push_back(strdup(strLang));
			vC.push_back(strdup(strCode));
			fLang.NextLine();
		}
		
		numLangs = vC.size();
		langs = new char *[numLangs];
		langCodes = new char *[numLangs];
		
		for (s32 i = 0; i < numLangs; ++i)
		{
			langs[i] = vL[i];
			langCodes[i] = vC[i];
		}
	}
	else
	{
		ret |= 2;
		return ret;
	}
								 
	const char *fname;
  char name[256];

	if (filename != 0)
		fname = filename;
	else
	{
#if !defined(_WIN32) && !defined(__amigaos__) /* Amiga: in the drawer, as on Windows */
		char *home;
		home = getenv("HOME");
		if (home != NULL)
		{
			snprintf(name, sizeof(name), "%s/.holotz-castle/" HCPREFERENCES_DEFFILENAME, home);
			fname = name;
		}
		else
			fname = HC_DATA_DIR HCPREFERENCES_DEFFILENAME;
#else
		fname = HC_DATA_DIR HCPREFERENCES_DEFFILENAME;
#endif
	}

	JTextFile f;

	if (!f.Load(fname, "rb"))
	{
		// Error opening
		// Overrides the default video mode with one that fits into the desktop, this prevents that the first time one plays
		// and exit without changing the video size, the next time they play the video was bigger than the desktop,
		// not allowing to see all the window, even the menu
		if (videoModes)
		{
			// Conservative first guess...
			videoMode = numVideoModes/2;

			// ...but tries to put in a boundary of 640x480
			for (s32 vmd = 0; vmd < numVideoModes; ++vmd)
			{
				if (videoModes[vmd].w <= 640 && videoModes[vmd].h <= 480)
				{
					videoMode = vmd;
					break;
				}
			}
		}

		return 4;
	}

	// Loads the preferences
  s32 fs, snd;
	if (6 != sscanf(f.GetPos(), "language=%d video=%d bpp=%d fullscreen=%d audio=%d difficulty=%d", &curLang, &videoMode, &bpp, &fs, &snd, &difficulty))
	{
		// Incorrect file
		return 8;
	}
  
  fullscreen = (fs != 0) ? true : false;
  sound = (snd != 0) ? true : false;

	BPP(bpp);
	fullscreen = (fullscreen != 0) ? true : false;
	sound = (sound != 0) ? true : false;
	JClamp(difficulty, HCPREFERENCES_HARD, HCPREFERENCES_TOY);

	return 0;
}

s32 HCPreferences::Save(const char *filename)
{
	const char *fname;
  char name[256];
	
	if (filename != 0)
		fname = filename;
	else
	{
#if !defined(_WIN32) && !defined(__amigaos__) /* Amiga: in the drawer, as on Windows */
		char *home;
		home = getenv("HOME");
		if (home != NULL)
		{
			snprintf(name, sizeof(name), "%s/.holotz-castle", home);
			mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			snprintf(name, sizeof(name), "%s/.holotz-castle/" HCPREFERENCES_DEFFILENAME, home);
			fname = name;
		}
		else
			fname = HC_DATA_DIR HCPREFERENCES_DEFFILENAME;
#else
		fname = HC_DATA_DIR HCPREFERENCES_DEFFILENAME;
#endif
	}

	JTextFile f;

	if (!f.Open(fname, "wb"))
	{
		// Error opening
		return 1;
	}

	// Saves the preferences
	if (0 == f.Printf("language=%d\nvideo=%d\nbpp=%d\nfullscreen=%d\naudio=%d\ndifficulty=%d", curLang, videoMode, bpp, fullscreen ? 1 : 0, sound ? 1 : 0, difficulty))
	{
		// Incorrect file
		return 2;
	}

	return 0;
}

void HCPreferences::Destroy()
{
	if (langs != 0)
	{
		for (s32 i = 0; i < numLangs; ++i)
		{
			if (langs[i] != 0)
			{
				free(langs[i]);
			}
		}
		
		delete[] langs;
	}

	if (langCodes != 0)
	{
		for (s32 i = 0; i < numLangs; ++i)
		{
			if (langCodes[i] != 0)
			{
				free(langCodes[i]);
			}
		}
		
		delete[] langCodes;
	}

	numLangs = 0;
	JDELETE_ARRAY(videoModes);
}
