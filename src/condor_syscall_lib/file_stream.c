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
#include "condor_syscall_mode.h"
#include "syscall_numbers.h"
#include "condor_debug.h"
#include "condor_file_info.h"

int open_tcp_stream( unsigned int ip_addr, unsigned short port );
int open_file_stream( const char *file, int flags, size_t *len );

/* remote system call prototypes */
extern int REMOTE_CONDOR_file_info(char *logical_name, int *fd, 
	char *physical_name);
extern int REMOTE_CONDOR_put_file_stream(char *file, size_t len, 
	unsigned int *ip_addr, u_short *port_num);
extern int REMOTE_CONDOR_get_file_stream(char *file, size_t *len, 
	unsigned int *ip_addr, u_short *port_num);
extern int REMOTE_CONDOR_file_info(char *logical_name, int *fd, 
	char *physical_name);


int _condor_in_file_stream;

/*
  Open a file using the stream access protocol.  If we are to access
  the file via remote system calls, then we use either CONDOR_put_file_stream
  or CONDOR_get_file_stream, according to whether the flags indicate the
  access is for reading or writing.  If we are to access the file
  some way other than remote system calls, just go ahead and do it.
*/
int
open_file_stream( const char *file, int flags, size_t *len )
{
	unsigned int	addr;
	unsigned short	port;
	int				fd = -1;
	char			local_path[ _POSIX_PATH_MAX ];
	int				pipe_fd;
	int				st;
	int				mode;

		/* first assume we'll open it locally */
	_condor_in_file_stream = FALSE;

		/* Ask the shadow how we should access this file */
	mode = REMOTE_CONDOR_file_info(file, &pipe_fd, local_path );

	if( mode < 0 ) {
		EXCEPT( "CONDOR_file_info failed in open_file_stream\n" );
	}

	if( mode == IS_PRE_OPEN ) {
		fprintf( stderr, "CONDOR ERROR: The shadow says a stream file "
				 "is a pre-opened pipe!\n" );
		EXCEPT( "The shadow says a stream file is a pre-opened pipe!\n" );
	}

		/* Try to access it using local system calls */
	if( mode == IS_NFS || mode == IS_AFS ) {
		fd = syscall( SYS_open, local_path, flags, 0664 );
		if( fd >= 0 ) {
			if( !(flags & O_WRONLY) ) {
				*len = syscall( SYS_lseek, fd, 0, 2 );
				syscall( SYS_lseek, fd, 0, 0 );
			}
			dprintf( D_ALWAYS, "Opened \"%s\" via local syscalls\n",local_path);
		}
	}

		/* Try to access it using the file stream protocol  */
	if( fd < 0 ) {
		if( flags & O_WRONLY ) {
			st = REMOTE_CONDOR_put_file_stream(file,*len,&addr,&port);
		} else {
			st = REMOTE_CONDOR_get_file_stream(file, len,&addr,&port);
			dprintf(D_NETWORK, "Shadow addr to get exec file: %s\n", ipport_to_string(htonl(addr), htons(port)));
/*
			st = REMOTE_syscall(CONDOR_get_file_stream, file, len, &addr, &port);
			dprintf(D_NETWORK, "Shadow addr to get exec file: %s\n", ipport_to_string(htonl(addr), htons(port)));
*/
		}

		if( st < 0 ) {
			dprintf( D_ALWAYS, "File stream access \"%s\" failed\n",local_path);
			return -1;
		}

			/* Connect to the specified party */
		fd = open_tcp_stream( addr, port );
		if( fd < 0 ) {
			dprintf( D_ALWAYS, "open_tcp_stream() failed\n" );
			return -1;
		} else {
			dprintf( D_ALWAYS,"Opened \"%s\" via file stream\n", local_path);
		}
		_condor_in_file_stream = TRUE;
	}

	return fd;
}

/*
  Open a TCP connection at the given hostname and port number.  This
  will result in a file descriptor where we can read or write the
  file.  N.B. both the IP address and the port number are given in host
  byte order.
*/
int
open_tcp_stream( unsigned int ip_addr, unsigned short port )
{
	struct sockaddr_in	sin;
	int		fd;
	int		status;
	int		scm;

	scm = SetSyscalls( SYS_LOCAL | SYS_UNMAPPED );

		/* generate a socket */
	fd = socket( AF_INET, SOCK_STREAM, 0 );
	assert( fd >= 0 );
	dprintf( D_FULLDEBUG, "Generated a data socket - fd = %d\n", fd );
		
		/* Now we need to bind to the right interface. */
	if( ! _condor_local_bind(fd) ) {
			/* error in bind() */
		close(fd);
		SetSyscalls( scm );
		return -1;
	}

		/* Now, set the remote address. */
	ip_addr = htonl( ip_addr );
	port = htons( port );
	if ( !_condor_connect(fd, ip_addr, port) ) {
		dprintf( D_ALWAYS, "open_tcp_stream: _condor_connect failed\n");
		SetSyscalls( scm );
		return -1;
	}
	dprintf( D_FULLDEBUG, "Connection completed - returning fd %d\n", fd );

	SetSyscalls( scm );

	return fd;
}
