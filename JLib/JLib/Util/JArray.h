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

/** Dynamic array template.
 * @file    JArray.h
 * @author  Juan Carlos Seijo Pérez
 * @date    23/10/2005
 * @version 0.0.1 - 23/10/2005 - First version.
 */

#ifndef _JARRAY_INCLUDED
#define _JARRAY_INCLUDED

#include <string.h>

#include <JLib/Util/JObject.h>

/** This class allows to track the size of a dinamically created array of elements, grow and shrink it.
 * Basically unloads from the task of keeping a variable with the array size associated to an array.
 */
template<class T, unsigned int max = 128>
class JArray
{
protected:
  int maxSize;                          /**< Maximum number of elements this array can store */
  T *elements;                          /**< Array with the elements */

 public:
  /** Creates an empty array. By default the memory is zeroed.
   */
	JArray(int sz = max) : maxSize(sz) {elements = new T[maxSize]; Zero();}
  
  /** Grows by the specified amount (by default, the amount is the max template parameter).
   * @param  amount Amount to grow.
   */
	void Grow(unsigned int amount = max) 
  {
    T *newElements = new T[maxSize + amount]; 
    memset(newElements, 0, sizeof(T) * (maxSize + amount));
    for (int i = 0; i < maxSize; ++i) newElements[i] = elements[i];
    delete[] elements; elements = newElements; maxSize += amount;
  } 

  /** Shrinks by the specified amount (by default, the amount is the max template parameter)..
   * If the amount is greater than the maximum size the it shrinks by the max size (making the array 0 sized).
   * @param  amount Amount to shrink.
   */
	void Shrink(unsigned int amount = max)
  {
    if (amount >= maxSize)
    {
      Clear();
      return;
    }

    T *newElements = new T[maxSize - amount]; 
    memset(newElements, 0, sizeof(T) * (maxSize - amount));
    for (int i = 0; i < maxSize - amount; ++i) newElements[i] = elements[i];
    delete[] elements; elements = newElements; maxSize -= amount;
  } 

  /** Fills the array (its memory) with zeros.
   */
  void Zero() {memset(elements, 0, sizeof(T) * maxSize);}

  /** Clears the array, shrinking it to 0 capacity.
   */
  void Clear() {JDELETE_ARRAY(elements); maxSize = 0;}

  /** Returns the capacity of the array.
   */
	int MaxSize() {return maxSize;}

  /** Returns the element at the given index.
   * @param  i Index of the element.
   * @return Element at that index.
   */
	T& operator[](int i) {return elements[i];}

  /** Destroys the array and frees allocated resources.
   */
  ~JArray() {JDELETE_ARRAY(elements); maxSize = 0;}
};

#endif // _JARRAY_INCLUDED
