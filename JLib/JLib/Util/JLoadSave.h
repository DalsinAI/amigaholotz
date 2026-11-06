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

/** Interface for the objects to be loaded from and saved to a JRW object.
 * @file    JLoadSave.h
 * @author  Juan Carlos Seijo PÃ©rez
 * @date    24/12/2003
 * @version 0.0.1 - 24/12/2003 - First version.
 */

#ifndef _JLOADSAVE_INCLUDED
#define _JLOADSAVE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>
#include <JLib/Util/JFile.h>
#include <JLib/Util/JRW.h>

/** Interface for the objects to be loaded from and saved to a JRW object.
 * Has methods to be implemented in the child classes to save and load from a JRW source.
 */
class JLoadSave
{
public:
  /** Loads the object from the actual position of the given source.
   * @param  jrw JRW object correctly positioned.
   * @return 0 if ok, 1 if I/O error, 2 if data integrity error.
   */
  virtual u32 Load(JRW &jrw){return 0;};
  
  /** Saves the object to the actual position of the given source.
   * @param  jrw JRW object correctly positioned.
   * @return 0 if ok, 1 if I/O error, 2 if data integrity error.
   */
  virtual u32 Save(JRW &jrw){return 0;};

  /** Loads an array of objects in the given source's actual  position.
	 * The format is: [numObjects (4bytes)][obj_1]...[obj_numObjects]
   * @param  numObjects Number of objects in the array.
   * @param  ls JLoadSave object array.
   * @param  f JRW object correctly positioned.
   * @return 0 if ok, 1 if I/O error, 2 if data integrity error.
   */
  virtual u32 LoadList(u32 *numObjects, JLoadSave *ls, JRW &f) {return 0;}

  /** Saves an array of objects in the given source's actual  position.
	 * The format is: [numObjects (4bytes)][obj_1]...[obj_numObjects]
   * @param  numObjects Number of objects in the array.
   * @param  ls JLoadSave object array.
   * @param  f JRW object correctly positioned.
   * @return 0 if ok, 1 if I/O error, 2 if data integrity error.
   */
  virtual u32 SaveList(u32 numObjects, JLoadSave *ls, JRW &f) {return 0;}

  /** Destroys the object.
   */
  virtual ~JLoadSave() {}
};

#endif  // _JLOADSAVE_INCLUDED
