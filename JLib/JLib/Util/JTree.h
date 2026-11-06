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

/** Simple recursive tree.
 * <pre>
 * The tree is organized as:
 *
 *(parent of cur and root of the whole tree)
 * |  (cur, iterator here)
 * |   |  
 * v   v  
 * O===O===O <-- (child)
 *     |
 *     O <-- (next)
 *     |
 *     O===O
 *     |   |
 *     O   O
 *     |
 *     O===O===O
 *     ^
 *     |
 *  (branch)
 * </pre>
 * 
 * @file    JTree.h.
 * @author  Juan Carlos Seijo Pérez
 * @date    26/10/2003
 * @version 0.0.1 - 26/10/2003 - First version.
 */

#ifndef _JTREE_INCLUDED
#define _JTREE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>
#include <list>

/** Simple recursive tree. The type must have a default constructor.
 */
template <typename T>
class JTree : public JObject
{
 public:
	/** Tree node.
	 */
	class Node
	{
	public:
		T data;                   /**< Node data */
		Node *prev;               /**< Previous node */
		Node *next;               /**< Next node */
		Node *child;              /**< Child node */
		Node *parent;             /**< Parent node */
		u32 depth;                /**< This node's depth */

		/** Creates a new node. The type T requires a default constructor.
		 */
		Node() : prev(0), next(0), child(0), parent(0)
		{
			if (parent)
				depth = parent->depth + 1;
			else
				depth = 0;
		}

		/** Creates a new node.
		 * @param  _data Node data.
		 * @param  _prev Previous node.
		 * @param  _next Next node.
		 * @param  _parent Parent node.
		 */
		Node(T _data, Node *_prev = 0, Node *_next = 0, Node *_parent = 0, Node *_gPrev = 0, Node *_gNext = 0)
		: data(_data), prev(_prev), next(_next), child(0), parent(_parent)
		{
			if (parent)
				depth = parent->depth + 1;
			else
				depth = 0;
		}
	};
	
	Node* root;                           /**< Root node. Its the parent of the first node */
	Node* end;                            /**< Is the next of the last of each branch */
	std::list<Node *> nodes;              /**< All the nodes as a list. */
	typename std::list<Node *>::iterator itG; /**< Global node iterator. */
		
	/** Creates an empty tree.
	 */
	JTree<T>()
	{
		root = new Node;
		root->parent = root;
		nodes.push_back(root);
	}

	/** Destroys the tree. Deletes created nodes (BUT NOT its contents if they are pointers)
	 */
	~JTree()
	{
		nodes.clear();
	}

  /** Tree iterator.
   */
  class Iterator
  {
  public:
		JTree *tree;                                /**< Tree this iterator belongs to. */
		Node *node;                                 /**< Actual node. */
    
    /** Creates an iterator from a tree.
     */
    Iterator(JTree &t)
    {
			tree = &t;
			node = *(tree->nodes.begin());
			if (node->child)
			{
				// Skips root to be at the first element
				node = node->child;
			}
    }

    /** Creates an iterator froam a tree and a node belonging to it.
     * @param  t Tree.
     * @param  n Node of the tree.
     */
    Iterator(JTree &t, Node *n)
    {
			tree = &t;
			node = n ? n : *(tree->nodes.begin());
    }

    /** Assigns the data of the current node.
     * @param data New data.
     */
    void Data(T data)
    {
      node->data = data;
    }

    /** Returns the data of the current node.
     * @return Current node data.
     */
    T& Data()
    {
      return node->data;
    }

    /** Goes to the next node.
     * @return <b>true</b> if it exists, <b>false</b> if not.
     */
    bool Next()
    {
      if (node->next)
      {
        node = node->next;
        return true;
      }

      return false;
    }
    
    /** Goes to the previous node.
     * @return <b>true</b> if it exists, <b>false</b> if not.
     */
    bool Prev()
    {
      if (node->prev)
      {
        node = node->prev;
        return true;
      }

      return false;
    }
    
    /** Goes to the parent node.
     * @return <b>true</b> if it exists, <b>false</b> if not.
     */
    bool Parent()
    {
      if (node->parent != tree->root)
      {
        node = node->parent;
        return true;
      }

      return false;
    }
    
    /** Goes to the child node.
     * @return <b>true</b> if it exists, <b>false</b> if not.
     */
    bool Child()
    {
      if (node->child)
      {
        node = node->child;
        return true;
      }

      return false;
    }

