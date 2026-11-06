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

/** Sprite de imágenes.
 * @file    JImageSprite.cpp.
 * @author  Juan Carlos Seijo Pérez.
 * @date    18/10/2003.
 * @version 0.0.1 - 18/10/2003 - Primera versión.
 * @version 0.0.2 - 01/06/2004 - Adición de método de copia.
 */

#include <JLib/Graphics/JImageSprite.h>

// Constructor copia
JImageSprite::JImageSprite(JImageSprite &spr)
 : wMax(0), hMax(0)
{
	Ref(spr);
}

// Carga el sprite desde un fichero.
// Si frameW != 0 y frameH == 0, considera que las frames están en horizontal.
// Si frameW == 0 y frameH != 0, considera que las frames están en vertical.
// Si frameW != 0 y frameH != 0, intenta encajarlas en H o V, si no intenta encajar los cuadrados.
// Si frameW == 0 y frameH == 0 o hay una incoherencia o un error, devuelve false.
// Si todo va bien, devuelve true.
bool JImageSprite::Load(const JString &fileName, u32 frameW,  u32 frameH,  u32 _numFrames, u32 colorKey)
{
	Destroy();
	
  JImage img;
  wMax = 0;
  hMax = 0;
  
  if (!img.Load(fileName))
    return false;

  if (_numFrames != 0)
  {
    numFrames = _numFrames;
  }

  // Para recorrer la imagen
  u32 iMax, jMax;

  if (frameW == 0)
  {
    if (frameH == 0)
    {
      return false;
    }
    else
    {
      // Frames en vertical.
      if (numFrames == 0)
      {
        // Averigua las que caben en la altura de la imagen...
        s32 mod = img.Height() % frameH;
        s32 numFr = img.Height()/frameH;
        
        // ERROR: No hay un número entero de frames
        if (mod != 0)
          return false;
        
        numFrames = numFr;
      }

      // Límites de recorrido de la imagen
      iMax = 1;
      jMax = numFrames;
      frameW = img.Width();
    }
  }
  else
  {
    if (frameH == 0)
    {
      // Frames en horizontal.
      if (numFrames == 0)
      {
        // Averigua las que caben en la anchura de la imagen...
        s32 mod = img.Width() % frameW;
        s32 numFr = img.Width()/frameW;
        
        // ERROR: No hay un número entero de frames
        if (mod != 0)
          return false;
        
        numFrames = numFr;
      }

      // LÃ­mites de recorrido de la imagen
      iMax = numFrames;
      jMax = 1;
      frameH = img.Height();
    }
    else
    {
      // Frames con ancho y alto.
      // Las supone dispuestas en un cuadrado de (n x frameW) x (m x frameH).
      s32 modi, modj;
      modi = img.Width()%frameW;
      modj = img.Height()%frameH;
      
      // ERROR: No hay un número entero de frames
      if (modi != 0 || modj != 0)
        return false;

      iMax = img.Width()/frameW;
      jMax = img.Height()/frameH;

      // Si no se da el número de frames se asumen n x m
      if (numFrames == 0)
      {
        numFrames = iMax * jMax;
      }
    }

    frames = (JDrawable**)new JImage*[numFrames];

    JImage *tmpImg;

    // Extrae los frames del sprite de la imagen
    for (u32 j = 0, count = 0; j < jMax && count < numFrames; ++j)
    {
      for (u32 i = 0; i < iMax && count < numFrames; ++i, ++count)
      {
        frames[(j * iMax) + i] = tmpImg = new JImage(frameW, frameH);
        tmpImg->Paste(&img, i * frameW, j * frameH, frameW, frameH);
        tmpImg->ColorKey(colorKey);
      }
    }
    
    // Tamaño fijo de frame
    wMax = frameW;
    hMax = frameH;

    return true;
  }

  return false;
}

