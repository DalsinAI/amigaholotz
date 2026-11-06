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
 * @file    HCLoadSaveSlot.h
 * @author  Juan Carlos Seijo Pérez
 * @date    14/09/2004
 * @version 0.0.1 - 14/09/2004 - Primera versión.
 */

#ifndef _HCLOADSAVESLOT_INCLUDED
#define _HCLOADSAVESLOT_INCLUDED

#include <JLib/Util/JTextFile.h>
#include <JLib/Graphics/JImage.h>

/** Game data slot for save and load.
 */
class HCLoadSaveSlot
{
	JString story;                        /**< Story name. */
	s32 level;                            /**< Level number within the story. */
	
 public:
	/** Creates an empty slot, Init() must be called before.
	 */
	HCLoadSaveSlot() : level(1)
	{}
	
	/** Loads this slot.
	 * @param  slot Slot number to load.
	 * @return <b>true</b> if succeeded, <b>false</b> otherwise.
	 */
	bool Load(s32 slot);
	
	/** Saves this slot with the given parameters.
	 * @param  s Name of the story.
	 * @param  levelNumber Ordinal of the level to save.
	 * @param  image Image to be stored.
	 * @param  f File already opened and positioned.
	 * @return 0 if succeeded, 1 if there was an I/O error, 2 if an integrity error.
	 */
	bool Save(s32 slot, const JString& s, s32 levelNumber);

	/** Returns the story name.
	 * @return story name.
	 */
	const char * Story() {return story.Str();}

	/** Returns the level number.
	 * @return level number.
	 */
	s32 Level() {return level;}
};

#endif // _HCLOADSAVESLOT_INCLUDED
