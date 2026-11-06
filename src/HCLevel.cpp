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

/** Game level. Includes a map and its enemies.
 * @file    HCLevel.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    29/05/2004
 * @version 0.0.1 - 29/05/2004 - First version
 */

#include <HCLevel.h>

#ifndef HC_DATA_DIR
#define HC_DATA_DIR "res/"
#endif

HCLevel::HCLevel()  : numEnemies(0), enemies(0), numRopes(0), 
											ropes(0), remainingObjects(0), numObjects(0), objects(0),
											scripted(false), narrative(0), wideMap(false), highMap(false),
											maxTime(1), timerFont(0)
{
}

bool HCLevel::Init()
{
	// Initializes the character
	character.Init(theme.MainChar(character.Subtype()), &map, ropes, numRopes);
	character.State(HCCS_STOP);

	// Creates an empty background
	bg.Destroy();
	if (!bg.Create(1, 1, 0))
	{
		fprintf(stderr, "Error creating empty level background.\n");
		return false;
	}

	// Loads sounds
	soundObjectAcquired.Destroy();
	musicEndLevel.Destroy();

	if (JApp::App()->SoundEnabled() && 
			(!soundObjectAcquired.LoadWave(HC_DATA_DIR "sound/HCObjectAcquired.wav") ||
			 !musicEndLevel.LoadWave(HC_DATA_DIR "sound/HCEndLevel.wav") ||
			 !soundExitUnlocked.LoadWave(HC_DATA_DIR "sound/HCExitUnlocked.wav")))
	{
		fprintf(stderr, "Error loading sounds, check installation.\n");
	}

	// Initializes the exit
	levelExit.Lock();
	levelExit.Init(&map, 100);
	
	// Initializes the timer
	levelTimer.Init(maxTime, timerFont);
	levelTimer.Pos(0, 0);

	return true;
}

s32 HCLevel::Update()
{
	s32 ret = 0;

	s32 i;
	
	// Updates the map
	map.Update();
	
	// Updates the ropes
	for (i = 0; i < numRopes; ++i)
	{
		ropes[i]->Update();
	}
	
	// Updates the character unless it's exiting the level
	if (!scripted)
	{
		switch (LevelExit()->State())
		{
		default:
		case HCEXITSTATE_LOCKED:
			{
				if (IsExitUnlocked())
				{
					LevelExit()->Unlock();
					
					if (JApp::App()->SoundEnabled())
					{
						soundExitUnlocked.Play();
					}
				}
				character.Update();
			}
			break;

		case HCEXITSTATE_UNLOCKED:
			{
				character.Update();
			
				// Checks if the character is in the exit sight and swallows it if so
				if (Character()->X() >= LevelExit()->X() && 
						Character()->X() <= LevelExit()->X1() && 
						Character()->Y() >= LevelExit()->Y() && 
						Character()->Y() <= LevelExit()->Y1())
				{
					// Time stops
					levelTimer.Pause();
					
					// Inside, swallow it!
					LevelExit()->Swallow(Character());

					if (JApp::App()->SoundEnabled())
					{
						musicEndLevel.Play();
					}

					for (i = 0; i < NumEnemies(); ++i)
					{
						Enemies()[i]->State(HCCS_DIE);
					}
				}
			}
			break;
		
		case HCEXITSTATE_SWALLOWED:
			{
				// Level achieved!!
				if (!JApp::App()->SoundEnabled() || !musicEndLevel.IsPlaying())
				{
					ret = 2;
				}
			}
			break;

		case HCEXITSTATE_SWALLOWING:
			break;
		}
	}
	else
	{
		character.Update();
	}
	
	// Updates the objects
	for (i = 0; i < numObjects; ++i)
	{
		objects[i]->Update();
	}

	// Checks for objects to pick up
	CheckObjects();

	// Positions the level according to the character position so it's always visible
	// but it can be overriden if any script exists
	spot = character.Pos();

	// Updates the enemies
	if (scripted)
	{
		for (i = 0; i < numEnemies; ++i)
		{
			// Updates the enemies as if they were normal characters
			enemies[i]->HCCharacter::Update();
		}
	}
	else
	{
		bool collide = false;
		
		for (i = 0; !collide && i < numEnemies; ++i)
		{
			enemies[i]->Update();

			if (HCPreferences::Prefs()->Difficulty() != HCPREFERENCES_TOY)
			{
				// Checks collisions with enemies, except in toy mode
				if (!collide && Collide(&character, enemies[i]))
				{
					character.State(HCCS_DIE);
					collide = true;
					ret = 1;
				}
			}
		}

		// Updates the time remaining, except in toy mode
		if (levelTimer.Update() <= 0)
		{
			if (HCPreferences::Prefs()->Difficulty() != HCPREFERENCES_TOY)
			{
				// Time up! (Except in toy mode)
				character.State(HCCS_DIE);
				ret = 1;
			}
		}
	}
	
	if (narrative)
	{
		narrative->Update();
	}

	if (!scripted)
	{
		levelExit.Update();
	}

	// Updates the level position according to the spot position
	s32 x = (s32)pos.x, y = (s32)pos.y;
	if (wideMap)
	{
		s32 w2 = JApp::App()->Width()/2;

		if (spot.x < (w2 - viewRadius))
		{
			// Spot outside viewing spot, moves towards the spot
			s32 vx = (s32)JMin((w2 - viewRadius - spot.x)/4, map.CellWidth());
					
			if (vx <= 1) vx = 1;
			
			x += vx;
		}
		else
		if (spot.x > (w2 + viewRadius))
		{
			// Spot outside viewing spot, moves towards the spot
			s32 vx = (s32)JMin((spot.x - (w2 + viewRadius))/4, map.CellWidth());
					
			if (vx <= 1) vx = 1;
			
			x -= vx;
		}
	}
	
	if (highMap)
	{
		s32 h2 = JApp::App()->Height()/2;

		if (spot.y < (h2 - viewRadius))
		{
			// Spot outside viewing spot, moves towards the spot
			s32 vy = (s32)JMin((h2 - viewRadius - spot.y)/5, map.CellHeight());
					
			if (vy <= 1) vy = 1;
			
			y += vy;
		}
		else
		if (spot.y > (h2 + viewRadius))
		{
			// Spot outside viewing spot, moves towards the spot
			s32 vy = (s32)JMin((spot.y - (h2 + viewRadius))/5, map.CellHeight());
					
			if (vy <= 1) vy = 1;
			
			y -= vy;
		}
	}
	
	if (wideMap || highMap)
	{
		Pos(x, y);
	}

	return ret;
}