// Crea el sprite a partir varios ficheros de imagen, uno por frame.
// Si todo va bien, devuelve true.
bool JImageSprite::Load(JString *imageFiles, u32 _numFrames, u32 colorKey)
{
	Destroy();

  bool error = false;
  wMax = hMax = 0;
  numFrames = _numFrames;
  JImage *tmpImg;
  frames = (JDrawable**)new JImage*[numFrames];
  u32 i;

  for (i = 0; i < numFrames && !error; ++i)
  {
    frames[i] = tmpImg = new JImage();
    
    if (tmpImg->Load(imageFiles[i]))
    {
      tmpImg->ColorKey(colorKey);

      // Asigna la anchura y altura máximas del sprite
      if (wMax < tmpImg->Width())
        wMax = tmpImg->Width();

      if (hMax < tmpImg->Height())
        hMax = tmpImg->Height();
    }
    else
    {
      delete tmpImg;
      error = true;
    }
  }

  if (error)
  {
    // Error al cargar
    for (u32 j = 0; j < i; ++j)
    {
      delete frames[i];
      frames[i] = 0;
    }
  }

  return !error;
}

// Dibuja el sprite en pantalla
void JImageSprite::Draw()
{
  SDL_Rect rc;
  rc.x = (s16)(X() + frames[curFrame]->X());
  rc.y = (s16)(Y() + frames[curFrame]->Y());

  SDL_BlitSurface(((JImage *)frames[curFrame])->Surface(), 0, SDL_GetVideoSurface(), &rc);
}

// Utilidad para alinear arriba los frames del sprite
void JImageSprite::AlignUp()
{
  for (u32 i = 0; i < numFrames; ++i)
  {
    frames[i]->Pos((s32)frames[i]->X(), 0);
  }
}

// Utilidad para alinear abajo los frames del sprite
void JImageSprite::AlignDown()
{
  for (u32 i = 0; i < numFrames; ++i)
  {
    frames[i]->Pos((s32)frames[i]->X(), (s32)(hMax - ((JImage*)frames[i])->Height()));
  }
}

// Utilidad para alinear a la izquierda los frames del sprite
void JImageSprite::AlignLeft()
{
  for (u32 i = 0; i < numFrames; ++i)
  {
    frames[i]->Pos(0, (s32)frames[i]->Y());
  }
}

// Utilidad para alinear derecha los frames del sprite
void JImageSprite::AlignRight()
{
  for (u32 i = 0; i < numFrames; ++i)
  {
    frames[i]->Pos((s32)(wMax - ((JImage*)frames[i])->Width()), (s32)frames[i]->Y());
  }
}

// Utilidad para ajustar al máximo el borde del sprite
void JImageSprite::AdjustSize()
{
  bool done;
  s32 x, y, w, h;
  s32 du = 0, dd = 0, dl = 0, dr = 0;     // Cantidad de borde sobrante
  JImage *img = 0, *tmpImg = 0;
  SDL_Rect rc;
  
  // Anchura y altura máximas del sprite
  wMax = hMax = 0;

  for (u32 n = 0; n < numFrames; ++n)
  {
    img = (JImage *)frames[n];
    if (0 == img->Lock())
    {
      w = img->Width();
      h = img->Height();

      // Calcula por arriba
      done = false;
      for (y = 0; y < h && !done; ++y)
      {
        for (x = 0; x < w && !done; ++x)
        {
          if (img->GetPixel(x, y) != img->ColorKey())
            done = true;
        }
      }

      du = y - 1;

      // Calcula por abajo
      done = false;
      for (y = h - 1; y >= 0 && !done; --y)
      {
        for (x = 0; x < w && !done; ++x)
        {
          if (img->GetPixel(x, y) != img->ColorKey())
            done = true;
        }
      }

      dd = h - 1 - y - 1;

      // Calcula por la izquierda
      done = false;
      for (x = 0; x < w && !done; ++x)
      {
        for (y = 0; y < h && !done; ++y)
        {
          if (img->GetPixel(x, y) != img->ColorKey())
            done = true;
        }
      }

      dl = x - 1;

      // Calcula por la derecha
      done = false;
      for (x = w - 1; x >= 0 && !done; --x)
      {
        for (y = 0; y < h && !done; ++y)
        {
          if (img->GetPixel(x, y) != img->ColorKey())
            done = true;
        }
      }

      dr = w - 1 - x - 1;

      rc.x = (s16)dl;
      rc.y = (s16)du;
      rc.w = (u16)(w - dl - dr);
      rc.h = (u16)(h - du - dd);

      if (wMax < rc.w) 
        wMax = rc.w;

      if (hMax < rc.h) 
        hMax = rc.h;

      img->Unlock();

      // Si no hay cambios lo deja
      if (rc.w < w || rc.h < h)
      {
        // Reduce el area de la imagen
        tmpImg = new JImage(rc.w, rc.h, img->BitsPP());
        tmpImg->Paste(img, rc.x, rc.y, rc.w, rc.h);
        tmpImg->ColorKey(img->ColorKey());
				
				// Actualiza la posición relativa del frame dentro del sprite
				// para no alterarla
				tmpImg->Pos((s32)(tmpImg->X() + rc.x), (s32)(tmpImg->Y() + rc.y));

        delete frames[n];
        frames[n] = tmpImg;
      }
    }
  }
}

