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

/** Text messages for Holotz's Castle. Base class.
 * @file    HCText.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    25/06/2004
 * @version 0.0.1 - 25/06/2004 - Primera versión.
 */

#include <HCText.h>

s32 HCText::textSpeed = 5;
s32 HCText::minDelay = 2000;
s32 HCText::maxAlpha = 180;

bool HCText::Init(HCTextType _type, const char *_text, 
									HCTheme *_theme, JFont *_font, 
									JFontAlign a, bool _left, s32 _subtype,
									u8 r, u8 g, u8 b)
{
	if (!_text || !_theme || !_font)
	{
		return false;
	}

	theme = _theme;
	font = _font;
	left = _left;
	subtype = _subtype;
	type = _type;
	
	JImage *img;

	switch (type)
	{
	case HCTEXTTYPE_DIALOG:
		img = theme->Dialog(subtype);
		break;

  default:
	case HCTEXTTYPE_NARRATIVE:
		img = theme->Narrative(subtype);
		break;
	}

	// Builds the text
	JImage *textImg;
	SDL_Color fg = {r, g, b, 0};
	textImg = font->PrintfBlended(a, fg, _text);

	// Builds the frame
	s32 rows = 0, cols = 0, x = 0, y = 0;
	cols = (s32)floor(textImg->Width()/img[HCTDT_5].Width()) + 2;
 	rows = (s32)floor(textImg->Height()/img[HCTDT_5].Height()) + 2;
	
	if (rows < 2)
		rows = 2;
	
	if (cols < 2)
		cols = 2;

	s32 w = cols * img[HCTDT_5].Width(), h = rows * img[HCTDT_5].Height();

	// Makes space for the peak, if necessary
	if (type == HCTEXTTYPE_DIALOG)
	{
		text.Create(w, h + img[HCTDT_5].Height(), SDL_GetVideoSurface()->format->BitsPerPixel);
	}
	else
	{
		text.Create(w, h, SDL_GetVideoSurface()->format->BitsPerPixel);
	}

	// Upper left corner
	text.Paste(&img[HCTDT_7], 0, 0, img[HCTDT_7].Width(), img[HCTDT_7].Height(), x, y);
	x += img[HCTDT_7].Width();

	// Upper side
	for (s32 i = 1; i < cols - 1; ++i)
	{
		text.Paste(&img[HCTDT_8], 0, 0, img[HCTDT_8].Width(), img[HCTDT_8].Height(), x, y);
		x += img[HCTDT_8].Width();
	}

	// Upper right corner
	text.Paste(&img[HCTDT_9], 0, 0, img[HCTDT_9].Width(), img[HCTDT_9].Height(), x, y);
	x += img[HCTDT_9].Width();
	
	y += img[HCTDT_5].Height();

	for (s32 j = 1; j < rows - 1; ++j)
	{
		x = 0;

		// Left side
		text.Paste(&img[HCTDT_4], 0, 0, img[HCTDT_4].Width(), img[HCTDT_4].Height(), x, y);
		x += img[HCTDT_4].Width();
		
		// Middle
		for (s32 i = 1; i < cols - 1; ++i)
		{
			text.Paste(&img[HCTDT_5], 0, 0, img[HCTDT_5].Width(), img[HCTDT_5].Height(), x, y);
			x += img[HCTDT_5].Width();
		}

		// Right side
		text.Paste(&img[HCTDT_6], 0, 0, img[HCTDT_6].Width(), img[HCTDT_6].Height(), x, y);
		x += img[HCTDT_6].Width();
		y += img[HCTDT_5].Height();
	}

	// Lower left corner
	x = 0;
	text.Paste(&img[HCTDT_1], 0, 0, img[HCTDT_1].Width(), img[HCTDT_1].Height(), x, y);
	x += img[HCTDT_1].Width();

	// Lower side
	for (s32 i = 1; i < cols - 1; ++i)
	{
		text.Paste(&img[HCTDT_2], 0, 0, img[HCTDT_2].Width(), img[HCTDT_2].Height(), x, y);
		x += img[HCTDT_2].Width();
	}

	// Lower right corner
	text.Paste(&img[HCTDT_3], 0, 0, img[HCTDT_3].Width(), img[HCTDT_3].Height(), x, y);

	if (type == HCTEXTTYPE_DIALOG)
	{
		y += img[HCTDT_5].Height() - (img[HCTDT_LEFT].Height() - img[HCTDT_5].Height());

		// The peak is placed in the middle point of the two first or last 
		// frame-squares (depending on the left parameter).
		// If the image y N pixels high than the full square (HCTDT_5) then
		// it is placed N pixels up than a normal square (to adjust it to the frame).
		if (left)
		{
			// Places the peak facing left
			x = img[HCTDT_1].Width()/2;
			text.Paste(&img[HCTDT_LEFT], 
								 0, 0, 
								 img[HCTDT_LEFT].Width(), img[HCTDT_LEFT].Height(), 
								 x, y);
		}
		else
		{
			// Places the peak facing rigth
			x = w - img[HCTDT_RIGHT].Width() - img[HCTDT_1].Width()/2;
			text.Paste(&img[HCTDT_RIGHT], 
								 0, 0, 
								 img[HCTDT_RIGHT].Width(), img[HCTDT_RIGHT].Height(), 
								 x, y);
		}
	}
	
	switch (a)
	{
	case JFONTALIGN_LEFT:
		x = img[HCTDT_7].Width()/2;
		break;

	case JFONTALIGN_RIGHT:
		x = text.Width() - textImg->Width() - img[HCTDT_7].Width();
		break;

	case JFONTALIGN_CENTER:
		x = (text.Width() - textImg->Width())/2;
		break;
	}

	y = (h - textImg->Height())/2;

	// Pega el texto
	text.Paste(textImg, 
						 0, 0,
						 textImg->Width(), textImg->Height(), 
						 x, y);
	
	text.ColorKey(0);

	delete textImg;

	// Initializes the fading timer
	if (textSpeed < 1)
		textSpeed = 1;

	if (textSpeed > 10)
		textSpeed = 10;
	
	if (minDelay < 1000)
		minDelay = 1000;

	if (maxAlpha > 255)
		maxAlpha = 255;

	if (maxAlpha < 0)
		maxAlpha = 180;

	// Must be a multiple of 15
	maxAlpha -= (maxAlpha % 15);

	float delay;
	delay = 500.0f * float(strlen(_text))/float(textSpeed);
	if (delay < minDelay)
		delay = minDelay;

	timer.Start((s32)delay);

	return true;
}

