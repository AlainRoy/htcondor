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
#ifndef STAT_WRAPPER_H
#define STAT_WRAPPER_H

#include "condor_common.h"
#include "condor_config.h"

	// Define a "standard" StatBuf type
#if HAVE_STAT64
typedef struct stat64 StatStructType;
#elif HAVE__STATI64
typedef struct _stati64 StatStructType;
#else
typedef struct stat StatStructType;
#endif

class StatWrapper
{
public:
	StatWrapper( const char *path );
	StatWrapper( int fd );
	~StatWrapper( );
	int Retry( void );

	const char *GetStatFn( void ) { return name; };
	int GetStatus( void ) { return status; };
	int GetErrno( void ) { return stat_errno; };
	const StatStructType *GetStatBuf( void ) { return &stat_buf; };
	const StatStructType *GetLstatBuf( void ) { return &lstat_buf; };

private:
	int DoStat( const char *path );
	int DoStat( int fd );
	StatStructType	stat_buf;
	StatStructType	lstat_buf;
	const char *name;
	int status;
	int stat_errno;
	int	fd;
	char *path;
};


#endif
