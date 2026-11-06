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

/** Theme for Holotz's castle. Loads several resources as floors
 * breaks, ladders and bars.
 * @file    HCTheme.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    01/06/2004
 * @version 0.0.1 - 01/06/2004 - First version.
 */

#include <HCTheme.h>

#ifndef HC_DATA_DIR
#define HC_DATA_DIR "res/"
#endif

HCTheme::HCTheme() : 	imgFloor(0), numFloors(0), imgContFloor(0), numContFloors(0), sprBreak(0), numBreaks(0), 
											sprObject(0), numObjects(0), imgBar(0), numBars(0), imgLadder(0), numLadders(0), imgRope(0), 
											numRopes(0), sprMain(0), numMains(0), sprGuest(0), numGuests(0), sprBall(0), numBalls(0), 
											sprRandom(0), numRandoms(0), sprStatic(0), numStatics(0), sprMaker(0), numMakers(0), 
											sprChaser(0), numChasers(0), imgDialog(0), numDialogs(0), imgNarrative(0), numNarratives(0)

{
	memset(name, 0, sizeof(name));
}

s32 HCTheme::CountDirs()
{
	char str[256];
	s32 num = 0;
	bool cont = true;

	char cwd[2048];
	getcwd(cwd, 2048);

	do
	{
		sprintf(str, "%d", num + 1);
		if (JFile::Exists(str))
		{
			++num;
		}
		else
		{
			cont = false;
		}
	} while (cont);

	return num;
}

bool HCTheme::LoadFloors()
{
	if (0 != chdir("floor"))
	{
		return false;
	}

	numFloors = CountDirs();
	if (0 == numFloors)
	{
		return false;
	}

	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	imgFloor = new JImage[numFloors];
	
	for (i = 1; ret && i <= numFloors; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			if (!imgFloor[i-1].Load("floor.tga"))
			{
				fprintf(stderr, "HCTheme: Couldn't load floor.\n");
				
				ret = false;
			}

			chdir("..");
		}
	}

	chdir("..");

  return ret;
}

bool HCTheme::LoadContFloors()
{
	if (0 != chdir("contfloor"))
	{
		return false;
	}

	numContFloors = CountDirs();
	if (0 == numContFloors)
	{
		return false;
	}

	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	imgContFloor = new JImage* [numContFloors];

	for (s32 c = 0; c < numContFloors; ++c)
	{
		imgContFloor[c] = new JImage[HCFDT_COUNT];
	}
	
	for (i = 1; ret && i <= numContFloors; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			// Load floor resources
			if (!imgContFloor[i-1][HCFDT_C].Load("c.tga") ||
					!imgContFloor[i-1][HCFDT_C1].Load("c1.tga") ||
					!imgContFloor[i-1][HCFDT_C1DL].Load("cdl.tga") ||
					!imgContFloor[i-1][HCFDT_C3].Load("c3.tga") ||
					!imgContFloor[i-1][HCFDT_C3DR].Load("cdr.tga") ||
					!imgContFloor[i-1][HCFDT_C7].Load("c7.tga") ||
					!imgContFloor[i-1][HCFDT_C7UL].Load("cul.tga") ||
					!imgContFloor[i-1][HCFDT_C9].Load("c9.tga") ||
					!imgContFloor[i-1][HCFDT_C9UR].Load("cur.tga") ||
					!imgContFloor[i-1][HCFDT_CU].Load("cu.tga") ||
					!imgContFloor[i-1][HCFDT_CD].Load("cd.tga") ||
					!imgContFloor[i-1][HCFDT_CL].Load("cl.tga") ||
					!imgContFloor[i-1][HCFDT_CR].Load("cr.tga") ||
					!imgContFloor[i-1][HCFDT_S2].Load("s2.tga") ||
					!imgContFloor[i-1][HCFDT_S4].Load("s4.tga") ||
					!imgContFloor[i-1][HCFDT_S6].Load("s6.tga") ||
					!imgContFloor[i-1][HCFDT_S8].Load("s8.tga") ||
					!imgContFloor[i-1][HCFDT_SH].Load("sh.tga") ||
					!imgContFloor[i-1][HCFDT_SV].Load("sv.tga") ||
					!imgContFloor[i-1][HCFDT_I].Load("i.tga"))
			{
				fprintf(stderr, "HCTheme: Couldn't load continuous floor.\n");
				ret = false;
			}
			
			chdir("..");
		}
		else
		{
			fprintf(stderr, "HCTheme: Couldn't load continuous floor.\n");
		}
	}

	chdir("..");

	return ret;
}