void HCLevel::Draw()
{
	s32 i;

	// Draws the background
	bg.Draw();

	// Draws the map
	map.Draw();
	
	// Draws the objects
	for (i = 0; i < numObjects; ++i)
	{
		objects[i]->Draw();
	}

	// Draws the enemies
	for (i = 0; i < numEnemies; ++i)
	{
		enemies[i]->Draw();
	}
	
	// Draws the ropes
	for (i = 0; i < numRopes; ++i)
	{
		ropes[i]->Draw();
	}

	// Draws the main character
	character.Draw();

	if (narrative)
	{
		narrative->Draw();
	}

	if (!scripted)
	{
		levelExit.Draw();

		if (HCPreferences::Prefs()->Difficulty() != HCPREFERENCES_TOY)
		{
			// Draws the time remaining
			levelTimer.Draw();
		}

		// Draws the remaining objects to complete the level
		float qw = float(JApp::App()->Width())/4.0f, dx = qw/float(numObjects + 1);
		for (i = 0; i < numObjects; ++i)
		{
			objects[i]->Draw();
			
			if (objects[i]->State() == HCOBJECTSTATE_NORMAL)
			{
				((JImage *)objects[i]->Normal().Frame(0))->Draw(s32(qw * 3.0f + dx * float(i)), 0);
			}
		}
	}
}

