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

/** Level editor for Holotz's Castle.
 * @file    HCed.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    30/05/2004
 * @version 0.0.1 - 30/05/2004 - First version
 * @version 0.0.2 - 30/05/2004 - Install support, Miriam Ruiz (Debian package). Load/Create story support.
 */

#include <HCed.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#ifndef HC_DATA_DIR
#define HC_DATA_DIR "res/"
#endif

#ifndef HCED_DATA_DIR
#define HCED_DATA_DIR "HCedHome/res/"
#endif

/** This application.
 */
HCed *theApp;
HCPreferences prefs;

void HCed::PrintUsage(char *program)
{
	fprintf(stderr, "HCed v1.3. (C) Juan Carlos Seijo Perez - 2004.\n\n");
	fprintf(stderr, "Usage: %s [-t themeName] [story name] [levelToLoad]", program);
	fprintf(stderr, " [-h] [-r numRows] [-c numColumns]");
	fprintf(stderr, " [-f]ullscreen [-w]indowed [--fps nnn] [-mWxHxBPP]\n");
	fprintf(stderr, "\n");
	exit(0);
}

void HCed::OnKeyUp(SDL_keysym key)
{
	if (theApp->state == HCEDSTATE_OPENSTORY)
	{
		if (key.sym == SDLK_ESCAPE)
		{
			// Ends opening/creating the a story
			theApp->OnFloor(0);
			theApp->inputNewStory = 0;
		}
		
		if (HCED_INPUT_STORY == theApp->inputNewStory)
		{
			bool upd = false;

			if ((key.sym >= SDLK_0 && key.sym <= SDLK_9) || 
					(key.sym >= SDLK_a && key.sym <= SDLK_z) ||
					key.sym == SDLK_MINUS)
			{
				if (key.sym == SDLK_MINUS && (theApp->KeyMods() & KMOD_SHIFT))
				{
					theApp->strNewStoryName += '_';
				}
				else
				{
					if ((theApp->KeyMods() & KMOD_SHIFT))
					{
						theApp->strNewStoryName += toupper(key.sym);
					}
					else
					{
						theApp->strNewStoryName += key.sym;
					}
				}

				upd = true;
			}
			else
			if (key.sym == SDLK_DELETE || key.sym == SDLK_BACKSPACE)
			{
				// Deletes last character
				if (theApp->strNewStoryName.Length() > 0)
				{
					JString str(theApp->strNewStoryName, 0, theApp->strNewStoryName.Length() - 1);
					theApp->strNewStoryName = str;
					upd = true;
				}
			}
			else
			if (key.sym == SDLK_RETURN || key.sym == SDLK_KP_ENTER)
			{
				theApp->inputNewStory = HCED_INPUT_THEME;

				// Updates the input string
				JDELETE(theApp->imgNewStory);
				SDL_Color fg = {0xff, 0xcc, 0x00, 0x00};
				SDL_Color bg = {0x00, 0x00, 0x00, 0x00};
				theApp->imgNewStory = theApp->fontLarge.RenderTextShaded("Select a theme", fg, bg);
			}
			
			if (upd)
			{
				// Updates the input string
				JDELETE(theApp->imgNewStory);
				SDL_Color fg = {0xff, 0xcc, 0x00, 0x00};
				SDL_Color bg = {0x00, 0x00, 0x00, 0x00};
				theApp->imgNewStory = theApp->fontLarge.RenderTextShaded((const char *)(JString("Name: ") + theApp->strNewStoryName), fg, bg);
			}
		}
		else
		if (HCED_INPUT_THEME == theApp->inputNewStory)
		{
			theApp->menuTheme->TrackKeyboard(key);
		}
		else
		{
			theApp->menuOpenStory->TrackKeyboard(key);
		}
		
		return;
	}
	
	switch (key.sym)
	{
		case SDLK_1:
			OnFloor(0);
			break;

		case SDLK_2:
			OnContFloor(0);
			break;

		case SDLK_3:
			OnLadder(0);
			break;

		case SDLK_4:
			OnBar(0);
			break;

		case SDLK_5:
			OnBreak(0);
			break;

		case SDLK_6:
			OnObject(0);
			break;

		case SDLK_7:
			OnRope(0);
			break;

		case SDLK_8:
			OnStart(0);
			break;

		case SDLK_9:
			OnExit(0);
			break;

		case SDLK_0:
			OnEnemy(0);
			break;

		case SDLK_s:
			OnSave(0);
			break;
			
		case SDLK_PAGEUP:
			switch (theApp->PrevLevel())
			{
			case -1:
				fprintf(stderr, "HCed: Error going to previous level.\n");
				break;
				
			case 1:
				fprintf(stderr, "HCed: This is the first level.\n");
				break;

			case 0:
			default:
				break;
			}
			break;
			
		case SDLK_PAGEDOWN:
			if (theApp->NextLevel() < 0)
			{
				fprintf(stderr, "HCed: Error going to next level.\n");
			}
			break;

		case SDLK_q:
			if (theApp->KeyMods() & KMOD_CTRL)
			{
				theApp->Exit();
			}
			break;

		default:
			break;
	}
}
 
void HCed::OnKeyDown(SDL_keysym key)
{
	switch (key.sym)
	{
		// Scrolls down the map
	case SDLK_UP:
		{
			HCMap *map = &theApp->level.Map();

			if (map->Y() < HCED_MARGIN)
			{
				theApp->level.Pos((s32)map->X(), 
													(s32)map->Y() + map->CellHeight());
			}
			else
			if (map->Y() > HCED_MARGIN && map->Height() > float(theApp->Height() - HCED_MARGIN))
			{
				theApp->level.Pos((s32)map->X(), HCED_MARGIN);
			}
		}
		break;

		// Scrolls up the map
	case SDLK_DOWN:
		{
			HCMap *map = &theApp->level.Map();

			if (map->Y() + map->Height() > theApp->Height())
			{
				theApp->level.Pos((s32)map->X(), 
													(s32)map->Y() - map->CellHeight());
			}
			else
			if (map->Y() + map->Height() > theApp->Height() &&
					map->Height() > theApp->Height() - HCED_MARGIN)
			{
				theApp->level.Pos((s32)map->X(), HCED_MARGIN);
			}
		}
		break;

		// Scrolls right the map
	case SDLK_LEFT:
		{
			HCMap *map = &theApp->level.Map();

			if (map->X() < HCED_MARGIN)
			{
				theApp->level.Pos((s32)map->X() + map->CellWidth(), 
													(s32)map->Y());
			}
			else
			if (map->X() > HCED_MARGIN && map->Width() > float(theApp->Width() - HCED_MARGIN))
			{
				theApp->level.Pos(HCED_MARGIN, (s32)map->Y());
			}
		}
		break;

		// Scrolls left the map
	case SDLK_RIGHT:
		{
			HCMap *map = &theApp->level.Map();

			if (map->X() + map->Width() > theApp->Width())
			{
				theApp->level.Pos((s32)map->X() - map->CellWidth(), 
													(s32)map->Y());
			}
			else
			if (map->X() + map->Width() > theApp->Width() &&
					map->Width() > theApp->Width() - HCED_MARGIN)
			{
				theApp->level.Pos(HCED_MARGIN, (s32)map->Y());
			}
		}
		break;

	case SDLK_KP_PLUS:
		{
			// Time to complete level up
			theApp->level.maxTime += 1;
			if (theApp->level.maxTime > 0xffff)
			{
				theApp->level.maxTime = 0xffff;
			}
			theApp->level.levelTimer.Init(theApp->level.maxTime, &theApp->fontNormal);
		}
		break;

	case SDLK_KP_MINUS:
		{
			// Time to complete level down
			theApp->level.maxTime -= 1;
			if (theApp->level.maxTime == 0)
			{
				theApp->level.maxTime = 1;
			}

			theApp->level.levelTimer.Init(theApp->level.maxTime, &theApp->fontNormal);
		}
		break;

	case SDLK_g:
		{
			if ((SDL_GetModState() & KMOD_SHIFT))
			{
				// Map gravity down
				theApp->level.map.Gravity(theApp->level.map.Gravity() - 0.1f);
			}
			else
			{
				// Map gravity up
				theApp->level.map.Gravity(theApp->level.map.Gravity() + 0.1f);
			}

			OnGravityChange();
		}
		break;

	case SDLK_x:
		{
			if ((SDL_GetModState() & KMOD_SHIFT))
			{
				// Main character's Vx down
				theApp->level.character.MaxVeloccity().x -= 0.1f;
			}
			else
			{
				// Main character's Vx up
				theApp->level.character.MaxVeloccity().x += 0.1f;
			}

			OnCharVxChange();
		}
		break;

	case SDLK_y:
		{
			if ((SDL_GetModState() & KMOD_SHIFT))
			{
				// Main character's Vy down
				theApp->level.character.MaxVeloccity().y -= 0.1f;
			}
			else
			{
				// Main character's Vy up
				theApp->level.character.MaxVeloccity().y += 0.1f;
			}

			OnCharVyChange();
		}
		break;

	case SDLK_j:
		{
			if ((SDL_GetModState() & KMOD_SHIFT))
			{
				// Main character's max jump rows down
				theApp->level.character.MaxJumpRows(theApp->level.character.MaxJumpRows() - 1);
			}
			else
			{
				// Main character's max jump rows up
				theApp->level.character.MaxJumpRows(theApp->level.character.MaxJumpRows() + 1);
			}

			OnCharJumpRowsChange();
		}
		break;

	case SDLK_r:
		{
			if ((SDL_GetModState() & KMOD_SHIFT))
			{
				if ((SDL_GetModState() & KMOD_CTRL))
				{
					// Decrease number of rows from the top
					theApp->level.map.Resize(theApp->level.map.Rows() - 1, theApp->level.map.Cols(), true, false);
				}
				else
				{
					// Decrease number of rows from the bottom
					theApp->level.map.Resize(theApp->level.map.Rows() - 1, theApp->level.map.Cols());
				}
			}
			else
			{
				if ((SDL_GetModState() & KMOD_CTRL))
				{
					// Increase number of rows from the top
					theApp->level.map.Resize(theApp->level.map.Rows() + 1, theApp->level.map.Cols(), true, false);
				}
				else
				{
					// Increase number of rows from the bottom
					theApp->level.map.Resize(theApp->level.map.Rows() + 1, theApp->level.map.Cols());
				}
			}

			OnMapSizeChange();
		}
		break;

	case SDLK_c:
		{
			if ((SDL_GetModState() & KMOD_SHIFT))
			{
				if ((SDL_GetModState() & KMOD_CTRL))
				{
					// Decrease number of columns from the left
					theApp->level.map.Resize(theApp->level.map.Rows(), theApp->level.map.Cols() - 1, false);
				}
				else
				{
					// Decrease number of columns from the right
					theApp->level.map.Resize(theApp->level.map.Rows(), theApp->level.map.Cols() - 1);
				}
			}
			else
			{
				if ((SDL_GetModState() & KMOD_CTRL))
				{
					// Increase number of columns from the left
					theApp->level.map.Resize(theApp->level.map.Rows(), theApp->level.map.Cols() + 1, false);
				}
				else
				{
					// Increase number of columns from the right
					theApp->level.map.Resize(theApp->level.map.Rows(), theApp->level.map.Cols() + 1);
				}
			}

			OnMapSizeChange();
		}
		break;

  default:
    break;
	}
}

