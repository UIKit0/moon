/*
 * list.h: a non-sucky linked list implementation
 *
 * Author: Jeffrey Stedfast <fejj@novell.com>
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 */


#ifndef __LIST_H__
#define __LIST_H__

#include <pthread.h>


class List {
public:
	class Node {
	public:
		Node *next;
		Node *prev;
		
		// public
		Node ();
		virtual ~Node () { }
	};
	
	typedef bool (* NodeAction) (Node *node, void *data);

protected:
	int length;
	Node *head;
	Node *tail;
	
public:
	// constructors
	List ();
	~List ();
	
	// properties
	Node *First ();
	Node *Last ();
	bool IsEmpty ();
	int Length ();
	
	// methods
	void Clear (bool freeNodes);
	
	Node *Append (Node *node);
	Node *Prepend (Node *node);
	Node *Insert (Node *node, int index);
	Node *InsertBefore (Node *node, Node *before);
	
	Node *Replace (Node *node, int index);
	
	Node *Find (NodeAction find, void *data);
	void Remove (NodeAction find, void *data);
	void Remove (Node *node);
	void RemoveAt (int index);
	void Unlink (Node *node);
	
	Node *Index (int index);
	
	int IndexOf (Node *node);
	int IndexOf (NodeAction find, void *data);
	
	void ForEach (NodeAction action, void *data);
};

class Queue {
protected:
	pthread_mutex_t lock;
	List *list;
	
public:
	
	Queue ();
	~Queue ();
	
	// convenience properties
	bool IsEmpty ();
	int Length ();
	
	// convenience methods
	void Clear (bool freeNodes);
	
	void Push (List::Node *node);
	List::Node *Pop ();
	
	void Lock ();
	void Unlock ();
	
	// accessing the internal linked list directly requires manual Locking/Unlocking.
	List *LinkedList ();
};

#endif /* __LIST_H__ */
