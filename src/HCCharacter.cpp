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

/** Holotz's Castle main character definition file.
 * @file    HCCharacter.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2004
 * @version 0.0.1 - 30/04/2004 - First version.
 */

#include <HCCharacter.h>

HCCharacter::HCCharacter()
{
	state = HCCS_STOP;
	actions = 0;
	lastAction = 0;
	maxFallRows = 2;
	startJumpRow = -1;
	maxJumpRows = 0;
	maxJumpCols = 0;
	v.x = v.y = 0.0f;
	a.x = a.y = 0.0f;
	vMax.x = 5.0f;
	vMax.y = 9.0f;
	vJumpMax.x = vMax.x;
	vJumpMax.y = 0.0f;
	aMax.x = vMax.x;
	aMax.y = vMax.y;

	breakRow = -1;
	trackRope = 0;
	dialog = 0;
	subtype = 0;
}

bool HCCharacter::Init(JImageSprite *sprites, HCMap *_map, HCRope **_ropes, s32 nRopes)
{
	if (0 == sprites)
	{
		fprintf(stderr, "No sprites for character.\n");

		return false;
	}

	if (0 == _map)
	{
		fprintf(stderr, "No map for character.\n");

		return false;
	}

	// References the themes' sprites
	for (s32 i = 0; i < HCCS_COUNT; ++i)
	{
		states[i].Ref(sprites[i]);
	}

	a.x = v.x = a.y = v.y = 0.0f;
	map = _map;
	ropes = _ropes;
	numRopes = nRopes;

	// Sets the number of rows for jump and computes the jump velocitty
	MaxJumpRows(maxJumpRows);

	// Sets hanging parameters depending on the largest frame of the jump left sprites
	// Sets the hang hotspot at the middle of the largest frame, the width and height 
	// are half the maximun of either the width or the height
	s32 max = 0;
	for (u32 i = 0; i < sprites[HCCS_JUMPLEFT].NumFrames(); ++i)
	{
		max = (s32)JMax(max, ((JImage *)sprites[HCCS_JUMPLEFT].Frame(i))->Width());
		max = (s32)JMax(max, ((JImage *)sprites[HCCS_JUMPLEFT].Frame(i))->Height());
	}
	
	hangCheckYOffset = max/2;
	hangCheckSize = max/2;

	return true;
}

void HCCharacter::MaxJumpRows(s32 newMaxJumpRows)
{
	maxJumpRows = newMaxJumpRows;
	JClamp(maxJumpRows, 0, map->Rows());

	// Simulates a jump to determine the veloccity in Y.
	s32 curJumpRows = 0;
	vJumpMax.y = 0.0f;
	float vx = vJumpMax.x, x = 0.0f, firstVy = 0.0f, lastVy = 0.0f, dy = float(map->CellHeight())/200.0f;
	bool first = true;

	while (curJumpRows <= maxJumpRows)
	{
		vJumpMax.y += dy;
		float vy = -vJumpMax.y, y = 0.0f;
		vx = vJumpMax.x;
		x = 0.0f;
		maxJumpCols = curJumpRows = 0;
	
		while (y <= 0.0f)
		{
			vy += map->Gravity();

			if (vy > vJumpMax.y)
			{
				vy = vJumpMax.y;
			}

			x += vx;
			y += vy;
			curJumpRows = (s32)JMax(curJumpRows, -y/map->CellHeight());
		}
		
		// Check the first and last v.y's that allow a vertical jump of maxJumpRows cells.
		if (curJumpRows == maxJumpRows)
		{
			if (first)
			{
				first = false;
				firstVy = vJumpMax.y;
			}

			lastVy = vJumpMax.y;
		}
	}
	
	vJumpMax.y = firstVy + ((lastVy - firstVy)/2.0f);
	
	// This is only informative for, perhaps, the child classes but includes it also
	maxJumpCols = (s32)x/map->CellWidth();

	// This is the maximum number of rows before starting to fall (and before the player
	// has no control over the character, except if it hangs to a rope during the fall).
	// If falling and the floor is reached, the character dies.
	maxFallRows = maxJumpCols * 2;
}

