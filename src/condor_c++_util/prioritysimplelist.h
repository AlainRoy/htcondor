/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2006, Condor Team, Computer Sciences Department,
  * University of Wisconsin-Madison, WI.
  *
  * This source code is covered by the Condor Public License, which can
  * be found in the accompanying LICENSE.TXT file, or online at
  * www.condorproject.org.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * AND THE UNIVERSITY OF WISCONSIN-MADISON "AS IS" AND ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY, OF SATISFACTORY QUALITY, AND FITNESS
  * FOR A PARTICULAR PURPOSE OR USE ARE DISCLAIMED. THE COPYRIGHT
  * HOLDERS AND CONTRIBUTORS AND THE UNIVERSITY OF WISCONSIN-MADISON
  * MAKE NO MAKE NO REPRESENTATION THAT THE SOFTWARE, MODIFICATIONS,
  * ENHANCEMENTS OR DERIVATIVE WORKS THEREOF, WILL NOT INFRINGE ANY
  * PATENT, COPYRIGHT, TRADEMARK, TRADE SECRET OR OTHER PROPRIETARY
  * RIGHT.
  *
  ****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/


#ifndef _PRIO_SMPL_LIST_H_
#define _PRIO_SMPL_LIST_H_

#include "simplelist.h"

template <class ObjType>
class PrioritySimpleList : public SimpleList<ObjType>
{
  public:
    // ctor, dtor
    PrioritySimpleList ();

    // Copy Constructor
    PrioritySimpleList (const PrioritySimpleList<ObjType> & list);

    virtual inline	~PrioritySimpleList () { delete [] priorities; }

    // General
    virtual bool	Append( const ObjType &item ) { return Append( item, 0 ); }
	virtual bool	Prepend( const ObjType &item ) { return Prepend( item, 0 ); }
    virtual bool	Append( const ObjType &item, int prio );
	virtual bool	Prepend( const ObjType &item, int prio );

    // Scans
    virtual void    DeleteCurrent();
	virtual bool	Delete(const ObjType &, bool delete_all = false);

  protected:
	virtual bool	resize (int);

	// Find the last item with a priority higher (numerically lower) than
	// the given priority.  Note:  can return -1.
	int				FindLastBefore(int priority);

	// Find the first item with a priority lower (numerically higher) than
	// the given priority.  Note:  can return size.
	int FindFirstAfter(int priority);

	bool			InsertAt( int index, const ObjType &item, int prio );

	int				*priorities;
};

template <class ObjType>
PrioritySimpleList<ObjType>::
PrioritySimpleList() : SimpleList<ObjType>()
{
	priorities = new int[maximum_size];
}

template <class ObjType>
PrioritySimpleList<ObjType>::
PrioritySimpleList (const PrioritySimpleList<ObjType> & list) :
	SimpleList<ObjType>(list)
{
	priorities = new int[maximum_size];
    memcpy( priorities, list.priorities, sizeof( int ) * size );
}

template <class ObjType>
bool PrioritySimpleList<ObjType>::
Append( const ObjType &item, int prio )
{
	int		index = FindFirstAfter( prio );

	return InsertAt( index, item, prio );
}

template <class ObjType>
bool PrioritySimpleList<ObjType>::
Prepend( const ObjType &item, int prio )
{
	int		index = FindLastBefore( prio );

	return InsertAt( index + 1, item, prio );
}

template <class ObjType>
void PrioritySimpleList<ObjType>::
DeleteCurrent()
{
    if ( current >= size || current < 0 ) {
		return;
	}

    for ( int i = current; i < size - 1; i++ ) {
		priorities [i] = priorities [i+1];
	}

	SimpleList<ObjType>::DeleteCurrent();
}

template <class ObjType>
bool PrioritySimpleList<ObjType>::
Delete( const ObjType &item, bool delete_all )
{
	bool found_it = false;

	for ( int i = 0; i < size; i++ ) {
		if ( items[i] == item ) {
			found_it = true;
			for ( int j = i; j < size - 1; j++ ) {
				items[j] = items[j+1];
				priorities[j] = priorities[j+1];
			}
			size--;
			if ( current >= i ) {
				current--;
			}
			if ( delete_all == false ) {
				return true;
			}
			i--;
		}
	}

	return found_it;
}

template <class ObjType>
bool PrioritySimpleList<ObjType>::
resize( int newsize )
{
	int		*buf = new int[newsize];
	if ( !buf ) return false;

	int smaller = (newsize < size) ? newsize : size;
	for ( int i = 0; i < smaller; i++ ) {
		buf[i] = priorities[i];
	}
		
	delete [] priorities;
	priorities = buf;

	return SimpleList<ObjType>::resize(newsize);
}

template <class ObjType>
int PrioritySimpleList<ObjType>::
FindLastBefore( int priority )
{
	int		index = size - 1;

	//TEMP -- switch to binary search!!!
	for ( int i = 0; i < size; i++ ) {
		if ( priorities[i] >= priority ) {
			index = i - 1;
			break;
		}
	}

	return index;
}

template <class ObjType>
int PrioritySimpleList<ObjType>::
FindFirstAfter( int priority )
{
	int		index = 0;

	//TEMP -- switch to binary search!!!
	for ( int i = size - 1; i >= 0; i-- ) {
		if ( priorities[i] <= priority ) {
			index = i + 1;
			break;
		}
	}

	return index;
}

template <class ObjType>
bool PrioritySimpleList<ObjType>::
InsertAt( int index, const ObjType &item, int prio )
{
	// Enlarge if necessary.
    if (size >= maximum_size) {
		if ( !resize( 2 * maximum_size ) ) {
			return false;
		}
	}

	// Move following stuff back one slot.
	for ( int i = size; i > index; i-- ) {
		items[i] = items[i-1];
		priorities[i] = priorities[i-1];
	}

	// Insert the new item.
	items[index] = item;
	priorities[index] = prio;
	size++;

	return true;
}

#endif // _PRIO_SMPL_LIST_H_