void HCed::OnMouseUp(s32 bt, s32 x, s32 y)
{
	switch (theApp->state)
	{
	case HCEDSTATE_FLOOR:
		theApp->menuFloorSubtype->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_CONTFLOOR:
		theApp->menuContFloorSubtype->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_LADDER:
		theApp->menuLadderSubtype->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_BAR:
		theApp->menuBarSubtype->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_BREAK:
		theApp->menuBreakSubtype->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_OBJECT:
		theApp->curObject = 0;
		theApp->menuObjectSubtype->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_ROPE:
		theApp->curRope = 0;
		theApp->menuRopePeriod->TrackMouse(bt, x, y);
		theApp->menuRopeSubtype->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_START:
		theApp->menuMainSubtype->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_EXIT:
		break;
	case HCEDSTATE_ENEMY:
		theApp->curEnemy = 0;
		theApp->menuEnemyType->TrackMouse(bt, x, y);
		theApp->menuEnemySubtype[theApp->enemyType]->TrackMouse(bt, x, y);
		theApp->menuEnemyParam1->TrackMouse(bt, x, y);
		theApp->menuEnemyParam2->TrackMouse(bt, x, y);
		break;
	case HCEDSTATE_SAVE:
		break;

	case HCEDSTATE_OPENSTORY:
		switch (theApp->inputNewStory)
		{
		case 0:
			theApp->menuOpenStory->TrackMouse(bt, x, y);
			break;
			
		case HCED_INPUT_THEME:
			theApp->menuTheme->TrackMouse(bt, x, y);
			break;
			
		default:
			break;
		}
		break;

	case HCEDSTATE_APPEXIT:
		break;
	default:
		break;
	}

	theApp->menuMain->TrackMouse(bt, x, y);
}

void HCed::OnMouseDown(s32 bt, s32 x, s32 y)
{
	switch (theApp->state)
	{
	case HCEDSTATE_OBJECT:
		theApp->curObject = 0;
		HCObject *obj;

		// Checks for objects bellow the mouse cursor
		for (s32 i = 0;	i < theApp->level.numObjects; ++i)
		{
			obj = theApp->level.objects[i];

			if (theApp->MouseX() > obj->X() - obj->Normal().MaxW()/2 && 
					theApp->MouseX() < obj->X() + obj->Normal().MaxW()/2 &&
					theApp->MouseY() > obj->Y() - obj->Normal().MaxH() && 
					theApp->MouseY() < obj->Y())
			{
				theApp->curObject = theApp->level.objects[i];
			}
		}
		break;

	case HCEDSTATE_ROPE:
		theApp->curRope = 0;

		// Checks for ropes bellow the mouse cursor
		for (s32 i = 0;	i < theApp->level.numRopes; ++i)
		{
			if (theApp->MouseX() > (theApp->level.ropes[i])->X() - theApp->level.Map().CellWidth()/2 && 
					theApp->MouseX() < (theApp->level.ropes[i])->X() + theApp->level.Map().CellWidth()/2 &&
					theApp->MouseY() > (theApp->level.ropes[i])->Y() && 
					theApp->MouseY() < (theApp->level.ropes[i])->Y() + theApp->level.Map().CellHeight())
			{
				theApp->curRope = theApp->level.ropes[i];
			}
		}

		if (theApp->curRope)
		{
			// Sets the period
			if (theApp->ropePeriod != theApp->curRope->Period())
			{
				theApp->curRope->Init(theApp->ropePeriod, 
															theApp->curRope->Amplitude(), 
															theApp->curRope->Length(), 
															theApp->level.Theme());
			}
		}
		break;

	case HCEDSTATE_ENEMY:
		theApp->curEnemy = 0;

		// Checks for enemies bellow the mouse cursor
		for (s32 i = 0; i < theApp->level.numEnemies; ++i)
		{
			if (theApp->MouseX() > (theApp->level.enemies[i])->X() - (theApp->level.enemies[i]->states[HCCS_STOP].MaxW()/2) && 
					theApp->MouseX() < (theApp->level.enemies[i])->X() + (theApp->level.enemies[i]->states[HCCS_STOP].MaxW()/2) && 
					theApp->MouseY() > (theApp->level.enemies[i])->Y() - (theApp->level.enemies[i]->states[HCCS_STOP].MaxH()/2) && 
					theApp->MouseY() < (theApp->level.enemies[i])->Y())
			{
				theApp->curEnemy = theApp->level.enemies[i];
			}
		}
		break;

	default:
		break;
	}
}

int HCed::ParseArg(char *args[], int argc)
{
	printf("Parsing %d args %s\n", argc, args[0]);
	if (args[0][0] != '-')
	{
		// Argument without score, treat it as the story name
		storyName = args[0];

		if (argc < 2)
		{
			levelNumber = 1;
			
			return 0; // no aditional arguments needed
		}
		
		levelNumber = atoi(args[1]);
			
		if (levelNumber <= 0)
		{
			levelNumber = 1;
			fprintf(stderr, "HCed: The level number must be greater than 0, defaulting to 1.\n");
		}
		return 1; // 1 aditional argument used
	}
	
	switch (args[0][1])
	{
		// '-t themeName' option
		case 't':
			if (argc<2)
				return -2;
			themeName = args[1];
			return 1;
			
		// '-r numRows' option
		case 'r':
			if (argc<2)
				return -2;
			defRows = atoi(args[1]);
				
			if (defRows <= 0)
			{
				defRows = 10;
			}
			return 1;

		// '-c numCols' option
		case 'c':
			if (argc<2)
				return -2;
			defCols = atoi(args[1]);
				
			if (defCols <= 0)
			{
				defCols = 10;
			}
			return 1;
	}

	return JApp::ParseArg(args, argc);
}

