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
 * @file    JImage.h.
 * @author  Juan Carlos Seijo Pérez
 * @date    14/10/2003
 * @version 0.0.1 - 14/10/2003 - Primera varsión.
 * @version 0.0.2 - 01/06/2004 - Adición de métodos de referencia y copia.
 * @version 0.0.3 - 04/04/2005 - Adición de métodos de manejo de JRW.
 */

#ifndef _JIMAGE_INCLUDED
#define _JIMAGE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JUtil.h>
#include <SDL.h>
#include <SDL_image.h>

/** Imágen genérica.
 */
class JImage : public JDrawable
{
 protected:
  SDL_Surface *surface;             /**< Superficie SDL. */

public:
  /** Crea una imagen vacía.
   */
  JImage();
  
  /** Crea una imagen de anchura altura y profundidad dadas.
   * Si la profundidad es cero, se toma la del framebuffer.
   * @param  w Anchura.
   * @param  h Altura.
   * @param  _bpp Profundidad en bits por pixel.
   */
  JImage(u32 w, u32 h, u32 _bpp = 0);

  /** Crea la imagen a partir de la superficie SDL dada.
	 * No hace una copia, establece esta como la superficie interna.
	 * @param  surf Superficie SDL con los datos.
   */
  JImage(SDL_Surface *surf) {surface = surf;}

  /** Crea una imagen copia de la imagen dada.
	 * @param  img Imagen que se quiere copiar.
	 * @see Ref().
   */
  JImage(JImage &img);

  /** Libera la memoria asociada.
   */
  virtual ~JImage() {Destroy();}

  /** Libera la memoria asociada.
   */
  void Destroy();

  /** Carga la imagen desde fichero.
   * @param  filename Nombre del fichero con la imagen.
   * @param  toDisplayFormat Indica si se debe convertir al formato de pantalla.
	 * @param  cKey Color transparente.
   * @return <b>true</b> si todo fue bien, <b>false</b> en caso contrario.
   */
  bool Load(const char *filename, bool toDisplayFormat = true, u32 cKey = 0x00000000);

  /** Crea la superficie de imagen vacía.
   * Si data no es cero carga una copia en la superficie.
   * @param  w Anchura.
   * @param  h Altura.
   * @param  _bpp Profundidad en bits por pixel.
   * @param  data Datos de la superficie en el formato correcto o cero para crearla vacía.
   * @param  rMask Máscara de bits para el rojo.
   * @param  gMask Máscara de bits para el verde.
   * @param  bMask Máscara de bits para el azul.
   * @param  aMask Máscara de bits para el alfa.
	 */
  bool Create(u32 w, u32 h, u32 _bpp, void *data = 0, u32 rMask = 0, u32 gMask = 0, u32 bMask = 0, u32 aMask = 0);

  /** Devuelve la anchura.
   * @return Anchura.
   */
  s32 Width() {return surface->w;};

  /** Devuelve la altura.
   * @return Altura.
   */
  s32 Height() {return surface->h;};

  /** Devuelve la profundidad de color en bits
   * @return Profundidad de color en bits.
   */
  s32 BitsPP() {return surface->format->BitsPerPixel;};

  /** Devuelve la profundidad de color en bytes
   * @return Profundidad de color en bytes.
   */
  s32 BytesPP() {return surface->format->BytesPerPixel;};

  /** Devuelve los datos.
   * @return Datos asociados.
   */
  u8 * Pixels() {return ((u8 *)surface->pixels);};

  /** Devuelve el tamaño de línea, en bytes.
   * @return El tamaño de línea, en bytes.
   */
  s32 Pitch() {return surface->pitch;};

  /** Devuelve un puntero a la línea dada
   * @param  nRow Línea (coordenada y) a obtener un puntero a su comienzo.
   * @return puntero a la línea dada.
   */
  u8 * Line(u32 nRow) {return (Pixels() + (surface->pitch * nRow));};

  /** Devuelve el tamaño total en bytes de la imagen.
   * @return Tamaño total en bytes de la imagen.
   */
  u32 Size() {return Width() * Height() * BytesPP();};

  /** Bloquea la superficie.
   * @return 0 si todo va bien, -1 en caso de error.
   */
  inline s32 Lock() {if (SDL_MUSTLOCK(surface)) return SDL_LockSurface(surface); return 0;}

