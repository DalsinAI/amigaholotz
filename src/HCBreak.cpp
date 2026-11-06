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

/** Break floor definition file.
 * @file    HCBreak.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2004
 * @version 0.0.1 - 30/04/2004 - First version.
 */

#include <HCBreak.h>

s32 HCBreak::Update()
{
  switch (state)
  {
  case HCBREAKSTATE_NORMAL:
		normal.Update();
    return 0;

  case HCBREAKSTATE_BREAKING:
    if (-1 == breaking.Update())
    {
      // If the breaking state has ended, places it in a non-drawable state
      state = HCBREAKSTATE_BROKEN;

			// and let the characters fall through it
			actionMask |= HCACTION_FALL | HCACTION_LEFT | HCACTION_RIGHT;
    }
    return 1;

  case HCBREAKSTATE_BROKEN:
  default:
    // Finished with this break
    return -1;
  }
}

void HCBreak::Draw()
{
  switch (state)
  {
  case HCBREAKSTATE_NORMAL:
		normal.Draw();
    break;
		
  case HCBREAKSTATE_BREAKING:
		breaking.Draw();
    break;

	default:
    break;
	}
}

void HCBreak::Break() 
{
	state = HCBREAKSTATE_BREAKING;
  breaking.Pos(pos.x, pos.y);

	if (prev && prev->State() == HCBREAKSTATE_NORMAL)
	{
		prev->Break();
	}

	if (next && next->State() == HCBREAKSTATE_NORMAL)
	{
		next->Break();
	}
}

void HCBreak::Pos(float xPos, float yPos)
{
	pos.x = xPos; 
	pos.y = yPos; 
  switch (state)
  {
  case HCBREAKSTATE_NORMAL:
		normal.Pos(xPos, yPos);
    break;
		
  case HCBREAKSTATE_BREAKING:
		breaking.Pos(xPos, yPos);
    break;

	case HCBREAKSTATE_BROKEN:
	default:
		broken.Pos(xPos, yPos);
    break;
	}
}