void HCed::ParseArgs(s32 argc, char **argv)
{
	JApp::ParseArgs(argc, argv);
	
	char str[4096];

#ifndef _WIN32	
	char *home;
	home = getenv("HOME");
	if (home != NULL)
	{
		snprintf(str, sizeof(str), "%s/.holotz-castle", home);
		mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		snprintf(str, sizeof(str), "%s/.holotz-castle/stories", home);
		mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		snprintf(str, sizeof(str), "%s/.holotz-castle/stories/%s", home, storyName.Str());
		mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
#endif
	
	if (!HCUtil::FindFile("stories"))
	{
		fprintf(stderr, "Directory 'stories' not found. Check manual.\n");
		exit(-1);
	}
	else
	{
		// Assigns this directory as the default working directory
		storyDir = HCUtil::File();
		storyDir += "/";
	}

	if (levelNumber == -1)
	{
		// Selects the next file name from the requested story
		levelNumber = 1;

		snprintf(str, sizeof(str), "%s%s/level%03d.hlv", storyDir.Str(), storyName.Str(), levelNumber);
		while (JFile::Exists(str))
		{
			snprintf(str, sizeof(str), "%s%s/level%03d.hlv", storyDir.Str(), storyName.Str(), ++levelNumber);
		}

		filename = str;
	}
	else
	{
		// Sets the name of the requested story and level number.
		snprintf(str, sizeof(str), "%s%s/level%03d.hlv", storyDir.Str(), storyName.Str(), levelNumber);
		filename = str;
	}

	OnFilenameChange();
}

HCed::HCed() : JApp("HCed v1.1", HCED_REFAPPWIDTH, HCED_REFAPPHEIGHT, false)
{
	state = HCEDSTATE_FLOOR;
	SetOnMouseUp(&OnMouseUp);
	SetOnMouseDown(&OnMouseDown);
	SetOnKeyUp(&OnKeyUp);
	SetOnKeyDown(&OnKeyDown);
	doInput = true;
	curObject = 0;
	curRope = 0;
	curEnemy = 0;
	defRows = defCols = 10;
	enemyParam1 = 1;
	enemyParam2 = 1;
	imgNewStory = 0;
	imgGravity = 0;
	imgCharVx = 0;
	imgCharVy = 0;
	imgCharJumpRows = 0;
	imgMapSize = 0;
	levelNumber = -1;
	menuMain = 0;
	menuEnemyType = 0;
	menuEnemyParam1 = 0;
	menuEnemyParam2 = 0;
	menuOpenStory = 0;
	menuTheme = 0;
	menuRopePeriod = 0;
	menuMainSubtype = 0;
	menuBreakSubtype = 0;
	menuObjectSubtype = 0;
	menuFloorSubtype = 0;
	menuContFloorSubtype = 0;
	menuBarSubtype = 0;
	menuLadderSubtype = 0;
	menuRopeSubtype = 0;
	appBackColor = 0;

	for (s32 i = 0; i < HCENEMYTYPE_COUNT; ++i)
	{
		menuEnemySubtype[i] = 0;
	}

	storyName = "unknown";
	themeName = "default";
	inputNewStory = 0;
}

s32 HCed::LoadLevel()
{
	// If the level exists, loads it
	bool ok = false;

	// Sets the timer font
	level.SetTimerFont(&fontNormal);

	if (JFile::Exists(filename))
	{
		JRW f;
		if (f.Create(filename, "rb"))
		{
			if (0 == level.Load(f, filename))
			{
				if (InitLoadedLevel())
				{
					ok = true;
				}
				else
				{
					fprintf(stderr, "Could not init the loaded level.\n");
				}
			}
			else
			{
				fprintf(stderr, "Could not load level from %s.\n", filename.Str());
			}
		}
		else
		{
			fprintf(stderr, "Could not open file %s for reading.\n", filename.Str());
		}
	}
	else
	{
		fprintf(stderr, "HCed: File %s doesn't exist, creating a new level.\n", filename.Str());
	}

	// If an error occurred or the file didn't exist, tries to load the default level/theme
	if (!ok)
	{
		if (!InitDefaultLevel())
		{
			fprintf(stderr, "Could not init default level.\n");
			return -1;
		}
	}

	// Places the level
	level.Pos(HCED_MARGIN, HCED_MARGIN);

	// Prepares the current gravity and main character's parameters to be shown
	OnGravityChange();
	OnCharVxChange();
	OnCharVyChange();
	OnCharJumpRowsChange();
	OnMapSizeChange();

	// Types and theme subtypes
	enemyType = HCENEMYTYPE_BALL;
	enemySubtype = 0;
	floorSubtype = 0;
	contFloorSubtype = 0;
	breakSubtype = 0;
	barSubtype = 0;
	ladderSubtype = 0;
	objectSubtype = 0;
	ropeSubtype = 0;
	ropePeriod = 0;
	
	// Re-initializes level/theme dependant menus
	return InitLevelMenus();
}

s32 HCed::InitLevelMenus()
{
	imgMouse = 0;

	// Inits the main character subtype menu
	if (!InitMainSubtypeMenu())
	{
		fprintf(stderr, "Failed to init main character's subtype menu.\n");
		
		return -1;
	}

	// Inits the break subtypes menu
	if (!InitBreakSubtypeMenu())
	{
		fprintf(stderr, "Failed to init break subtypes menu.\n");
		
		return -1;
	}

	// Inits the object subtypes menu
	if (!InitObjectSubtypeMenu())
	{
		fprintf(stderr, "Failed to init object subtypes menu.\n");
		
		return -1;
	}

	// Inits the enemy representations menu
	if (!InitEnemySubtypeMenu())
	{
		fprintf(stderr, "Failed to init enemy subtypes menu.\n");
		
		return -1;
	}

	// Inits the floor subtypes menu
	if (!InitFloorSubtypeMenu())
	{
		fprintf(stderr, "Failed to init floor subtypes menu.\n");
		
		return false;
	}


	// Inits the continuous floor subtypes menu
	if (!InitContFloorSubtypeMenu())
	{
		fprintf(stderr, "Failed to init floor subtypes menu.\n");
		
		return false;
	}

	// Inits the bar subtypes menu
	if (!InitBarSubtypeMenu())
	{
		fprintf(stderr, "Failed to init bar subtypes menu.\n");
		
		return false;
	}

	// Inits the ladder subtypes menu
	if (!InitLadderSubtypeMenu())
	{
		fprintf(stderr, "Failed to init ladder subtypes menu.\n");
		
		return false;
	}

	// Inits the rope subtypes menu
	if (!InitRopeSubtypeMenu())
	{
		fprintf(stderr, "Failed to init rope subtypes menu.\n");
		
		return false;
	}

	// Inits the open story menu
	if (!InitOpenStoryMenu())
	{
		fprintf(stderr, "Failed to init stories menu.\n");
		
		return false;
	}

	// Inits the theme subtypes menu
	if (!InitThemeMenu())
	{
		fprintf(stderr, "Failed to init stories menu.\n");
		
		return false;
	}

	return 0;
}

s32 HCed::NextLevel()
{
	char str[256];

	// Sets the name of the story with the next level number.
	snprintf(str, sizeof(str), "%s%s/level%03d.hlv", storyDir.Str(), storyName.Str(), ++levelNumber);
	filename = str;

	if (0 == LoadLevel())
	{
		themeName = level.Theme().Name();
		OnFilenameChange();

		if (0 == InitLevelMenus())
		{
			OnFloor(this);

			return 0;
		}
	}

	return -1;
}

s32 HCed::PrevLevel()
{
	if (levelNumber > 1)
	{
		char str[256];
		
		// Sets the name of the story with the next level number.
		snprintf(str, sizeof(str), "%s%s/level%03d.hlv", storyDir.Str(), storyName.Str(), --levelNumber);
		filename = str;
		
		if (0 == LoadLevel())
		{
			OnFilenameChange();

			if (0 == InitLevelMenus())
			{
				OnFloor(this);

				return 0;
			}
		}

		return -1;
	}

	// No more levels before
	return 1;
}

s32 HCed::NewLevel()
{
	if (0 == LoadLevel())
	{
		OnFilenameChange();
		
		if (0 == InitLevelMenus())
		{
			OnFloor(this);
			
			return 0;
		}
	}

	return -1;
}

bool HCed::Init(int argc, char **argv)
{
	// Parse standard JApp args and HCed args
	ParseArgs(argc, argv);

	// Inits base system
	if (!JApp::Init())
	{
		fprintf(stderr, "Failed to init base system. Check manual.\n");

		return false;
	}
	
	// Loads preferences
  // 	if (0 != prefs.Load())
  // 	{
  // 		fprintf(stderr, "Error loading preferences. Check manual.\n");
  // 	}
	
	// In edition mode the time must be that of the highest level of difficulty
	prefs.Difficulty(HCPREFERENCES_HARD);
	
	// Initializes fonts
	if (!JFile::Exists(HC_DATA_DIR "font/font.ttf"))
	{
		fprintf(stderr, 
						"Could not find data directory.\n\n"
						"Possible solutions are:\n"
						" - Open folder JLib-1.3.1/Games/HolotzCastle and double.\n"
						"   click 'holotz-castle' application icon.\n"
						" - Maybe you did 'make' but didn't do 'make install'.\n"
						" - Else, try to reinstall the game.\n");
		return false;
	}

	if (!fontSmall.Open(HC_DATA_DIR "font/font.ttf", 12) ||
			!fontNormal.Open(HC_DATA_DIR "font/font.ttf", 18) ||
			!fontLarge.Open(HC_DATA_DIR "font/font.ttf", 24))
	{
		fprintf(stderr, "Couldn't open fonts. Check manual.\n");
		return false;
	}

	// Loads the level
	if (0 != LoadLevel())
	{
		fprintf(stderr, "Couldn't load level. Check manual.\n");
		return false;
	}

	// Inits the main menu
	if (!InitMainMenu())
	{
		fprintf(stderr, "Failed to init main menu.\n");
		
		return false;
	}

	// Inits the level dependant menus
	if (0 != InitLevelMenus())
	{
		fprintf(stderr, "Failed to init level menus.\n");
		
		return false;
	}

	// Inits the rope periods menu
	if (!InitRopePeriodMenu())
	{
		fprintf(stderr, "Failed to init rope periods menu.\n");
		
		return false;
	}

	// Inits the enemy types menu
	if (!InitEnemyTypeMenu())
	{
		fprintf(stderr, "Failed to init enemy types menu.\n");
		
		return false;
	}

	// Inits the enemy param 1 menu
	if (!InitEnemyParam1Menu())
	{
		fprintf(stderr, "Failed to init enemy param 1 menu.\n");
		
		return false;
	}

	// Inits the enemy param 2 menu
	if (!InitEnemyParam2Menu())
	{
		fprintf(stderr, "Failed to init enemy param 2 menu.\n");
		
		return false;
	}

	appBackColor = SDL_MapRGB(screen->format, 0x00, 0x00, 0x07);

	OnFloor(this);

	return true;
}

bool HCed::InitLoadedLevel()
{
	// Nothing for the moment
	return true;
}

bool HCed::InitDefaultLevel()
{
	// Destroy a previous level
	level.Destroy();

	// Sets the timer font
	level.SetTimerFont(&fontNormal);
	
	// Loads the theme
	printf("Using theme %s\n", themeName.Str());
	if (!level.Theme().Load(themeName.Str()))
	{
		fprintf(stderr, "Error loading theme.\n");
		return false;
	}
	
	// Initializes the map
	level.Map().Init(level.Theme());
	level.Map().Resize(defRows, defCols);
	level.Map().CellWidth(level.Theme().Floor(0).Width());
	level.Map().CellHeight(level.Theme().Floor(0).Height());
	
	// Initializes the character
	level.character.Pos(20, 20);

	if (!level.Init())
	{
		fprintf(stderr, "Error initializing level.\n");
		return false;
	}

	return true;
}

bool HCed::InitMainMenu()
{
	JDELETE(menuMain);
	menuMain = new JImageMenu;

	char str[256];
	
	for (s32 i = 0; i < HCEDSTATE_COUNT; ++i)
	{
		snprintf(str, sizeof(str), HCED_DATA_DIR "MainMenu/%d.tga", i);
		//fprintf(stderr, "Loading UI Item " HCED_DATA_DIR "MainMenu/%d.tga\n", i);
		
		if (!imgMenu[i].Load(str))
		{
			return false;
		}
		
		snprintf(str, sizeof(str), HCED_DATA_DIR "MainMenu/%d_hi.tga", i);
		//fprintf(stderr, "Loading UI Item " HCED_DATA_DIR "MainMenu/%d_hi.tga\n", i);

		if (!imgMenuOver[i].Load(str))
		{
			return false;
		}
	}

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuMain->Menu();

	it->Data(new JImageMenuEntry(&imgMenu[HCEDSTATE_FLOOR], 
															 &imgMenuOver[HCEDSTATE_FLOOR], &OnFloor, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_CONTFLOOR], 
																		&imgMenuOver[HCEDSTATE_CONTFLOOR], &OnContFloor, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_LADDER], 
																		&imgMenuOver[HCEDSTATE_LADDER], &OnLadder, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_BAR], 
																		&imgMenuOver[HCEDSTATE_BAR], &OnBar, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_BREAK], 
																		&imgMenuOver[HCEDSTATE_BREAK], &OnBreak, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_OBJECT], 
																		&imgMenuOver[HCEDSTATE_OBJECT], &OnObject, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_ROPE], 
																		&imgMenuOver[HCEDSTATE_ROPE], &OnRope, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_START], 
																		&imgMenuOver[HCEDSTATE_START], &OnStart, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_EXIT], 
																		&imgMenuOver[HCEDSTATE_EXIT], &OnExit, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_ENEMY], 
																		&imgMenuOver[HCEDSTATE_ENEMY], &OnEnemy, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_SAVE], 
																		&imgMenuOver[HCEDSTATE_SAVE], &OnSave, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_OPENSTORY], 
																		&imgMenuOver[HCEDSTATE_OPENSTORY], &OnOpenStory, this));
	it->AddNodeGo(new JImageMenuEntry(&imgMenu[HCEDSTATE_APPEXIT], 
																		&imgMenuOver[HCEDSTATE_APPEXIT], &OnAppExit, this));
	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_LEFT;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	
	if (!menuMain->Init(cfg))
	{
		return false;
	}

	menuMain->Pos(0, 0);
	menuMain->Menu()->Root();
	
	return true;
}

bool HCed::InitMainSubtypeMenu()
{
	DestroyMainSubtypeMenu();
	menuMainSubtype = new JImageMenu;

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuMainSubtype->Menu();
	it->Root();

	it->Data(new JImageMenuEntry(new JImage(*((JImage*)level.theme.MainChar(0)[HCCDT_STOP].Frame(0))), 
															 new JImage(*((JImage*)level.theme.MainChar(0)[HCCDT_LEFT].Frame(0))), 
															 &OnMainSubtype, (void *)0));

	// Adds the subtypes
	for (s32 i = 1; i < level.theme.NumMainChars(); ++i)
	{
		it->AddNodeGo(new JImageMenuEntry(new JImage(*((JImage*)level.theme.MainChar(i)[HCCDT_STOP].Frame(0))), 
																			new JImage(*((JImage*)level.theme.MainChar(i)[HCCDT_LEFT].Frame(0))), 
																			&OnMainSubtype, JCAST_S32_TO_VOIDPTR(i)));
	}

	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	menuMainSubtype->Pos(HCED_MARGIN, 0);
		
	if (!menuMainSubtype->Init(cfg))
	{
		return false;
	}
		
	menuMainSubtype->Menu()->Root();
	
	return true;
}

bool HCed::InitFloorSubtypeMenu()
{
	JDELETE(menuFloorSubtype);
	menuFloorSubtype = new JImageMenu;
	
	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuFloorSubtype->Menu();
	
	// At least we must have this
	it->Data(new JImageMenuEntry(&level.Theme().Floor(0), &level.Theme().Floor(0), &OnFloorSubtype, (void *)0));

	// Adds the rest of subtypes
	for (s32 i = 1; i < level.Theme().NumFloors(); ++i)
	{
		it->AddNodeGo(new JImageMenuEntry(&level.Theme().Floor(i), &level.Theme().Floor(i), &OnFloorSubtype, JCAST_S32_TO_VOIDPTR(i)));
	}

	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	
	if (!menuFloorSubtype->Init(cfg))
	{
		fprintf(stderr, "Error\n");
		return false;
	}

	menuFloorSubtype->Pos(HCED_MARGIN, 0);
	menuFloorSubtype->Menu()->Root();
	
	return true;
}
	
bool HCed::InitContFloorSubtypeMenu()
{
	JDELETE(menuContFloorSubtype);
	menuContFloorSubtype = new JImageMenu;

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuContFloorSubtype->Menu();
	
	// At least we must have this
	it->Data(new JImageMenuEntry(&level.Theme().ContFloor(0)[HCFDT_I], &level.Theme().ContFloor(0)[HCFDT_I], &OnContFloorSubtype, (void *)0));

	// Adds the rest of subtypes
	for (s32 i = 1; i < level.Theme().NumContFloors(); ++i)
	{
		it->AddNodeGo(new JImageMenuEntry(&level.Theme().ContFloor(i)[HCFDT_I], &level.Theme().ContFloor(i)[HCFDT_I], &OnContFloorSubtype, JCAST_S32_TO_VOIDPTR(i)));
	}

	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	
	if (!menuContFloorSubtype->Init(cfg))
	{
		fprintf(stderr, "Error\n");
		return false;
	}

	menuContFloorSubtype->Pos(HCED_MARGIN, 0);
	menuContFloorSubtype->Menu()->Root();
	
	return true;
}
	
