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
 * @file    JUtil.h.
 * @author  Juan Carlos Seijo Pérez
 * @date    16/10/2003
 * @version 0.0.1 - 16/10/2003 - Primera versión.
 */

#ifndef _JUTIL_INCLUDED
#define _JUTIL_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JTextFile.h>
#include <vector>

class JImage;

/** Devuelve la palabra (32) menos significativa */
#define JLOWORD(x) ((x) & 0x0000FFFF)

/** Devuelve la palabra más significativa */
#define JHIWORD(x) ((x) & 0xFFFF0000)

/** Devuelve el byte menos significativo */
#define JLOBYTE(x) ((x) & 0x00FF)

/** Devuelve el byte más significativo */
#define JHIBYTE(x) ((x) & 0xFF00)

/** Devuelve el mínimo entre dos números. 
 * @param a Primer número.
 * @param b Segundo número.
 * @return Menor de los dos números.
 */
inline float JMin(float a, float b)
{
	return a < b ? a : b;
}

/** Devuelve el máximo entre dos números. 
 * @param a Primer número.
 * @param b Segundo número.
 * @return Mayor de los dos números.
 */
inline float JMax(float a, float b)
{
	return a > b ? a : b;
}

/** Recorta el valor por arriba y por abajo para que no sobrepase min y max.
 * @param  v Valor a recortar.
 * @param  min Valor mínimo que puede tomar.
 * @param  max Valor máximo que puede tomar.
 */
inline void JClamp(s8& v, s8 min, s8 max)
{
	if (v < min) {v = min; return;}	if (v > max) {v = max; return;}
}

/** Recorta el valor por arriba y por abajo para que no sobrepase min y max.
 * @param  v Valor a recortar.
 * @param  min Valor mínimo que puede tomar.
 * @param  max Valor máximo que puede tomar.
 */
inline void JClamp(u8& v, u8 min, u8 max)
{
	if (v < min) {v = min; return;}	if (v > max) {v = max; return;}
}

/** Recorta el valor por arriba y por abajo para que no sobrepase min y max.
 * @param  v Valor a recortar.
 * @param  min Valor mínimo que puede tomar.
 * @param  max Valor máximo que puede tomar.
 */
inline void JClamp(s16& v, s16 min, s16 max)
{
	if (v < min) {v = min; return;}	if (v > max) {v = max; return;}
}

/** Recorta el valor por arriba y por abajo para que no sobrepase min y max.
 * @param  v Valor a recortar.
 * @param  min Valor mínimo que puede tomar.
 * @param  max Valor máximo que puede tomar.
 */
inline void JClamp(u16& v, u16 min, u16 max)
{
	if (v < min) {v = min; return;}	if (v > max) {v = max; return;}
}

/** Recorta el valor por arriba y por abajo para que no sobrepase min y max.
 * @param  v Valor a recortar.
 * @param  min Valor mínimo que puede tomar.
 * @param  max Valor máximo que puede tomar.
 */
inline void JClamp(s32& v, s32 min, s32 max)
{
	if (v < min) {v = min; return;}	if (v > max) {v = max; return;}
}

/** Recorta el valor por arriba y por abajo para que no sobrepase min y max.
 * @param  v Valor a recortar.
 * @param  min Valor mínimo que puede tomar.
 * @param  max Valor máximo que puede tomar.
 */
inline void JClamp(u32& v, u32 min, u32 max)
{
	if (v < min) {v = min; return;}	if (v > max) {v = max; return;}
}

/** Recorta el valor por arriba y por abajo para que no sobrepase min y max.
 * @param  v Valor a recortar.
 * @param  min Valor mínimo que puede tomar.
 * @param  max Valor máximo que puede tomar.
 */
inline void JClamp(float& v, float min, float max)
{
	if (v < min) {v = min; return;}	if (v > max) {v = max; return;}
}

/** Escribe un fichero raw con los bits de la fuente del fichero TGA dado.
 * @param TGAFileName Nombre del fichero targa con la fuente ordenada.
 * deben ser caracteres de 8x8 organizados en 16 filas por 16 columnas.
 * @param outFileName Nombre del fichero de salida.
 */
void JDumpTGAFontBits(const s8 *TGAFileName, const s8 *outFileName);

/** Modo gráfico.
 */
struct JVideoMode
{
	s32 w;                                /**< Anchura del modo. */
	s32 h;                                /**< Altura del modo. */
	s32 bpp;                              /**< Profundidad de color en bits del modo. */
	bool hw;                              /**< Indicador de aceleración hardware. */
};

/** Devuelve un puntero a una tabla con los modos gráficos disponibles. Es necesario
 * borrar con JDELETE_ARRAY() el array resultante.
 * @param depth Profundidad para la que se quieren obtener los modos.
 * @param count Salida con el número de modos encontrados.
 * @return Lista de modos disponibles en pantalla completa tanto acelerados como no acelerados por HW.
 * El último elemento del array tiene anchura y altura -1.
 * Si devuelve 0 es que no existe ningún modo.
 */
JVideoMode * JListVideoModes(s32 *count, s32 depth = 0);

/** Muestra en stderr las propiedades de una imagen, los n primeros 
 * píxeles y los n primeros píxeles diferentes de la CKEY.
 * @param  img Imagen a examinar.
 * @param  n Número de píxeles a mostrar.
 */
void JDumpImage(JImage *img, s32 n = 10);

#endif  // _JUTIL_INCLUDED
