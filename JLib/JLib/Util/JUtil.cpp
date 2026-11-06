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

/** Macros y funciones de utilidad diversa.
 * @file    JUtil.cpp.
 * @author  Juan Carlos Seijo Pérez
 * @date    16/10/2003
 * @version 0.0.1 - 16/10/2003 - Primera versión.
 */

#include <JLib/Util/JUtil.h>

// Carga una fuente de un fichero Targa, con caracteres de 8x8 píxels,
// 16 filas y 16 columnas (256). 
void JDumpTGAFontBits(const JString &TGAFileName, const JString &outFileName)
{
  JImage img;
  if (!img.Load(TGAFileName))
    return;

  u8 *chars[256];
  u8 *ptr;
  u32 rowSize;
  s32 ind, imgInd;

  rowSize = (u32)(img.Line(1) - img.Line(0));
  
  for (s32 j = 0; j < 16; ++j)
  {
    // Avanza al primer caracter de la siguiente fila
    ptr = img.Line(j * 8);

    for (s32 i = 0; i < 16; ++i)
    {
      ind = ((15 - j) * 16) + i; // = (row * nCharsRow) + col

      // Reservamos 8 bytes para cada caracter
      chars[ind] = new u8[8];
      memset(chars[ind], 0, 8 * sizeof(s8));

      for (s32 y = 0; y < 8; ++y)
      {
        imgInd = (y * rowSize);

        for (s32 x = 0; x < 8; ++x)
        {
          if (0xFF & *(ptr + imgInd))
          {
						// 7 - y empieza a rellenar por abajo, ya que en memoria está invertido
            chars[ind][y] |= 0x80 >> x;
          }

          imgInd += 4;
        }
      }

      // Avanza al siguiente caracter de la fila
      ptr += 32;
    }
  }

  // Vuelca en formato de array los datos de la fuente al fichero dado
  JTextFile outFile(outFileName);
  if (outFile.Open())
	{
		outFile.Printf("const u8 font[256][] = {\n");
		for (s32 row = 0; row < 16; ++row)
		{
			for (s32 col = 0; col < 16; ++col)
			{
				outFile.Printf("{");
				for (s32 idx = 0; idx < 8; ++idx)
				{
					outFile.Printf("0x%02x,", chars[(row * 16) + col][idx]);
				}
				outFile.Printf("},\n");
			}
		}
		
		outFile.Printf("};\n");
	}
	else
	{
		printf("DumpTGAFontBits: Error al abrir el archivo.\n");
	}

  for (s32 i = 0; i < 256; ++i)
    delete[] chars[i];
}

