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

/** Clase base de todos los objetos.
 * @file    JObject.h
 * @author  Juan Carlos Seijo Pérez
 * @date    15/11/2003
 * @version 0.0.1 - 15/11/2003 - Primera versión
 */

#ifndef _JOBJECT_INCLUDED
#define _JOBJECT_INCLUDED

#include <JLib/Util/JTypes.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef _JLIB_DEBUG
#include <vector>
#endif

/** Borra el puntero si no es nulo */
#define JDELETE(p)  if (p){delete p;p = 0;}

/** Borra el array si no es nulo */
#define JDELETE_ARRAY(p) if(p){delete[] p;p = 0;}

/** Borra el array y cada uno de sus punteros si no son nulos ni él mismo es nulo */
#define JDELETE_POINTER_ARRAY(p, sz) if(p){for (s32 __i___ = 0; __i___ < sz; ++__i___) JDELETE((p)[__i___]); delete[] (p); (p) = 0;}

/** Borra el array y cada uno de sus arrays si no son nulos ni él mismo es nulo */
#define JDELETE_ARRAY_ARRAY(p, sz) if(p){for (s32 __i___ = 0; __i___ < sz; ++__i___) JDELETE_ARRAY((p)[__i___]); delete[] (p); (p) = 0;}

/** Clase base de todos los objetos.
 */
class JObject
{
#ifdef _JLIB_DEBUG
protected:
	static std::vector<JObject *> objects;
  static s32 instanceCount;
  static s32 instanceID;
  s32 instanceNum;

public:
	/** Crea un nuevo objeto.
	 */
  JObject();

  /** Destruye el objeto.
   */
  virtual ~JObject();

#else  // NOT _JLIB_DEBUG

public:
  /** Destruye el objeto.
   */
  virtual ~JObject(){}
  
#endif // _JLIB_DEBUG

};

#endif  // _JOBJECT_INCLUDED