void HCCharacter::Draw()
{
	states[state].Draw();

	if (dialog != 0)
	{
		dialog->Draw();
	}
	
// 	SDL_Rect rc;
// 	rc.x = map->ToX(map->ToCol((s32)pos.x)) - 1;
// 	rc.y = map->ToY(map->ToRow((s32)pos.y)) - (map->CellHeight()/2 - 1);
// 	rc.w = 3;
// 	rc.h = 3;
	
// 	SDL_FillRect(SDL_GetVideoSurface(), &rc, 0xffffffff);
}

s32 HCCharacter::Update()
{
	states[state].Update();
	
	if (state == HCCS_HANG)
	{
		UpdateHang();
	}
	else
	{
		UpdateVeloccity();
		CheckHang();
	}

	// Collisions
	UpdateCollisions();
	CheckBrokenFloor();

	Pos(pos.x + v.x, pos.y + v.y);

	// Updates dialogs
	UpdateDialog();

	return 0;
}

void HCCharacter::UpdateVeloccity()
{
	v.x += a.x;
	v.y += a.y + map->Gravity();

	if (v.x > vMax.x)
	{
		v.x = vMax.x;
	}
	else
	if (v.x < -vMax.x)
	{
		v.x = -vMax.x;
	}

	switch (state)
	{
	case HCCS_JUMPLEFT:
	case HCCS_JUMPRIGHT:
	case HCCS_JUMP:
		// Falling veloccity limited to the cell height - 1
		if (v.y > 0.0f)
		{
			if (v.y > map->CellHeight() - 1)
			{
				v.y = map->CellHeight() - 1;
			}
		}
		else
		if (v.y < -vJumpMax.y)
		{
			v.y = -vJumpMax.y;
		}
		break;

	case HCCS_DIE:
		v.x = a.x = 0.0f;
		if (v.y > vMax.y)
		{
			v.y = vMax.y;
		}
		else
		if (v.y < -vMax.y)
		{
			v.y = -vMax.y;
		}
		break;

	default:
		if (v.y > vMax.y)
		{
			v.y = vMax.y;
		}
		else
		if (v.y < -vMax.y)
		{
			v.y = -vMax.y;
		}
		break;
	}
}

void HCCharacter::UpdateCollisions()
{
	float newX = pos.x + v.x;
	float newY = pos.y + v.y;
	s32 row = map->ToRow((s32)pos.y), col = map->ToCol((s32)pos.x);
	s32 newCol = map->ToCol((s32)(newX));
	s32 newRow = map->ToRow((s32)(newY));
	
	switch (map->Cell(row, newCol)->Type())
	{
	case HCCELLTYPE_FLOOR:
	case HCCELLTYPE_CONTFLOOR:
		v.x = 0.0f;
		break;
		
	case HCCELLTYPE_BREAK:
		if (((HCBreak *)map->Cell(row, newCol))->State() == HCBREAKSTATE_NORMAL)
		{
			v.x = 0.0f;
		}
		break;

	default:
		col = newCol;
		break;
	}

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
					// Stands over the floor
					state = HCCS_STOP;
					FixY(row);
				}
				else
				if (newRow == row)
				{
					// Stands over the floor above
					state = HCCS_STOP;
					FixY(row - 1);
				}
				break;

			case HCCS_DOWN:
			case HCCS_SLIDE:
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
					state != HCCS_JUMPRIGHT &&
					state != HCCS_DIE)
			{
				State(HCCS_JUMP);
			}
		}
		break;		
	}
}

void HCCharacter::UpdateDialog()
{
	if (dialog != 0)
	{
		dialog->Update();
	}
}

void HCCharacter::CheckBrokenFloor()
{
	// Checks for broken floor
	if (breakRow != -1)
	{
		s32 row = map->ToRow((s32)pos.y + map->CellHeight()/2);
		s32 col = map->ToCol((s32)pos.x);

		// Inside a break, got out?
		if (map->Cell(row, col)->Type() != HCCELLTYPE_BREAK || ((HCBreak *)map->Cell(row, col))->State() != HCBREAKSTATE_NORMAL)
		{
			// Got out!
			((HCBreak *)map->Cell(breakRow, breakCol))->Break();
			breakRow = -1;
		}
	}
	else
	{
		s32 row = map->ToRow((s32)pos.y + map->CellHeight()/2);
		s32 col = map->ToCol((s32)pos.x);

		if (state == HCCS_STOP || 
				state == HCCS_LEFT || 
				state == HCCS_RIGHT)
		{ 
			// Outside a break, got in?
			if (row < map->Rows() - 1 && 
					map->Cell(row, col)->Type() == HCCELLTYPE_BREAK &&
					((HCBreak *)map->Cell(row, col))->State() == HCBREAKSTATE_NORMAL)
			{
				// Got in!
				breakRow = row;
				breakCol = col;
			}
		}
	}
}

