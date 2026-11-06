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

/** Defines an enemy ball.
 * @file    HCEnemyBall.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    27/05/2004
 * @version 0.0.1 - 27/05/2004 - First version.
 */

#include <HCEnemyBall.h>

HCEnemyBall::HCEnemyBall() : HCEnemy(HCENEMYTYPE_BALL)
{
	// Depende de param1 o param2
	actions = (param1 % 2  ? HCCA_LEFT : HCCA_RIGHT);
	v.x = (float)(param1 % 2 ? -(param1 + 1) : (param1 + 1));
	vMax.x = v.x;
	state = HCCS_LEFT;
}

void HCEnemyBall::UpdateCollisions()
{
	s32 row = map->ToRow((s32)pos.y), col = map->ToCol((s32)pos.x), newCol = map->ToCol((s32)(pos.x + v.x));

	// Side collisions
	switch (map->Cell(row, newCol)->Type())
	{
	case HCCELLTYPE_FLOOR:
	case HCCELLTYPE_CONTFLOOR:
		v.x = -v.x;
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
		break;
		
	case HCCELLTYPE_BREAK:
		if (((HCBreak *)map->Cell(row, newCol))->State() == HCBREAKSTATE_NORMAL)
		{
			v.x = -v.x;
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

	default:
		col = newCol;
		break;
	}

	s32 newRow = map->ToRow((s32)(pos.y + v.y));

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
				}
				break;

			case HCCS_DOWN:
			case HCCS_SLIDE:
				actions &= ~HCCA_DOWN;
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
			}
		}
		break;		
	}
}
