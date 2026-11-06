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

/** Exit gadget for Holtz's castle game.
 * @file    HCExit.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    17/08/2004
 * @version 0.0.1 - 17/08/2004 - First version.
 */

#include <HCExit.h>

HCExit::HCExit() : map(0), state(HCEXITSTATE_LOCKED), x1(0), y1(0), 
                   sparks(0), numSparks(0), character(0), imgCharacter(0)
{
}

bool HCExit::Init(HCMap *_map, s32 nSparks, s32 w)
{
	if (!_map)
		return false;
	
	map = _map;

	if (nSparks <= 0)
	{
		nSparks = 1;
	}
	
	Destroy();
	numSparks = nSparks;
	sparks = new HCExitSpark[numSparks];
	
	// Hangs from the ceiling of the cell
	pos.x = map->ToX(map->ExitCol()) - (map->CellWidth()/2);
	pos.y = map->ToY(map->ExitRow()) - (map->CellHeight() - 1);
	
	float x0 = pos.x, y0 = pos.y;
	x1 = x0 + (w == 0 ? map->CellWidth() : w);

	// Computes the height of the exit box
	s32 i, j;
	bool end = false;
	for (j = map->ExitRow() + 1, i = map->ExitCol(); !end && j < map->Rows(); ++j)
	{
		if (map->Cell(j, i)->Type() != HCCELLTYPE_BLANK)
		{
			end = true;
		}
	}

	y1 = map->ToY((j > 0 ? j - 2: 1));
	
	// Randomly initializes the sparks into the exit box
	srand(time(0));

	for (s32 k = 0; k < numSparks; ++k)
	{
		sparks[k].c.r = 255;//128 + (rand()%127);
		sparks[k].c.g = 255;//128 + (rand()%127);
		sparks[k].c.b = 255;//128 + (rand()%127);
		sparks[k].y = y0;
		sparks[k].vy = -(1.7 + (float(rand()%50)/50.0f));
		sparks[k].c.g = sparks[k].c.r = 0;
		sparks[k].x = x0 + (rand()%s32(x1 - x0));
		sparks[k].y = y0 + (rand()%s32(y1 - y0));
		sparks[k].vy = -(0.1 + (float(rand()%50)/500.0f));
		sparks[k].vx = -sparks[k].vy * (((x1 - x0)/2) - (sparks[k].x - x0))/(y1 - y0);
		sparks[k].dc = 255 - (u8)(255.0f * (float(y1 - sparks[k].y)/(y1 - y0)));
	}
	
	// Sets the timer to 25 FPS
	timer.Start(1000/25);
	
	// By default, the exit is locked
	Lock();
 
  return true;
}

void HCExit::Draw()
{
	switch (state)
	{
	default:
	case HCEXITSTATE_LOCKED:
		{
			JImage img(SDL_GetVideoSurface());
			if (0 == img.Lock())
			{
				for (s32 k = 0; k < numSparks; ++k)
				{
					if (sparks[k].x > 0 && sparks[k].x < JApp::App()->Width() &&
							sparks[k].y > 0 && sparks[k].y < JApp::App()->Height())
					{
						img.PutPixel((s32)sparks[k].x, (s32)sparks[k].y, 
                         SDL_MapRGB(img.Format(), 
																		sparks[k].c.r, 
																		sparks[k].c.g, 
																		sparks[k].c.b));
					}
				}
				
				img.Unlock();
			}
		}
		break;

	case HCEXITSTATE_SWALLOWING:
	case HCEXITSTATE_UNLOCKED:
		{
			JImage img(SDL_GetVideoSurface());

			if (0 == img.Lock())
			{
				for (s32 k = 0; k < numSparks; ++k)
				{
					if (sparks[k].x > 0 && sparks[k].x < JApp::App()->Width() &&
							sparks[k].y > 0 && sparks[k].y < JApp::App()->Height() - 3)
					{
						for (s32 i = 0; i < 3; ++i)
						{
							img.PutPixel((s32)sparks[k].x, (s32)sparks[k].y + i, 
                           SDL_MapRGB(img.Format(), 
																			(sparks[k].c.r/(i+1)), 
																			(sparks[k].c.g/(i+1)), 
																			(sparks[k].c.b)));
						}
					}
				}
				
				img.Unlock();
			}
		}
		break;
	}
}