bool HCTheme::LoadBreaks()
{
	if (0 != chdir("break"))
	{
		return false;
	}

	numBreaks = CountDirs();
	if (0 == numBreaks)
	{
		return false;
	}

	JRW f;
	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	sprBreak = new JImageSprite*[numBreaks];

	for (int c = 0; c < numBreaks; ++c)
	{
		sprBreak[c] = new JImageSprite[HCBDT_COUNT];
	}
	
	for (i = 1; ret && i <= numBreaks; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			if (!f.Create("normal.spr","rb") || (0 != sprBreak[i-1][HCBDT_NORMAL].Load(f)) ||
					!f.Create("breaking.spr","rb") || (0 != sprBreak[i-1][HCBDT_BREAKING].Load(f)) ||
					!f.Create("broken.spr","rb") || (0 != sprBreak[i-1][HCBDT_BROKEN].Load(f)))
			{
				fprintf(stderr, "HCTheme: Couldn't load break.\n");
				ret = false;
			}

			chdir("..");
		}
	}

	chdir("..");

  return ret;
}

bool HCTheme::LoadBars()
{
	if (0 != chdir("bar"))
	{
		return false;
	}

	numBars = CountDirs();
	if (0 == numBars)
	{
		return false;
	}

	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	imgBar = new JImage[numBars];
	
	for (i = 1; ret && i <= numBars; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			if (!imgBar[i-1].Load("bar.tga"))
			{
				fprintf(stderr, "HCTheme: Couldn't load bar.\n");
				ret = false;
			}

			chdir("..");
		}
	}

	chdir("..");

  return ret;
}

bool HCTheme::LoadLadders()
{
	if (0 != chdir("ladder"))
	{
		return false;
	}

	numLadders = CountDirs();
	if (0 == numLadders)
	{
		return false;
	}

	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	imgLadder = new JImage[numLadders];
	
	for (i = 1; ret && i <= numLadders; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			if (!imgLadder[i-1].Load("ladder.tga"))
			{
				fprintf(stderr, "HCTheme: Couldn't load ladder.\n");
				ret = false;
			}
			
			chdir("..");
		}
	}

	chdir("..");
  
	return ret;
}

bool HCTheme::LoadDecos()
{
  /**< @todo: load several decos */

  return true;
}

bool HCTheme::LoadObjects()
{
	if (0 != chdir("object"))
	{
		return false;
	}
	
	numObjects = CountDirs();
	if (0 == numObjects)
	{
		return false;
	}

	JRW f;
	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	sprObject = new JImageSprite*[numObjects];

	for (int c = 0; c < numObjects; ++c)
	{
		sprObject[c] = new JImageSprite[HCODT_COUNT];
	}
	
	for (i = 1; ret && i <= numObjects; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			if (!f.Create("normal.spr","rb") || 0 != sprObject[i-1][HCODT_NORMAL].Load(f) ||
					!f.Create("acquired.spr","rb") || 0 != sprObject[i-1][HCODT_ACQUIRED].Load(f))
			{
				fprintf(stderr, "HCTheme: Couldn't load object.\n");
				ret = false;
			}
			
			chdir("..");
		}
	}

	chdir("..");

  return ret;
}

bool HCTheme::LoadRopes()
{
	if (0 != chdir("rope"))
	{
		return false;
	}

	numRopes = CountDirs();
	if (0 == numRopes)
	{
		return false;
	}

	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	imgRope = new JImage*[numRopes];

	for (int c = 0; c < numRopes; ++c)
	{
		imgRope[c] = new JImage[HCRDT_COUNT];
	}
	
	for (i = 1; ret && i <= numRopes; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			if (!imgRope[i-1][HCRDT_EDGE].Load("edge.tga") ||
					!imgRope[i-1][HCRDT_TOP].Load("top.tga") ||
					!imgRope[i-1][HCRDT_MIDDLE].Load("middle.tga"))
			{
				fprintf(stderr, "HCTheme: Couldn't load rope.\n");
				ret = false;
			}

			chdir("..");
		}
	}

	chdir("..");

  return ret;
}