void HCCharacter::CheckHang()
{
	if (ropes == 0)
		return;
	
	if (state != HCCS_HANG &&
			state != HCCS_DIE)
	{
		JImage *edge;
		for (s32 i = 0; i < numRopes; ++i)
		{
			edge = &ropes[i]->Edge();
			s32 cx = (s32)edge->X() + (edge->Width()/2);
			s32 cy = (s32)edge->Y() + (edge->Height()/2);
			
			if (cx > pos.x - hangCheckSize && 
					cx < pos.x + hangCheckSize && 
					cy > pos.y - hangCheckYOffset - hangCheckSize && 
					cy < pos.y - hangCheckYOffset + hangCheckSize)
			{
				state = HCCS_HANG;
				v.x = v.y = a.x = 0.0f;
				a.y = -map->Gravity();
				trackRope = ropes[i];
				Pos(trackRope->Edge().X(), trackRope->Edge().Y());
				break;
			}
		}
	}
}

s32 HCCharacter::UpdateStop()
{
  return 0;
}

s32 HCCharacter::UpdateRight()
{
  return 0;
}

s32 HCCharacter::UpdateLeft()
{
  return 0;
}

s32 HCCharacter::UpdateUp()
{
  return 0;
}

s32 HCCharacter::UpdateDown()
{
	return 0;
}

s32 HCCharacter::UpdateSlide()
{
	return 0;
}

s32 HCCharacter::UpdateJump()
{
	return 0;
}

s32 HCCharacter::UpdateJumpLeft()
{
	return 0;
}

s32 HCCharacter::UpdateJumpRight()
{
	return 0;
}

s32 HCCharacter::UpdateFall()
{
	return 0;
}

s32 HCCharacter::UpdateDie()
{
  return 0;
}

s32 HCCharacter::UpdateHang()
{
	// Move with the rope!
	Pos(trackRope->Edge().X() + trackRope->Edge().Width()/2, 
			trackRope->Edge().Y() + trackRope->Edge().Height());

  return 0;
}

void HCCharacter::Pos(float x, float y)
{
	//	fprintf(stderr, "En Pos() antes pos.x: %f pos.y: %f  v.x: %f v.y: %f  x:%f y:%f  px+vx:%f py+vy:%f\n", pos.x, pos.y, v.x, v.y, x, y, pos.x + v.x, pos.y + v.y);
	pos.x = x;
	pos.y = y;
	//	fprintf(stderr, "En Pos() después pos.x: %f pos.y: %f  v.x: %f v.y: %f  x:%f y:%f\n", pos.x, pos.y, v.x, v.y, x, y);
	states[state].Pos(x, y);
}

s32 HCCharacter::Row() 
{
	return map->ToRow((s32)pos.y);
}

s32 HCCharacter::Col()
{
	return map->ToCol((s32)pos.x);
}


void HCCharacter::FixPos(s32 col, s32 row)
{
	Pos(map->ToX(col), map->ToY(row));
}

void HCCharacter::FixX(s32 col)
{
	Pos(map->ToX(col), (s32)pos.y);
}

void HCCharacter::FixY(s32 row)
{
	Pos(pos.x, map->ToY(row));
}