s32 HCText::Update()
{
	if (text.Alpha() > 0)
	{
		// Checks for fade-in
		if (timer.Cycles() == 0 && text.Alpha() < maxAlpha)
		{
			// make it appear
			text.Alpha(text.Alpha() + 15);
		}
		else
		if (timer.Cycles() > 0 && text.Alpha() > 0)
		{
			// make it disappear
			text.Alpha(text.Alpha() - 15);
		}
	
		// Updates the pos refered to the peak, if following something, else
		// let it as is
		if (trackX != &pos.x)
		{ 
			if (left)
			{
				text.X((s32)*trackX - theme->Dialog(subtype)[HCTDT_1].Width()/2 + pos.x);
			}
			else
			{
				text.X((s32)*trackX - text.Width() + theme->Dialog(subtype)[HCTDT_1].Width()/2 + pos.x);
			}
			
			text.Y((s32)*trackY - text.Height() + pos.y);

			return 1;
		}
	}
	else
	{
		return 2;
	}

	return 0;
}

void HCText::Draw()
{
	text.Draw();
}

void HCText::Track(float *x, float *y)
{
	trackX = x ? x : &pos.x;
	trackY = y ? y : &pos.y;
}

void HCText::Reset()
{
	timer.Start(timer.CycleTime());
	text.Alpha(15);
}

void HCText::Destroy()
{
	text.Destroy();
}