bool HCTheme::LoadChar(const char *directory, JImageSprite ** &sprArr, s32 &num)
{
	if (0 != chdir(directory))
	{
		return false;
	}

	const char *files[] = {"stop.spr",
												 "right.spr",
												 "left.spr",
												 "up.spr",
												 "down.spr",
												 "slide.spr",
												 "jump.spr",
												 "jumpleft.spr",
												 "jumpright.spr",
												 "fall.spr",
												 "dead.spr",
												 "hang.spr"};

	JRW f;
	num = CountDirs();
	
	if (0 == num)
	{
		return false;
	}

	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	sprArr = new JImageSprite*[num];

	for (s32 c = 0; c < num; ++c)
	{
		sprArr[c] = new JImageSprite[HCCDT_COUNT];
	}
	
	// For each numbered dir loads each character's state
	for (s32 k = 1; ret && k <= num; ++k)
	{
		sprintf(strDir, "%d", k);

		if (0 == chdir(strDir))
		{
			for (i = 0; i < HCCDT_COUNT; ++i)
			{
				if (JFile::Exists(files[i]))
				{
					if (f.Create(files[i],"rb"))
					{
						if (0 != sprArr[k - 1][i].Load(f))
						{
							fprintf(stderr, "Error loading character sprite file %s\n", files[i]);
							return false;
						}
					}
				} // if res/char exists
				else
				{
					fprintf(stderr, "File %s does not exist.\n", files[i]);
				}
			} // for every state
			
			chdir("..");
		} // if chdir()
	}

	chdir("../..");

	return true;
}

bool HCTheme::LoadCharacters()
{
	if (LoadChar("char/main", sprMain, numMains) &&
			LoadChar("char/ball", sprBall, numBalls) &&
			LoadChar("char/random", sprRandom, numRandoms) &&
			LoadChar("char/static", sprStatic, numStatics) &&
			LoadChar("char/maker", sprMaker, numMakers) &&
			LoadChar("char/chaser", sprChaser, numChasers) &&
			LoadChar("char/guest", sprGuest, numGuests))
	{
		return true;
	}

	return false;
}

bool HCTheme::LoadDialogs()
{
	if (0 != chdir("dialog"))
	{
		return false;
	}

	numDialogs = CountDirs();
	if (0 == numDialogs)
	{
		return false;
	}

	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	imgDialog = new JImage* [numDialogs];

	for (s32 k = 0; k < numDialogs; ++k)
	{
		imgDialog[k] = new JImage[11];
	}
	
	for (i = 1; ret && i <= numDialogs; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			if (!imgDialog[i-1][HCTDT_1].Load("1.tga") ||
					!imgDialog[i-1][HCTDT_2].Load("2.tga") ||
					!imgDialog[i-1][HCTDT_3].Load("3.tga") ||
					!imgDialog[i-1][HCTDT_4].Load("4.tga") ||
					!imgDialog[i-1][HCTDT_5].Load("5.tga") ||
					!imgDialog[i-1][HCTDT_6].Load("6.tga") ||
					!imgDialog[i-1][HCTDT_7].Load("7.tga") ||
					!imgDialog[i-1][HCTDT_8].Load("8.tga") ||
					!imgDialog[i-1][HCTDT_9].Load("9.tga") ||
					!imgDialog[i-1][HCTDT_LEFT].Load("left.tga") ||
					!imgDialog[i-1][HCTDT_RIGHT].Load("right.tga"))
			{
        char strcwd[256];
        getcwd(strcwd, 256);
        perror("error");
				fprintf(stderr, "HCTheme: Couldn't load dialog. %s en dir %s\n", SDL_GetError(), strcwd);
				ret = false;
			}

			chdir("..");
		}
	}

	chdir("..");

  return ret;
}

