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

/** Playlist for Holotz's castle.
 * @file    HCPlaylist.h
 * @author  Juan Carlos Seijo Pérez
 * @date    25/08/2004
 * @version 0.0.1 - 25/08/2004 - First version.
 */

#ifndef _HCPLAYLIST_INCLUDED
#define _HCPLAYLIST_INCLUDED

#include <vector>
#include <JLib/Util/JTypes.h>
#include <JLib/Util/JTextFile.h>
#include <JLib/Util/JObject.h>
#include <HCUtil.h>

#include <string.h>

#ifndef HC_DATA_DIR
#define HC_DATA_DIR "res/"
#endif

#define HCPLAYLIST_DEFFILENAME          "playlist.txt"

class HCPlaylist
{
 protected:
	std::vector<char *> stories;          /**< Playlist with the stories ordered. */
	s32 curStory;                         /**< Current story playing. */
	char storyDir[4096];
	
 public:
	/** Creates an empty playlist object. Load must be called before using it.
	 */
	HCPlaylist() : curStory(0)
	{ strncpy(storyDir, HC_DATA_DIR, sizeof(storyDir)); }

	/** Return the current story directory.
	 */
	const char * StoryDir() { return storyDir;}

	/** Return the current story name.
	 */
	const char * StoryName() {return stories[curStory];}

	/** Advances to the next story.
	 * @return <true> if there was another story, <b>false</b> otherwise.
	 */
	bool NextStory();

	/** Loads the playlist from the specified file.
	 * @param  file Name of the file with the playlist.
	 * @return <b>true</b> if succeeded, <b>false</b>otherwise.
	 */
	bool Load(const char *file = HCPLAYLIST_DEFFILENAME);
	
	/** Goes to the first story.
	 */
	void Reset() {curStory = 0;}

	/** Goes to the given story.
	 * @param  _storyName Name of the story to go to.
	 * @return <b>true</b> if the story exists, <b>false</b> otherwise.
	 */
	bool GoTo(const char *_storyName);

	/** Destroys the object.
	 */
	void Destroy();

	/** Returns the size of the playlist.
	 * @return Size of the playlist.
	 */
	s32 Size() {return stories.size();}

	/** Returns the element at position index.
	 * @param  index Position of the element to retrieve.
	 * @return Element at position index.
	 */
	char * operator[](s32 index) {return stories[index];}

	/** Orders alphabetically the stories.
	 */
	void OrderStories();

	/** Destroys the object.
	 */
	~HCPlaylist() {Destroy();}
};

#endif // _HCPLAYLIST_INCLUDED