bool HCCharacter::Act(u32 act)
{
	lastAction = actions;
	actions = act;
	
	s32 row = map->ToRow((s32)pos.y);
	s32 col = map->ToCol((s32)pos.x);

  if (state == HCCS_DIE)
	{
		return false;
	}

	// Nothing
	if (actions == 0)
	{
		// Stop moving aside
		v.x = a.x = 0.0f;
		
		switch (state)
		{
		case HCCS_JUMP:
		case HCCS_JUMPLEFT:
		case HCCS_JUMPRIGHT:
			{
				// Vertical jump
				State(HCCS_JUMP);
				
				if (v.y < 0.0f)
				{
					// Stop going up but not down
					v.y = 0.0f;
				}
			}
			break;

		case HCCS_UP:
		case HCCS_DOWN:
			{
				v.y = 0.0f;
				a.y = -map->Gravity();
				states[state].Paused(true);
			}
			break;

		case HCCS_SLIDE:
			break;

		case HCCS_LEFT:
		case HCCS_RIGHT:
			State(HCCS_STOP);
			break;

		default:
			break;
		}
		
		return true;
	}

	/////////////////////////////////////////////////////////////////////
	// Side movement
	if ((actions & HCCA_LEFT) && (map->Cell(row, col)->Actions() & HCACTION_LEFT))
	{
		// Goes left
		a.x = -aMax.x;

		if (actions & HCCA_JUMP)
		{
			a.y = 0.0f;
			// Jump requested
			if (state != HCCS_JUMP &&
					state != HCCS_JUMPLEFT &&
					state != HCCS_JUMPRIGHT)
			{
				// Not jumping, make it jump
				v.y = -vJumpMax.y;
			}

			// Jumps facing left
			State(HCCS_JUMPLEFT);
			
			// Jump processed, clean it from the pending actions
			actions &= ~HCCA_JUMP;
		}
		else
		{
			a.y = 0.0f;
			
			// Jump not requested
			if (state == HCCS_JUMPLEFT ||
					state == HCCS_JUMPRIGHT ||
					state == HCCS_JUMP)
			{
				State(HCCS_JUMPLEFT);
			}
			else
			if (state != HCCS_HANG)
			{
				if (map->Cell(row + 1, col)->Actions() & HCACTION_FALL)
				{
					// Falls facing right if not climbing or hanging
					State(HCCS_JUMPLEFT);
					v.y = 0.0f;
				}
				else
				{
					// Walks facing right if not climbing or hanging
					State(HCCS_LEFT);
				}
			}
		}
	}
	else
	if ((actions & HCCA_RIGHT) && (map->Cell(row, col)->Actions() & HCACTION_RIGHT))
	{
		// Goes right
		a.x = aMax.x;

		if (actions & HCCA_JUMP)
		{
			a.y = 0.0f;
			// Jump requested
			if (state != HCCS_JUMP &&
					state != HCCS_JUMPLEFT &&
					state != HCCS_JUMPRIGHT)
			{
				// Not jumping, make it jump
				v.y = -vJumpMax.y;
			}

			// Jumps facing right
			State(HCCS_JUMPRIGHT);
			
			// Jump processed, clean it from the pending actions
			actions &= ~HCCA_JUMP;
		}
		else
		{
			a.y = 0.0f;
			if (state == HCCS_JUMPLEFT ||
					state == HCCS_JUMPRIGHT ||
					state == HCCS_JUMP)
			{
				State(HCCS_JUMPRIGHT);
			}
			else
			// Jump not requested
			if (state != HCCS_HANG)
			{
				if (map->Cell(row + 1, col)->Actions() & HCACTION_FALL)
				{
					// Falls facing right if not climbing or hanging
					State(HCCS_JUMPRIGHT);
					v.y = 0.0f;
				}
				else
				{
					// Walks facing right if not climbing or hanging
					State(HCCS_RIGHT);
				}
			}
		}
	}
	else
	{
		v.x = a.x = 0.0f;
	}

	/////////////////////////////////////////////////////////////////////
	// Vertical movement
	if (actions & HCCA_UP && state != HCCS_HANG)
	{
		// Up requested
		if (map->Cell(row, col)->Actions() & HCACTION_UP != 0)
		{
			State(HCCS_UP);
			// Can go up in the current cell
			if (map->Cell(row - 1, col)->Actions() & HCACTION_UP ||
					map->Cell(row - 1, col)->Actions() & HCACTION_FALL)
			{
				// Can go up in the upper cell or cell above empty, continue
				a.y = -map->Gravity() - aMax.y;
				states[state].Paused(false);
				FixX(col);
			}
			else
			{
				// Cannot go up in the upper cell, stops
				a.y = -map->Gravity();
				v.y = 0.0f;
				states[state].Paused(true);
			}
		}
		else
		{
			a.y = 0.0f;

			if (state == HCCS_UP)
			{
				// Cannot go up, wait
				State(HCCS_STOP);
				v.y = 0.0f;
			}
		}
	}
	else
	if (actions & HCCA_DOWN)
	{
		// Down requested
		if (state == HCCS_HANG)
		{
			// If hanging, falls
			v.x = v.y = a.y = 0.0f;
			state = HCCS_JUMP;
			trackRope = 0;
			Pos(pos.x, pos.y + states[state].MaxH());
		}
		else
		if (map->Cell(row + 1, col)->Actions() & HCACTION_SLIDE)
		{
			// Bar like object bellow
			a.y = -map->Gravity() + aMax.y;
			state = HCCS_SLIDE;
			FixX(col);
		}
		else
		if (map->Cell(row + 1, col)->Actions() & HCACTION_DOWN)
		{
			// Ladder like object bellow
			a.y = -map->Gravity() + aMax.y;
			state = HCCS_DOWN;
			states[state].Paused(false);
			FixX(col);
		}
		else
		{
			// Not a ladder or a bar bellow
			if (map->Cell(row, col)->Actions() & HCACTION_DOWN || 
					map->Cell(row, col)->Actions() & HCACTION_SLIDE)
			{
				// If still over the ladder or bar, keep going on
				a.y = -map->Gravity() + aMax.y;
				states[state].Paused(false);
			}
			else
			if (map->Cell(row + 1, col)->Actions() & HCACTION_FALL)
			{
				// There is no bar or ladder either in the current cell or the cell bellow
				// and the cell bellow is empty
				if (state != HCCS_JUMP &&
						state != HCCS_JUMPLEFT &&
						state != HCCS_JUMPRIGHT &&
						!(actions & HCCA_JUMP))
				{
					// Jump not requested, normal fall
					v.y = a.y = 0.0f;
					State(HCCS_JUMP);
				}
			}
			else
			{
				a.y = 0.0f;
				
				if (state == HCCS_DOWN)
				{
					// Cannot go down, wait
					state = HCCS_STOP;
					FixY(row);
				}
			}
		}
	}
	
	// Jumps only if not sliding and the current cell and the cell above are empty or
	// it's hanging
	if ((actions & HCCA_JUMP) && 
			state != HCCS_SLIDE && 
			(!(map->Cell(row + 1, col)->Actions() & HCACTION_FALL) || 
			 //!(map->Cell(row, col)->Actions() & HCACTION_FALL) || 
			 state == HCCS_HANG))
	{
		// Cell bellow or this cell are not empty or the character is hanging
		v.x = a.x = 0.0f;

		if (state != HCCS_JUMP &&
				state != HCCS_JUMPLEFT &&
				state != HCCS_JUMPRIGHT)
		{
			a.y = 0.0f;
			v.y = -vJumpMax.y;
			State(HCCS_JUMP);
		}
	}

	return true;
}