bool HCed::InitBreakSubtypeMenu()
{
	DestroyBreakSubtypeMenu();
	menuBreakSubtype = new JImageMenu;

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuBreakSubtype->Menu();
	it->Root();
	
	// At least we must have this
	it->Data(new JImageMenuEntry(new JImage(*((JImage*)level.Theme().Break(0)[HCBDT_NORMAL].Frame(0))), 
															 new JImage(*((JImage*)level.Theme().Break(0)[HCBDT_NORMAL].Frame(0))), 
															 &OnBreakSubtype, (void *)0));

	// Adds the rest of subtypes
	for (s32 i = 1; i < level.Theme().NumBreaks(); ++i)
	{
		it->AddNodeGo(new JImageMenuEntry(new JImage(*((JImage*)level.Theme().Break(i)[HCBDT_NORMAL].Frame(0))), 
																			new JImage(*((JImage*)level.Theme().Break(i)[HCBDT_NORMAL].Frame(0))), 
																			&OnBreakSubtype, JCAST_S32_TO_VOIDPTR(i)));
	}

	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	menuBreakSubtype->Pos(HCED_MARGIN, 0);

	if (!menuBreakSubtype->Init(cfg))
	{
		return false;
	}

	menuBreakSubtype->Menu()->Root();
	
	return true;
}
	
bool HCed::InitBarSubtypeMenu()
{
	JDELETE(menuBarSubtype);
	menuBarSubtype = new JImageMenu;

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuBarSubtype->Menu();
	
	// At least we must have this
	it->Data(new JImageMenuEntry(&level.Theme().Bar(0), &level.Theme().Bar(0), &OnBarSubtype, (void *)0));

	// Adds the rest of subtypes
	for (s32 i = 1; i < level.Theme().NumBars(); ++i)
	{
		it->AddNodeGo(new JImageMenuEntry(&level.Theme().Bar(i), &level.Theme().Bar(i), &OnBarSubtype, JCAST_S32_TO_VOIDPTR(i)));
	}

	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	menuBarSubtype->Pos(HCED_MARGIN, 0);

	if (!menuBarSubtype->Init(cfg))
	{
		return false;
	}

	menuBarSubtype->Menu()->Root();
	
	return true;
}
	
bool HCed::InitLadderSubtypeMenu()
{
	JDELETE(menuLadderSubtype);
	menuLadderSubtype = new JImageMenu;

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuLadderSubtype->Menu();
	
	// At least we must have this
	it->Data(new JImageMenuEntry(&level.Theme().Ladder(0), &level.Theme().Ladder(0), &OnLadderSubtype, (void *)0));

	// Adds the rest of subtypes
	for (s32 i = 1; i < level.Theme().NumLadders(); ++i)
	{
		it->AddNodeGo(new JImageMenuEntry(&level.Theme().Ladder(i), &level.Theme().Ladder(i), &OnLadderSubtype, JCAST_S32_TO_VOIDPTR(i)));
	}

	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	menuLadderSubtype->Pos(HCED_MARGIN, 0);

	if (!menuLadderSubtype->Init(cfg))
	{
		return false;
	}

	menuLadderSubtype->Menu()->Root();
	
	return true;
}
	
bool HCed::InitObjectSubtypeMenu()
{
	DestroyObjectSubtypeMenu();
	menuObjectSubtype = new JImageMenu;

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuObjectSubtype->Menu();
	it->Root();
	
	// At least we must have this
	it->Data(new JImageMenuEntry(new JImage(*((JImage*)level.Theme().Object(0)[HCODT_NORMAL].Frame(0))), 
															 new JImage(*((JImage*)level.Theme().Object(0)[HCODT_ACQUIRED].Frame(0))), 
															 &OnObjectSubtype, 
															 (void *)0));

	// Adds the rest of subtypes
	for (s32 i = 1; i < level.Theme().NumObjects(); ++i)
	{
		it->AddNodeGo(new JImageMenuEntry(new JImage(*((JImage*)level.Theme().Object(i)[HCODT_NORMAL].Frame(0))), 
																			new JImage(*((JImage*)level.Theme().Object(i)[HCODT_ACQUIRED].Frame(0))), 
																			&OnObjectSubtype, 
																			JCAST_S32_TO_VOIDPTR(i)));
	}

	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	menuObjectSubtype->Pos(HCED_MARGIN, 0);

	if (!menuObjectSubtype->Init(cfg))
	{
		return false;
	}

	menuObjectSubtype->Menu()->Root();
	
	return true;
}
	
bool HCed::InitRopeSubtypeMenu()
{
	JDELETE(menuRopeSubtype);
	menuRopeSubtype = new JImageMenu;

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuRopeSubtype->Menu();
	
	// At least we must have this
	it->Data(new JImageMenuEntry(&level.Theme().Rope(0)[HCRDT_TOP], &level.Theme().Rope(0)[HCRDT_TOP], &OnRopeSubtype, (void *)0));

	// Adds the rest of subtypes
	for (s32 i = 1; i < level.Theme().NumRopes(); ++i)
	{
		it->AddNodeGo(new JImageMenuEntry(&level.Theme().Rope(i)[HCRDT_TOP], &level.Theme().Rope(i)[HCRDT_TOP], &OnRopeSubtype, JCAST_S32_TO_VOIDPTR(i)));
	}

	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
		menuRopeSubtype->Pos(HCED_MARGIN, 0);

	if (!menuRopeSubtype->Init(cfg))
	{
		return false;
	}

	menuRopeSubtype->Menu()->Root();
	
	return true;
}

bool HCed::InitRopePeriodMenu()
{
	JDELETE(menuRopePeriod);
	menuRopePeriod = new JTextMenu;

	// Crea el menú
	JTree<JTextMenuEntry *>::Iterator *it = menuRopePeriod->Menu();

	it->Data(new JTextMenuEntry("1", &OnRopePeriod, (void *)1));
	it->AddNodeGo(new JTextMenuEntry("2", &OnRopePeriod, (void *)2));
	it->AddNodeGo(new JTextMenuEntry("3", &OnRopePeriod, (void *)3));
	it->AddNodeGo(new JTextMenuEntry("4", &OnRopePeriod, (void *)4));
	it->AddNodeGo(new JTextMenuEntry("5", &OnRopePeriod, (void *)5));
	it->AddNodeGo(new JTextMenuEntry("6", &OnRopePeriod, (void *)6));
	it->AddNodeGo(new JTextMenuEntry("7", &OnRopePeriod, (void *)7));
	it->AddNodeGo(new JTextMenuEntry("8", &OnRopePeriod, (void *)8));
	it->AddNodeGo(new JTextMenuEntry("9", &OnRopePeriod, (void *)9));
	it->AddNodeGo(new JTextMenuEntry("10", &OnRopePeriod, (void *)10));
	it->Root();

	JTextMenuConfig cfg;
	cfg.font = &fontSmall;
	cfg.color.r = 0x00;
	cfg.color.g = 0x00;
	cfg.color.b = 0x00;
	cfg.backColor.r = 0xff;
	cfg.backColor.g = 0xcc;
	cfg.backColor.b = 0x00;
	cfg.hiColor.r = 0xff;
	cfg.hiColor.g = 0xff;
	cfg.hiColor.b = 0xff;
	cfg.hiBackColor.r = 0xff;
	cfg.hiBackColor.g = 0xdd;
	cfg.hiBackColor.b = 0x00;
	cfg.renderMode = JTEXTMENU_SHADED;
	cfg.layout = JTEXTMENU_SAMELINE;
	cfg.trackKeyboard = true;
	cfg.trackMouse = true;
	cfg.autoEnter = true;
	
	if (!menuRopePeriod->Init(cfg))
	{
		return false;
	}
		
	menuRopePeriod->Pos(Width() - (12 * HCED_MARGIN), 0);
	menuRopePeriod->Menu()->Root();

	return true;
}

bool HCed::InitEnemySubtypeMenu()
{
	DestroyEnemySubtypeMenu();

	for (s32 n = 0; n < HCENEMYTYPE_COUNT; ++n)
	{
		menuEnemySubtype[n] = new JImageMenu;
		
		// Adds the options
		JTree<JImageMenuEntry *>::Iterator *it = menuEnemySubtype[n]->Menu();
		it->Root();

		it->Data(new JImageMenuEntry(new JImage(*((JImage*)GetEnemySprites((HCEnemyType)n, 0)[HCCDT_STOP].Frame(0))), 
																 new JImage(*((JImage*)GetEnemySprites((HCEnemyType)n, 0)[HCCDT_STOP].Frame(0))), 
																 &OnEnemySubtype, (void *)0));

		// Adds the subtypes
		for (s32 i = 1; i < GetNumEnemySprites((HCEnemyType)n); ++i)
		{
			// Scales the normal and highlighted images to fit 20 and 32 pixels, respectively
			it->AddNodeGo(new JImageMenuEntry(new JImage(*((JImage*)GetEnemySprites((HCEnemyType)n, i)[HCCDT_STOP].Frame(0))), 
																				new JImage(*((JImage*)GetEnemySprites((HCEnemyType)n, i)[HCCDT_STOP].Frame(0))), 
																				&OnEnemySubtype, JCAST_S32_TO_VOIDPTR(i)));
		}

		it->Root();

		JImageMenuConfig cfg;
		cfg.layout = JIMAGEMENU_SAMELINE;
		cfg.trackMouse = true;
		cfg.trackKeyboard = false;
		cfg.autoEnter = false;
		menuEnemySubtype[n]->Pos(HCED_MARGIN, 0);
		
		if (!menuEnemySubtype[n]->Init(cfg))
		{
			return false;
		}
		
		menuEnemySubtype[n]->Menu()->Root();
	}
	
	return true;
}

bool HCed::InitEnemyTypeMenu()
{
	char str[256];
	
	if (!menuEnemyType)
	{
		for (s32 i = 0; i < HCENEMYTYPE_COUNT; ++i)
		{
			snprintf(str, sizeof(str), HCED_DATA_DIR "EnemyMenu/%d.tga", i);
			//fprintf(stderr, "Loading UI Item " HCED_DATA_DIR "EnemyMenu/%d.tga\n", i);
			
			if (!imgEnemy[i].Load(str))
			{
				return false;
			}
			
			snprintf(str, sizeof(str), HCED_DATA_DIR "EnemyMenu/%d_hi.tga", i);
			//fprintf(stderr, "Loading UI Item " HCED_DATA_DIR "EnemyMenu/%d_hi.tga\n", i);
			
			if (!imgEnemyOver[i].Load(str))
			{
				return false;
			}
		}
	}
	
	JDELETE(menuEnemyType);
	menuEnemyType = new JImageMenu;

	// Adds the options
	JTree<JImageMenuEntry *>::Iterator *it = menuEnemyType->Menu();

	it->Data(new JImageMenuEntry(&imgEnemy[HCENEMYTYPE_BALL], 
															 &imgEnemyOver[HCENEMYTYPE_BALL], 
															 &OnEnemyType, (void *)HCENEMYTYPE_BALL));
	it->AddNodeGo(new JImageMenuEntry(&imgEnemy[HCENEMYTYPE_RANDOM], 
																		&imgEnemyOver[HCENEMYTYPE_RANDOM], 
																		&OnEnemyType, (void *)HCENEMYTYPE_RANDOM));
	it->AddNodeGo(new JImageMenuEntry(&imgEnemy[HCENEMYTYPE_STATIC], 
																		&imgEnemyOver[HCENEMYTYPE_STATIC], 
																		&OnEnemyType, (void *)HCENEMYTYPE_STATIC));
	it->AddNodeGo(new JImageMenuEntry(&imgEnemy[HCENEMYTYPE_MAKER], 
																		&imgEnemyOver[HCENEMYTYPE_MAKER], 
																		&OnEnemyType, (void *)HCENEMYTYPE_MAKER));
	it->AddNodeGo(new JImageMenuEntry(&imgEnemy[HCENEMYTYPE_CHASER], 
																		&imgEnemyOver[HCENEMYTYPE_CHASER], 
																		&OnEnemyType, (void *)HCENEMYTYPE_CHASER));
	it->Root();

	JImageMenuConfig cfg;
	cfg.layout = JIMAGEMENU_SAMELINE;
	cfg.trackMouse = true;
	cfg.trackKeyboard = false;
	cfg.autoEnter = false;
	menuEnemyType->Pos(Width() - (5 * HCED_MARGIN), 0);

	if (!menuEnemyType->Init(cfg))
	{
		return false;
	}

	menuEnemyType->Menu()->Root();

	return true;
}

