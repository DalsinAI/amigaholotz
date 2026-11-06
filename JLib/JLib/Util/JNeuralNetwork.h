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
 * @file    JNeuralNetwork.h
 * @author  Juan Carlos Seijo Pérez
 * @date    23/10/2005
 * @version 0.0.1 - 23/10/2005 - First version.
 */

#ifndef _JNEURALNETWORK_INCLUDED
#define _JNEURALNETWORK_INCLUDED

#include <assert.h>

#include <JLib/Util/JTypes.h>
#include <vector>
#include <list>

/** This template provides a neural network.
 */
template<class T>
class JNeuralNetwork
{
public:
  class Iterator;

  /** Network node.
   */
  class Node
  {
    friend class JNeuralNetwork;
    friend class Iterator;
    T data;                             /**< Node data */
    std::vector<Node *> links;          /**< Vector of links. */

    Node(const Node& n) {}

  public:
    Node() {}
    Node(const T &d) : data(d) {}
    void Link(Node *n) {links.push_back(n);}
    void DLink(Node *n) {links.push_back(n); n->links.push_back(this);}
    bool Linked(Node *n) {for (s32 i = 0; i < links.size(); ++i) if (links[i] == n) return true; return false;}
    bool DLinked(Node *n) {return Linked(n) && n->Linked(this);}
    void Unlink(Node *n) {for (s32 i = 0; i < links.size(); ++i) if (links[i] == n) {links.erase(links.begin() + i); return;}}
    void DUnlink(Node *n) {Unlink(n); n->Unlink(this);}
    void Isolate() {links.clear();}
    s32 NumLinks() {return links.size();}
    Node * NodeAt(s32 index) {return links[index];}
    T& Data() {return data;}
    Node & operator=(const Node &n) {data = n.data; links.clear(); links.insert(n.links); return *this;}
    ~Node() {links.clear();}
  };

  /** List iterator.
   */
  class Iterator
  {
    friend class JNeuralNetwork;
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

    /** Goes to the element linked by the given path.
     */
    void Move(s32 pathNum) {if (pathNum >= cur->NumLinks()) return; cur = cur->links[pathNum];}
    
    /** Returns the number of links in the current node.
     */
    s32 NumLinks() {return cur->links.size();}

    void Unlink(s32 i) {if (cur->links.size() > i) erase(cur->links.begin() + i);}
    void DUnlink(s32 i) {if (cur->links.size() > i) {erase(cur->links.begin() + i); cur->links[i]->Unlink(cur);}}

    void Link(Node *n) {cur->links.push_back(n);}
    void DLink(Node *n) {cur->links.push_back(n); n.links.push_back(cur);}
    bool Linked(Node *n) {for (s32 i = 0; i < cur->links.size(); ++i) if (cur->links[i] == n) return true; return false;}
    bool DLinked(Node *n) {return cur->Linked(n) && n->Linked(cur);}
    void Unlink(Node *n) {for (s32 i = 0; i < cur->links.size(); ++i) if (cur->links[i] == n) {erase(cur->links.begin() + i); return;}}
    void DUnlink(Node *n) {cur->Unlink(n); n->Unlink(cur);}
    void Isolate() {for (s32 i = 0; i < cur->links.size(); ++i) links[i]->Unlink(this); cur->links.clear();}

    /** Gives the node at the given link number.
     */
    Node * NodeAt(s32 index) {return cur->links[index];}
    
    /** Returns the current node.
     * @return Current node.
     */
    const Node* GetNode() {return cur;}

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
  std::vector<Node *> nodes;            /**< List of network nodes. */
  Iterator begin;                       /**< Iterator to the first element. */
  Iterator end;                         /**< Iterator to one past the last element. */
  
public:
  /** Creates an empty network.
   */
  JNeuralNetwork()
  {}

  /** Creates and adds a node to the network.
   * @param  data Data of the node to be added.
   * @return Created node.
   */
  Node * New(const T& data)
  {
    Node *n = new Node(data);
    nodes.push_back(n);
    return n;
  }

  /** Adds a node to the network.
   * @param  data Data of the node to be added.
   */
  void Add(Node *n)
  {
    nodes.push_back(n);
  }

  /** Removes the given element.
   */
  void Del(Node *n)
  {
    if (n)
    {
      n->Isolate();
      for (s32 i = 0; i < nodes.size(); ++i)
      {
        if (nodes[i] == n)
        {
          delete nodes[i];
          nodes.erase(nodes.begin() + i);
          return;
        }
      }
    }
  }

  /** Returns the first node with the given data.
   * @return First node containing the given data or null if it isn't found.
   */
  Node * Get(const T &data)
  {
    for (s32 i = 0; i < nodes.size(); ++i)
    {
      if (*nodes[i] == data)
      {
        return nodes[i];
      }
    }
    
    return 0;
  }

  /** Returns the first node with the given data.
   * @return First node containing the given data or null if it isn't found.
   */
  Node * Find(const T &data)
  {
    for (s32 i = 0; i < nodes.size(); ++i)
    {
      if (*nodes[i] == data)
      {
        return nodes[i];
      }
    }
    
    return 0;
  }

  /** Returns a logical iterator to the first element of the network.
   * If the list is empty, this iterator is the same (in the sense of operator=) to that of End().
   * @return Iterator at the first element or at one past the last element if the list is empty.
   */
  const Iterator & Begin() {begin.cur = nodes.begin(); return begin;}

  /** Returns an iterator to one past the last element of the list.
   * @return Iterator at one past the last element.
   */
  const Iterator & End() {end.cur = nodes.end(); return end;}

/*   /\** Returns an iterator to the first element of the internal list. */
/*    * @return Iterator at the first element. */
/*    *\/ */
/*   const std::vector<Node *>::iterator & GBegin() {return nodes.begin();} */

/*   /\** Returns an iterator to one past the last element of the list. */
/*    * @return Iterator at one past the last element. */
/*    *\/ */
/*   const std::vector<Node *>::iterator & GEnd() {return nodes.end();} */

  /** Returns the list of nodes.
   * @return List of nodes.
   */
  std::vector<Node *>& Nodes() {return nodes;}

  /** Returns the number of nodes in the network.
   * @return Number of nodes in the network.
   */
  int Size() {return nodes.size();}

  /** Destroys the list, frees used resources.
   */
  ~JNeuralNetwork()
  {
    for (s32 i = 0; i < nodes.size(); ++i)
    {
      delete nodes[i];
    }
  }
};

#endif // _JNEURALNETWORK_INCLUDED
