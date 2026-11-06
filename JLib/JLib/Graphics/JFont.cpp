/*
 *  JLib - Jacob's Library.
 *  Copyright (C) 2003, 2004  Juan Carlos Seijo Pérez
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 * 
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 * 
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

/** Fuente para mostrar texto en pantalla.
 * @file    JFont.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    18/04/2004
 * @version 0.0.1 - 18/04/2004 - Primera versión.
 */

#include <JLib/Graphics/JFont.h>

JImage * JFont::Printf(JFontRenderType type, JFontAlign align, SDL_Color &fg, SDL_Color &bg, char *str)
{
	// Renderiza cada línea
	char *c = str;
	s32 lines = 1;

	// Cuenta las líneas
	while (*c)
	{
		if (*c == '\n')
		{
			++lines;
		}

		++c;
	}

	JImage **img = new JImage*[lines];
	memset(img, 0, lines*sizeof(JImage*));
	char *cur = str;
	s32 line = 0;
	s32 w = 0, h = 0;
	c = str;

	while (*c)
	{
		if (*c == '\n' || *(c + 1) == 0)
		{
			// New line!
			if (*c == '\n')
				*c = 0;
			
			if (!*cur)
			{
				// Skip empty strings
				img[line] = 0;
				h += LineDistance();
				++line;
				cur = c + 1;
				*c = '\n';
				++c;
				continue;
			}

			switch (type)
			{
			default:
			case JFONTRENDERTYPE_SOLID:
				img[line] = RenderTextSolid(cur, fg);
				
				// Tenemos 8 bits, queremos MÁS!
				img[line]->Convert(SDL_GetVideoSurface()->format, SDL_GetVideoSurface()->flags);

				// Quita el flag SDL_SRCALPHA de forma que se copie el valor del canal alfa a la composición
				img[line]->Alpha(0, 0);
				break;

			case JFONTRENDERTYPE_SHADED:
				img[line] = RenderTextShaded(cur, fg, bg);

				// Tenemos 8 bits, queremos MÁS!
				img[line]->Convert(SDL_GetVideoSurface()->format, SDL_GetVideoSurface()->flags);

				// Quita el flag SDL_SRCALPHA de forma que se copie el valor del canal alfa a la composición
				img[line]->Alpha(0, 0);
				break;

			case JFONTRENDERTYPE_BLENDED:
				img[line] = RenderTextBlended(cur, fg);
				// Quita el flag SDL_SRCALPHA de forma que se copie el valor del canal alfa a la composición
				img[line]->Alpha(0, 0);
				
				break;
			}

			// Checks the max width
			if (img[line]->Width() > w)
			{
			  w = img[line]->Width();
			}

			switch (align)
			{
			case JFONTALIGN_LEFT:
				img[line]->Pos(0, h);
				break;
				
			case JFONTALIGN_RIGHT:
				img[line]->Pos(w - img[line]->Width(), h);
				break;
				
			case JFONTALIGN_CENTER:
				img[line]->Pos((w - img[line]->Width())/2, h);
				break;
			}

			h += LineDistance();
			
			++line;
			cur = c + 1;
			*c = '\n';
		}

		++c;
	}

	JImage *ret = new JImage;
	
	SDL_PixelFormat *fmt = img[0]->Format();
	ret->Create(w, h,	img[0]->Format()->BitsPerPixel, 0, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

	s32 x;

	for (line = 0; line < lines; ++line)
	{
		if (img[line])
		{
			switch (align)
			{
			case JFONTALIGN_LEFT:
				x = 0;
				break;
				
			case JFONTALIGN_RIGHT:
				x = w - img[line]->Width();
				break;
				
			case JFONTALIGN_CENTER:
				x = (w - img[line]->Width())/2;
				break;
				
			default:
				x = 0;
				break;
			}
			
			ret->Paste(img[line], 
								 0, 
								 0, 
								 (s32)img[line]->Width(), 
								 (s32)img[line]->Height(),
								 x,
								 (s32)img[line]->Y());

			delete img[line];
		}
	}

	delete[] img;

	return ret;
}

JImage * JFont::PrintfSolid(JFontAlign align, SDL_Color &fg, const char *strFormat, ...)
{
	// Formatea el texto
  va_list vlist;
  s8 str[4096];

  va_start(vlist, strFormat);
  vsprintf(str, strFormat, vlist);
  va_end(vlist);

	// Renderiza el texto
	SDL_Color color; // Dummy
	return Printf(JFONTRENDERTYPE_SOLID, align, fg, color, str);
}

JImage * JFont::PrintfShaded(JFontAlign align, SDL_Color &fg, SDL_Color &bg, const char *strFormat, ...)
{
	// Formatea el texto
  va_list vlist;
  s8 str[4096];

  va_start(vlist, strFormat);
  vsprintf(str, strFormat, vlist);
  va_end(vlist);

	// Renderiza el texto
	return Printf(JFONTRENDERTYPE_SHADED, align, fg, bg, str);
}

JImage * JFont::PrintfBlended(JFontAlign align, SDL_Color &fg, const char *strFormat, ...)
{
	// Formatea el texto
  va_list vlist;
  s8 str[4096];

  va_start(vlist, strFormat);
  vsprintf(str, strFormat, vlist);
  va_end(vlist);

	// Renderiza el texto
	SDL_Color color; // Dummy
	return Printf(JFONTRENDERTYPE_BLENDED, align, fg, color, str);
}