bool HCed::InitEnemyParam1Menu()
{
	JDELETE(menuEnemyParam1);
	menuEnemyParam1 = new JTextMenu;

	// Crea el menú
	JTree<JTextMenuEntry *>::Iterator *it = menuEnemyParam1->Menu();

	it->Data(new JTextMenuEntry(" 1 ", &OnEnemyParam1, (void *)1));
	it->AddNodeGo(new JTextMenuEntry(" 2 ", &OnEnemyParam1, (void *)2));
	it->AddNodeGo(new JTextMenuEntry(" 3 ", &OnEnemyParam1, (void *)3));
	it->AddNodeGo(new JTextMenuEntry(" 4 ", &OnEnemyParam1, (void *)4));
	it->AddNodeGo(new JTextMenuEntry(" 5 ", &OnEnemyParam1, (void *)5));
	it->AddNodeGo(new JTextMenuEntry(" 6 ", &OnEnemyParam1, (void *)6));
	it->AddNodeGo(new JTextMenuEntry(" 7 ", &OnEnemyParam1, (void *)7));
	it->AddNodeGo(new JTextMenuEntry(" 8 ", &OnEnemyParam1, (void *)8));
	it->AddNodeGo(new JTextMenuEntry(" 9 ", &OnEnemyParam1, (void *)9));
	it->AddNodeGo(new JTextMenuEntry(" 10 ", &OnEnemyParam1, (void *)10));
	it->Root();

	JTextMenuConfig cfg;
	cfg.font = &fontSmall;
	cfg.color.r = 0x00;
	cfg.color.g = 0x00;
	cfg.color.b = 0x00;
	cfg.backColor.r = 0x00;
	cfg.backColor.g = 0x00;
	cfg.backColor.b = 0xcc;
	cfg.hiColor.r = 0xff;
	cfg.hiColor.g = 0xff;
	cfg.hiColor.b = 0xff;
	cfg.hiBackColor.r = 0x00;
	cfg.hiBackColor.g = 0x00;
	cfg.hiBackColor.b = 0xcc;
	cfg.renderMode = JTEXTMENU_SHADED;
	cfg.layout = JTEXTMENU_SAMELINE;
	cfg.trackKeyboard = true;
	cfg.trackMouse = true;
	cfg.autoEnter = true;
	
	if (!menuEnemyParam1->Init(cfg))
	{
		return false;
	}
		
	menuEnemyParam1->Pos(Width() - (20 * 20), 32);
	menuEnemyParam1->Menu()->Root();

	return true;
}

bool HCed::InitEnemyParam2Menu()
{
	JDELETE(menuEnemyParam2);
	menuEnemyParam2 = new JTextMenu;

	// Crea el menú
	JTree<JTextMenuEntry *>::Iterator *it = menuEnemyParam2->Menu();

	it->Data(new JTextMenuEntry(" 1 ", &OnEnemyParam2, (void *)1));
	it->AddNodeGo(new JTextMenuEntry(" 2 ", &OnEnemyParam2, (void *)2));
	it->AddNodeGo(new JTextMenuEntry(" 3 ", &OnEnemyParam2, (void *)3));
	it->AddNodeGo(new JTextMenuEntry(" 4 ", &OnEnemyParam2, (void *)4));
	it->AddNodeGo(new JTextMenuEntry(" 5 ", &OnEnemyParam2, (void *)5));
	it->AddNodeGo(new JTextMenuEntry(" 6 ", &OnEnemyParam2, (void *)6));
	it->AddNodeGo(new JTextMenuEntry(" 7 ", &OnEnemyParam2, (void *)7));
	it->AddNodeGo(new JTextMenuEntry(" 8 ", &OnEnemyParam2, (void *)8));
	it->AddNodeGo(new JTextMenuEntry(" 9 ", &OnEnemyParam2, (void *)9));
	it->AddNodeGo(new JTextMenuEntry(" 10 ", &OnEnemyParam2, (void *)10));
	it->Root();

	JTextMenuConfig cfg;
	cfg.font = &fontSmall;
	cfg.color.r = 0x00;
	cfg.color.g = 0x00;
	cfg.color.b = 0x00;
	cfg.backColor.r = 0x00;
	cfg.backColor.g = 0xcc;
	cfg.backColor.b = 0x00;
	cfg.hiColor.r = 0xff;
	cfg.hiColor.g = 0xff;
	cfg.hiColor.b = 0xff;
	cfg.hiBackColor.r = 0x00;
	cfg.hiBackColor.g = 0xcc;
	cfg.hiBackColor.b = 0x00;
	cfg.renderMode = JTEXTMENU_SHADED;
	cfg.layout = JTEXTMENU_SAMELINE;
	cfg.trackKeyboard = true;
	cfg.trackMouse = true;
	cfg.autoEnter = true;
	
	if (!menuEnemyParam2->Init(cfg))
	{
		return false;
	}
		
	menuEnemyParam2->Pos(Width() - (10 * 20), 32);
	menuEnemyParam2->Menu()->Root();

	return true;
}

bool HCed::InitOpenStoryMenu()
{
	HCUtil::FindStories(true);
	
	JDELETE(menuOpenStory);
	menuOpenStory = new JTextMenu;

	// Crea el menú
	JTree<JTextMenuEntry *>::Iterator *it = menuOpenStory->Menu();

	if (HCUtil::Stories().size() > 0)
	{
		it->Data(new JTextMenuEntry(HCUtil::Stories()[0], &OnSelectStory, (void *)0));

		for (u32 i = 1; i < HCUtil::Stories().size(); ++i)
		{
			it->AddNodeGo(new JTextMenuEntry(HCUtil::Stories()[i], &OnSelectStory, JCAST_S32_TO_VOIDPTR(i)));
		}
		
		it->AddNodeGo(new JTextMenuEntry("<------>", &OnSelectStory, (void *)-1));
	}
	else
	{
		it->Data(new JTextMenuEntry("<------>", &OnSelectStory, (void *)-1));
	}

	it->Root();

	JTextMenuConfig cfg;
	cfg.font = &fontSmall;
	cfg.lineDistance = 3;
	cfg.color.r = 0xff;
	cfg.color.g = 0xcc;
	cfg.color.b = 0x00;
	cfg.backColor.r = 0x00;
	cfg.backColor.g = 0x00;
	cfg.backColor.b = 0x00;
	cfg.hiColor.r = 0xff;
	cfg.hiColor.g = 0xff;
	cfg.hiColor.b = 0xff;
	cfg.hiBackColor.r = 0x00;
	cfg.hiBackColor.g = 0x00;
	cfg.hiBackColor.b = 0x00;
	cfg.renderMode = JTEXTMENU_BLENDED;
	cfg.layout = JTEXTMENU_CENTER;
	cfg.layoutV = JTEXTMENU_CENTER;
	cfg.trackKeyboard = true;
	cfg.trackMouse = true;
	cfg.autoEnter = false;
	
	if (!menuOpenStory->Init(cfg))
	{
		return false;
	}
		
	menuOpenStory->Pos(Width()/2, Height()/2);
	menuOpenStory->Menu()->Root();

	return true;
}

bool HCed::InitThemeMenu()
{
	if (HCUtil::FindThemes())
	{
		JDELETE(menuTheme);
		menuTheme = new JTextMenu;

		// Crea el menú
		JTree<JTextMenuEntry *>::Iterator *it = menuTheme->Menu();

		it->Data(new JTextMenuEntry(HCUtil::Themes()[0], &OnSelectTheme, 0));
		for (u32 i = 1; i < HCUtil::Themes().size(); ++i)
		{
			it->AddNodeGo(new JTextMenuEntry(HCUtil::Themes()[i], &OnSelectTheme, JCAST_S32_TO_VOIDPTR(i)));
		}

		it->Root();

		JTextMenuConfig cfg;
		cfg.font = &fontSmall;
		cfg.lineDistance = 3;
		cfg.color.r = 0xff;
		cfg.color.g = 0xcc;
		cfg.color.b = 0x00;
		cfg.backColor.r = 0x00;
		cfg.backColor.g = 0x00;
		cfg.backColor.b = 0x00;
		cfg.hiColor.r = 0xff;
		cfg.hiColor.g = 0xff;
		cfg.hiColor.b = 0xff;
		cfg.hiBackColor.r = 0x00;
		cfg.hiBackColor.g = 0x00;
		cfg.hiBackColor.b = 0x00;
		cfg.renderMode = JTEXTMENU_BLENDED;
		cfg.layout = JTEXTMENU_CENTER;
		cfg.layoutV = JTEXTMENU_CENTER;
		cfg.trackKeyboard = true;
		cfg.trackMouse = true;
		cfg.autoEnter = false;
	
		if (!menuTheme->Init(cfg))
		{
			return false;
		}
		
		menuTheme->Pos(Width()/2, Height()/2);
		menuTheme->Menu()->Root();

		return true;
	}
	
	return false;
}

bool HCed::Draw()
{
	bool ret = true;

	SDL_FillRect(screen, 0, appBackColor);
	level.Draw();
	menuMain->Draw();
	
	// Draws the mouse cursor
	imgMouse->Draw(MouseX() + imgMouse->Width(), MouseY() + imgMouse->Height());

	switch (state)
	{
	case HCEDSTATE_FLOOR:
    ret = DrawFloor();
		break;
	case HCEDSTATE_CONTFLOOR:
		ret = DrawContFloor();
		break;
	case HCEDSTATE_LADDER:
    ret = DrawLadder();
		break;
	case HCEDSTATE_BAR:
    ret = DrawBar();
		break;
	case HCEDSTATE_BREAK:
    ret = DrawBreak();
		break;
	case HCEDSTATE_OBJECT:
    ret = DrawObject();
		break;
	case HCEDSTATE_ROPE:
    ret = DrawRope();
		break;
	case HCEDSTATE_START:
    ret = DrawStart();
		break;
	case HCEDSTATE_EXIT:
    ret = DrawExit();
		break;
	case HCEDSTATE_ENEMY:
    ret = DrawEnemy();
		break;
	case HCEDSTATE_SAVE:
    ret = DrawSave();
		break;
	case HCEDSTATE_OPENSTORY:
    ret = DrawOpenStory();
		break;
	case HCEDSTATE_APPEXIT:
    ret = DrawAppExit();
		break;
	default:
		break;
	}

	level.levelTimer.Y(13 * 32);
	level.levelTimer.Draw();

	imgGravity->Y(14 * 32);
	imgGravity->Draw();

	imgCharVx->Y(15 * 32);
	imgCharVx->Draw();

	imgCharVy->Y(16 * 32);
	imgCharVy->Draw();

	imgCharJumpRows->Y(17 * 32);
	imgCharJumpRows->Draw();

	imgMapSize->Y(18 * 32);
	imgMapSize->Draw();

	// Swaps buffers!
	Flip();

	return ret;
}