JVideoMode * JListVideoModes(s32 *count, s32 depth)
{
	bool uninit = false;
	s32 bpps[] = {32, 24, 16, 8};

	s32 *pBpps = bpps, nBpps = 4;
	
	if (depth != 0)
	{
		pBpps = &depth;
		nBpps = 1;
	}
	
	if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
	{
		// Video no inicializado, lo inicializa
		if (-1 == SDL_Init(SDL_INIT_VIDEO))
		{
			// No se puede iniciar el video
			return 0;
		}

		// Deinicializar SDL al final
		uninit = true;
	}
	
	std::vector<JVideoMode*> modes;
	SDL_Rect **modeList = 0;
	JVideoMode *mode;
	
	// Busca los modos de vídeo sin aceleración para la profundidad actual y comprueba para el resto de profundidades
	modeList = SDL_ListModes(0, SDL_FULLSCREEN);
	if ((long)modeList == 0 || (long)modeList == -1)
	{
		return 0;
	}

	s32 rec;

	for (s32 k = 0; k < nBpps; ++k)
	{
		for (s32 j = 0; modeList[j] != 0; ++j)
		{
			if (0 != (rec = SDL_VideoModeOK(modeList[j]->w, modeList[j]->h, pBpps[k], SDL_HWSURFACE | SDL_FULLSCREEN))
					&& (j == 0 || (modeList[j]->w != modeList[j - 1]->w || modeList[j]->h != modeList[j - 1]->h)))
			{
				mode = new JVideoMode;
				mode->w = modeList[j]->w;
				mode->h = modeList[j]->h;
				mode->bpp = pBpps[k];
				mode->hw = true;
				modes.push_back(mode);
			}
		}
	}

#ifdef __amigaos__
	// AmigaOS (OpenGPU): SDL lists the Workbench's own size, but the game is
	// drawn for 640x480 and plays in a window of any size: offer 640x480 and
	// 800x600 too, so that its first choice (a mode no bigger than 640x480)
	// is one, and the index it saves means the same mode next time
	{
		static const s32 own[][2] = {{640, 480}, {800, 600}};
		for (s32 k = 0; k < nBpps; ++k)
		{
			for (u32 o = 0; o < sizeof(own) / sizeof(own[0]); ++o)
			{
				bool have = false;
				for (u32 i = 0; i < modes.size(); ++i)
				{
					if (modes[i]->w == own[o][0] && modes[i]->h == own[o][1] && modes[i]->bpp == pBpps[k])
						have = true;
				}
				if (!have)
				{
					mode = new JVideoMode;
					mode->w = own[o][0];
					mode->h = own[o][1];
					mode->bpp = pBpps[k];
					mode->hw = true;
					modes.push_back(mode);
				}
			}
		}
	}
#endif

	JVideoMode *result = 0;

	if (modes.size() == 0)
	{
		result = 0;
	}
	else
	{
		if (count)
			*count = modes.size();

		result = new JVideoMode [modes.size() + 1];
		for (u32 i = 0; i < modes.size(); ++i)
		{
			memcpy(&result[i], modes[i], sizeof(JVideoMode));
			delete modes[i];
		}
		
		// Último elemento
		result[modes.size()].w = result[modes.size()].h = -1;
		modes.clear();
	}
	
	if (uninit)
	{
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
	
	return result;
}

void JDumpImage(JImage *img, s32 n)
{
	SDL_Surface *s = img->Surface();
	SDL_PixelFormat *fmt = s->format;

	if (0 != SDL_LockSurface(s))
	{
		return;
	}

	fprintf(stderr, 
					"%dx%d@%d CKEY: %08x ALPHA: %d SRCALPHA: %s SRCCOLORKEY: %s RLE: %s\n", 
					s->w, s->h, fmt->BitsPerPixel, fmt->colorkey, fmt->alpha, 
					s->flags & SDL_SRCALPHA ? "yes" : "no", 
					s->flags & SDL_SRCCOLORKEY ? "yes" : "no",
					s->flags & SDL_RLEACCEL ? "yes" : "no");
	fprintf(stderr, 
					"RGBAmask: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", 
					fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
	fprintf(stderr, 
					"RGBAshift: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", 
					fmt->Rshift, fmt->Gshift, fmt->Bshift, fmt->Ashift);
	fprintf(stderr, 
					"RGBAloss: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", 
					fmt->Rloss, fmt->Gloss, fmt->Bloss, fmt->Aloss);

	// Muestra el valor de algunos píxeles
	if (n != 0)
	{
		fprintf(stderr, "First %d pixels:\n", n);
		for (s32 j = 0, count = 1; count <= n && j < s->h; ++j)
		{
			for (s32 i = 0; count <= n && i < s->w; ++i)
			{
				fprintf(stderr, "%08x ", img->GetPixel(i, j));

				if (count % 10 == 0)
					fprintf(stderr, "\n");

				++count;
			}
		}

		fprintf(stderr, "First %d pixels != CKEY:\n", n);
		for (s32 j = 0, count = 1; count <= n && j < s->h; ++j)
		{
			for (s32 i = 0; count <= n && i < s->w; ++i)
			{
				if (img->GetPixel(i, j) != fmt->colorkey)
				{
					fprintf(stderr, "%08x ", img->GetPixel(i, j));

					if (count % 10 == 0)
						fprintf(stderr, "\n");

					++count;
				}
			}
		}
		
		fprintf(stderr, "\n");
	}
	
	SDL_UnlockSurface(s);
}

