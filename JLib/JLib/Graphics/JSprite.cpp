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

/** Sprite genérico.
 * @file    JSprite.cpp.
 * @author  Juan Carlos Seijo Pérez.
 * @date    18/10/2003.
 * @version 0.0.1 - 18/10/2003 - Primera versión.
 */

#include <JLib/Graphics/JSprite.h>

// Inicializa el sprite con los parámetros dados. No crea el array de frames,
// puesto que lo debe hacer la clase hija.
void JSprite::Init(u32 nFrames, u32 _FPS, bool mustLoop, bool mustGoBack, bool mustGoBackwards)
{
  if (nFrames)
    numFrames = nFrames;

  loop = mustLoop;
  goBack = mustGoBack;
  backwards = mustGoBackwards;
  
  if (backwards)
  {
    firstFrame = numFrames - 1;
    lastFrame = 0;
    frameAdvance = -1;
  }
  else
  {
    firstFrame = 0;
    lastFrame = numFrames - 1;
    frameAdvance = 1;
  }

  curFrame = firstFrame;

  FPS(_FPS);
}

// Pasa al siguiente frame si han pasado 1/fps segundos.
// Devuelve el frame actual si ha cambiado, -1 si no cambió 
// y -2 si está en pausa.
s32 JSprite::Update()
{
  if (!paused)
  {
    if (timer.Changed())
    {
      // Cambia de frame
      if (curFrame == lastFrame)
      {
        if (loop)
        {
          // Si debe volver cambia el sentido
          if (goBack)
          {
            s32 tmp;
            tmp = lastFrame;
            lastFrame = firstFrame;
            firstFrame = tmp;

            frameAdvance *= -1;
            curFrame += frameAdvance;

						return curFrame;
          }
          else
          {
            curFrame = firstFrame;

						return curFrame;
          }
        }
				
				return -1;
      }
      else
      {
        curFrame += frameAdvance;
				
				return curFrame;
      }
    }
  }

	return -2;
}

// Establece el número de cuadros por segundo
void JSprite::FPS(const u32 newFPS)
{
  fps = newFPS;

  if (fps)
    timer.Start(1000L/fps);
  else
    timer.Pause();
}

// Va al frame anterior
bool JSprite::PrevFrame()
{
  if (curFrame > 0) 
  {
    --curFrame;
    return true;
  }

  return false;
}

// Va al frame siguiente
bool JSprite::NextFrame()
{
  if (curFrame < numFrames - 1)
  {
    ++curFrame;
    return true;
  }

  return false;
}

// Destruye el sprite, liberando la memoria asociada
void JSprite::Destroy()
{
	if (frames)
	{
		for (u32 i = 0; i < numFrames; ++i)
		{
			JDELETE(frames[i]);
		}

		delete[] frames;
		frames = 0;
	}
}