bool HCed::DrawFloor()
{
	menuFloorSubtype->Draw();

	return true;
}

bool HCed::DrawContFloor()
{
	menuContFloorSubtype->Draw();

	return true;
}

bool HCed::DrawLadder()
{
	menuLadderSubtype->Draw();

	return true;
}

bool HCed::DrawBar()
{
	menuBarSubtype->Draw();

	return true;
}

bool HCed::DrawBreak()
{
	menuBreakSubtype->Draw();

	return true;
}

bool HCed::DrawObject()
{
	menuObjectSubtype->Draw();

	return true;
}

bool HCed::DrawRope()
{
	menuRopeSubtype->Draw();
	menuRopePeriod->Draw();

	return true;
}

bool HCed::DrawStart()
{
	menuMainSubtype->Draw();

	return true;
}

bool HCed::DrawExit()
{
	return true;
}

bool HCed::DrawEnemy()
{
	menuEnemyType->Draw();
	menuEnemySubtype[enemyType]->Draw();
	menuEnemyParam1->Draw();
	menuEnemyParam2->Draw();

	return true;
}

bool HCed::DrawSave()
{
    

	return true;
}

bool HCed::DrawOpenStory()
{
	switch (inputNewStory)
	{
	case 0:
		// Selecting story
		menuOpenStory->Draw();
		break;

	case HCED_INPUT_STORY:
		// Input of new story name
		if (imgNewStory)
		{
			imgNewStory->Draw();
		}
		break;

	case HCED_INPUT_THEME:
		// Selection of theme
		if (imgNewStory)
		{
			imgNewStory->Draw();
		}
		
		menuTheme->Draw();
		break;
	}

	return true;
}

bool HCed::DrawAppExit()
{
    

	return true;
}

bool HCed::Update()
{
	menuMain->Update();

	// Updates the objects
	for (s32 i = 0; i < level.numObjects; ++i)
	{
		level.objects[i]->Update();
	}
	
	// Updates the ropes
	for (s32 i = 0; i < level.numRopes; ++i)
	{
		level.ropes[i]->Update();
	}
	
	// Updates the enemies
	//for (s32 i = 0; i < level.numEnemies; ++i)
	//{
	//	level.enemies[i]->Update();
	//}
	
	// Updates the exit
	level.levelExit.Update();
	
	switch (state)
	{
	case HCEDSTATE_FLOOR:
    return UpdateFloor();
	case HCEDSTATE_CONTFLOOR:
		return UpdateContFloor();
	case HCEDSTATE_LADDER:
    return UpdateLadder();
	case HCEDSTATE_BAR:
    return UpdateBar();
	case HCEDSTATE_BREAK:
    return UpdateBreak();
	case HCEDSTATE_OBJECT:
    return UpdateObject();
	case HCEDSTATE_ROPE:
    return UpdateRope();
	case HCEDSTATE_START:
    return UpdateStart();
	case HCEDSTATE_EXIT:
    return UpdateExit();
	case HCEDSTATE_ENEMY:
    return UpdateEnemy();
	case HCEDSTATE_SAVE:
    return UpdateSave();
	case HCEDSTATE_OPENSTORY:
    return UpdateOpenStory();
	case HCEDSTATE_APPEXIT:
    return UpdateAppExit();
  
  default:
    break;
	}
	
	return true;
}

void HCed::UpdateEraseFloor()
{
	if (MouseBt() & SDL_BUTTON_RIGHT)
	{
		s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
		
		// Put a blank cell border around the map, do no let edit those cells
		if (row > 0 && col > 0 && row < level.Map().Rows() - 1 && col < level.Map().Cols() - 1)
		{
			HCCell ***cells = level.Map().Cells();
			float x = cells[row][col]->X(), y = cells[row][col]->Y();
			
			// Erases the floor
			JDELETE(cells[row][col]);
			cells[row][col] = new HCCell;
			level.Map().BuildContFloor(row, col);
			cells[row][col]->Subtype(0);
			cells[row][col]->Pos((s32)x, (s32)y);
			level.Map().BuildCellLinkList();
		}
	}
}
	
bool HCed::UpdateFloor()
{
	// Button down
  if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
		
			// Put a blank cell border around the map, do no let edit those cells
			if (row > 0 && col > 0 && row < level.Map().Rows() - 1 && col < level.Map().Cols() - 1)
			{
				HCCell ***cells = level.Map().Cells();
				float x = cells[row][col]->X(), y = cells[row][col]->Y();

				if (cells[row][col]->Type() != HCCELLTYPE_FLOOR || 
						cells[row][col]->Subtype() != floorSubtype)
				{
					// Substitutes the floor
					JDELETE(cells[row][col]);
					cells[row][col] = new HCFloorCell(&level.Theme().Floor(floorSubtype));
					cells[row][col]->Subtype(floorSubtype);
					cells[row][col]->Pos((s32)x, (s32)y);
				}

				level.Map().BuildContFloor(row, col);
				level.Map().BuildCellLinkList();
			}
		}
		else
		{
			UpdateEraseFloor();
		}
	}

	return true;
}

bool HCed::UpdateContFloor()
{
	// Button down
  if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
		
			// Put a blank cell border around the map, do no let edit those cells
			if (row > 0 && col > 0 && row < level.Map().Rows() - 1 && col < level.Map().Cols() - 1)
			{
				HCCell ***cells = level.Map().Cells();
				float x = cells[row][col]->X(), y = cells[row][col]->Y();
			
				if (cells[row][col]->Type() != HCCELLTYPE_CONTFLOOR || 
						cells[row][col]->Subtype() != contFloorSubtype)
				{
					// Substitutes the floor
					JDELETE(cells[row][col]);
					cells[row][col] = new HCContFloor;
					cells[row][col]->Subtype(contFloorSubtype);
					level.Map().BuildContFloor(row, col);
					cells[row][col]->Pos((s32)x, (s32)y);
					level.Map().BuildCellLinkList();
				}
			}
		}
		else
		{
			UpdateEraseFloor();
		}
	}

	return true;
}

bool HCed::UpdateLadder()
{
	// Button down
  if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
		
			// Put a blank cell border around the map, do no let edit those cells
			if (row > 0 && col > 0 && row < level.Map().Rows() - 1 && col < level.Map().Cols() - 1)
			{
				HCCell ***cells = level.Map().Cells();
				float x = cells[row][col]->X(), y = cells[row][col]->Y();
			
				if (cells[row][col]->Type() != HCCELLTYPE_LADDER || 
						cells[row][col]->Subtype() != ladderSubtype)
				{
					// Substitutes the ladder
					JDELETE(cells[row][col]);
					cells[row][col] = new HCLadderCell(&level.Theme().Ladder(ladderSubtype));
					cells[row][col]->Subtype(ladderSubtype);
					cells[row][col]->Pos((s32)x, (s32)y);
					level.Map().BuildCellLinkList();
				}

				level.Map().BuildContFloor(row, col);
			}
		}
		else
		{
			UpdateEraseFloor();
		}
	}

	return true;
}

bool HCed::UpdateBar()
{
	// Button down
  if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
		
			// Put a blank cell border around the map, do no let edit those cells
			if (row > 0 && col > 0 && row < level.Map().Rows() - 1 && col < level.Map().Cols() - 1)
			{
				HCCell ***cells = level.Map().Cells();
				float x = cells[row][col]->X(), y = cells[row][col]->Y();
			
				if (cells[row][col]->Type() != HCCELLTYPE_BAR || 
						cells[row][col]->Subtype() != barSubtype)
				{
					// Substitutes the bar
					JDELETE(cells[row][col]);
					cells[row][col] = new HCBarCell(&level.Theme().Bar(barSubtype));
					cells[row][col]->Subtype(barSubtype);
					cells[row][col]->Pos((s32)x, (s32)y);
				}

				level.Map().BuildContFloor(row, col);
				level.Map().BuildCellLinkList();
			}
		}
		else
		{
			UpdateEraseFloor();
		}
	}

	return true;
}

bool HCed::UpdateBreak()
{
	// Button down
  if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
		
			// Put a blank cell border around the map, do no let edit those cells
			if (row > 0 && col > 0 && row < level.Map().Rows() - 1 && col < level.Map().Cols() - 1)
			{
				HCCell ***cells = level.Map().Cells();
				float x = cells[row][col]->X(), y = cells[row][col]->Y();
			
				if (cells[row][col]->Type() != HCCELLTYPE_BREAK || 
						cells[row][col]->Subtype() != breakSubtype)
				{
					// Substitutes the cell
					JDELETE(cells[row][col]);
					cells[row][col] = new HCBreak(level.Theme().Break(breakSubtype));
					cells[row][col]->Subtype(breakSubtype);
					cells[row][col]->Pos((s32)x, (s32)y);
				}

				level.Map().BuildContFloor(row, col);
				level.Map().BuildCellLinkList();
			}
		}
		else
		{
			UpdateEraseFloor();
		}
	}

	return true;
}

bool HCed::UpdateObject()
{
	// Button down
  if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			if (curObject != 0)
			{
				// An object is selected
				if (KeyMods() & KMOD_CTRL)
				{
					// Adjust to map cell
					s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
					curObject->Pos(level.Map().ToX(col), level.Map().ToY(row));
				}
				else
				{
					// Place the object right bellow the mouse cursor
					curObject->Pos(MouseX(), MouseY());
				}
			}
			else
			{
				// No selected object, create it
				AddObject(MouseX(), MouseY());
			}
		}
		else
		// Erase object
		if (MouseBt() & SDL_BUTTON_RIGHT)
		{
			// Delete the current object
			DeleteObject();
		}
	}

	return true;
}

bool HCed::UpdateRope()
{
	// Button down
  if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			if (curRope != 0)
			{
				// A rope is selected
				if (KeyMods() & KMOD_CTRL)
				{
					// Adjust to map cell ('ceiling')
					s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
					curRope->Pos(level.Map().ToX(col) - level.Map().CellWidth()/2, 
											 level.Map().ToY(row) - level.Map().CellHeight() + 1);
				}
				else
				if (KeyMods() & KMOD_SHIFT)
				{
					// Adjust the amplitude
					s32 amplitude = abs(MouseX() - (s32)curRope->X());
					curRope->Init(ropePeriod, amplitude, curRope->Length(), level.Theme());
				}
				else
				if (KeyMods() & KMOD_ALT)
				{
					// Adjust the length
					s32 length = abs(MouseY() - (s32)curRope->Y());
					curRope->Init(ropePeriod, curRope->Amplitude(), length, level.Theme());
				}
				else
				{
					// Place the rope right bellow the mouse cursor
					curRope->Pos(MouseX(), MouseY());
				}
			}
			else
			{
				// No selected rope, create it
				AddRope(MouseX(), MouseY());
			}
		}
		else
		// Erase rope
		if (MouseBt() & SDL_BUTTON_RIGHT)
		{
			// Delete the current rope
			DeleteRope();
		}
	}

	return true;
}

bool HCed::UpdateStart()
{
	if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			HCMap *m = &level.Map();
			
			if ((KeyMods() & KMOD_CTRL))
			{
				s32 row = m->ToRow(MouseY()), col = m->ToCol(MouseX());
				level.character.Pos(m->ToX(col), m->ToY(row));
			}
			else
			{
				level.character.Pos(MouseX(), MouseY());
			}
		}
	}

	return true;
}

