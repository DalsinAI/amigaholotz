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
 * @file    HCUtil.h
 * @author  Juan Carlos Seijo Pérez
 * @date    05/02/2005
 * @version 0.0.1 - 05/02/2005 - First version.
 */

#ifndef _HCUTIL_INCLUDED
#define _HCUTIL_INCLUDED

#ifndef HC_DATA_DIR
#define HC_DATA_DIR "res/"
#endif

#ifndef FILESYS_BAR
#ifdef _WIN32
#define FILESYS_BAR '\\'
#else
#define FILESYS_BAR '/'
#endif  // _WIN32
#endif  // FILESYS_BAR

#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <vector>
#include <JLib/Util/JFile.h>
#include <JLib/Util/JString.h>

class HCUtil
{
	static const char *curDir;
	static const char *installHCDir;
	static const char *installHCedDir;
	static char homeDir[4096];
	static char lastFile[4096];
	static const char *lastPath;
	static std::vector<JString> themes;
	static std::vector<JString> stories;

 public:
	/** Searchs the given file or directory within the standard dirs:
	 * - Current dir
	 * - Installation dir
	 * - Home dir
	 * @return <b>true</b> if found, <b>false<b> otherwise.
	 */
	static bool FindFile(const char *filename);
	
	/** Returns the last queried file. If none, returns an empty string.
	 * @return Last queried file, with its full resolved path preceding it.
	 */
	static const char* File() {return lastFile;}

	/** Returns the last queried file's path, 0 if it wasn't found.
	 * @return Last queried file, with its full resolved path preceding it.
	 */
	static const char* Path() {return lastPath;}

	/** Searchs for themes within the standard dirs:
	 * - Current dir
	 * - Installation dir (si onlyEdit es false, por defecto)
	 * - Home dir
	 * @param  onlyEdit Indica si se debe buscar en el directorio de instalación (protegido) o no.
	 * @return <b>true</b> if any found, <b>false<b> otherwise.
	 */
	static bool FindThemes(bool onlyEdit = false);

	/** Searchs for stories within the standard dirs:
	 * - Current dir
	 * - Installation dir (si onlyEdit es false, por defecto)
	 * - Home dir
	 * @param  onlyEdit Indica si se debe buscar en el directorio de instalación (protegido) o no.
	 * @return <b>true</b> if any found, <b>false<b> otherwise.
	 */
	static bool FindStories(bool onlyEdit = false);

	/** Returns the found themes.
	 */
	static std::vector<JString> & Themes() {return themes;}

	/** Returns the found stories.
	 */
	static std::vector<JString> & Stories() {return stories;}

	/** Creates a new story in the first stories directory found in the standard path:
	 * - Current dir
	 * - Installation dir
	 * - Home dir
	 * @return 0 if stories dir found and there was no other story with the same name, 1 if a story with the same name existed, 
	 * 2 if the stories directory couldn't be found and 3 if there was an error creating the story's directory.
	 */
	static s32 CreateStory(const char *story);

	/** Frees allocated resources and invalidates the themes and stories.
	 */
	static void Destroy();
};

#endif // _HCUTIL_INCLUDED