u32 HCLevel::Load(JRW &file, const char *filename)
{
	Destroy();

	// Maximum time to complete the level, 0 means no timed
	if (0 == file.ReadLE32(&maxTime))
	{
		fprintf(stderr, "Error reading the level completion time.\n");
		perror("");

		return 1;
	}
	
	// Adjusts max time according to the level of difficulty (x1, x2, x3)
	maxTime *= HCPreferences::Prefs()->Difficulty();
	
	// Theme name
	JString str;
	if (0 != str.Load(file))
	{
		fprintf(stderr, "Error loading the theme name.\n");

		return 2;
	}

	// Loads the theme
	if (!theme.Load(str))
	{
		fprintf(stderr, "Error loading the theme %s.\n", theme.Name());

		return 2;
	}

	// Initializes and loads the map
	if (!map.Init(theme) ||
			0 != map.Load(file))
	{
		fprintf(stderr, "Error loading the level map.\n");

		return 2;
	}

	// Main character
	if (0 != character.Load(file))
	{
		fprintf(stderr, "Error loading main character.\n");

		return 1;
	}

	// Initializes enemie's maximum global veloccities
	HCEnemy::MaxXVeloccity(character.MaxVeloccity().x);
	HCEnemy::MaxYVeloccity(character.MaxVeloccity().y);
		
	// Loads the enemies
	if (0 == file.ReadLE32(&numEnemies))
	{
		fprintf(stderr, "Error reading the number of enemies.\n");

		return 1;
	}
	
	if (numEnemies > 0)
	{
		enemies = new HCEnemy *[numEnemies];
	
		s32 enemyType;

		for (s32 i = 0; i < numEnemies; ++i)
		{
			if (0 != file.ReadLE32((s32 *)&enemyType))
			{
        // Lets the file at ist original position
        file.Seek(-4, SEEK_CUR);

				// Peeps the type of enemy
				switch (enemyType)
				{
				default:
				case HCENEMYTYPE_BALL:
					enemies[i] = new HCEnemyBall;
					break;

				case HCENEMYTYPE_RANDOM:
					enemies[i] = new HCEnemyRandom;
					break;

				case HCENEMYTYPE_STATIC:
					enemies[i] = new HCEnemyStatic;
					break;

				case HCENEMYTYPE_MAKER:
					enemies[i] = new HCEnemyMaker;
					break;

				case HCENEMYTYPE_CHASER:
					enemies[i] = new HCEnemyChaser;
					break;
				}
			
				if (0 != enemies[i]->Load(file, theme, &map))
				{
					Destroy();
				
					fprintf(stderr, "Error loading the enemies.\n");
					return 2;
				}
			}
		}
	}
	
	// Loads the objects
	if (0 == file.ReadLE32(&numObjects))
	{
		fprintf(stderr, "Error reading the number of objects.\n");

		return 1;
	}
	
	if (numObjects > 0)
	{
		remainingObjects = numObjects;
		objects = new HCObject *[numObjects];
	
		for (s32 i = 0; i < numObjects; ++i)
		{
			objects[i] = new HCObject;
			
			if (0 != objects[i]->Load(file))
			{
				Destroy();
				
				fprintf(stderr, "Error loading the objects.\n");
				return 2;
			}

			objects[i]->Init(&theme);
		}
	}

	// Loads the ropes
	if (0 == file.ReadLE32(&numRopes))
	{
		fprintf(stderr, "Error reading the number of ropes.\n");

		return 1;
	}
	
	if (numRopes > 0)
	{
		ropes = new HCRope *[numRopes];
	
		for (s32 i = 0; i < numRopes; ++i)
		{
			ropes[i] = new HCRope();

			if (0 != ropes[i]->Load(file, theme))
			{
				Destroy();
				
				fprintf(stderr, "Error loading the ropes.\n");
				return 2;
			}
		}
	}

	if (!Init())
	{
		fprintf(stderr, "Error initializing the level.\n");
		return 2;
	}

	// Checks if a background named as the file and ended in '.tga' exists
	str.Format("%s.tga", filename);
	
	if (JFile::Exists(str))
	{
		bg.Destroy();

		if (!bg.Load(str))
		{
			fprintf(stderr, "Error loading level background %s.\n", str.Str());
			return 1;
		}

		bg.Pos(0, 0);
	}
	else
	{
		// Creates an empty background
		if (!bg.Create(1, 1, 0))
		{
			fprintf(stderr, "Error creating empty level background.\n");
			return 2;
		}
	}

	// Checks for chaser enemies in the level
	for (s32 i = 0; i < numEnemies; ++i)
	{
		if (enemies[i]->Type() == HCENEMYTYPE_CHASER)
		{
			((HCEnemyChaser *)enemies[i])->Chase(&character);
		}
	}

	// Initializes the viewing parameters
	s32 x = 0, y = 0;
	viewRadius = 0;
	
	if (map.Width() > JApp::App()->Width())
	{
		wideMap = true;
		
		// Defaults the viewing square size to a quarter of the application width
		viewRadius = JApp::App()->Width()/8;
	}
	else
	{
		x = (JApp::App()->Width() - map.Width())/2;
		wideMap = false;
	}

	if (map.Height() > JApp::App()->Height())
	{
		highMap = true;

		// Defaults the viewing square size to a quarter of the application height or
		// the viewRadius, the greatest
		if (viewRadius != 0)
		{
			viewRadius = (s32)JMin(JApp::App()->Height()/6, viewRadius);
		}
		else
		{
			viewRadius = JApp::App()->Height()/6;
		}
	}
	else
	{
		y = (JApp::App()->Height() - map.Height())/2;
		highMap = false;
	}
	
	Pos(x, y);

	return 0;
}

