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

/** Utility functions for Holotz's Castle.
 * @file    HCUtil.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    05/02/2005
 * @version 0.0.1 - 05/02/2005 - First version.
 */

#include <HCUtil.h>

const char * HCUtil::curDir = "";
const char * HCUtil::installHCDir = HC_DATA_DIR;
const char * HCUtil::installHCedDir = HCED_DATA_DIR;
char HCUtil::homeDir[4096];
char HCUtil::lastFile[4096];
const char *HCUtil::lastPath;
std::vector<JString> HCUtil::themes;
std::vector<JString> HCUtil::stories;

bool HCUtil::FindFile(const char *filename)
{
	char *str;

#ifndef _WIN32
	if (strstr(filename, "holotzcastle") == 0 && 0 != (str = getenv("HOME")))
	{
		snprintf(homeDir, sizeof(homeDir), "%s/.holotz-castle/", str);
	}
	else
	{
		homeDir[0] = 0;
	}

	lastFile[0] = 0;
	

	if (homeDir[0] != 0)
	{
		// Searchs in the home dir
		snprintf(lastFile, sizeof(lastFile), "%s%s", homeDir, filename);
		
		if (JFile::Exists(lastFile))
		{
			lastPath = homeDir;
			return true;
		}
	}
#endif // _WIN32

	// Searchs in the current dir
	if (JFile::Exists(filename))
	{
		strncpy(lastFile, filename, sizeof(lastFile));
		lastPath = curDir;
		return true;
	}
	
	// Searchs in the HC installation dir
	snprintf(lastFile, sizeof(lastFile), "%s%s", installHCDir, filename);

	if (JFile::Exists(lastFile))
	{
		lastPath = installHCDir;
		return true;
	}

	// Searchs in the HCED installation dir
	snprintf(lastFile, sizeof(lastFile), "%s%s", installHCedDir, filename);

	if (JFile::Exists(lastFile))
	{
		lastPath = installHCedDir;
		return true;
	}

	lastFile[0] = 0;
	lastPath = 0;

	return false;
}


bool HCUtil::FindThemes(bool onlyEdit)
{
	char *str;

#ifndef _WIN32
	if (0 != (str = getenv("HOME")))
	{
		snprintf(homeDir, sizeof(homeDir), "%s/.holotz-castle/", str);
	}
	else
	{
		homeDir[0] = 0;
	}

	const char *dirs[] = {homeDir, curDir, installHCDir, installHCedDir};
#else
	const char *dirs[] = {0, curDir, installHCDir, installHCedDir};
#endif // _WIN32

#ifndef _WIN32
	if (onlyEdit)
	{
		dirs[2] = dirs[3] = 0;
	}
#endif // _WIN32
	
	DIR *dp;
	struct dirent *ep;
	char themesDir[4096];

	// Starts with a new set
	themes.clear();

	for (s32 i = 0; i < 4; ++i)
	{
		if (dirs[i])
		{
			snprintf(themesDir, sizeof(themesDir), "%stheme", dirs[i]);
			dp = opendir(themesDir);

			if (dp != 0)
			{
				while ((ep = readdir(dp)))
				{
					if (0 != strcmp(ep->d_name, ".") && 0 != strcmp(ep->d_name, ".."))
					{
						u32 j;
						for (j = 0; j < themes.size(); ++j)
						{
							if (themes[j] == ep->d_name)
								break;
						}
					
						if (j == themes.size())
						{
							themes.push_back(ep->d_name);
						}
					}
				}
				
				closedir(dp);
			}
		}
	}
	
	return themes.size() > 0;
}

bool HCUtil::FindStories(bool onlyEdit)
{
	char *str;

#ifndef _WIN32
	if (0 != (str = getenv("HOME")))
	{
		snprintf(homeDir, sizeof(homeDir), "%s/.holotz-castle/", str);
	}
	else
	{
		homeDir[0] = 0;
	}

	const char *dirs[] = {homeDir, curDir, installHCDir, installHCedDir};
#else
	const char *dirs[] = {0, curDir, installHCDir, installHCedDir};
#endif // _WIN32
	
#ifndef _WIN32
	if (onlyEdit)
	{
		dirs[2] = dirs[3] = 0;
	}
#endif // _WIN32

	DIR *dp;
	struct dirent *ep;
	char storiesDir[4096];

	// Starts with a new set
	stories.clear();

	for (s32 i = 0; i < 4; ++i)
	{
		if (dirs[i])
		{
			snprintf(storiesDir, sizeof(storiesDir), "%sstories", dirs[i]);
			dp = opendir(storiesDir);

			if (dp != 0)
			{
				while ((ep = readdir(dp)))
				{
					if (0 != strcmp(ep->d_name, ".") && 0 != strcmp(ep->d_name, ".."))
					{
						u32 j;
						for (j = 0; j < stories.size(); ++j)
						{
							if (stories[j] == ep->d_name)
								break;
						}
					
						if (j == stories.size())
						{
							stories.push_back(ep->d_name);
						}
					}
				}
				
				closedir(dp);
			}
		}
	}

	return stories.size() > 0;
}

s32 HCUtil::CreateStory(const char *story)
{
	if (FindFile("stories"))
	{
		// Directory 'stories' found
		char name[4096];
		snprintf(name, sizeof(name), "%s%c%s", lastFile, FILESYS_BAR, story);
		
		if (!FindFile(name))
		{
			// The story does not exist, creates its directory
#ifndef _WIN32
			if (0 != mkdir(name, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
#else
			if (0 != mkdir(name))
#endif //_WIN32
			{
				// Can't create new dir
				return 3;
			}
			else
			{
				// Ok!
				return 0;
			}
		}
		else
		{
			// Story already exists
			return 1;
		}
	}
	else
	{
		// Stories directory not found
		return 2;
	}
}

void HCUtil::Destroy()
{
	themes.clear();
	stories.clear();
}
