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
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

/** Wrapper C++ para una muestra de audio de SDL_mixer.
 * @file    JChunk.h
 * @author  Juan Carlos Seijo Pérez
 * @date    25/03/2004
 * @version 0.0.1 - 25/03/2004 - Primera versión.
 */

#ifndef _JCHUNK_INCLUDED
#define _JCHUNK_INCLUDED

#include <SDL_mixer.h>

/** Encapsula una muestra de audio.
 */
class JChunk
{
	s32 last_channel;                     /**< Último canal en el que se reprodujo. */
	Mix_Chunk *chunk;                     /**< Muestra asociada. */

 public:
	/** Crea una muestra vacía.
	 */
	JChunk() : chunk(0)
	{}
	
	/** Libera los recursos asociados a esta muestra.
	 */
	void Destroy()
	{
		if (chunk != 0)
		{
			// Stop() no se debe liberar en reproducción!
			Mix_FreeChunk(chunk);
			chunk = 0;
		}
	}
	
	/** Libera los recursos asociados a esta muestra.
	 */
	~JChunk()
	{
		Destroy();
	}

	/** Carga una muestra de audio desde fichero. El archivo puede ser de
	 * tipo WAVE, AIFF, RIFF, OGG o VOC.
	 * @param  filename Nombre del fichero de audio a abrir.
	 * @return <b>true</b> si todo fue bien, <b>false</b> en caso contrario.
	 */
	bool LoadWave(const char *filename)
	{
		Destroy();
		chunk = Mix_LoadWAV(filename);

		return chunk != 0;
	}

	/** Establece el volumen de esta muestra. El volumen tiene un valor comprendido
	 * entre 0 y MIX_MAX_VOLUME (128 actualmente). Si el valor es negativo el volumen
	 * no cambia, si es mayor que el máximo, se establece al máximo.
	 * @param  newVolume Nuevo valor para el volumen.
	 * @return Valor del volumen antiguo.
	 */
	s32 Volume(s32 newVolume)
	{
		return Mix_VolumeChunk(chunk, newVolume);
	}

	/** Reproduce esta muestra.
	 * @param  channel Canal en el que reproducir (o -1 para cualquiera libre).
	 * @param  loops Número de veces a reproducir. -1 hace que se repita
	 * todo el tiempo.
	 * @return Canal donde se reproducirá la muestra ó -1 en caso de 
	 * error. El error se podría deber a falta de canales libres.
	 */
	s32 Play(s32 channel = -1, s32 loops = 0)
	{
		return (last_channel = Mix_PlayChannel(channel, chunk, loops));
	}

	/** Reproduce esta muestra con límite de tiempo.
	 * @param  msMax Número máximo de milisegundos a reproducir de la muestra.
	 * @param  channel Canal en el que reproducir (o -1 para cualquiera libre).
	 * @param  loops Número de veces a reproducir. -1 hace que se repita
	 * todo el tiempo.
	 * @return Canal donde se reproducirá la muestra ó -1 en caso de 
	 * error. El error se podría deber a falta de canales libres.
	 */
	s32 PlayTimed(s32 msMax, s32 channel = -1, s32 loops = 0)
	{
		return (last_channel = Mix_PlayChannelTimed(channel, chunk, loops, msMax));
	}

	/** Reproduce esta muestra con un fundido.
	 * @param  msFade Milisegundos de duración del efecto fade-in.
	 * @param  channel Canal en el que reproducir (o -1 para cualquiera libre).
	 * @param  loops Número de veces a reproducir. -1 hace que se repita
	 * todo el tiempo.
	 * @return Canal donde se reproducirá la muestra ó -1 en caso de 
	 * error. El error se podría deber a falta de canales libres.
	 */
	s32 FadeIn(s32 msFade, s32 channel = -1, s32 loops = 0)
	{
		return (last_channel = Mix_FadeInChannel(channel, chunk, loops, msFade));
	}

	/** Reproduce esta muestra con límite tiempo y un fundido.
	 * @param  msFade Milisegundos de duración del efecto fade-in.
	 * @param  msMax Número máximo de milisegundos a reproducir de la muestra.
	 * @param  channel Canal en el que reproducir (o -1 para cualquiera libre).
	 * @param  loops Número de veces a reproducir. -1 hace que se repita
	 * todo el tiempo.
	 * @return Canal donde se reproducirá la muestra ó -1 en caso de 
	 * error. El error se podría deber a falta de canales libres.
	 */
	s32 FadeInTimed(s32 msFade, s32 msMax, s32 channel = -1, s32 loops = 0)
	{
		return (last_channel = Mix_FadeInChannelTimed(channel, chunk, loops, msFade, msMax));
	}


	/** Pausa la reproducción.
	 * @see Resume(), Halt().
	 */
	void Pause()
	{
		if (Mix_GetChunk(last_channel) == chunk)
		{
			Mix_Pause(last_channel);
		}
	}
	
	/** Reanuda la reproducción.
	 * @see Pause(), Halt().
	 */
	void Resume()
	{
		if (Mix_GetChunk(last_channel) == chunk)
		{
			Mix_Resume(last_channel);
		}
	}
	
	/** Detiene la reproducción.
	 * @see Resume(), Pause().
	 */
	void Halt()
	{
		if (Mix_GetChunk(last_channel) == chunk)
		{
			Mix_HaltChannel(last_channel);
		}
	}

	/** Detiene la reproducción con un fundido.
	 * @param  ms Milisegundos de fade.
	 */
	void FadeOut(s32 ms)
	{
		if (Mix_GetChunk(last_channel) == chunk)
		{
			Mix_FadeOutChannel(last_channel, ms);
		}
	}

	/** Determina si está reproduciendo o en pausa.
	 * @return <b>true</b> si se está reproduciendo, <b>false</b> si no.
	 */
	bool IsPlaying()
	{
		if (Mix_GetChunk(last_channel) == chunk)
		{
			return 1 == Mix_Playing(last_channel);
		}
		
		return false;
	}

	/** Determina si está en pausa o parado.
	 * @return <b>true</b> si se está en pausa o parado, <b>false</b> si no.
	 */
	s32 IsPaused()
	{
		if (Mix_GetChunk(last_channel) == chunk)
		{
			return Mix_Paused(last_channel);
		}

		return false;
	}

	/** Determina el estado de fade.
	 * @return MIX_NO_FADING si no hace fade o no está reproduciendo, 
	 * MIX_FADING_IN si hace fade-in o MIX_FADING_OUT si hace fade-out.
	 */
	s32 Fading()
	{
		if (Mix_GetChunk(last_channel) == chunk)
		{
			return Mix_Fading(last_channel);
		}

		return MIX_NO_FADING;
	}

	/** Devuelve el canal en el que se reproduce la muestra.
	 * @return Canal en el que reproduce el sonido o -1 si no se está reproduciendo.
	 */
	s32 GetChannel()
	{
		if (Mix_GetChunk(last_channel) == chunk)
		{
			return last_channel;
		}

		return -1;
	}

	/** Devuelve si el objeto es válido o no.
	 * @return <b>true</b> si lo es, <b>false</b> si no.
	 */
	bool Valid() {return chunk != 0;}
	
	operator Mix_Chunk * () {return chunk;}
};

#endif // _JCHUNK_INCLUDED
