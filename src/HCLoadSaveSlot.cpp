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

/** Game data slot for save and load.
 * @file    HCLoadSaveSlot.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    14/09/2004
 * @version 0.0.1 - 14/09/2004 - Primera versión.
 */

#include <HCLoadSaveSlot.h>
#include <HCLevel.h>

#ifndef _WIN32
#include <unistd.h>
#endif

bool HCLoadSaveSlot::Load(s32 slot)
{
	// Loads the name of the story and the level number
	JRW f;
	char name[256];

#ifndef _WIN32
	char *home;
	home = getenv("HOME");
	if (home != NULL)
		snprintf(name, sizeof(name), "%s/.holotz-castle/slot%02d", home, slot);
	else
		snprintf(name, sizeof(name), HC_DATA_DIR "savedata/slot%02d", slot);
#else
	snprintf(name, sizeof(name), HC_DATA_DIR "savedata/slot%02d", slot);
#endif
	
	if (f.Create(name, "rb") && 
			0 == story.Load(f) &&
			0 != f.ReadLE32(&level))
	{
		return true;
	}

	return false;
}

bool HCLoadSaveSlot::Save(s32 slot, const JString& s, s32 levelNumber)
{
	level = levelNumber;
	story = s;

	JRW f;
	char name[256];

#ifndef _WIN32
	char *home;
	home = getenv("HOME");
	snprintf(name, sizeof(name), "%s/.holotz-castle", home);
	mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (home != NULL)
		snprintf(name, sizeof(name), "%s/.holotz-castle/slot%02d", home, slot);
	else
		snprintf(name,  sizeof(name), HC_DATA_DIR "savedata/slot%02d", slot);
#else
	snprintf(name,  sizeof(name), HC_DATA_DIR "savedata/slot%02d", slot);
#endif
	
	// Saves the name of the story and the level number
	if (f.Create(name, "wb"))
	{
		if (0 == story.Save(f))
		{
			if (0 != f.WriteLE32(&level))
			{
				return true;
			}
			else
			{
				fprintf(stderr, "Error saving level %d\n", level);
			}
		}
		else
		{
			fprintf(stderr, "Error saving story %s\n", story.Str());
		}
	}
	else
	{
		fprintf(stderr, "Error opening %s\n", name);
	}

	return false;
}
