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

/** Wrapper C++ para SDL_mixer.
 * @author Juan Carlos Seijo Pérez
 * @file JMixer.h
 * @date 06/03/2004
 * @version 0.0.1 - Primera versión. 06/03/2004.
 */

#ifndef _JMIXER_INCLUDED
#define _JMIXER_INCLUDED

#include <SDL_mixer.h>
#include <JLib/Util/JTypes.h>
#include <JLib/Sound/JChunk.h>

/** Esta clase permite configurar los parámetros del sonido.
 */
class JMixer
{
	bool valid;

 public:
  /** Crea el objeto.
   */
  JMixer() : valid(false)
	{}
	
	/** Determina si el objeto está inicializado o no.
	 * @return <b>true</b> si se puede usar, <b>false</b> si no.
	 */
	bool Valid() {return valid;}

  /** Recupera la versión de SDL_mixer. El objeto devuelto tiene
   * tres campos: major, minor y patch, que identifican la versión.
   * @return La versión de SDL_mixer.
   */
  const SDL_version * GetLinkedVersion() {return Mix_Linked_Version();}

  /** Establece los parámetros del audio. Este método reserva además 16 canales para la
	 * reproducción. Este parámetro se puede cambiar por medio de AllocateChannels().
   * @param freq Frecuencia de muestreo (44.100 Hz, 22050 Hz, etc.) [22050].
   * @param fmt Formato de la muestra (uno de AUDIO_{U|S}{8|16[LSB|MSB|SYS]}) [AUDIO_S16SYS].
   * @param chann Número de canales de salida (1 - mono, 2 - stereo) [2].
   * @param size Bytes por muestra [1024].
   * @return 0 si todo fue bien, -1 en caso de no poderse inicializar SDL y -2 en caso
	 * de no poder abrir el dispositivo.
   */
  s32 Init(s32 freq = MIX_DEFAULT_FREQUENCY, Uint16 fmt = MIX_DEFAULT_FORMAT, s32 chann = 2, s32 size = 1024)
  {
    if (!SDL_WasInit(SDL_INIT_AUDIO))
    {
      if (SDL_Init(SDL_INIT_AUDIO) == -1)
			{
				return -1;
			}
    }

    if (Mix_OpenAudio(freq, fmt, chann, size) == -1)
    {
      return -2;
    }
		
		valid = true;

		AllocateChannels(16);

    return 0;
  }
	
  /** Devuelve la cadena de error de SDL_mixer.
   * @return Cadena del último error producido.
   */
  const char * GetError()
  {
    return Mix_GetError();
  }

  /** Recupera los parámetros del dispositivo de audio.
	 * @param  freq Variable donde se guardará la frecuencia de muestreo.
	 * @param  fmt Variable donde se guardará el formato de la muestra.
	 * @param  chann Variable donde se guardará el número de canales.
	 * @return El número de veces que se ha abierto el dispositivo ó 0 (cero)
	 * si se produjo algún error.
	 */
	s32 QuerySpec(s32 freq, u16 fmt, s32 chann)
	{
		return Mix_QuerySpec(&freq, &fmt, &chann);
	}
	
	/** Reserva canales de reproducción.
	 * Si el número de canales a reservar es menor que el actual, se paran
	 * los que están en exceso. Si fuese negativo se obtiene el número
	 * total de canales reservados, sin que esto afecte a los existentes.
	 * @param  numChannels Número total de canales a reservar.
	 * @return Número de canales reservados en total.
	 */
	s32 AllocateChannels(s32 numChannels)
	{
		return Mix_AllocateChannels(numChannels);
	}

	/** Establece el nivel de volumen de un canal.
	 * @param  channel Canal a modificar (-1 para todos).
	 * @param  volume Nuevo volumen del canal.
	 * @return Valor actual del volumen del canal o un promedio de todos si
	 * channel es -1.
	 */
	s32 Volume(s32 channel = -1, s32 volume = MIX_MAX_VOLUME)
	{
		return Mix_Volume(channel, volume);
	}

	/** Pausa el canal especificado.
	 * @param  channel Número de canal.
	 * @see Resume(), Halt().
	 */
	void Pause(s32 channel = -1)
	{
		Mix_Pause(channel);
	}
	
	/** Reanuda la reproducción del canal especificado.
	 * @param  channel Número de canal.
	 * @see Pause(), Halt().
	 */
	void Resume(s32 channel = -1)
	{
		Mix_Resume(channel);
	}
	
	/** Detiene la reproducción del canal especificado.
	 * @param  channel Número de canal.
	 * @see Resume(), Pause().
	 */
	void Halt(s32 channel = -1)
	{
		Mix_HaltChannel(channel);
	}

	/** Detiene el canal con un fundido.
	 * @param  ms Milisegundos de fade.
	 * @param  channel Canal a detener.
	 * @return Número de canales a detener.
	 */
	s32 FadeOut(s32 ms, s32 channel = -1)
	{
		return Mix_FadeOutChannel(channel, ms);
	}

	/** Determina si el canal dado está reproduciendo o en pausa.
	 * @param  channel Canal a determinar ó -1.
	 * @return 1 si se está reproduciendo, 0 si no o el número de canales
	 * en reproducción o pausa en caso de ser channel = -1.
	 */
	s32 Playing(s32 channel = -1)
	{
		return Mix_Playing(channel);
	}

	/** Determina si el canal dado está en pausa o parado.
	 * @param  channel Canal a determinar ó -1.
	 * @return 1 si está en pausa o parado, 0 si no o el número de canales
	 * en pausa o detenidos en caso de ser channel = -1.
	 */
	s32 Paused(s32 channel = -1)
	{
		return Mix_Paused(channel);
	}

	/** Determina el estado de fade del canal dado.
	 * @param  channel Canal a determinar.
	 * @return MIX_NO_FADING si no hace fade, MIX_FADING_IN si hace fade-in
	 * o MIX_FADING_OUT si hace fade-out.
	 */
	s32 Fading(s32 channel)
	{
		return Mix_Fading(channel);
	}

	/** Detiene la reproducción del canal especificado en un tiempo dado.
	 * @param  ms Milisegundos a esperar antes de detenerlo.
	 * @param  channel Número de canal.
	 * @see Halt().
	 */
	void HaltTimed(s32 ms, s32 channel = -1)
	{
		Mix_ExpireChannel(channel, ms);
	}

	/** Cierra el dispositvo de audio.
   */
	void Destroy()
	{
    // La especificación no dice que se le puedan pasar ceros a Mix_QuerySpec().
    s32 freq, chann;
    u16 fmt;

    // Debe cerrarse tantas veces como haya sido abierto
		s32 numTimes = Mix_QuerySpec(&freq, &fmt, &chann);
    for (int i = 0; i < numTimes; ++i)
    {
      Mix_CloseAudio();
    }
	}

  /** Cierra el dispositvo de audio.
   */
  ~JMixer()
  {
		Destroy();
  }
};

#endif  // _JMIXER_INCLUDED