  /** Desbloquea la superficie.
   */
  void Unlock() {if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);}

  /** Establece el alfa global de la superficie, habilita el flag de alfa de superficie y deshabilita el alfa por píxel.
   * @param alpha Nuevo valor de transparencia.
   * @return 0 si todo va bien, -1 en caso de error.
   */
  s32 Alpha(u8 alpha, u32 flags = SDL_SRCALPHA | SDL_RLEACCEL) {return SDL_SetAlpha(surface, flags, alpha);}

  /** Devuelve el alfa asociado a la superficie
   * @return Alfa de la superficie.
   */
  u8 Alpha() {return surface->format->alpha;}
	
  /** Establece el color transparente y habilita el uso de color transparente en la superficie.
   * @param  key Color transparente.
   * @return 0 si todo va bien, -1 en caso de error.
   */
  s32 ColorKey(u32 key, u32 flags = SDL_SRCCOLORKEY | SDL_RLEACCEL) {return SDL_SetColorKey(surface, flags, key);}

	/** Devuelve el color transparente
   * @return Color transparente.
   */
  u32 ColorKey() {return surface->format->colorkey;}

  /** Pega el contenido de una imagen en esta. No comprueba que 
   * no se sobrepasen los límites de la imagen.
   * @return <b>true</b> si todo fue bien, <b>false</b> en caso contrario.
   */
  bool Paste(JImage *srcImg, s32 xSrc, s32 ySrc, s32 wSrc, s32 hSrc, s32 xDst = 0, s32 yDst = 0);

  /** Devuelve la superficie.
   * @return Superficie de datos de la imagen.
   */
  SDL_Surface * Surface() const {return surface;}

	/** Devuelve el formato de pixel de esta imagen.
	 * @return Formato de pixel de esta imagen.
	 */
	SDL_PixelFormat * Format() {return surface->format;}

  /** Dibuja la imagen.
   */
  virtual void Draw();

  /** Dibuja la imagen en la posición dada.
   * @param  x Posición x en pantalla.
   * @param  y Posición y en pantalla.
   */
  virtual void Draw(s32 x, s32 y);

  /** Devuelve el valor del pixel en la posición dada.
   * La imagen debe estar bloqueada.
   * @param  x Posición x desde la esquina superior izquierda.
   * @param  y Posición y desde la esquina superior izquierda.
   * @return Color del pixel pedido.
   */
  u32 GetPixel(s32 x, s32 y);

  /** Establece el valor del pixel en la posición dada.
   * La imagen debe estar bloqueada.
   * @param  x Posición x desde la esquina superior izquierda.
   * @param  y Posición y desde la esquina superior izquierda.
   * @param  color Color del pixel dado.
   */
  void PutPixel(s32 x, s32 y, u32 color);

  /** Guarda la imagen como BMP
   * @param file Nombre del fichero donde se guardará.
   * @return <b>true</b> si todo fue bien, <b>false</b> en caso contrario.
   */
  bool SaveAsBMP(const char *file);

  /** Carga la imagen.
   * @param  f Fichero abierto y posicionado para cargar el objeto.
   * @return 0 si todo va bien, 1 en caso de error de E/S.
   * 2 en caso de incoherencia de los datos.
   */
  virtual u32 Load(JRW &f);

  /** Salva la imagen.
   * @param  f Fichero abierto y posicionado para salvar el objeto.
   * @return 0 si todo va bien, 1 en caso de error de E/S.
   * 2 en caso de incoherencia de los datos.
   */
  virtual u32 Save(JRW &f);

  /** Carga el objeto desde la posición actual del fichero dado. El objeto debe contener los datos de fichero
	 * en formato de imagen estándar de SDL_Image (TGA, BMP, JPG, etc.)
   * @param  jrw Objeto JRW posicionado correctamente.
   * @return 0 si todo va bien, 1 en caso de error de E/S.
   * 2 en caso de incoherencia de los datos.
   */
  virtual u32 LoadImage(JRW &jrw);
  
	/** Crea una referencia de la imagen dada. Los datos apuntan al mismo sitio
	 * y cualquier modificación en una se reflejará en la otra. La imagen no
	 * se destruye hasta que se borran todas las referencias. Si la imagen dada
	 * está vacía, no crea la referencia.
	 */
	void Ref(JImage &img);

	/** Devuelve una referencia de la imagen asignada. Los datos apuntan al mismo sitio
	 * y cualquier modificación en una se reflejará en la otra. La imagen no
	 * se destruye hasta que se borran todas las referencias. Si la imagen dada
	 * está vacía, no crea la referencia.
	 */
	void operator =(JImage &img);

	/** Devuelve una copia de la imagen dada. Es equivalente al constructor copia.
	 */
	void Copy(JImage &img);

	/** Rellena la imagen con el color dado.
	 * @param  color Color con el que rellenar.
	 */
	void Fill(u32 color) {SDL_FillRect(surface, &surface->clip_rect, color);}

	/** Escala la imagen el porcentaje dado en cada dirección. No altera esta imagen.
	 * @param  xp Porcentaje de escalado en la dirección x.
	 * @param  yp Porcentaje de escalado en la dirección y.
	 * @return Nueva imagen escalada.
	 */
	JImage * Scale(float xp, float yp);
	
	/** Convierte esta imagen al formato de la dada. También toma sus flags.
	 * @param fmt Formato de pixel al que convertir.
	 * @param flags Flags a asignar.
	 * @return <b>true</b> si se pudo convertir, <b>false</b> si no.
	 */
	bool Convert(SDL_PixelFormat *fmt, u32 flags);
	
	/** Retruns a string with information about this image.
	 * @return String with information about this image.
	 */
	JString Dump() {return DumpFromSurface(surface);}

	/** Retruns a string with information about an sdl surface.
	 * @return String with information about an sdl surface.
	 */
	static JString DumpFromSurface(SDL_Surface *s);
};

#endif  // _JIMAGE_INCLUDED
