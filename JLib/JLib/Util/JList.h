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

/** Linked list class
 * @file    JList.h
 * @author  Juan Carlos Seijo Pérez
 * @date    23/10/2005
 * @version 0.0.1 - 23/10/2005 - First version.
 */

#ifndef _JLIST_INCLUDED
#define _JLIST_INCLUDED

#include <assert.h>

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JStack.h>

/** This template provides a doubly-linked list.
 */
template<class T, unsigned int growBy=128>
class JList
{
public:
  class Iterator;

  /** List node.
   */
  class Node
  {
    friend class JList;
    friend class Iterator;
    T data;                             /**< Node data */
    Node *prev;                         /**< Previous node */
    Node *next;                         /**< Next node */

  public:
    Node() {}
    Node(const T& n) {data = n.data; prev = n.prev; next = n.next;}
    Node(const T &d, Node *p, Node *n) : data(d), prev(0), next(0) {}
    Link(const T &d, Node *p, Node *n) {data = d; prev = p; next = n; n->prev = this; p->next = this;}
    Unlink() {prev->next = next; next->prev = prev;}
    Node & operator=(const Node &n) {data = n.data; prev = n.prev; next = n.next; return *this;}
  };

  /** List iterator.
   */
  class Iterator
  {
    friend class JList;
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
    void Next() {if (cur->next == 0) return; cur = cur->next;}

    /** Goes to the next element.
     */
    void operator++() {if (cur->next == 0) return; cur = cur->next;}

    /** Goes to the previous element.
     */
    void Prev() {if (cur->prev == 0) return; cur = cur->prev;}

    /** Goes to the previous element.
     */
    void operator--() {if (cur->prev == 0) return; cur = cur->prev;}

    /** Returns the data of the current element.
     * @return Data of the current element.
     */
    T& Data() {return cur->data;}

    /** Returns the data of the current element.
     * @return Data of the current element.
     */
    T& operator*() {return cur->data;}
  };

protected:
  Node first;                           /**< First element */
  Node last;                            /**< Last element */
  int grow;                             /**< Growing amount */
  int size;                             /**< Current size of the list */
  int nextFree;                         /**< Next free node */
  JStack<Node *> mem;                   /**< Memory allocator */
  Iterator begin;                       /**< Iterator to first element */
  Iterator end;                         /**< Iterator to last element */

public:
  /** Creates an empty list.
   * @param  growBy Grow amount, i.e. the initial capacity of the list before grow is necessary and the
   * memory steps of the grow phase. Note that the underlying memory allocator is a JStack so the growing
   * takes place first as the growBy, then at 2*growBy, then at 4*growBy, and so on. The value affects
   * considerably the insertion of new elements, as a rule of thumb select a value that is close to your
   * needs (if known, of course). If not known, the default (128) is conservative and gives good speed results.
   */
  JList(unsigned int _growBy = growBy) : grow(_growBy), size(0), nextFree(0), mem(1)
  {
    mem.Push(new Node[grow]);
    first.next = &last;
    last.prev = &first;
    end.cur = &last;
  }

  /** Inserts the given data at the front of the list.
   * @param  data Data to be inserted.
   */
  void PushFront(const T& data)
  {
    if (nextFree >= grow)
    {
      nextFree = 0;
      mem.Push(new Node[grow]);
      //printf("mem has now %d max items\n", mem.MaxSize());
    }
    
    mem.Top()[nextFree].Link(data, &first, first.next);
    ++nextFree;
    ++size;
  }

  /** Inserts the given data at the back of the list.
   * @param  data Data to be inserted.
   */
  void PushBack(const T& data)
  {
    if (nextFree >= grow)
    {
      nextFree = 0;
      mem.Push(new Node[grow]);
      //printf("mem has now %d max items\n", mem.MaxSize());
    }

    mem.Top()[nextFree].Link(data, last.prev, &last);
    ++nextFree;
    ++size;
  }
  
