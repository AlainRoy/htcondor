#include "condor_common.h"
#include "condor_syscall_mode.h"
#include "syscall_numbers.h"
#include "syscall_macros.h"

/* Prototypes, just so no one is confused */
char* getcwd( char*, size_t );
char* _getcwd( char*, size_t );
char* __getcwd( char*, size_t );
char* GETCWD( char*, size_t );
char* _GETCWD( char*, size_t );
char* __GETCWD( char*, size_t );
char* getwd( char* );
char* _getwd( char* );
char* __getwd( char* );


/* Definitions of the getwd() and getcwd() that the user job will
   probably call */

char *
getwd( char *path )
{
	if( LocalSysCalls() ) {
		return GETCWD( path, _POSIX_PATH_MAX );
	} else {
		REMOTE_syscall( CONDOR_getwd, path );
		return path;
	}
}

char *
getcwd( char *path, size_t size )
{
	if( LocalSysCalls() ) {
		return GETCWD( path, size );
	} else {
		REMOTE_syscall( CONDOR_getwd, path );
		return path;
	}
}


/* We want to remap these underscore versions so we don't miss them. */
REMAP_ONE( getwd, _getwd, char *, char * )
REMAP_ONE( getwd, __getwd, char *, char * )
#if !defined(IRIX)
REMAP_TWO( getcwd, _getcwd, char *, char *, size_t )
REMAP_TWO( getcwd, __getcwd, char *, char *, size_t )
#else

/* 
   On IRIX, we can't just do the direct remapping, b/c the getcwd() in
   libc.so really calls _getcwd(), which would cause in an infinite
   loop.  So, we have our own definitions of _getcwd() and __getcwd()
   that work just like getcwd(), but they call _GETCWD() and
   __GETCWD() instead.  These are defined below to use dlsym() on the
   underscore versions of the functions from libc.so.

   However, getcwd() does seem to work at all if we trap __getcwd()
   and do our magic with it.  It's unclear why, unless we have the
   source to the IRIX C library.  So, for now, we'll just ignore this
   and hopefully someday we'll get to the bottom of it.

   -Derek Wright 7/10/98
*/

char *
_getcwd( char *path, size_t size )
{
	if( LocalSysCalls() ) {
		return (char *)_GETCWD( path, size );
	} else {
		REMOTE_syscall( CONDOR_getwd, path );
		return path;
	}
}

#endif /* IRIX */


/*
  If we're not extracting and ToUpper'ing to get GETCWD(), we're
  going to have DL_EXTRACT defined so we know to use dlopen() and
  dlsym() to call the versions from the dynamic C library. 
*/
#if defined( DL_EXTRACT )
#include <dlfcn.h>
char *
GETCWD( char* path, size_t size )
{
	void *handle;
	char * (*fptr)(char *, size_t);
	if ((handle = dlopen("/usr/lib/libc.so", RTLD_LAZY)) == NULL) {
		return (char *)-1;
	}
	
	if ((fptr = (char * (*)(char *, size_t))dlsym(handle, "getcwd")) == NULL) {
		return (char *)-1;
	}

	return (*fptr)(path, size);
}	

char *
_GETCWD( char* path, size_t size )
{
	void *handle;
	char * (*fptr)(char *, size_t);
	if ((handle = dlopen("/usr/lib/libc.so", RTLD_LAZY)) == NULL) {
		return (char *)-1;
	}
	
	if ((fptr = (char * (*)(char *, size_t))dlsym(handle, "_getcwd")) == NULL) {
		return (char *)-1;
	}

	return (*fptr)(path, size);
}	

#endif /* DL_EXTRACT */
		  
