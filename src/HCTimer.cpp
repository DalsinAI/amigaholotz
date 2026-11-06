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

/** Level timer for holotz's castle.
 * @file    HCTimer.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    04/10/2004
 * @version 0.0.1 -  4/10/2004 - First version.
 */

#include <HCTimer.h>

bool HCTimer::Build(u16 t, u8 r, u8 g, u8 b)
{
	char str[32];
	sprintf(str, "%d", t);
	
	SDL_Color c = {r, g, b, 0};

	Destroy();
	if ((img = font->RenderTextBlended(str, c)))
	{
		return true;
	}
	
	return false;
}

bool HCTimer::Init(u16 t, JFont *f)
{
	if (!f)
	{
		fprintf(stderr, "HCTimer: No hay fuente\n");
		return false;
	}

	cycleTime = 1000;
	maxTime = t * 1000;
	font = f;
	
	return Build(t, 255, 255, 255);
}
	
void HCTimer::Draw()
{
	img->Draw((s32)pos.x, (s32)pos.y);
}

s32 HCTimer::Update()
{
	s32 t = TimeRemaining();

	if (Changed() > 0)
	{
		// Turns red as time goes by
		u8 r = 255;
		u8 g = 255 * t/maxTime;
		u8 b = 255 * t/maxTime;

		if (t < 0)
		{
			t = 0;
			Pause();
		}
		else
		{
			t /= 1000;
		}

		Build(t, r, g, b);
	}
	
	return t;
}