bool HCed::UpdateExit()
{
	if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			HCMap *m = &level.Map();
			s32 row = m->ToRow(MouseY()), col = m->ToCol(MouseX());
			if (m->ExitRow() != row || m->ExitCol() != col)
			{ 
				m->ExitRow(row);
				m->ExitCol(col);
				level.levelExit.Init(m, 100);
			}
		}
	}

	return true;
}

bool HCed::UpdateEnemy()
{
	// Button down
  if (MouseOverMap())
	{
		if (MouseBt() & SDL_BUTTON_LEFT)
		{
			if (curEnemy != 0)
			{
				// An enemy is selected
				if (KeyMods() & KMOD_CTRL)
				{
					// Adjust to map cell
					s32 row = level.Map().ToRow(MouseY()), col = level.Map().ToCol(MouseX());
					curEnemy->Pos(level.Map().ToX(col), level.Map().ToY(row));
				}
				else
				{
					// Place the enemy right bellow the mouse cursor
					curEnemy->Pos(MouseX(), MouseY());
				}

				curEnemy->Param1(enemyParam1);
				curEnemy->Param2(enemyParam2);
			}
			else
			{
				// No selected enemy, create it
				AddEnemy(MouseX(), MouseY());
			}
		}
		else
		// Erase enemy
		if (MouseBt() & SDL_BUTTON_RIGHT)
		{
			// Delete the current enemy
			DeleteEnemy();
		}
	}

	return true;
}

bool HCed::UpdateSave()
{
	

	return true;
}

bool HCed::UpdateOpenStory()
{
	switch (inputNewStory)
	{
	case 0:
		// Only updates if not waiting for user input on new story
		menuOpenStory->Update();
		break;
		
	case 2:
		menuTheme->Update();
		break;
		
	default:
		break;
	}

	return true;
}

bool HCed::UpdateAppExit()
{
  

	return true;
}

void HCed::OnFilenameChange()
{
	JString strTitle;
	strTitle.Format("%s - HCed v1.0", filename.Str());
	Title(strTitle);
}

void HCed::OnGravityChange()
{
	SDL_Color c;
	
	float maxG = theApp->level.Map().CellHeight()/5.0f;

	// Forces the gravity value to be between maxG and -maxG
	float g = theApp->level.Map().Gravity();
	JClamp(g, -maxG, maxG);
	theApp->level.Map().Gravity(g);

	// Positive values tend to green, negative ones tend to red.
	c.unused = 255;
	c.b = 0;
	c.g = (u8)(255 * ((g + maxG)/(2.0f * maxG)));
	c.r = 255 - c.g;

	JDELETE(theApp->imgGravity);
	theApp->imgGravity = theApp->fontSmall.PrintfBlended(JFONTALIGN_LEFT, c, "g: %1.1f", theApp->level.Map().Gravity());
}

void HCed::OnCharVxChange()
{
	SDL_Color c;
	
	float maxVx = theApp->level.Map().CellWidth() - 1;

	// Forces the Vx value to be between maxVx and 0
	float v = theApp->level.character.MaxVeloccity().x;
	JClamp(v, 0.0f, maxVx);
	theApp->level.character.MaxVeloccity().x = v;

	// Greater values tend to yellow, small ones tend to grey.
	c.unused = 255;
	c.b = 127 + (u8)(255 * (v/(2.0f * maxVx)));
	c.r = c.g;
	c.g = 127;

	JDELETE(theApp->imgCharVx);
	theApp->imgCharVx = theApp->fontSmall.PrintfBlended(JFONTALIGN_LEFT, c, "vX:%1.1f", theApp->level.character.MaxVeloccity().x);
}

void HCed::OnCharVyChange()
{
	SDL_Color c;
	
	float maxVy = theApp->level.Map().CellWidth() - 1;

	// Forces the Vy value to be between maxVy and 0
	float v = theApp->level.character.MaxVeloccity().y;
	JClamp(v, 0.0f, maxVy);
	theApp->level.character.MaxVeloccity().y = v;

	// Greater values tend to yellow, small ones tend to grey.
	c.unused = 255;
	c.g = 127 + (u8)(255 * (v/(2.0f * maxVy)));
	c.r = c.g;
	c.b = 127;

	JDELETE(theApp->imgCharVy);
	theApp->imgCharVy = theApp->fontSmall.PrintfBlended(JFONTALIGN_LEFT, c, "vY:%1.1f", theApp->level.character.MaxVeloccity().y);
}

void HCed::OnCharJumpRowsChange()
{
	SDL_Color c;
	
	s32 maxJ = theApp->level.Map().Rows();

	// Forces the jump value to be between maxJ and 0
	s32 j = theApp->level.character.MaxJumpRows();
	JClamp(j, 0, maxJ);
	theApp->level.character.MaxJumpRows(j);

	// Greater values tend to light blue, small ones tend to dark blue.
	c.unused = 255;
	c.b = 127 + (u8)(255 * (j/(2.0f * maxJ)));
	c.r = c.b/2;
	c.g = 127;

	JDELETE(theApp->imgCharJumpRows);
	theApp->imgCharJumpRows = theApp->fontSmall.PrintfBlended(JFONTALIGN_LEFT, c, "Jump:%d", theApp->level.character.MaxJumpRows());
}

void HCed::OnMapSizeChange()
{
	SDL_Color c;
	
	c.unused = 255;
	c.r = 0xff;
	c.b = 0xcc;
	c.g = 0x00;

	JDELETE(theApp->imgMapSize);
	theApp->imgMapSize = theApp->fontSmall.PrintfBlended(JFONTALIGN_LEFT, 
																											 c, 
																											 "Map: %dx%d", 
																											 theApp->level.map.Rows(), 
																											 theApp->level.map.Cols());
}

void HCed::OnFloor(void *data)
{
	theApp->state = HCEDSTATE_FLOOR;
	theApp->imgMouse = &theApp->level.Theme().Floor(theApp->floorSubtype);
}

