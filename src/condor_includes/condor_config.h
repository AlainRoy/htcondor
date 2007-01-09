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
#ifndef CONFIG_H
#define CONFIG_H

#if defined(__cplusplus)

#include "condor_classad.h"
#include "MyString.h"
#endif

#define CONDOR_GCB_INVALID_BROKER "0.0.0.1"

typedef struct bucket {
	char	*name;
	char	*value;
	struct bucket	*next;
} BUCKET;


/*
**  Types of macro expansion
*/
#define EXPAND_LAZY         1
#define EXPAND_IMMEDIATE    2
#define TABLESIZE 113

#if defined(__cplusplus)
	extern MyString global_config_source;
	extern MyString global_root_config_source;
	extern StringList local_config_sources;

    int param_integer( const char *name, int default_value,
					   int min_value = INT_MIN, int max_value = INT_MAX );
    float param_float( const char *name, float default_value,
					   float min_value = FLT_MIN, float max_value = FLT_MAX );
	bool param_boolean( const char *name, const bool default_value );
	bool param_get_location(const char *parameter, MyString &filename,
							int &line_number);

/* here we provide C linkage to C++ defined functions. This seems a bit
	odd since if a .c file includes this, these prototypes technically don't
	exist.... */
extern "C" {
	void config( int wantsQuiet=0 );
	void config_host( char* host=NULL );
	void config_fill_ad( ClassAd*, const char* prefix=NULL );
	void condor_net_remap_config( bool force_param=false );
    int  param_boolean_int( const char *name, int default_value );  
	int  set_persistent_config(char *admin, char *config);
	int  set_runtime_config(char *admin, char *config);
	/** Expand parameter references of the form "left$(middle)right".  
	
	This is deceptively simple, but does handle multiple and or nested
	references.  If self is not NULL, then we only expand references to to the
	parameter specified by self.  This is used when we want to expand
	self-references only.

	table is a hash table (managed with insert() and macro_lookup)
	that contains the various $(middle)s.

	Also expand references of the form "left$ENV(middle)right",
	replacing $ENV(middle) with getenv(middle).

	Also expand references of the form "left$RANDOM_CHOICE(middle)right".

	NOTE: Returns malloc()ed memory; caller is responsible for calling free().
	*/
	char * expand_macro ( const char *value, BUCKET *table[], int table_size,
						  char *self=NULL );

	// Iterator for the hash array managed by insert() and expand_macro().  See
	// hash_iter_begin(), hash_iter_next(), hash_iter_key(), hash_iter_value(),
	// and hash_iter_delete() for details.
	struct hash_iter {
		BUCKET ** table;
		int table_size;
		int index;
		bucket * current;
	};
	typedef hash_iter * HASHITER;

	/** Begin iteration over the table table with size table_size.

	Returns an opaque object for iterating over all entries in a hash (of the
	sort managed by insert() and expand_macro().  Use hash_iter_key() and
	hash_iter_value() to retrieve keys (names) and values for the current item.
	Use hash_iter_next() to move through the entries.  After hash_iter_begin()
	the "current" entry is set to the first entry; it is not necessary to call
	hash_iter_next() immediately.  When done be sure to call hash_iter_delete
	to free any storage that has been allocated.

	The iterator remains valid if entries are inserted into the hash, but the
	iterator may overlook them.  The iterator becomes undefined if entries are
	deleted from the hash.  If an entry is deleted, the only safe thing to
	do is call hash_iter_delete.
	*/
	HASHITER hash_iter_begin(BUCKET ** table, int table_size);


	/** Return true if we're out of entries in the hash table */
	int hash_iter_done(HASHITER iter);

	/** Move to the next entry in the hash table

	Returns false when trying to advance from the last item in the
	hash table.  Once this has returned false, the only safe things
	you can do is call hash_iter_done or hash_iter_delete.
	*/
	int hash_iter_next(HASHITER iter);

	/** Return key(name) for the current entry in the hash table

	Do no modify or free the returned value.
	*/
	char * hash_iter_key(HASHITER iter);

	/** Return value for the current entry in the hash table

	Do no modify or free the returned value.
	*/
	char * hash_iter_value(HASHITER iter);

	/** Destroy iterator and reclaim memory.
	You must pass in a pointer to the hash iterator.
	After calling the HASHITER is no longer valid.
	*/
	void hash_iter_delete(HASHITER * iter);




	int get_var( register char *value, register char **leftp,
	      register char **namep, register char **rightp, char *self=NULL,
		  bool getdollardollar=false);
	int get_special_var( const char *prefix, bool only_id_chars, register char *value, register char **leftp,
				 register char **namep, register char **rightp);
}

#endif

BEGIN_C_DECLS

	char * get_tilde(void);
	char * param ( const char *name );
	/** Insert a value into a hash table.

	The value of 'value' is associated with the name 'name'.  This is
	inserted into the table 'table' which is 'table_size' big.
	insert keeps copies of the name and value.
	*/
	void insert ( const char *name, const char *value, BUCKET *table[], int table_size );

	/** Look up a value by the name 'name' from the table 'table' which is table_size big.
	
	Values should have been inserted with insert() (above).
	Treats name case insensitively.  Returns NULL if the name isn't in the
	table.  On success returns a pointer to the associated value.  The
	value is owned by the table; do not free it.
	*/
	char * lookup_macro ( const char *name, BUCKET *table[], int table_size );
	/*
	As expand_macro() (above), but assumes the table 'ConfigTab' which is
	of size TABLESIZE.
	*/
	char * macro_expand ( const char *name );
	int param_in_pattern ( char *parameter, char *pattern);
	void init_config ( void );
	void clear_config ( void );
	void set_debug_flags( char * );
	void config_insert( const char*, const char* );
	int  param_boolean_int( const char *name, int default_value );  

END_C_DECLS


#endif /* CONFIG_H */

