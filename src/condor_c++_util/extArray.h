/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
 * CONDOR Copyright Notice
 *
 * See LICENSE.TXT for additional notices and disclaimers.
 *
 * Copyright (c)1990-1998 CONDOR Team, Computer Sciences Department, 
 * University of Wisconsin-Madison, Madison, WI.  All Rights Reserved.  
 * No use of the CONDOR Software Program Source Code is authorized 
 * without the express consent of the CONDOR Team.  For more information 
 * contact: CONDOR Team, Attention: Professor Miron Livny, 
 * 7367 Computer Sciences, 1210 W. Dayton St., Madison, WI 53706-1685, 
 * (608) 262-0856 or miron@cs.wisc.edu.
 *
 * U.S. Government Rights Restrictions: Use, duplication, or disclosure 
 * by the U.S. Government is subject to restrictions as set forth in 
 * subparagraph (c)(1)(ii) of The Rights in Technical Data and Computer 
 * Software clause at DFARS 252.227-7013 or subparagraphs (c)(1) and 
 * (2) of Commercial Computer Software-Restricted Rights at 48 CFR 
 * 52.227-19, as applicable, CONDOR Team, Attention: Professor Miron 
 * Livny, 7367 Computer Sciences, 1210 W. Dayton St., Madison, 
 * WI 53706-1685, (608) 262-0856 or miron@cs.wisc.edu.
****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/
#ifndef __EXT_ARRAY_H__
#define __EXT_ARRAY_H__

#include "condor_debug.h"

template <class Element>
class ExtArray 
{
  public:

	// ctors/dtor
	ExtArray (int = 64); // initial size of array
	ExtArray (const ExtArray &);
	~ExtArray ();

	// accessor functions
	Element operator[] (int) const;
	Element &operator[] (int);

	// control functions
	int	getsize (void) const;
	int	getlast (void) const;
	Element *getarray (void);
	void resize (int);
	void truncate (int);
	void fill (Element);
	void setFiller( Element );

  private:

	Element *array;
	int 	size;
	int		last;
	Element filler;
};


template <class Element>
ExtArray<Element>::
ExtArray (int sz)
{
	// create array of required size
	size = sz;
	last = -1;
	array = new Element[size];
	if (!array)
	{
		dprintf (D_ALWAYS, "ExtArray: Out of memory");
		exit (1);
	}
}

template <class Element>
ExtArray<Element>::
ExtArray (const ExtArray &old)
{
	int i;

	// sanity check
	if (&old == this) return;

	// establish new array of required size;
	size = old.size;
	last = old.last;
	array = new Element[size];
	if (!array) 
	{
		dprintf (D_ALWAYS, "ExtArray: Out of memory");
		exit (1);
	}

	// copy elements over
	for (i = 0; i < size; i++) 
	{
		array[i] = old.array[i];
	}
	
	// copy filler element as well
	filler = old.filler;
}


template <class Element>
ExtArray<Element>::
~ExtArray ()
{
	delete [] array;
}


template <class Element>
inline Element ExtArray<Element>::
operator[] (int i) const
{
	// check bounds
	if (i < 0) 
	{
		i = 0;
	}
	else if (i > last) 
	{
		return filler;
	}

	// index array
	return array[i];
}


template <class Element>
inline Element& ExtArray<Element>::
operator[] (int i)
{
	// check bounds ...
	if (i < 0) 
	{
		i = 0;
	}
	else 
	if (i >= size) 
	{
		resize (2*i);
	}

	// index array
	if( i > last ) last = i;
	return array[i];
}


template <class Element>
inline int ExtArray<Element>::
getsize (void) const
{
	return size;
}


template <class Element>
inline int ExtArray<Element>::
getlast (void) const
{
	return last;
}

template <class Element>
inline Element *ExtArray<Element>::
getarray (void) 
{
	return array;
}


template <class Element>
void ExtArray<Element>::
resize (int newsz) {
	Element *newarray = new Element[newsz];
	int		index = (size < newsz) ? size : newsz;
	int i;

	// check if memory allocation was successful
	if (!newarray) 
	{
		dprintf (D_ALWAYS, "ExtArray: Out of memory");
		exit (1);
	}

	// if the new array is larger, and a 'filler' value is
	// defined, initialize with the filler
	for (i=index;i<newsz;i++) 
	{
		newarray[i] = filler;
	}

	// copy elements over to new array
	while (--index >= 0) 
	{
		newarray[index] = array[index];
	}

	// use new array
	delete [] array;
	size = newsz;	
	array = newarray;
}


template <class Element>
void ExtArray<Element>::
truncate (int newlast)
{
	last = newlast;
}
		

template <class Element>
void ExtArray<Element>::
fill (Element elt)
{
	int i;
	for (i = 0; i < size; i++)
		array[i] = elt;
	filler = elt;
}

template <class Element>
void ExtArray<Element>::
setFiller( Element elt )
{
	filler = elt;
}

#endif//__EXT_ARRAY_H__
