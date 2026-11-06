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

/** Cargador de ficheros ASE.
 * @author: Juan Carlos Seijo Pérez
 * @date: 20/09/2003
 * @notes: Curiosidad de MAX 5, pongo el ejemplo:
 * Supongamos una escena con tres bolas que comparten el mismo material multi/subobjeto
 * compuesto de tres submateriales de id 1, 2 y 3, respectivamente. Si las bolas
 * tienen id's 1, 2 y 3 respectivamente se asignarán de esa forma los submateriales
 * a cada una de ellas. Sin embargo si alguna de ellas tiene un valor diferente (mayor),
 * el material que aplica MAX es el resto de dividir el id de la bola menos uno entre el número
 * de submateriales del material y todo eso más uno. 
 * Así si las bolas tienen id's 1, 22, 8, respectivamente,
 * - a la bola con MTLID 1 se le asigna el submaterial: ((1-1)%3) + 1 = (0%3) + 1 = 0 + 1 = 1
 * - a la bola con MTLID 23 se le asigna el submaterial: ((23-1)%3) + 1 = (21%3) + 1 = 0 + 1 = 1
 * - a la bola con MTLID 7 se le asigna el submaterial: ((8-1)%3) + 1 = (7%3) + 1 = 1 + 1 = 2
 * 
 * Notad que en el ASE el MTLID es uno menos que en la escena ya.
 */

#ifndef _JASELOADER_INCLUDED
#define _JASELOADER_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JTextFile.h>
#include <JLib/Graphics/JASEFormat.h>

/** Cargador de ficheros ASE.
 */
class JASELoader : public JObject
{
  JTextFile f;                      /**< Fichero ASE  */

  /** Carga los objetos (geometría) del fichero ASE.
   * @return <b>true</b> si todo fue bien, <b>false</b> en otro caso.
   */
  bool LoadObjects(JASEFormat *fmt);

  /** Carga los materiales del fichero ASE.
   * @return <b>true</b> si todo fue bien, <b>false</b> en otro caso.
   */
  bool LoadMaterials(JASEFormat *fmt);

public:
  /** Carga un fichero ase.
   * @return: 0 - si el fichero se cargó correctamente,
   * 1 - si no existe el fichero,
   * 2 - si el formato está corrupto,
   * 3 - si fmt es nulo.
   */
  s32 Load(const s8 *fileName, JASEFormat *fmt);
};

#endif  // _JASELOADER_INCLUDED
