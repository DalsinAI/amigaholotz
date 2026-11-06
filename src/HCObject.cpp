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

/** Object definition file.
 * @file    HCObject.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    30/04/2004
 * @version 0.0.1 - 30/04/2004 - First version.
 */

#include <HCObject.h>

s32 HCObject::Update()
{
  switch (state)
  {
  case HCOBJECTSTATE_NORMAL:
    normal.Update();
    return 0;

  case HCOBJECTSTATE_ACQUIRED:
    if (-1 == acquired.Update())
    {
      // If the acquired state has ended, places it in a non-drawable state
      state = HCOBJECTSTATE_NONE;
			acquired.FirstFrame();
    }
    return 1;

  case HCOBJECTSTATE_NONE:
  default:
    // Finished with this object
    return -1;
  }
}

void HCObject::Draw()
{
  switch (state)
  {
  case HCOBJECTSTATE_NORMAL:
    normal.Draw();
    return;
  case HCOBJECTSTATE_ACQUIRED:
    acquired.Draw();
    return;
  case HCOBJECTSTATE_NONE:
  default:
    return;
  }
}

bool HCObject::Init(HCTheme *theme)
{
	normal.Destroy();
	acquired.Destroy();
	normal.Ref(theme->Object(subtype)[HCODT_NORMAL]);
	acquired.Ref(theme->Object(subtype)[HCODT_ACQUIRED]);
  
  return true;
}

u32 HCObject::Load(JRW &file)
{
	if (0 == file.ReadLE32(&subtype) ||
			0 == file.ReadLE32((u32 *)&pos.x) ||
			0 == file.ReadLE32((u32 *)&pos.y))
	{
		fprintf(stderr, "Error loading the object.\n");
			
		return 2;
	}

	Pos(pos.x, pos.y);

	return 0;
}

u32 HCObject::Save(JRW &file)
{
	if (0 == file.WriteLE32(&subtype) ||
			0 == file.WriteLE32((u32 *)&pos.x) ||
			0 == file.WriteLE32((u32 *)&pos.y))
	{
		fprintf(stderr, "Error saving the object.\n");
			
		return 2;
	}
		
	return 0;
}
