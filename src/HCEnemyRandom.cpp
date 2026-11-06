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

/** Defines a random enemy.
 * @file    HCEnemyRandom.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    27/05/2004
 * @version 0.0.1 - 27/05/2004 - First version.
 */

#include <HCEnemyRandom.h>

HCEnemyRandom::HCEnemyRandom() : HCEnemy(HCENEMYTYPE_RANDOM)
{
	lastCol = lastRow = 0;
}

s32 HCEnemyRandom::Update()
{
	s32 ret = HCEnemy::Update();
	s32 col = map->ToCol((s32)pos.x);
	s32 row = map->ToRow((s32)pos.y);

	if (state == HCCS_STOP || actions == 0 || states[state].Paused())
	{
		s32 rnd = rand() % 4;
		u32 guess[4];
		for (s32 i = 0; i < 4; ++i)
		{
			guess[i] = 1 << ((rnd + i)% 4);
		}

		if (map->Cell(row, col - 1)->Actions() & guess[0]) 
		{
			actions = guess[0];
		}
		else if (map->Cell(row, col + 1)->Actions() & guess[1]) 
		{
			actions = guess[1];
		}
		else if (map->Cell(row, col)->Actions() & guess[2]) 
		{
			actions = guess[2];
		}
		else if (map->Cell(row + 1, col)->Actions() & guess[3]) 
		{
			actions = guess[3];
		}
	}
	else
	{
		if (lastCol != col || lastRow != row)
		{
			lastCol = col;
			lastRow = row;
			s32 random = rand() % 10;

			// Only if it has moved 1 cell or more
			switch (state)
			{
				// If walking and a ladder or bar is nearby, decide to use it or not
			case HCCS_LEFT:
			case HCCS_RIGHT:
				switch (random)
				{
				default:
					// Current cell
					if (map->Cell(row, col)->Type() == HCCELLTYPE_LADDER)
					{
						actions = HCCA_UP;
					}
					break;
						
				case 0:
				case 1:
					// Lower cell
					if (map->Cell(row+1, col)->Type() == HCCELLTYPE_BAR ||
							map->Cell(row+1, col)->Type() == HCCELLTYPE_LADDER)
					{
						actions = HCCA_DOWN;
					}
					else
					// Current cell
					if (map->Cell(row, col)->Type() == HCCELLTYPE_LADDER)
					{
						actions = HCCA_UP;
					}
					break;
				}
				break;

				// If going up or down, checks for floor at the sides
			case HCCS_UP:
			case HCCS_DOWN:
				switch (random)
				{
				default:
					// Lower left cell
					if (!(map->Cell(row + 1, col - 1)->Actions() & HCACTION_FALL))
					{
						actions = HCCA_LEFT;
						break;
					}
					break;
					
				case 0:
				case 1:
				case 2:
				case 3:
					// Lower right cell
					if (!(map->Cell(row + 1, col + 1)->Actions() & HCACTION_FALL))
					{
						actions = HCCA_RIGHT;
						break;
					}
					break;
				}
				break;

      default:
        break;
			}
		}
	}

	return ret;
}

void HCEnemyRandom::UpdateCollisions()
{
	float newX = pos.x + v.x;
	float newY = pos.y + v.y;
	s32 row = map->ToRow((s32)pos.y), col = map->ToCol((s32)pos.x);
	s32 newCol = map->ToCol((s32)(newX));
	s32 newRow = map->ToRow((s32)(newY));
	
	// Side collisions
	switch (map->Cell(row, newCol)->Type())
	{
	case HCCELLTYPE_FLOOR:
	case HCCELLTYPE_CONTFLOOR:
		v.x = a.x = 0;
		actions = 0;
		state = HCCS_STOP;
		break;
		
	case HCCELLTYPE_BREAK:
		if (((HCBreak *)map->Cell(row, newCol))->State() == HCBREAKSTATE_NORMAL)
		{
			v.x = a.x = 0;
			actions = 0;
			state = HCCS_STOP;
		}
		break;

	default:
		col = newCol;
		break;
	}

	// Updown collisions
	switch (map->Cell(newRow, col)->Type())
	{
	case HCCELLTYPE_LADDER:
	case HCCELLTYPE_BAR:

		// New cell is a ladder or a bar
		switch (state)
		{
		case HCCS_UP:
		case HCCS_DOWN:
		case HCCS_SLIDE:
			// In this states do nothing
			break;

		case HCCS_JUMPLEFT:
		case HCCS_JUMPRIGHT:
		case HCCS_JUMP:
			if (newRow > row && map->Cell(row, col)->Type() == HCCELLTYPE_BLANK)
			{
				// Falls over a ladder or bar, must stop
				v.y = 0.0f;
				state = HCCS_STOP;
				FixY(row);

				if (actions & HCCA_RIGHT)
				{
					actions &= ~HCCA_RIGHT;
					actions |= HCCA_LEFT;
				}
				else
				{
					actions &= ~HCCA_LEFT;
					actions |= HCCA_RIGHT;
				}
			}
			break;

		case HCCS_DIE:
			if (newRow > row && map->Cell(row, col)->Type() == HCCELLTYPE_BLANK)
			{
				// Falls over a ladder or bar, must stop
				v.y = 0.0f;
			}
			break;

		default:
			// In any other state, stops the character
			v.y = 0.0f;
			break;
		}
		break;
		
	default:
		if ((map->Cell(newRow, col)->Actions() & HCACTION_FALL) == 0)
		{
			v.y = 0.0f;

			switch (state)
			{
				// If jumping, must stop
			case HCCS_JUMPLEFT:
			case HCCS_JUMPRIGHT:
			case HCCS_JUMP:
				if (newRow > row)
				{
					actions &= ~HCCA_JUMP;

					// Stands over the floor
					state = HCCS_STOP;
					FixY(row);
				}
				break;

			case HCCS_DOWN:
			case HCCS_SLIDE:
				actions &= ~HCCA_DOWN;

				// Stands over the floor
				state = HCCS_STOP;

				FixY(row);
				break;

			case HCCS_DIE:
				if (newRow > row)
				{
					// Stands over the floor
					FixY(row);
				}
				break;
				
			default:
				break;
			}
		}
		else
		{
			if (newRow > row &&
					state != HCCS_JUMP &&
					state != HCCS_JUMPLEFT &&
					state != HCCS_JUMPRIGHT)
			{
				actions &= ~HCCA_JUMP;
				State(HCCS_JUMP);
			}
		}
		break;		
	}
}