u32 HCLevel::Save(JRW &file)
{
	s32 x, y;

	// Time to complete the level
	if (0 == file.WriteLE32(&maxTime))
	{
		fprintf(stderr, "Error writing the completion time.\n");

		return 1;
	}
	
	// Theme name
	JString str;
	str = theme.Name();
	if (0 != str.Save(file))
	{
		fprintf(stderr, "Error saving the theme name.\n");

		return 2;
	}

	if (0 != map.Save(file))
	{
		fprintf(stderr, "Error saving the level map.\n");

		return 2;
	}

	// Main character
	x = (s32)character.X();
	y = (s32)character.Y();
	character.Pos(x - (s32)map.X(), y - (s32)map.Y());

	if (0 != character.Save(file))
	{
		fprintf(stderr, "Error saving main character.\n");

		return 1;
	}
	
	character.Pos(x, y);
	
	// Stores the objects, enemies, ropes relative to the map top-left corner
	if (0 == file.WriteLE32(&numEnemies))
	{
		fprintf(stderr, "Error writing the number of enemies.\n");

		return 1;
	}
	
	for (s32 i = 0; i < numEnemies; ++i)
	{
		x = (s32)enemies[i]->X();
		y = (s32)enemies[i]->Y();
		enemies[i]->Pos(x - (s32)map.X(), y - (s32)map.Y());

		if (0 != enemies[i]->Save(file))
		{
			fprintf(stderr, "Error saving the enemies.\n");
			return 2;
		}
		
		enemies[i]->Pos(x, y);
	}

	if (0 == file.WriteLE32(&numObjects))
	{
		fprintf(stderr, "Error writing the number of objects.\n");

		return 1;
	}
	
	for (s32 i = 0; i < numObjects; ++i)
	{
		x = (s32)objects[i]->X();
		y = (s32)objects[i]->Y();
		objects[i]->Pos(x - (s32)map.X(), y - (s32)map.Y());

		if (0 != objects[i]->Save(file))
		{
			fprintf(stderr, "Error saving the objects.\n");
			return 2;
		}

		objects[i]->Pos(x, y);
	}

	if (0 == file.WriteLE32(&numRopes))
	{
		fprintf(stderr, "Error writing the number of ropes.\n");

		return 1;
	}
	
	for (s32 i = 0; i < numRopes; ++i)
	{
		x = (s32)ropes[i]->X();
		y = (s32)ropes[i]->Y();
		ropes[i]->Pos(x - (s32)map.X(), y - (s32)map.Y());

		if (0 != ropes[i]->Save(file))
		{
			fprintf(stderr, "Error saving the ropes.\n");
			return 2;
		}

		ropes[i]->Pos(x, y);
	}

	return 0;
}

void HCLevel::Pos(float xPos, float yPos)
{
	float xMap, yMap, xBg, yBg, dx, dy;
	float xMapOrg = map.X();
	float yMapOrg = map.Y();

	pos.x = xPos;
	pos.y = yPos;

	if (map.Width() > bg.Width())
	{
		xMap = xPos;
		xBg = xPos + ((map.Width() - bg.Width())/2);
	}
	else
	{
		xBg = xPos;
		xMap = xPos + ((bg.Width() - map.Width())/2);
	}
		
	if (map.Height() > bg.Height())
	{
		yMap = yPos;
		yBg = yPos + ((map.Height() - bg.Height())/2);
	}
	else
	{
		yBg = yPos;
		yMap = yPos + ((bg.Height() - map.Height())/2);
	}

	map.Pos(xMap, yMap);
	bg.Pos(xBg, yBg);

	dx = xMap - xMapOrg;
	dy = yMap - yMapOrg;

	// Now move the objects
	float t1, t2;
	for (s32 i = 0; i < numObjects; ++i)
	{
		t1 = objects[i]->X();
		t2 = objects[i]->Y();
		
		objects[i]->Pos(objects[i]->X() + (s32)dx, 
										objects[i]->Y() + (s32)dy);
	}

	// Now move the ropes
	for (s32 i = 0; i < numRopes; ++i)
	{
		ropes[i]->Pos(ropes[i]->X() + (s32)dx, 
									ropes[i]->Y() + (s32)dy);
	}

	// Now move the enemies
	for (s32 i = 0; i < numEnemies; ++i)
	{
		enemies[i]->Pos(enemies[i]->X() + (s32)dx, 
										enemies[i]->Y() + (s32)dy);
	}
	
	// Move the level exit
	levelExit.Pos(levelExit.X() + (s32)dx, (s32)levelExit.Y() + (s32)dy);

	// And finally the main character
	character.Pos(character.X() + (s32)dx,
								character.Y() + (s32)dy);
}

