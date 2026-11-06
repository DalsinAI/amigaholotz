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

/** Definiton of a rope.
 * @file    HCRope.h
 * @author  Juan Carlos Seijo Pérez
 * @date    05/06/2004
 * @version 0.0.1 -  05/06/2004 - First version.
 */

#include <HCRope.h>

bool HCRope::Init(float p, s32 a, u32 l, HCTheme &theme)
{
	// Creates references of the images and places the rope in its original position
	top.Ref(theme.Rope(subtype)[HCRDT_TOP]);
	top.Pos(pos.x, pos.y);
	
	period = p >= 1.0f ? p : 1.0f;
	w = 6.2831853/period;
	length = l > 0 ? l : 5;
	amplitude = abs(a) < length * theme.Rope(subtype)[HCRDT_MIDDLE].Height() ? abs(a) : length * theme.Rope(subtype)[HCRDT_MIDDLE].Height();

	s32 i;

	JDELETE_ARRAY(middle);
	middle = new JImage [length];

	if (middle == 0)
		return false;
	
	for (i = 0; i < length; ++i)
	{
		middle[i].Ref(theme.Rope(subtype)[HCRDT_MIDDLE]);
		middle[i].Pos(top.X() + (top.Width()/2) - (middle[i].Width()/2), pos.y);
	}
	
	edge.Ref(theme.Rope(subtype)[HCRDT_EDGE]);
	edge.Pos(top.X() + (top.Width()/2) - (edge.Width()/2), pos.y);

	// 25 fps resolution
	timer.Start(40);

	return true;
}

void HCRope::Draw()
{
	top.Draw();
	
	for (s32 i = 0; i < length; ++i)
	{
		middle[i].Draw();
	}

	edge.Draw();
}

s32 HCRope::Update()
{
	float t = float(timer.TotalLap())/1000.0f;
	
	edge.X((top.X() + top.Width()/2) - (edge.Width()/2) + (amplitude * cos(w * t)));

	float alpha = asin((float(amplitude) * cos(w * t))/float(length * middle[0].Height()));
	float newY = length * middle[0].Height() * cos(alpha);

	edge.Y(top.Y() + top.Height() + newY);
	
	float dx = ((edge.X() + edge.Width()/2) - (top.X() + top.Width()/2))/float(length);
	float dy = ((edge.Y() + edge.Height()/2) - (top.Y() + top.Height()))/float(length);

	for (s32 i = 0; i < length; ++i)
	{
		middle[i].X((top.X() + top.Width()/2) + (i * dx) - (middle[i].Width()/2));
		middle[i].Y((top.Y() + top.Height()) + (i * dy));
	}

	direction = (s32)(10.0f * dx);

	return 0;
}

void HCRope::Pos(float x, float y)
{
	float dx, dy;
	dx = x - pos.x;
	dy = y - pos.y;

	pos.x = x;
	pos.y = y;

	for (s32 i = 0; i < length; ++i)
	{
		middle[i].X(middle[i].X() + dx);
		middle[i].Y(middle[i].Y() + dy);
	}

	edge.X(edge.X() + dx);
	edge.Y(edge.Y() + dy);

	top.X(x);
	top.Y(y);
}

u32 HCRope::Load(JRW &file)
{
	if (0 == file.ReadLE32(&period) ||
			0 == file.ReadLE32(&amplitude) ||
			0 == file.ReadLE32(&length) ||
			0 == file.ReadLE32(&subtype) ||
			0 == file.ReadLE32(&pos.x) ||
			0 == file.ReadLE32(&pos.y))
	{
		fprintf(stderr, "Error reading rope parameters.\n");
		
		return 1;
	}

	return 0;
}

u32 HCRope::Load(JRW &file, HCTheme &theme)
{
	if (0 != Load(file))
	{
		return 1;
	}

	if (!Init(period, amplitude, length, theme))
	{
		return 2;
	} 

	Pos(pos.x, pos.y);

	return 0;
}

u32 HCRope::Save(JRW &file)
{
	if (0 == file.WriteLE32(&period) ||
			0 == file.WriteLE32(&amplitude) ||
			0 == file.WriteLE32(&length) ||
			0 == file.WriteLE32(&subtype) ||
			0 == file.WriteLE32(&pos.x) ||
			0 == file.WriteLE32(&pos.y))
	{
		fprintf(stderr, "Error writing rope parameters.\n");
		
		return 1;
	}

	return 0;
}

HCRope::~HCRope()
{
	JDELETE_ARRAY(middle);
}
