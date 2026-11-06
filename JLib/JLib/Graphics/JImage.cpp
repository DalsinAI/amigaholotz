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

/** Imagen genérica
 * @file    JImage.cpp.
 * @author  Juan Carlos Seijo Pérez
 * @date    14/10/2003
 * @version 0.0.1 - 14/10/2003 - Primera varsión.
 * @version 0.0.2 - 01/06/2004 - Adición de métodos de referencia y copia.
 */

#include <JLib/Graphics/JImage.h>

// Constructor
JImage::JImage() : surface(0)
{
}

JImage::JImage(u32 w, u32 h, u32 _bpp)
: surface(0)
{
  Create(w, h, _bpp);
}

// Constructor copia
JImage::JImage(JImage &img)
{
  surface = SDL_ConvertSurface(img.Surface(), img.Surface()->format, img.Surface()->flags);
}

// Carga la imagen desde fichero. Soporta '.tga', '.bmp', '.jpg' y otros
bool JImage::Load(const char *filename, bool toDisplayFormat, u32 cKey)
{
  surface = IMG_Load(filename);
  if (surface != 0)
  {
    if (toDisplayFormat)
    {
      SDL_Surface *surf = SDL_DisplayFormat(surface);

      if (0 != surf)
      {
        SDL_FreeSurface(surface);
        surface = surf;
        ColorKey(cKey);
      }
    }

    return true;
  }

  return false;
}

void endian_swap2(void *_data, u32 bytes) 
{
	u32 i;
	u16 *data = (u16*)_data;

	for(i = 0; i < bytes>>1; ++i) 
	{
		data[i] = SDL_Swap16(data[i]);
	}
}

void endian_swap4(void *_data, u32 bytes) 
{
	u32 i;
	u32 *data = (u32*)_data;

	for(i = 0; i < bytes>>2; ++i) {
		data[i] = SDL_Swap32(data[i]);
	}
}


// Crea la superficie vacía
// Si data no es cero carga una copia en la superficie.
bool  JImage::Create(u32 w, u32 h, u32 _bpp, void *data, u32 rMask, u32 gMask, u32 bMask, u32 aMask)
{
  Destroy();

  if (_bpp == 0)
  {
    _bpp = SDL_GetVideoSurface()->format->BitsPerPixel;
  }

  if (rMask == 0 &&
      gMask == 0 &&
      bMask == 0 &&
      aMask == 0)
  {
    rMask = SDL_GetVideoSurface()->format->Rmask;
    gMask = SDL_GetVideoSurface()->format->Gmask;
    bMask = SDL_GetVideoSurface()->format->Bmask;
    aMask = SDL_GetVideoSurface()->format->Amask;
  }

  // Si hay datos los carga a una superficie temporal...
  if (data)
  {
    SDL_Surface *tmp;
    
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
		{
			if(_bpp == 16) 
			{
        endian_swap2(data, _bpp/8 * w * h);
      } 
			else if(_bpp == 32) 
			{
        endian_swap4(data, _bpp/8 * w * h);
      }
    }
    
    tmp = SDL_CreateRGBSurfaceFrom(data, w, h, _bpp, _bpp/8 * w,
                                   rMask, gMask, bMask, aMask);

    if (tmp == 0)
    {
      printf("JImage::Create(): Error al crear tmp!\n");
      return false;
    }

    surface = SDL_DisplayFormat(tmp);

    SDL_FreeSurface(tmp);
    if(surface == 0)
    {
      fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
      return false;
    }
		
		return true;
  }
  else
  {
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, _bpp,
                                  rMask, gMask, bMask, aMask);
    if(surface == 0)
    {
      fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
      return false;
    }
		
		Fill(ColorKey());
  }
  
  return true;
}

// Dibuja la imagen
void JImage::Draw()
{
  SDL_Rect rc;
  rc.x = (s16)pos.x;
  rc.y = (s16)pos.y;
  rc.w = (u16)surface->w;
  rc.h = (u16)surface->h;

  SDL_BlitSurface(surface, 0, SDL_GetVideoSurface(), &rc);
}

