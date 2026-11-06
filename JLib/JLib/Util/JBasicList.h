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

/** Basic linked list class.
 * @file    JBasicList.h
 * @author  Juan Carlos Seijo Pérez
 * @date    03/11/2005
 * @version 0.0.1 - 03/11/2005 - First version.
 */

#ifndef _JBASICLIST_INCLUDED
#define _JBASICLIST_INCLUDED

#include <assert.h>

#include <JLib/Util/JTypes.h>

/** This template provides a doubly-linked list with basic memory management.
 */
template<class T>
class JBasicList
{
public:
  class Iterator;

  /** List node.
   */
  class Node
  {
    friend class JBasicList;
    friend class Iterator;
    T data;                             /**< Node data */
    Node *prev;                         /**< Previous node */
    Node *next;                         /**< Next node */

  public:
    Node() {}
    Node(const T& n) {data = n.data; prev = n.prev; next = n.next;}
    Node(const T &d, Node *p, Node *n) : data(d), prev(p), next(n) {n->prev = p->next = this;}
    inline Link(const T &d, Node *p, Node *n) {data = d; prev = p; next = n; n->prev = p->next = this;}
    inline Unlink() {prev->next = next; next->prev = prev;}
    Node & operator=(const Node &n) {data = n.data; prev = n.prev; next = n.next; return *this;}
  };

  /** List iterator.
   */
  class Iterator
  {
    friend class JBasicList;
    Node *cur;                          /**< Node this iterator points to */
    
  public:
    
    /** Creates an empty iterator (pointing to nowhere).
     */
    Iterator() : cur(0) {}

    /** Creates an iterator pointing to the given node.
     * @param  node Node this iterator must point to.
     */
    Iterator(Node *node) : cur(node) {}

    /** Assings another iterator to this iterator.
     * @param  other Other iterator.
     * @return This iterator.
     */
    Iterator & operator=(const Iterator &other) {cur = other.cur; return *this;}

    /** Test equality between iterators.
     * @param  other Other iterator.
     * @return <b>true</b> if they are equal, <b>false</b> otherwise.
     */
    bool operator==(const Iterator &other) {return cur == other.cur;}

    /** Test inequality between iterators.
     * @param  other Other iterator.
     * @return <b>true</b> if they are different, <b>false</b> otherwise.
     */
    bool operator!=(const Iterator &other) {return cur != other.cur;}

    /** Goes to the next element.
     */
    inline void Next() {if (cur->next == 0) return; cur = cur->next;}

    /** Goes to the next element.
     */
    void operator++() {if (cur->next == 0) return; cur = cur->next;}

    /** Goes to the previous element.
     */
    inline void Prev() {if (cur->prev == 0) return; cur = cur->prev;}

    /** Goes to the previous element.
     */
    void operator--() {if (cur->prev == 0) return; cur = cur->prev;}

    /** Returns the data of the current element.
     * @return Data of the current element.
     */
    inline T& Data() {return cur->data;}

    /** Returns the data of the current element.
     * @return Data of the current element.
     */
    inline T& operator*() {return cur->data;}
  };

protected:
  Node first;                           /**< First element */
  Node last;                            /**< Last element */
  int size;                             /**< Current size of the list */
  Iterator begin;                       /**< Iterator to first element */
  Iterator end;                         /**< Iterator to last element */

public:
  /** Creates an empty list.
   */
  JBasicList() : size(0)
  {
    first.next = &last;
    last.prev = &first;
    end.cur = &last;
  }

  /** Inserts the given data at the front of the list.
   * @param  data Data to be inserted.
   */
  inline void PushFront(const T& data)
  {
    new Node(data, &first, first.next);
    ++size;
  }

  /** Inserts the given data at the back of the list.
   * @param  data Data to be inserted.
   */
  inline void PushBack(const T& data)
  {
    new Node(data, last.prev, &last);
    ++size;
  }
  
  /** Inserts the given data before the element at the given iterator.
   * @param  data Data to be inserted.
   * @param  it Iterator to the element before which insert.
   */
  inline void Insert(const T& data, Iterator &it)
  {
    new Node(data, it.cur->prev, it.cur);
    ++size;
  }
  
  /** Removes the element at the back of the list.
   */
  inline void PopBack()
  {
    if (last.prev != &first)
    {
      Node *tmp = last.prev;
      last.prev->Unlink();
      delete tmp;
      --size;
    }
  }

  /** Removes the element at the front of the list.
   */
  inline void PopFront()
  {
    if (first.next != &last)
    {
      Node *tmp = first.next;
      first.next->Unlink();
      delete tmp;
      --size;
    }
  }

  /** Removes the element at the given position and places the iterator in the next element.
   * WARNING: this method does not check if the iterator is valid, one must must check it outside if
   * needed.
   * @param  it Iterator at the element to be removed.
   */
  inline void Remove(Iterator &it)
  {
    assert(it.cur != &last && it.cur != &first);
    Node *tmp = it.cur;
    it.cur = it.cur->next;
    tmp->Unlink();
    delete tmp;
    --size;
  }

  /** Returns an iterator at the first element of the list.
   * If the list is empty, this iterator is the same (in the sense of operator=) to that of End().
   * @return Iterator at the first element or at one past the last element if the list is empty.
   */
  inline const Iterator & Begin() {begin.cur = first.next; return begin;}

  /** Returns the data of the first element in the list.
   * @return data of the first element in the list.
   */
  inline const T& Front() {return first.cur->next->data;}

  /** Returns an iterator at one past the last element of the list.
   * @return Iterator at one past the last element.
   */
  inline const Iterator & End() {return end;}

  /** Returns the data of the last element in the list.
   * @return data of the first element in the list.
   */
  inline const T& Back() {return end.cur->prev->data;}

  /** Returns the number of elements in the list.
   * @return Number of elements in the list.
   */
  inline int Size() {return size;}

  /** Destroys the list, frees used resources.
   */
  ~JBasicList()
  {
    Node *n = first.next, *tmp;

    while (n != &last)
    {
      tmp = n->next;
      delete n;
      n = tmp;
    }
  }
};

#endif // _JBASICLIST_INCLUDED