    /** Adds a node.
     * @param  nodeData Data for the new node.
     * @param  after <b>true</b> to put it after the current node,
     * <b>false</b> to put it before.
     */
    void AddNode(T nodeData, bool after = true)
    {
      if (after)
			{
        node->next = new Node(nodeData, node, node->next, node->parent);
				tree->nodes.push_back(node->next);
			}
      else
			{
        node->prev = new Node(nodeData, node->prev, node, node->parent);
				tree->nodes.push_back(node->prev);
			}
    }

    /** Adds a node and places the iterator there.
     * @param  nodeData Node data.
     * @param  after <b>true</b> to put it after the current node,
     * <b>false</b> to put it before.
     */
    void AddNodeGo(T nodeData, bool after = true)
    {
      if (after)
      {
        node->next = new Node(nodeData, node, node->next, node->parent);
        node = node->next;
      }
      else
      {
        node->prev = new Node(nodeData, node->prev, node, node->parent);
        node = node->prev;
      }

			tree->nodes.push_back(node);
    }
    
    /** Adds a child node if it didn't exist yet.
     * @param  nodeData Data of the child node.
     */
    void AddBranch(T nodeData)
    {
      if (!node->child)
			{
        node->child = new Node(nodeData, 0, 0, node);
				tree->nodes.push_back(node->child);
			}
    }

    /** Adds a child node if it didn't exist yet and places the iterator there.
     * @param  nodeData Data of the child node.
     */
    void AddBranchGo(T nodeData)
    {
      if (!node->child)
      {
        node->child = new Node(nodeData, 0, 0, node);
        node = node->child;
				tree->nodes.push_back(node);
      }
    }

    /** Removes a node and its childs. The iterator after this operation is placed
     * in th eprevious node or in th eparent node if it doesn't exist.
     */
    void RemoveNode()
    {
			if (node != tree->root)
			{
				// Places the iterator in the previous node or int the parent node
				Node *n = (node->prev) ? node->prev : node->parent;
				
				if (node->next)
					node->next->prev = node->prev;
				
				if (node->prev)
					node->prev->next = node->next;
				
				// Tell the father this node was unique
				if (!node->next && !node->prev && node->parent)
					node->parent->child = 0;
				
				tree->nodes.remove(node);
				JDELETE(node);
			
				node = n;
			}
    }
    
    /** Goes to the first node in branch.
     */
    void FirstInBranch()
    {
      while (Prev());
    }

    /** Goes to the last node in branch.
     * @see Root()
     */
    void LastInBranch()
    {
      while (Next());
    }

    /** Goes to the first node in the tree.
     */
    void FirstInTree()
    {
      while (Parent())
			{}

      FirstInBranch();
    }

    /** Goes to the first node in the tree.
     * @see FirstInTree()
     */
    void Root()
    {
      FirstInTree();
    }

    /** Checks if the current node has childs.
		 * @return <b>true</b> if so, <b>false</b> if not.
     */
    bool HasChilds()
    {
      return (0 != node->child);
    }

    /** Checks if the current node has previous.
		 * @return <b>true</b> if so, <b>false</b> if not.
     */
    bool HasPrev()
    {
      return (0 != node->prev);
    }

    /** Checks if the current node has next.
		 * @return <b>true</b> if so, <b>false</b> if not.
     */
    bool HasNext()
    {
      return (0 != node->next);
    }
  };

	/** Returns a new iterator. The caller must delete (with operator delete) the object after using it.
	 * @return Iterator at the root node.
	 */
	Iterator * NewIterator() {return new Iterator(*this);}
	
	/** Returns a new iterator in the given node. 
	 * The caller must delete (with operator delete) the object after using it.
   * @param  n Node at which the iterator must be placed.
	 * @return Iterator at the given node.
	 */
	Iterator * NewIterator(Node *n) {return new Iterator(*this, n);}
	
	/** Goes to the begin of the global node list.
	 */
	void Begin() {itG = nodes.begin();}
	
	/** Check if it's at the end of the global node list.
   * @return <b>true</b> if so, <b>false</b> if not.
	 */
	bool End() {return itG == nodes.end();}
	
	/** Returns the object at the current position in the global list of nodes.
   * @return Object at the current position in the global list of nodes.
	 */
	T& Cur() {return (*itG)->data;}

	/** Goes to the previous node in the global list of nodes.
	 */
	void Prev() {--itG;}
	
	/** Goes to the next node in the global list of nodes.
	 */
	void Next() {++itG;}

	/** Returns the number of elements in the tree.
	 * @return Total number of elements in the tree.
	 */
	s32 Size() {return nodes.size();}

	/** Clear the whole tree (BUT not its contents if they were pointers).
	 */
	void Clear() {nodes.clear();}
	
  friend class Iterator;
};

#endif  // _JTREE_INCLUDED