// Dibuja la imagen en la posición dada.
void JImage::Draw(s32 x, s32 y)
{
  SDL_Rect rc;
  rc.x = (s16)x;
  rc.y = (s16)y;
  rc.w = (u16)surface->w;
  rc.h = (u16)surface->h;
  
  SDL_BlitSurface(surface, 0, SDL_GetVideoSurface(), &rc);
}

// Pega el contenido de una imagen en esta.
bool JImage::Paste(JImage *srcImg, s32 xSrc, s32 ySrc, s32 wSrc, s32 hSrc, s32 xDst, s32 yDst)
{
  SDL_Rect srcRc, dstRc;
  srcRc.x = (s16)xSrc;
  srcRc.y = (s16)ySrc;
  srcRc.w = (u16)wSrc;
  srcRc.h = (u16)hSrc;
  dstRc.x = (s16)xDst;
  dstRc.y = (s16)yDst;
  dstRc.w = (u16)wSrc;
  dstRc.h = (u16)hSrc;
  
  return 0 == SDL_BlitSurface(srcImg->surface, &srcRc, surface, &dstRc);
}

// Devuelve el valor del pixel en la posición dada.
// La imagen debe estar bloqueada.
u32 JImage::GetPixel(s32 x, s32 y)
{
  s32 bpp = surface->format->BytesPerPixel;
  u8 *p = (u8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
  {
    case 1:
      return *p;

    case 2:
      return *(u16 *)p;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      {
				return p[0] << 16 | p[1] << 8 | p[2];
      }
      else
      {
				return p[0] | p[1] << 8 | p[2] << 16;
      }

    case 4:
      return *(u32 *)p;

    default:
      return 0;
  }
}

// Establece el valor del pixel en la posición dada.
// La imagen debe estar bloqueada.
void JImage::PutPixel(s32 x, s32 y, u32 color)
{
  s32 bpp = surface->format->BytesPerPixel;
  u8 *p = (u8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
  {
    case 1:
      *p = (u8)color;
      break;

    case 2:
      *(u16 *)p = (u16)color;
      break;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
          p[0] = ((u8)(color >> 16)) & 0xff;
          p[1] = ((u8)(color >> 8)) & 0xff;
          p[2] = ((u8)color) & 0xff;
      } else {
          p[0] = ((u8)color) & 0xff;
          p[1] = ((u8)(color >> 8)) & 0xff;
          p[2] = ((u8)(color >> 16)) & 0xff;
      }
      break;

    case 4:
      *(u32 *)p = color;
      break;
  }
}

// Guarda la imagen como BMP
bool JImage::SaveAsBMP(const char *file)
{
  return (0 == SDL_SaveBMP(surface, file));
}

// Carga la imagen.
u32 JImage::Load(JRW &f)
{
	Destroy();

  s32 w, h;
  u32 ckey, sz, Rmask, Gmask, Bmask, Amask;
  u8 bpp, *data;
  if (f.ReadLE32(&w) && f.ReadLE32(&h) && f.Read8(&bpp) &&
      f.ReadLE32(&Rmask) && f.ReadLE32(&Gmask) && f.ReadLE32(&Bmask) &&
      f.ReadLE32(&Amask) && f.ReadLE32(&ckey) && f.ReadLE32(&sz))
  {
    data = new u8[sz];
    if (f.Read(data, sz))
    {
      if (Create(w, h, bpp, data, Rmask, Gmask, Bmask, Amask))
      {
        ColorKey(ckey);

				delete[] data;

        return 0;
      }
    }

		delete[] data;
  }

  return 1;
}

// Salva la imagen.
u32 JImage::Save(JRW &f)
{
  u32 sz = Size();
  if (f.WriteLE32(&surface->w) &&
      f.WriteLE32(&surface->h) &&
      f.Write8(&surface->format->BitsPerPixel) &&
      f.WriteLE32(&surface->format->Rmask) &&
      f.WriteLE32(&surface->format->Gmask) &&
      f.WriteLE32(&surface->format->Bmask) &&
      f.WriteLE32(&surface->format->Amask) &&
      f.WriteLE32(&surface->format->colorkey) &&
      f.WriteLE32(&sz))
  {
    if (0 == Lock())
    {
			// Salva cada línea, pues hemos de tener en cuenta el pitch en memoria
			for (s32 j = 0; j < surface->h; ++j)
			{
				u8 *data = Line(j);
				if (0 == f.Write(data, surface->w * BytesPP()))
				{
					// Error E/S
					Unlock();
					return 1;
				}
			}

      Unlock();
      
      return 0;
    }
  }

  return 2;
}

u32 JImage::LoadImage(JRW &jrw)
{
	Destroy();

  if (0 == (surface = IMG_Load_RW(jrw.rwops, 0)))
  {
    return 1; 
  }
  
  return 0;
}

JImage * JImage::Scale(float xp, float yp)
{
	if (-1 == Lock())
	{
		return 0;
	}

	s32 newW = (s32)(surface->w * xp), newH = (s32)(surface->h * yp);
	
	float dx = 1.0f/xp, dy = 1.0f/yp;                // Tamaño de 1 pixel original en coordenadas de la nueva imagen
	

	SDL_Color samp;                                  // Muestra actual

	JImage *img = new JImage(newW, newH, surface->format->BitsPerPixel);
	
	if (-1 == img->Lock())
	{
		return 0;
	}

	// Recorre los puntos de la nueva imagen
	float nx0, ny0;                                  // Primer pixel interviniente en la imagen original para un pixel de la nueva
	float rAccum, gAccum, bAccum, aAccum;            // Acumulados de las componentes de la imagen
	float xSize, ySize, ratio, x1, x2, y1, y2;

	for (s32 j = 0; j < newH; ++j)
	{
		for (s32 i = 0; i < newW; ++i)
		{
			nx0 = float(i)/xp;
			ny0 = float(j)/yp;
			rAccum = gAccum = bAccum = aAccum = 0.0f;

			for (float ny = ny0; ny < ny0 + dy; ++ny)     // Acumula los píxeles intervinientes de la imagen original en el nuevo pixel
			{
				for (float nx = nx0; nx < nx0 + dx; ++nx)
				{
					SDL_GetRGBA(GetPixel((s32)nx, (s32)ny), surface->format, &samp.r, &samp.g, &samp.b, &samp.unused);
					
					// Averigua la cuota de participación del pixel.
					// Para ello vemos el cociente entre la superficie de pixel original en la nueva imagen
					// y la supeficie total de un pixel en unidades de la nueva imagen
					x1 = JMax(nx * xp, i);
					x2 = JMin((nx + 1) * xp, (i + 1));
					y1 = JMax(ny * yp, j);
					y2 = JMin((ny + 1) * yp, (j + 1));
					
					xSize = x2 - x1;
					ySize = y2 - y1;
					ratio = (xSize * ySize);
					
					rAccum += float(samp.r) * ratio;
					gAccum += float(samp.g) * ratio;
					bAccum += float(samp.b) * ratio;
					aAccum += float(samp.unused) * ratio;
				}
			}

			// Finalmente añade el pixel a la nueva imagen
			img->PutPixel(i, j, SDL_MapRGBA(surface->format, (u8)rAccum, (u8)gAccum, (u8)bAccum, (u8)aAccum));
		}
	}
	
	Unlock();
	img->Unlock();

	return img;
}

// Destructor
void JImage::Destroy()
{
	if (surface != 0)
  {
    SDL_FreeSurface(surface);
    surface = 0;
  }
}

void JImage::Ref(JImage &img)
{
	Destroy();

	/**< @todo Quitar esto cuando SDL soporte crear referencias con alguna función. */
  surface = img.surface;

	if (surface != 0)
	{
		++surface->refcount;
		pos = img.Pos();
	}
}

void JImage::operator =(JImage &img)
{
	Destroy();

	/**< @todo Quitar esto cuando SDL soporte crear referencias con alguna función. */
  surface = img.surface;

	if (surface != 0)
	{
		++surface->refcount;
		pos = img.Pos();
	}
}

void JImage::Copy(JImage &img)
{
	Destroy();
	surface = SDL_CreateRGBSurfaceFrom(img.Pixels(), img.Width(), img.Height(), img.BitsPP(), img.Surface()->pitch, 
																		 img.Format()->Rmask, img.Format()->Gmask, img.Format()->Bmask, img.Format()->Amask);

}

bool JImage::Convert(SDL_PixelFormat *fmt, u32 flags)
{
	SDL_Surface *tmp;
  if (0 != (tmp = SDL_ConvertSurface(surface, fmt, flags)))
	{
		Destroy();
		surface = tmp;
		
		return true;
	}
	
	return false;
}

JString JImage::DumpFromSurface(SDL_Surface *s)
{
	JString s1, s2, s3, s4, s5, s6;
	SDL_PixelFormat *fmt = s->format;

	s1.Format("%dx%d@%d COLOR_KEY: %08x SURFACE_ALPHA: %d\n", s->w, s->h, fmt->BitsPerPixel, fmt->colorkey, fmt->alpha);
	s2.Format("pitch: %d clip_rect: x%d y%d w%d h%d pixels: %p\n", s->pitch, s->clip_rect.x, s->clip_rect.y, s->clip_rect.w, s->clip_rect.h, s->pixels);
	s3.Format("RGBAmask: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
	s4.Format("RGBAshift: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", fmt->Rshift, fmt->Gshift, fmt->Bshift, fmt->Ashift);
	s5.Format("RGBAloss: R: 0x%08x G: 0x%08x B: 0x%08x A: 0x%08x\n", fmt->Rloss, fmt->Gloss, fmt->Bloss, fmt->Aloss);
	s6.Format("Flags:\n"
						"SDL_SWSURFACE: %s\n" 
						"SDL_HWSURFACE: %s\n" 
						"SDL_ASYNCBLIT: %s\n" 
						"SDL_ANYFORMAT: %s\n" 
						"SDL_HWPALETTE: %s\n" 
						"SDL_DOUBLEBUF: %s\n" 
						"SDL_FULLSCREEN: %s\n" 
						"SDL_OPENGL: %s\n" 
						"SDL_OPENGLBLIT: %s\n" 
						"SDL_RESIZABLE: %s\n" 
						"SDL_HWACCEL: %s\n" 
						"SDL_SRCCOLORKEY: %s\n" 
						"SDL_RLEACCEL: %s\n" 
						"SDL_SRCALPHA: %s\n" 
						"SDL_PREALLOC: %s\n",
						(s->flags & SDL_SWSURFACE) ? "yes" : "no", 
						(s->flags & SDL_HWSURFACE) ? "yes" : "no", 
						(s->flags & SDL_ASYNCBLIT) ? "yes" : "no", 
						(s->flags & SDL_ANYFORMAT) ? "yes" : "no", 
						(s->flags & SDL_HWPALETTE) ? "yes" : "no", 
						(s->flags & SDL_DOUBLEBUF) ? "yes" : "no", 
						(s->flags & SDL_FULLSCREEN) ? "yes" : "no", 
						(s->flags & SDL_OPENGL) ? "yes" : "no", 
						(s->flags & SDL_OPENGLBLIT) ? "yes" : "no", 
						(s->flags & SDL_RESIZABLE) ? "yes" : "no", 
						(s->flags & SDL_HWACCEL) ? "yes" : "no", 
						(s->flags & SDL_SRCCOLORKEY) ? "yes" : "no", 
						(s->flags & SDL_RLEACCEL) ? "yes" : "no", 
						(s->flags & SDL_SRCALPHA) ? "yes" : "no", 
						(s->flags & SDL_PREALLOC) ? "yes" : "no");
	
	JString str;
	str += s1;
	str += s2;
	str += s3;
	str += s4;
	str += s5;
	str += s6;
	
	return str;
}