void HCLevel::CheckObjects()
{
	JVector vc, vo;
	vc = character.Pos();

	// Y Offset of the center of the character
	JImage * img = (JImage *)character.CurSprite()->Frame(character.CurSprite()->CurFrame());
	vc.y += img->Pos().y + (img->Width()/2);

	for (s32 i = 0; i < numObjects; ++i)
	{
		if (objects[i]->State() == HCOBJECTSTATE_NORMAL)
		{
			vo = objects[i]->Pos();
			
			// Y Offset of the center of the character
			img = (JImage *)objects[i]->Normal().Frame(objects[i]->Normal().CurFrame());
			vo.y += img->Pos().y + (img->Width()/2);

			if (vo == vc)
			{
				// 0 length case
				ObjectAcquired(objects[i]);

				if (JApp::App()->SoundEnabled())
				{
					soundObjectAcquired.Play();
				}
			}
			else
			{
				if ((vo - vc).Length() < (0.9f * character.CurSprite()->MaxW()))
				{
					ObjectAcquired(objects[i]);

					if (JApp::App()->SoundEnabled())
					{
						soundObjectAcquired.Play();
					}
				}
			}
		}
	}
}

void HCLevel::ObjectAcquired(HCObject *object)
{
	if (object)
	{
		--remainingObjects;

		object->Acquire();
	}
}

bool HCLevel::Collide(HCCharacter *c1, HCCharacter *c2)
{
	float px1, py1, px2, py2, pw, ph;
	float ex1, ey1, ex2, ey2, ew, eh;
	JImage *cfp, *cfe;
	float difficulty = 0.75;
	cfp = (JImage *)c1->CurSprite()->Frame(c1->CurSprite()->CurFrame());
	cfe = (JImage *)c2->CurSprite()->Frame(c2->CurSprite()->CurFrame());

	pw = cfp->Width() * difficulty;
	ph = cfp->Height() * difficulty;
	px1 = c1->CurSprite()->Pos().x + cfp->X() + (cfp->Width()/2) - (pw/2);
	px2 = px1 + pw;
	py1 = c1->CurSprite()->Pos().y + cfp->Y() + (cfp->Height()/2) - (ph/2);
	py2 = py1 + ph;
	
	ew = cfe->Width() * difficulty;
	eh = cfe->Height() * difficulty;
	ex1 = c2->CurSprite()->Pos().x + cfe->X() + (cfe->Width()/2) - (ew/2);
	ex2 = ex1 + ew;
	ey1 = c2->CurSprite()->Pos().y + cfe->Y() + (cfe->Height()/2) - (eh/2);
	ey2 = ey1 + eh;

// 	SDL_Rect rcp = {px1, py1, pw, ph};
// 	SDL_Rect rce = {ex1, ey1, ew, eh};
// 	SDL_FillRect(SDL_GetVideoSurface(), &rce, 0xffffffff);
// 	SDL_FillRect(SDL_GetVideoSurface(), &rcp, 0xffffffff);
// 	SDL_Flip(SDL_GetVideoSurface());

	// Approximate collisions using half the width and height, centered of the characters
	if ((px1 > ex1 && px1 < ex2) ||
			(px2 > ex1 && px2 < ex2) ||
			(px1 < ex1 && px2 > ex2) ||
			(px1 > ex1 && px2 < ex2))
	{
		if ((py1 > ey1 && py1 < ey2) ||
				(py2 > ey1 && py2 < ey2) ||
				(py1 < ey1 && py2 > ey2) ||
				(py1 > ey1 && py2 < ey2))
		{
			// Collide!
			return true;
		}
	}
	
	return false;
}

void HCLevel::Destroy()
{
	JDELETE_POINTER_ARRAY(enemies, numEnemies);
	numEnemies = 0;
	JDELETE_POINTER_ARRAY(ropes, numRopes);
	numRopes = 0;
	JDELETE_POINTER_ARRAY(objects, numObjects);
	numObjects = 0;
	map.Destroy();
	theme.Destroy();
	levelExit.Destroy();
	bg.Destroy();
	character.Destroy();
	narrative = 0;
	levelTimer.Destroy();
	soundObjectAcquired.Destroy();
	musicEndLevel.Destroy();
}
