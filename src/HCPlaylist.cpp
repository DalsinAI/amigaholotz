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
 * @file    HCPlaylist.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    25/08/2004
 * @version 0.0.1 - 25/08/2004 - First version.
 */

#include <HCPlaylist.h>

#include <string.h>

#ifdef _WIN32
#define FILESYS_BAR '\\'
#else
#define FILESYS_BAR '/'
#endif

bool HCPlaylist::Load(const char *file)
{
	if (file == 0)
	{
		if (!HCUtil::FindStories())
		{
			fprintf(stderr, "No stories found. Check manual.\n");
			
			return false;
		}
		
		Destroy();

		for (u32 i = 0; i < HCUtil::Stories().size(); ++i)
		{
			stories.push_back(strdup(HCUtil::Stories().at(i).Str()));
		}

		OrderStories();
		GoTo(stories[0]);
		
		return true;
	}

	if (!HCUtil::FindFile(file))
	{
		fprintf(stderr, "Couldn't find playlist file %s. Check manual.\n", file);
	}

	JTextFile f;
	if (!f.Load(HCUtil::File()))
	{
		return false;
	}

	Destroy();

	JString strFile;
	char str[4096];
	
	// Fills the playlist with each line in the file
	while (f.ReadLine(str))
	{
		strFile = "stories/";
		strFile += str;
		if (HCUtil::FindFile(strFile))
		{
			stories.push_back(strdup(str));
		}
		else
		{
			fprintf(stderr, "Playlist entry not found: %s. Check manual.\n", str);
		}
	}
	
	if (stories.size() > 0)
	{
		OrderStories();
		GoTo(stories[0]);
	}

	return stories.size() > 0;
}
	
bool HCPlaylist::NextStory()
{
	if (curStory + 1 >= (s32)stories.size())
	{
		return false;
	}
	
	++curStory;
	
	if (!HCUtil::FindFile(JString("stories/") + stories[curStory]))
	{
    --curStory;
		return false;
	}
	
	strncpy(storyDir, HCUtil::Path(), sizeof(storyDir));
	strncat(storyDir, "stories/", sizeof(storyDir));

	return true;
}

bool HCPlaylist::GoTo(const char *_storyName)
{
	bool found = false;

	for (u32 i = 0; !found && i < stories.size(); ++i)
	{
		if (strcmp(stories[i], _storyName) == 0)
		{
			curStory = i;
			found = true;
		}
	}

	if (found)
	{
		if (!HCUtil::FindFile(JString("stories/") + stories[curStory]))
		{
			return false;
		}
	
		strncpy(storyDir, HCUtil::Path(), sizeof(storyDir));
		strncat(storyDir, "stories/", sizeof(storyDir));
	}

	return found;
}

void HCPlaylist::OrderStories()
{
	char *tmp;

	for (u32 i = 0; i < stories.size() - 1; ++i) 
	{
		for (u32 j = 0; j < stories.size() - 1 - i; ++j)
		{
			if (strcmp(stories[j+1], stories[j]) < 0) 
			{
				tmp = stories[j];
				stories[j] = stories[j+1];
				stories[j+1] = tmp;
			}
		}
	}
}

void HCPlaylist::Destroy()
{
	for (u32 i = 0; i < stories.size(); ++i)
	{
		free(stories[i]);
	}
	
	stories.clear();
}
