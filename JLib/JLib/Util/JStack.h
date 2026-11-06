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

/** Stack template
 * @file    JStack.h
 * @author  Juan Carlos Seijo Pérez
 * @date    23/10/2005
 * @version 0.0.1 - 23/10/2005 - First version.
 */

#ifndef _JSTACK_INCLUDED
#define _JSTACK_INCLUDED

#include <JLib/Util/JObject.h>
#include <JLib/Util/JTypes.h>
#include <assert.h>

/** This class encapsulates a stack.
 */
template<class T, unsigned int max = 64>
class JStack
{
protected:
	int maxSize;                          /**< Current maximum size of the stack */
	int size;                             /**< Number of elements in the stack */
	int grow;                             /**< Grow amount of the stack */
	T *elements;                          /**< Elements in the stack */

public:
  
  /** Creates an empty stack.
   * @param  sz Initial capacity of the stack. If needed, the stack will grow its capacity
   * to allow more elements to be added.
   */
	JStack(int sz = max) : maxSize(sz), size(0), grow(sz) {elements = new T[maxSize];}

  /** Places an element at the top of the stack.
   * @param  element Element to push.
   */
	void Push(const T& element)
  {
    if (size == maxSize)
    {
      T *newElements = new T[maxSize*2]; 
      for (int i = 0; i < size; ++i) newElements[i] = elements[i];
      delete[] elements; elements = newElements; maxSize *= 2;
    } 
    elements[size] = element; ++size;
  }

  /** Removes the element at the top of the stack. If Size() is MaxSize()/2, shrinks the stack memory.
   */
  void Pop() 
  {
    if (size > 0)
    {
      --size;

      if (size <= (maxSize/2) && (maxSize > (grow*2)))
      {
        T *newElements = new T[(maxSize/2) + grow]; 
        for (int i = 0; i < size; ++i) newElements[i] = elements[i];
        delete[] elements; elements = newElements; maxSize = (maxSize/2) + grow;
      }
    }
  }

  /** Returns the number of elements in the stack.
   * @return Number of elements in the stack.
   */
	int Size() {return size;}

  /** Checks if this stack is empty.
   * @return <b>true</b> if it's empty, <b>false</b> otherwise.
   */
	bool Empty() {return size <= 0;}
  
  /** Returns the current capacity of this stack (total number of elements that can be
   * pushed before growing the stack is necessary).
   * @return Current capacity of the stack.
   */
	int MaxSize() {return maxSize;}

  /** Returns the element at the top of the stack.
   * @return Element at the top of the stack.
   */
  T& Top() {assert(size > 0 || printf("JStack: size = 0 and Top() invoked!\n")); return elements[size - 1];}

  /** Returns the element at the given position of the stack. WARNING: this method does not check bound so one
   * must ensure the index is inside the range of the stack.
   * @param  i Index to retrieve.
   * @return Element at the given index.
   */
	T& operator[](int i) {assert(i < size || printf("JStack: operator[%d], size = %d!\n", i, size)); return elements[i];}

  /** Destroys the stack and frees allocated resources.
   */
  ~JStack() {JDELETE_ARRAY(elements); size = 0;}
};

#endif // _JSTACK_INCLUDED