void HCed::OnContFloor(void *data)
{
	theApp->state = HCEDSTATE_CONTFLOOR;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnLadder(void *data)
{
	theApp->state = HCEDSTATE_LADDER;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnBar(void *data)
{
	theApp->state = HCEDSTATE_BAR;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnBreak(void *data)
{
	theApp->state = HCEDSTATE_BREAK;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnObject(void *data)
{
	theApp->state = HCEDSTATE_OBJECT;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnRope(void *data)
{
	theApp->state = HCEDSTATE_ROPE;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnStart(void *data)
{
	theApp->state = HCEDSTATE_START;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnExit(void *data)
{
	theApp->state = HCEDSTATE_EXIT;	
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnEnemy(void *data)
{
	theApp->state = HCEDSTATE_ENEMY;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnSave(void *data)
{
	HCedState old = theApp->state;
	theApp->state = HCEDSTATE_SAVE;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
	theApp->Draw();
	
	// Save in the command line-given file.
	JRW file;

	if (file.Create(theApp->filename, "wb"))
	{
		fprintf(stderr, "Saving the level to %s.\n", theApp->filename.Str());
		
		if (0 != theApp->level.Save(file))
		{
			fprintf(stderr, "Could not save the level to %s.\n", theApp->filename.Str());
		}
		
		file.Close();
	}
	else
	{
		fprintf(stderr, "Could not open file %s.\n", theApp->filename.Str());
	}
	
	// Return to the old state
	theApp->state = old;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
}

void HCed::OnOpenStory(void *data)
{
	theApp->state = HCEDSTATE_OPENSTORY;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
	theApp->strNewStoryName = "";
}

void HCed::OnSelectStory(void *data)
{
	if ((long)data < 0)
	{
		theApp->inputNewStory = HCED_INPUT_STORY;

		// Updates the input string
		JDELETE(theApp->imgNewStory);
		SDL_Color fg = {0xff, 0xcc, 0x00, 0x00};
		SDL_Color bg = {0x00, 0x00, 0x00, 0x00};
		theApp->imgNewStory = theApp->fontLarge.RenderTextShaded("Name: ", fg, bg);
	}
	else
	{
		theApp->OnFloor(0);
		theApp->filename = JString(theApp->storyDir) + JString(HCUtil::Stories()[(long)data]) + "/level001.hlv";
		
		if (0 != theApp->NewLevel())
		{
			fprintf(stderr, "Error loading %s\n", theApp->filename.Str());
			return;
		}
		
		theApp->storyName = HCUtil::Stories()[(long)data];
		theApp->levelNumber = 1;
	}
}

void HCed::OnSelectTheme(void *data)
{
	s32 rt = HCUtil::CreateStory(theApp->strNewStoryName);
				
	switch (rt)
	{
	case 0:
		{
			// Creates new story
			JString oldName = theApp->filename;
			JString oldTheme = theApp->themeName;
			s32 oldLevel = theApp->levelNumber;
			theApp->filename = JString(theApp->storyDir) + JString(theApp->strNewStoryName) + JString("/level001.hlv");
			theApp->themeName = HCUtil::Themes()[(long)data];
			theApp->levelNumber = 1;
						
			if (0 != theApp->NewLevel())
			{
				fprintf(stderr, "Error initializing new level %s\n", (const char *)theApp->filename);
				theApp->filename = oldName;
				theApp->themeName = oldTheme;
				theApp->levelNumber = oldLevel;
			}
			else
			{
				// All went right
				JDELETE(theApp->imgNewStory);
				theApp->storyName = theApp->strNewStoryName;
				theApp->inputNewStory = 0;
			}
		}
		break;
					
	case 1:	fprintf(stderr, "Story %s already exists, give it a different name, please.\n", (const char *)theApp->strNewStoryName);	break;
	case 2:	fprintf(stderr, "Directory 'stories' could not be found, check installation.\n");	break;
	case 3:	fprintf(stderr, "Couldn't create story. Check permissions for %s.\n", (const char *)theApp->strNewStoryName);	break;
	default:
		break;
	}
}

void HCed::OnAppExit(void *data)
{
	theApp->state = HCEDSTATE_APPEXIT;
	theApp->imgMouse = &theApp->imgMenu[theApp->state];
	theApp->Exit();
}

void HCed::OnMainSubtype(void *data)
{
	theApp->level.character.subtype = (long)data;
	theApp->level.character.Init(theApp->level.theme.MainChar((long)data), &theApp->level.map);
}

void HCed::OnFloorSubtype(void *data)
{
	theApp->floorSubtype = (long)data;
	theApp->imgMouse = &(theApp->level.Theme().Floor(theApp->floorSubtype));
}

void HCed::OnContFloorSubtype(void *data)
{
	theApp->contFloorSubtype = (long)data;
	theApp->imgMouse = &(theApp->level.Theme().ContFloor(theApp->contFloorSubtype)[HCFDT_I]);
}

void HCed::OnBarSubtype(void *data)
{
	theApp->barSubtype = (long)data;
	theApp->imgMouse = &(theApp->level.Theme().Bar(theApp->barSubtype));
}

void HCed::OnLadderSubtype(void *data)
{
	theApp->ladderSubtype = (long)data;
	theApp->imgMouse = &(theApp->level.Theme().Ladder(theApp->ladderSubtype));
}

void HCed::OnBreakSubtype(void *data)
{
	theApp->breakSubtype = (long)data;
	theApp->imgMouse = (JImage *)(theApp->level.Theme().Break(theApp->breakSubtype)[HCBDT_NORMAL].Frame(0));
}

void HCed::OnObjectSubtype(void *data)
{
	theApp->objectSubtype = (long)data;
	theApp->imgMouse = (JImage *)(theApp->level.Theme().Object(theApp->objectSubtype)[HCODT_NORMAL].Frame(0));
}

void HCed::OnRopeSubtype(void *data)
{
	theApp->ropeSubtype = (long)data;
	theApp->imgMouse = &(theApp->level.Theme().Rope(theApp->ropeSubtype)[HCRDT_TOP]);
}

void HCed::OnRopePeriod(void *data)
{
	theApp->ropePeriod = 0.5f + (0.25f * float((long)data));
}

void HCed::OnEnemySubtype(void *data)
{
	theApp->enemySubtype = (long)data;
	theApp->imgMouse = (JImage *)(theApp->GetEnemySprites(theApp->enemyType, theApp->enemySubtype)[HCCDT_STOP].Frame(0));
}

void HCed::OnEnemyType(void *data)
{
	// 0 always is a valid subtype so init to that
	theApp->enemySubtype = 0;

	theApp->enemyType = (HCEnemyType)(long)data;
	theApp->imgMouse = (JImage *)(theApp->GetEnemySprites(theApp->enemyType, theApp->enemySubtype)[HCCDT_STOP].Frame(0));
}

void HCed::OnEnemyParam1(void *data)
{
	theApp->enemyParam1 = (long)data;
}

void HCed::OnEnemyParam2(void *data)
{
	theApp->enemyParam2 = (long)data;
}

bool HCed::MouseOverMap()
{
	return 	(MouseX() > HCED_MARGIN && 
					 MouseX() < (level.Map().X() + (level.Map().Cols() * level.Map().CellWidth())) &&
					 MouseY() > HCED_MARGIN && 
					 MouseY() < (level.Map().Y() + (level.Map().Rows() * level.Map().CellHeight())));
}

void HCed::AddObject(s32 x, s32 y)
{
	curObject = new HCObject;

	// Update the level objects
	HCObject **tmp = level.objects;

	level.objects = new HCObject *[level.numObjects + 1];

	// Copies the old elements
	s32 i = 0;
	for (i = 0; i < level.numObjects; ++i)
	{
		level.objects[i] = tmp[i];
	}
	
	// New and last element
	level.objects[i] = curObject;

	++level.numObjects;

	curObject->Subtype(objectSubtype);
	curObject->Init(&level.Theme());
	curObject->Pos(x, y);

	// If it exists, deletes the old array of pointers (not the pointers inside)
	JDELETE_ARRAY(tmp);
}
	
void HCed::AddRope(s32 x, s32 y)
{
	curRope = new HCRope();
	curRope->Init(ropePeriod, 0, 0, level.Theme());
	curRope->Subtype(ropeSubtype);

	// Update the level ropes
	HCRope **tmp = level.ropes;

	level.ropes = new HCRope *[level.numRopes + 1];

	// Copies the old elements
	s32 i = 0;
	for (i = 0; i < level.numRopes; ++i)
	{
		level.ropes[i] = tmp[i];
	}
	
	// New and last element
	level.ropes[i] = curRope;

	++level.numRopes;

	curRope->Pos(x, y);

	// If it exists, deletes the old array of pointers (not the pointers inside)
	JDELETE_ARRAY(tmp);
}
	
void HCed::AddEnemy(s32 x, s32 y)
{
	switch (enemyType)
	{
	default:
	case HCENEMYTYPE_BALL:
		curEnemy = new HCEnemyBall();
		curEnemy->Init(level.Theme().Ball(enemySubtype), &level.Map());
		break;

	case HCENEMYTYPE_RANDOM:
		curEnemy = new HCEnemyRandom();
		curEnemy->Init(level.Theme().Random(enemySubtype), &level.Map());
		break;

	case HCENEMYTYPE_STATIC:
		curEnemy = new HCEnemyStatic();
		curEnemy->Init(level.Theme().Static(enemySubtype), &level.Map());
		break;

	case HCENEMYTYPE_MAKER:
		curEnemy = new HCEnemyMaker();
		curEnemy->Init(level.Theme().Maker(enemySubtype), &level.Map());
		break;

	case HCENEMYTYPE_CHASER:
		curEnemy = new HCEnemyChaser();
		curEnemy->Init(level.Theme().Chaser(enemySubtype), &level.Map());
		break;
	}

	curEnemy->Subtype(enemySubtype);
	curEnemy->Param1(enemyParam1);
	curEnemy->Param2(enemyParam2);

	// Update the level enemies
	HCEnemy **tmp = level.enemies;

	level.enemies = new HCEnemy *[level.numEnemies + 1];

	// Copies the old elements
	s32 i = 0;
	for (i = 0; i < level.numEnemies; ++i)
	{
		level.enemies[i] = tmp[i];
	}
	
	// New and last element
	level.enemies[i] = curEnemy;

	++level.numEnemies;

	curEnemy->Pos(x, y);

	// If it exists, deletes the old array of pointers (not the pointers inside)
	JDELETE_ARRAY(tmp);
}

void HCed::DeleteObject()
{
	if (curObject)
	{
		// Update the level objects
		HCObject **tmp = level.objects;

		level.objects = new HCObject *[level.numObjects - 1];

		// Copies all but the selected object
		for (s32 i = 0, newI = 0; i < level.numObjects; ++i, ++newI)
		{
			if (tmp[i] != curObject)
			{
				level.objects[newI] = tmp[i];
			}
			else
			{
				// Note that tmp is 1 element larger than level.objects
				// we have to delay 1 its index after curObject has been found inside of it
				--newI;
			}
		}
	
		--level.numObjects;

		// If it exists, deletes the old array of pointers (not the pointers inside)
		JDELETE_ARRAY(tmp);

		delete curObject;
		curObject = 0;
	}
}
	
void HCed::DeleteRope()
{
	if (curRope)
	{
		// Update the level ropes
		HCRope **tmp = level.ropes;

		level.ropes = new HCRope *[level.numRopes - 1];

		// Copies all but the selected rope
		for (s32 i = 0, newI = 0; i < level.numRopes; ++i, ++newI)
		{
			if (tmp[i] != curRope)
			{
				level.ropes[newI] = tmp[i];
			}
			else
			{
				// Note that tmp is 1 element larger than level.ropes
				// we have to delay 1 its index after curRope has been found inside of it
				--newI;
			}
		}
	
		--level.numRopes;

		// If it exists, deletes the old array of pointers (not the pointers inside)
		JDELETE_ARRAY(tmp);

		delete curRope;
		curRope = 0;
	}
}
	
void HCed::DeleteEnemy()
{
	if (curEnemy)
	{
		// Update the level enemies
		HCEnemy **tmp = level.enemies;

		level.enemies = new HCEnemy *[level.numEnemies - 1];

		// Copies all but the selected enemy
		for (s32 i = 0, newI = 0; i < level.numEnemies; ++i, ++newI)
		{
			if (tmp[i] != curEnemy)
			{
				level.enemies[newI] = tmp[i];
			}
			else
			{
				// Note that tmp is 1 element larger than level.enemies
				// we have to delay 1 its index after curEnemy has been found inside of it
				--newI;
			}
		}
	
		--level.numEnemies;

		// If it exists, deletes the old array of pointers (not the pointers inside)
		JDELETE_ARRAY(tmp);

		delete curEnemy;
		curEnemy = 0;
	}
}

JImageSprite * HCed::GetEnemySprites(HCEnemyType t, s32 s)
{
	switch (t)
	{
	case HCENEMYTYPE_BALL:
		return level.theme.Ball(s);

	case HCENEMYTYPE_STATIC:
		return level.theme.Static(s);

	case HCENEMYTYPE_MAKER:
		return level.theme.Maker(s);

	case HCENEMYTYPE_RANDOM:
		return level.theme.Random(s);

	case HCENEMYTYPE_CHASER:
		return level.theme.Chaser(s);
  
  default:
    break;
	}

	return 0;
}

s32 HCed::GetNumEnemySprites(HCEnemyType t)
{
	switch (t)
	{
	case HCENEMYTYPE_BALL:
		return level.theme.NumBalls();

	case HCENEMYTYPE_STATIC:
		return level.theme.NumStatics();

	case HCENEMYTYPE_MAKER:
		return level.theme.NumMakers();

	case HCENEMYTYPE_RANDOM:
		return level.theme.NumRandoms();

	case HCENEMYTYPE_CHASER:
		return level.theme.NumChasers();
  
  default:
    break;
	}

	return 0;
}

void HCed::DestroyMainSubtypeMenu()
{
	if (menuMainSubtype)
	{
		JImage *img;
		JTree<JImageMenuEntry *>::Iterator *it;
		it = menuMainSubtype->Menu();
		it->Root();

		do
		{
			if (it->Data())
			{
				img = it->Data()->Image();
				JDELETE(img);
				img = it->Data()->HiImage();
				JDELETE(img);
			}
		} while (it->Next());

		delete menuMainSubtype;
	}
}
	
void HCed::DestroyBreakSubtypeMenu()
{
	if (menuBreakSubtype)
	{
		JImage *img;
		JTree<JImageMenuEntry *>::Iterator *it;
		it = menuBreakSubtype->Menu();
		it->Root();

		do
		{
			if (it->Data())
			{
				img = it->Data()->Image();
				JDELETE(img);
				img = it->Data()->HiImage();
				JDELETE(img);
			}
		} while (it->Next());

		delete menuBreakSubtype;
	}
}
	
void HCed::DestroyObjectSubtypeMenu()
{
	if (menuObjectSubtype)
	{
		JImage *img;
		JTree<JImageMenuEntry *>::Iterator *it;
		it = menuObjectSubtype->Menu();
		it->Root();
		
		do
		{
			if (it->Data())
			{
				img = it->Data()->Image();
				JDELETE(img);
				img = it->Data()->HiImage();
				JDELETE(img);
			}
		} while (it->Next());

		delete menuObjectSubtype;
	}
}

void HCed::DestroyEnemySubtypeMenu()
{
	JImage *img;
	JTree<JImageMenuEntry *>::Iterator *it;

	for (s32 n = 0; n < HCENEMYTYPE_COUNT; ++n)
	{
		if (menuEnemySubtype[n])
		{
			it = menuEnemySubtype[n]->Menu();
			it->Root();
		
			do
			{
				if (it->Data())
				{
					img = it->Data()->Image();
					JDELETE(img);
					img = it->Data()->HiImage();
					JDELETE(img);
				}
			} while (it->Next());
			
			delete menuEnemySubtype[n];
		}
	}
}

void HCed::DestroyLevelMenus()
{
  DestroyMainSubtypeMenu();
  DestroyBreakSubtypeMenu();
  DestroyObjectSubtypeMenu();
  DestroyEnemySubtypeMenu();
}

HCed::~HCed()
{
	JDELETE(menuMain);
	JDELETE(menuEnemyType);
	JDELETE(menuEnemyParam1);
	JDELETE(menuEnemyParam2);
	JDELETE(menuRopePeriod);
	JDELETE(menuFloorSubtype);
	JDELETE(menuContFloorSubtype);
	JDELETE(menuBarSubtype);
	JDELETE(menuLadderSubtype);
	JDELETE(menuRopeSubtype);
	JDELETE(menuOpenStory);
	JDELETE(menuTheme);
	
	DestroyLevelMenus();
	
	JDELETE(imgGravity);
	JDELETE(imgCharVx);
	JDELETE(imgCharVy);
	JDELETE(imgCharJumpRows);
	JDELETE(imgMapSize);
}

int main(s32 argc, char **argv)
{
	HCed editor;

	theApp = &editor;
	
	if (!editor.Init(argc, argv))
	{
		fprintf(stderr, "Error launching the editor.\n");
		
		return -1;
	}

	return editor.MainLoop();
}
