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

#include "condor_common.h"
#include "condor_debug.h"

int
copy_file(const char *old_filename, const char *new_filename)
{
#if defined(WIN32)
   		// overwrite destination path
   BOOL retval;
   retval = CopyFile(old_filename, new_filename, TRUE);

   if (retval == 0) {
   		// failure
	  dprintf(D_ALWAYS, "CopyFile() failed with error=%li\n",
			  GetLastError());
	  return -1; 
   }
   else { return 0; } // success

#else
	int rc;
	int num_bytes;
	int in_fd = -1;
	int out_fd = -1;
	int new_file_created = 0;
	char buff[1024];
	struct stat fs;
	mode_t old_umask;

	old_umask = umask(0);

	rc = stat( old_filename, &fs );
	if ( rc < 0 ) {
		dprintf( D_ALWAYS, "stat(%s) failed with errno %d\n",
				 old_filename, errno );
		goto copy_file_err;
	}
	fs.st_mode &= S_IRWXU | S_IRWXG | S_IRWXO;

	in_fd = open( old_filename, O_RDONLY );
	if ( in_fd < 0 ) {
		dprintf( D_ALWAYS, "open(%s, O_RDONLY) failed with errno %d\n",
				 old_filename, errno );
		goto copy_file_err;
	}

	out_fd = open( new_filename, O_WRONLY | O_CREAT | O_TRUNC, fs.st_mode );
	if ( out_fd < 0 ) {
		dprintf( D_ALWAYS, "open(%s, O_WRONLY|O_CREAT|O_TRUNC, %d) failed with errno %d\n",
				 new_filename, fs.st_mode, errno );
		goto copy_file_err;
	}

	new_file_created = 1;

	errno = 0;
	rc = read( in_fd, buff, sizeof(buff) );
	while ( rc > 0 ) {
		num_bytes = rc;
		rc = write( out_fd, buff, num_bytes );
		if ( rc < num_bytes ) {
			dprintf( D_ALWAYS, "write(%d) to file %s return %d, errno %d\n",
					 num_bytes, new_filename, rc, errno );
			goto copy_file_err;
		}
		rc = read( in_fd, buff, sizeof(buff) );
	}

	if ( rc < 0 ) {
		dprintf( D_ALWAYS, "read() from file %s failed with errno %d\n",
				 old_filename, errno );
		goto copy_file_err;
	}

	close (in_fd);
	close (out_fd);

	umask( old_umask );

	return 0;

 copy_file_err:
	if ( in_fd != -1 ) {
		close( in_fd );
	}
	if ( out_fd != -1 ) {
		close( out_fd );
	}
	if ( new_file_created ) {
		unlink( new_filename );
	}
	umask( old_umask );
	return -1;
#endif
}