  /** Inserts the given data before the element at the given iterator.
   * @param  data Data to be inserted.
   * @param  it Iterator to the element before which insert.
   */
  void Insert(const T& data, Iterator &it)
  {
    if (nextFree >= grow)
    {
      nextFree = 0;
      mem.Push(new Node[grow]);
      //printf("mem has now %d max items\n", mem.MaxSize());
    }

    mem.Top()[nextFree].Link(data, it.cur->prev, it.cur);
    ++nextFree;
    ++size;
  }
  
  /** Removes the element at the back of the list. WARNING: This method doesn't check for
   * existing elements in the list, one must check it if necessary from outside.
   */
  void PopBack()
  {
    assert(nextFree != 0 || printf("JList PopBack with no elements!\n"));
    
    // DBG ///////////
//     printf("  Están así    : FIRST-");
//     Node *n = first.next;
//     int jj = 0;
//     while (n != &last || jj++ > 5)
//     {
//       if (n->data)
//       {
//         printf("%s-", n->data);
//       }
//       n = n->next;
//     }
//     printf("LAST\n");
    // DBG ///////////

    if (last.prev != &(mem.Top()[nextFree - 1]))
    {
      // Not at the top of the stack, replaces that element with this one
      Node n(*last.prev);
      *last.prev = mem.Top()[nextFree - 1];
      n.Unlink();
    }
    else
    {
      last.prev->Unlink();
    }

    // DBG ///////////
//     printf("  Tras Unlink(): FIRST-");
//     n = first.next;
//     jj = 0;
//     while (n != &last || jj++ > 5)
//     {
//       if (n->data)
//       {
//         printf("%s-", n->data);
//       }
//       n = n->next;
//     }
//     printf("LAST\n\n");
    // DBG ///////////

    if (--nextFree == 0)
    {
      nextFree = grow;
      mem.Pop();
    }
    
    --size;
  }

  /** Removes the element at the front of the list. WARNING: This method doesn't check for
   * existing elements in the list, one must check it if necessary from outside.
   */
  void PopFront()
  {
    assert(nextFree != 0 || printf("JList PopBack with no elements!\n"));
    
    if (first.next != &(mem.Top()[nextFree - 1]))
    {
      // Not at the top of the stack, replaces that element with this one
      Node n(*first.next);
      *first.next = mem.Top()[nextFree - 1];
      n.Unlink();
    }
    else
    {
      first.next->Unlink();
    }

    if (--nextFree == 0)
    {
      nextFree = grow;
      mem.Pop();
    }
    
    --size;
  }

  /** Removes the element at the given position and places the iterator in the next element.
   * WARNING: this method does not check if the iterator is valid, one must must check it outside if
   * needed.
   * @param  it Iterator at the element to be removed.
   */
  void Remove(Iterator &it)
  {
    assert(nextFree != 0 || printf("JList PopBack with no elements!\n"));
    
    if (it.cur != &(mem.Top()[nextFree - 1]))
    {
      // Not at the top of the stack, replaces that element with this one
      Node n(*it.cur);
      *it.cur = mem.Top()[nextFree - 1];
      n.Unlink();
      it.cur = n.next;
    }
    else
    {
      it.cur->Unlink();
      it.cur = it.cur->next;
    }

    if (--nextFree == 0)
    {
      nextFree = grow;
      mem.Pop();
    }
    
    --size;
  }

  /** Returns an iterator at the first element of the list.
   * If the list is empty, this iterator is the same (in the sense of operator=) to that of End().
   * @return Iterator at the first element or at one past the last element if the list is empty.
   */
  const Iterator & Begin() {begin.cur = first.next; return begin;}

  /** Returns an iterator at one past the last element of the list.
   * @return Iterator at one past the last element.
   */
  const Iterator & End() {return end;}

  /** Returns the number of elements in the list.
   * @return Number of elements in the list.
   */
  int Size() {return size;}

  /** Returns the maximum number of elements that fit in the list.
   * @return Maximum number of elements that fit in the list before growing is necessary.
   */
  int MaxSize() {return mem.MaxSize();}

  /** Destroys the list, frees used resources.
   */
  ~JList()
  {
    while (!mem.Empty())
    {
      delete[] mem.Top();
      mem.Pop();
    }
  }
};

#endif // _JLIST_INCLUDED