void HCCharacter::State(HCCharacterState newState)
{
	// Gets the current row and col
	state = newState;
	states[state].Pos(pos.x, pos.y);
}

u32 HCCharacter::Load(JRW &file)
{
	if (0 == file.ReadLE32(&subtype) ||
			0 == file.ReadLE32((u32 *)&pos.x) ||
			0 == file.ReadLE32((u32 *)&pos.y) ||
			0 == file.ReadLE32((u32 *)&vMax.x) ||
			0 == file.ReadLE32((u32 *)&vMax.y) ||
			0 == file.ReadLE32(&maxJumpRows))
	{
		fprintf(stderr, "Error reading character's common parameters.\n");
		return 1;
	}

	Pos(pos.x, pos.y);

	return 0;
}

u32 HCCharacter::Save(JRW &file)
{
	if (0 == file.WriteLE32(&subtype) ||
			0 == file.WriteLE32((u32 *)&pos.x) ||
			0 == file.WriteLE32((u32 *)&pos.y) ||
			0 == file.WriteLE32((u32 *)&vMax.x) ||
			0 == file.WriteLE32((u32 *)&vMax.y) ||
			0 == file.WriteLE32(&maxJumpRows))
	{
		fprintf(stderr, "Error writing character's common parameters.\n");
		return 1;
	}

	return 0;
}

void HCCharacter::Destroy()
{
	for (s32 i = 0; i < HCCS_COUNT; ++i)
	{
		states[i].Destroy();
	}
	
	dialog = 0;
	breakCol = breakRow = -1;
}