s32 HCExit::Update()
{
	if (!timer.Changed())
		return 0;

	float x0 = pos.x, y0 = pos.y;

	switch (state)
	{
	default:
	case HCEXITSTATE_LOCKED:
		for (s32 k = 0; k < numSparks; ++k)
		{
			if (sparks[k].y < pos.y)
			{
				sparks[k].vy = -(1.7 + (float(rand()%50)/50.0f));
				sparks[k].c.g = sparks[k].c.r = 0;
				sparks[k].x = x0 + (rand()%s32(x1 - x0));
				sparks[k].y = y0 + (rand()%s32(y1 - y0));
				sparks[k].vy = -(0.1 + (float(rand()%50)/500.0f));
				sparks[k].vx = -sparks[k].vy * (((x1 - x0)/2) - (sparks[k].x - x0))/(y1 - y0);
				sparks[k].dc = 255 - (u8)(255.0f * (float(y1 - sparks[k].y)/(y1 - y0)));
			}
			sparks[k].c.r += sparks[k].dc;
			sparks[k].c.g += sparks[k].dc;
			sparks[k].x += sparks[k].vx;
			sparks[k].y += sparks[k].vy;
		}
		break;

	case HCEXITSTATE_UNLOCKED:
		for (s32 k = 0; k < numSparks; ++k)
		{
			if (sparks[k].y <= y0)
			{
				sparks[k].c.g = sparks[k].c.r = 0;
				sparks[k].x = x0 + (rand()%s32(x1 - x0));
				sparks[k].y = y0 + (rand()%s32(y1 - y0));
				sparks[k].vy = -(4.7 + (float(rand()%10)/5.0f));
				sparks[k].vx = -sparks[k].vy * 0.1f * (((x1 - x0)/2) - (sparks[k].x - x0))/(x1 - x0);
				sparks[k].vx = -sparks[k].vy * (((x1 - x0)/2) - (sparks[k].x - x0))/(y1 - y0);
				sparks[k].dc = 255 - (u8)(255.0f * (float(y1 - sparks[k].y)/(y1 - y0)));
			}
			sparks[k].c.g += sparks[k].dc;
			sparks[k].c.r += sparks[k].dc;
			sparks[k].x += sparks[k].vx;
			sparks[k].y += sparks[k].vy;
		}
		break;

	case HCEXITSTATE_SWALLOWING:
		for (s32 k = 0; k < numSparks; ++k)
		{
			if (sparks[k].y <= y0)
			{
				// Faster, from top to the character's feet
				sparks[k].c.g = sparks[k].c.r = 0;
				sparks[k].x = x0 + (rand()%s32(x1 - x0));
				sparks[k].y = character->Y();//y0 + (rand()%s32(character->Y() - y0 + 5));
				sparks[k].vy = -(6.7 + (float(rand()%10)/2.0f));
				sparks[k].vx = -sparks[k].vy * 0.1f * (((x1 - x0)/2) - (sparks[k].x - x0))/(x1 - x0);
				sparks[k].vx = -sparks[k].vy * (((x1 - x0)/2) - (sparks[k].x - x0))/(y1 - y0);
				sparks[k].dc = 255 - (u8)(255.0f * (float(y1 - sparks[k].y)/(y1 - y0)));
			}
			sparks[k].c.g += sparks[k].dc;
			sparks[k].c.r += sparks[k].dc;
			sparks[k].x += sparks[k].vx;
			sparks[k].y += sparks[k].vy;
		}

		if (character->Y() <= (y0 + map->CellHeight()))
		{
			// Done swallowing
			state = HCEXITSTATE_SWALLOWED;
		}
		else
		{
			character->Pos(character->X(), character->Y() + character->Veloccity().y);
		}
		break;

	case HCEXITSTATE_SWALLOWED:
		break;
	}
	
	return 1;
}

void HCExit::Unlock()
{
	// Suddenly accelerates the sparks
	state = HCEXITSTATE_UNLOCKED;
	for (s32 k = 0; k < numSparks; ++k)
	{
		sparks[k].vy = -(1.7 + (float(rand()%10)/5.0f));
	}
}
	
void HCExit::Swallow(HCCharacter *ch)
{
	state = HCEXITSTATE_SWALLOWING; 
	character = ch;
	character->State(HCCS_STOP);
	character->Veloccity().y = -5.0f;
	
	// Calculates the veloccity to end in the midpoint of the ceiling
	character->Veloccity().x = character->Veloccity().y * (((character->X() - pos.x) - (x1 - pos.x)/2))/(y1 - pos.y);
}

void HCExit::Pos(float xPos, float yPos)
{
	float dx = xPos - pos.x, dy = yPos - pos.y;
	pos.x = xPos;
	pos.y = yPos;
	x1 += dx;
	y1 += dy;

	for (s32 k = 0; k < numSparks; ++k)
	{
		sparks[k].x += dx;
		sparks[k].y += dy;
	}
}

void HCExit::Destroy()
{
	JDELETE_ARRAY(sparks);
	JDELETE(imgCharacter);
}

HCExit::~HCExit()
{
	Destroy();
}
