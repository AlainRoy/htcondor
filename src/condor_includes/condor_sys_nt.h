/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

#ifndef CONDOR_SYS_NT_H
#define CONDOR_SYS_NT_H

// Disable warning about protected copy constr or assignment ops
#pragma warning( disable : 4661 )  

// Disable performance warning about casting to a bool
#pragma warning( disable : 4800 )  

// Disable warnings about multiple template instantiations (done for gcc)
#pragma warning( disable : 4660 )  

// #define NOGDI
#define NOSOUND

#define _WIN32_WINNT 0x0500 // ray's stupid kludge to get jobobject stuff to build

// Make sure to define this *before* we include winsock2.h
#define FD_SETSIZE 1024

// the ordering of the two following header files 
// is important! Starting with the new SDK, we want 
// winsock2.h not winsock.h, so we include it first. 

#include <winsock2.h>
#include <windows.h>


#include <io.h>
#include <fcntl.h>
#include <direct.h>		// for _chdir , etc
#include <sys/utime.h>	// for struct _utimbuf & friends
#include <string.h>
#define lseek _lseek
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_RDWR _O_RDWR
#define O_CREAT _O_CREAT
#define O_APPEND _O_APPEND
#define O_TRUNC _O_TRUNC
#include <sys/stat.h>
typedef unsigned short mode_t;
typedef int socklen_t;
typedef DWORD pid_t;
typedef	unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
#define stat _fixed_windows_stat
#define fstat _fixed_windows_fstat
#define MAXPATHLEN 1024
#define MAXHOSTNAMELEN 64
#define	_POSIX_PATH_MAX 255
#define pipe(fds) _pipe(fds,2048,_O_BINARY)
#define popen _popen
#define pclose _pclose
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strincmp _strnicmp
#define strdup _strdup
#define strupr _strupr
#define strlwr _strlwr
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define chdir _chdir
#define fsync _commit
#define access _access
#define execl _execl  
#define execv _execv
#define putenv _putenv
#define itoa _itoa
#define utime _utime
#define utimbuf _utimbuf
#define R_OK 4
#define W_OK 2
#define X_OK 4
#define F_OK 0
#define ssize_t SSIZE_T
#define sleep(x) Sleep(x*1000)
#define getpid _getpid
#include <process.h>
#include <time.h>
#include <lmcons.h> // for UNLEN
#if !defined(_POSIX_)
#	define _POSIX_
#	define _CONDOR_DEFINED_POSIX_
#endif
#include <limits.h>
#if defined(_CONDOR_DEFINED_POSIX_)
#	undef _POSIX_
#	undef _CONDOR_DEFINED_POSIX_
#endif

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>   // for DBL_MAX and other constants
#include <errno.h>
#include "condor_file_lock.h"
#include "condor_fix_assert.h"

#define getwd(path) (int)_getcwd(path, _POSIX_PATH_MAX)
#define mkdir(path,mode) (int)_mkdir(path)
#define S_IRWXU 0
#define S_IRWXG 1
#define S_IRWXO 2
#define S_ISDIR(mode) (((mode)&_S_IFDIR) == _S_IFDIR)
#define S_ISREG(mode) (((mode)&_S_IFREG) == _S_IFREG)
#define rint(num) ((num<0.)? -floor(-num+.5):floor(num+.5))

#define ETIMEDOUT ERROR_TIMEOUT

/* Some missing ERRNO values.... */
#ifndef ETXTBSY
#	define ETXTBSY EBUSY
#endif
#ifndef EWOULDBLOCK
#	define EWOULDBLOCK EAGAIN
#endif

typedef fd_set *SELECT_FDSET_PTR;

struct rusage {
    struct timeval ru_utime;    /* user time used */
    struct timeval ru_stime;    /* system time used */
    long    ru_maxrss;      /* XXX: 0 */
    long    ru_ixrss;       /* XXX: 0 */
    long    ru_idrss;       /* XXX: sum of rm_asrss */
    long    ru_isrss;       /* XXX: 0 */
    long    ru_minflt;      /* any page faults not requiring I/O */
    long    ru_majflt;      /* any page faults requiring I/O */
    long    ru_nswap;       /* swaps */
    long    ru_inblock;     /* block input operations */
    long    ru_oublock;     /* block output operations */
    long    ru_msgsnd;      /* messages sent */
    long    ru_msgrcv;      /* messages received */
    long    ru_nsignals;        /* signals received */
    long    ru_nvcsw;       /* voluntary context switches */
    long    ru_nivcsw;      /* involuntary " */
};

	/* Throw in signal values. Don't do values > 20. */

#define	SIGHUP		1	/* Hangup (POSIX).  */
#define	SIGINT		2	/* Interrupt (ANSI).  */
#define	SIGQUIT		3	/* Quit (POSIX).  */
#define	SIGILL		4	/* Illegal instruction (ANSI).  */
#define	SIGTRAP		5	/* Trace trap (POSIX).  */
#define	SIGABRT		6	/* Abort (ANSI).  */
#define	SIGIOT		6	/* IOT trap (4.2 BSD).  */
#define	SIGBUS		7	/* BUS error (4.2 BSD).  */
#define	SIGFPE		8	/* Floating-point exception (ANSI).  */
#define	SIGKILL		9	/* Kill, unblockable (POSIX).  */
#define	SIGUSR1		10	/* User-defined signal 1 (POSIX).  */
#define	SIGSEGV		11	/* Segmentation violation (ANSI).  */
#define	SIGUSR2		12	/* User-defined signal 2 (POSIX).  */
#define	SIGPIPE		13	/* Broken pipe (POSIX).  */
#define	SIGALRM		14	/* Alarm clock (POSIX).  */
#define	SIGTERM		15	/* Termination (ANSI).  */
#define	SIGSTKFLT	16	/* Stack fault.  */
#define	SIGCLD		SIGCHLD	/* Same as SIGCHLD (System V).  */
#define	SIGCHLD		17	/* Child status has changed (POSIX).  */
#define	SIGCONT		18	/* Continue (POSIX).  */
#define	SIGSTOP		19	/* Stop, unblockable (POSIX).  */
#define	SIGTSTP		20	/* Keyboard stop (POSIX).  */

// other macros and protos needed on WIN32 for exit status
#define WEXITSTATUS(stat) ((int)(stat))
#define WTERMSIG(stat) ((int)(stat))
#define WIFSTOPPED(stat) ((int)(0))


BEGIN_C_DECLS
// these two are usually macros, but in fact, we implement our own
// C versions of them...  
int WIFEXITED(DWORD stat);
int WIFSIGNALED(DWORD stat);

char* index(const char *s, int c);

END_C_DECLS

/* Some Win32 specifics */
#if defined(WIN32)
/* Win32 uses _stati64(); this should be detected by configure */
# define HAVE__STATI64 1
  /* Win32 has a __int64 type defined; this should be detected by configure */
#  define HAVE___INT64 1
#endif

/* Define the PRIx64 macros */

// If no inttypes, try to define our own
#if !defined( PRId64 )
# define PRId64 "I64d"
#endif
#if !defined( PRIi64 )
# define PRIi64 "I64i"
#endif
#if !defined( PRIu64 )
# define PRIu64 "I64u"
#endif

/* fix [f]stat on Windows */
#include "stat.WINDOWS.h"

#endif /* CONDOR_SYS_NT_H */
