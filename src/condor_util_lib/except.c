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
#include "condor_sys.h"
#include "exit.h"
#include "condor_debug.h"

#ifdef LINT
/* VARARGS */
_EXCEPT_(foo)
char	*foo;
{ printf( foo ); }
#else LINT

extern int	condor_nerr;
extern char	*condor_errlist[];

int		_EXCEPT_Line;
int		_EXCEPT_Errno;
char	*_EXCEPT_File;
int		(*_EXCEPT_Cleanup)(int,int,char*);
int		SetSyscalls(int);

void
_EXCEPT_(char *fmt, ...)
{
	va_list pvar;
	char buf[ BUFSIZ ];

	(void)SetSyscalls( SYS_LOCAL | SYS_RECORDED );
	va_start(pvar, fmt);

#if vax || (i386 && !LINUX && !defined(Solaris)) || bobcat || ibm032
	{
		FILE _strbuf;
		int *argaddr = &va_arg(pvar, int);

		_strbuf._flag = _IOWRT/*+_IOSTRG*/;
		_strbuf._ptr  = buf;
		_strbuf._cnt  = BUFSIZ;
		_doprnt( fmt, argaddr, &_strbuf );
		putc('\0', &_strbuf);
	}
#else vax || (i386 && !LINUX && !defined(Solaris)) || bobcat || ibm032
	vsprintf( buf, fmt, pvar );
#endif vax || (i386 && !LINUXi && !defined(Solaris)) || bobcat || ibm032

	if( _EXCEPT_Errno < 0 ) {
		_EXCEPT_Errno = -_EXCEPT_Errno;
		dprintf( D_ALWAYS, "ERROR \"%s\" at line %d in file %s: %s\n",
					buf, _EXCEPT_Line, _EXCEPT_File,
					(_EXCEPT_Errno<condor_nerr) ? condor_errlist[_EXCEPT_Errno]
												 : "Unknown CONDOR error" );
	} else {
		dprintf( D_ALWAYS, "ERROR \"%s\" at line %d in file %s: %s\n",
						buf, _EXCEPT_Line, _EXCEPT_File,
						(_EXCEPT_Errno<sys_nerr) ? sys_errlist[_EXCEPT_Errno]
												 : "Unknown error" );
	}

	if( _EXCEPT_Cleanup ) {
		(*_EXCEPT_Cleanup)( _EXCEPT_Line, _EXCEPT_Errno, buf );
	}

	va_end(pvar);

	exit( JOB_EXCEPTION );
}
#endif LINT
