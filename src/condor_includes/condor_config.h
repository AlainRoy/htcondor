/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2004, Condor Team, Computer Sciences Department,
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
	extern MyString global_config_file;
	extern MyString global_root_config_file;
	extern MyString local_config_files;

    int param_integer( const char *name, int default_value,
					   int min_value = INT_MIN, int max_value = INT_MAX );
	bool param_boolean( const char *name, const bool default_value );
	bool param_get_location(const char *parameter, MyString &filename,
							int &line_number);

/* here we provide C linkage to C++ defined functions. This seems a bit
	odd since if a .c file includes this, these prototypes technically don't
	exist.... */
extern "C" {
	void config( int wantsQuiet=0 );
	void config_host( char* host=NULL );
	void config_fill_ad( ClassAd* );
        int  param_boolean_int( const char *name, int default_value );  
	int  set_persistent_config(char *admin, char *config);
	int  set_runtime_config(char *admin, char *config);
	char * expand_macro ( const char *value, BUCKET *table[], int table_size,
						  char *self=NULL );
	int get_var( register char *value, register char **leftp,
	      register char **namep, register char **rightp, char *self=NULL,
		  bool getdollardollar=false);
	int get_special_var( const char *prefix, bool only_id_chars, register char *value, register char **leftp,
				 register char **namep, register char **rightp);
}
#endif

BEGIN_C_DECLS

	char * get_tilde();
	char * param ( const char *name );
	void insert ( const char *name, const char *value, BUCKET *table[], int table_size );
	char * lookup_macro ( const char *name, BUCKET *table[], int table_size );
	char * macro_expand ( const char *name );
	int param_in_pattern ( char *parameter, char *pattern);
	void init_config ( void );
	void clear_config ( void );
	void set_debug_flags( char * );
	void config_insert( const char*, const char* );

END_C_DECLS


#endif /* CONFIG_H */