// Carga el sprite.
u32 JImageSprite::Load(JRW &f)
{
  if (f.ReadLE32(&fps) &&
      f.ReadLE32(&numFrames) &&
      f.ReadBool(&loop) &&
      f.ReadBool(&goBack) &&
      f.ReadBool(&backwards))
  {
    JImage *img;
    s32 xPos, yPos;
    u32 ok = 0;
    u32 i;

    frames = (JDrawable**)new JImage*[this->numFrames];
    
    for (i = 0; i < numFrames && ok == 0; ++i)
    {
      frames[i] = img = new JImage;

      // Carga las coordenadas relativas del frame
      if (0 == f.ReadLE32(&xPos) ||
          0 == f.ReadLE32(&yPos))
      {
        ok = 1; // Fallo de lectura
      }
      else
      {
        img->Pos(xPos, yPos);

        // Carga la imagen
        ok |= ((JImage *)frames[i])->Load(f);
				if (ok == 0)
				{
					// Asigna la anchura y altura máximas del sprite
					if (wMax < ((JImage *)frames[i])->Width())
						wMax = ((JImage *)frames[i])->Width();
					
					if (hMax < ((JImage *)frames[i])->Height())
						hMax = ((JImage *)frames[i])->Height();
				}
      }
    }

    if (ok == 0)
    {
      Init(numFrames, fps, loop, goBack, backwards);
    }
    else
    {
      // Borra lo creado si hubo un error
      for (u32 j = 0; j < i; ++j)
      {
        delete frames[j];
      }

      delete[] frames;
      frames = 0;
    }

    return ok;
  }

  return 1;
}

// Salva el sprite
u32 JImageSprite::Save(JRW &f)
{
  if (f.WriteLE32(&fps) &&
      f.WriteLE32(&numFrames) &&
      f.WriteBool(&loop) &&
      f.WriteBool(&goBack) &&
      f.WriteBool(&backwards))
  {
    JImage *img;
    s32 xPos, yPos;

    u32 ok = 0;
    
    for (u32 i = 0; i < numFrames; ++i)
    {
      img = ((JImage *)frames[i]);

      // Salva las coordenadas relativas del frame
      xPos = (s32)img->X();
      yPos = (s32)img->Y();
      f.WriteLE32(&xPos);
      f.WriteLE32(&yPos);

      ok |= img->Save(f);
    }

    return ok;
  }

  return 1;
}

void JImageSprite::Ref(JImageSprite& spr)
{
  Destroy();

	Init(spr.NumFrames(), spr.FPS(), spr.Loop(), spr.GoBack(), spr.Backwards());
  paused = spr.Paused();
	wMax = spr.MaxW();
	hMax = spr.MaxH();
  
  frames = (JDrawable**)new JImage*[spr.NumFrames()];
	
  // Referencia los frames del sprite
  for (u32 i = 0; i < numFrames; ++i)
  {
    frames[i] = new JImage;
    ((JImage *)frames[i])->Ref(*((JImage *)spr.Frame(i)));
  }
}