bool HCTheme::LoadNarratives()
{
	if (0 != chdir("narrative"))
	{
		return false;
	}

	numNarratives = CountDirs();
	if (0 == numNarratives)
	{
		return false;
	}

	char strDir[5];
	memset(strDir, 0, sizeof(strDir));
	s32 i = 1;
	bool ret = true;

	// Creates the array
	imgNarrative = new JImage *[numNarratives];

	for (s32 k = 0; k < numNarratives; ++k)
	{
		imgNarrative[k] = new JImage[9];
	}
	
	for (i = 1; ret && i <= numNarratives; ++i)
	{
		sprintf(strDir, "%d", i);

		if (0 == chdir(strDir))
		{
			if (!imgNarrative[i-1][HCTDT_1].Load("1.tga") ||
					!imgNarrative[i-1][HCTDT_2].Load("2.tga") ||
					!imgNarrative[i-1][HCTDT_3].Load("3.tga") ||
					!imgNarrative[i-1][HCTDT_4].Load("4.tga") ||
					!imgNarrative[i-1][HCTDT_5].Load("5.tga") ||
					!imgNarrative[i-1][HCTDT_6].Load("6.tga") ||
					!imgNarrative[i-1][HCTDT_7].Load("7.tga") ||
					!imgNarrative[i-1][HCTDT_8].Load("8.tga") ||
					!imgNarrative[i-1][HCTDT_9].Load("9.tga"))
			{
				fprintf(stderr, "HCTheme: Couldn't load narrative.\n");
				ret = false;
			}

			chdir("..");
		}
	}

	chdir("..");

  return ret;
}

bool HCTheme::Load(const char *themeName)
{
	Destroy();

	char themeDir[PATH_MAX];
	char curDir[PATH_MAX];
	getcwd(curDir, PATH_MAX);
	
	// Checks for the theme in any of:
	// - installation directory
	// - current directory
	// - home directory
	strncpy(name, themeName, sizeof(name));
	snprintf(themeDir, sizeof(themeDir), "theme/%s", name);
	
	if (!HCUtil::FindFile(themeDir))
	{
		fprintf(stderr, "Couldn't find the theme %s\n", themeDir);
		return false;
	}

	if (0 != chdir(HCUtil::File()))
	{
		fprintf(stderr, "Couldn't find the theme %s\n", HCUtil::File());
	}

	if (!LoadFloors() || 
			!LoadContFloors() || 
			!LoadDecos() || 
			!LoadBars() || 
			!LoadBreaks() || 
			!LoadLadders() || 
			!LoadObjects() || 
			!LoadRopes() || 
			!LoadCharacters() || 
			!LoadDialogs() || 
			!LoadNarratives())
	{
		fprintf(stderr, "HCTheme: Couldn't load resources.\n");

		chdir(curDir);

		return false;
	}

	chdir(curDir);

	return true;
}

void HCTheme::Destroy()
{
	JDELETE_ARRAY(imgFloor);
	JDELETE_ARRAY_ARRAY(imgContFloor, numContFloors);
	JDELETE_ARRAY_ARRAY(sprBreak, numBreaks);
	JDELETE_ARRAY(imgBar);
	JDELETE_ARRAY(imgLadder);
	JDELETE_ARRAY_ARRAY(sprObject, numObjects);
	JDELETE_ARRAY_ARRAY(imgRope, numRopes);
	JDELETE_ARRAY_ARRAY(sprMain, numMains);
	JDELETE_ARRAY_ARRAY(sprBall, numBalls);
	JDELETE_ARRAY_ARRAY(sprRandom, numRandoms);
	JDELETE_ARRAY_ARRAY(sprStatic, numStatics);
	JDELETE_ARRAY_ARRAY(sprMaker, numMakers);
	JDELETE_ARRAY_ARRAY(sprChaser, numChasers);
	JDELETE_ARRAY_ARRAY(sprGuest, numGuests);
	JDELETE_ARRAY_ARRAY(imgDialog, numDialogs);
	JDELETE_ARRAY_ARRAY(imgNarrative, numNarratives);
}
