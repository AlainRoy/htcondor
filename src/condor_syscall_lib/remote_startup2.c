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

 

/*
  This is the startup routine for "normal" condor programs - that is
  linked for both remote system calls and checkpointing.  "Standalone"
  condor programs - those linked for checkpointing, but not remote
  system calls, get linked with a completely different version of
  MAIN() (found in the "condor_ckpt" directory.  We assume here that
  our parent will provide command line arguments which tell us how to
  attach to a "command stream", and then provide commands which control
  how we start things.

  The command stream here is a list of commands issued by the
  condor_starter to the condor user process on a special file
  descriptor which the starter will make available to the user process
  when it is born (via a pipe).  The purpose is to control the user
  process's initial execution state and the checkpoint, restart, and
  migrate functions.

  If the command line arguments look like:
  		<program_name> '-_condor_cmd_fd' <fd_number>
  then <fd_number> is the file descriptor from which the process should
  read its commands.

  If the command line arguments look like
		<program_name> '-_condor_cmd_file' <file_name>
  then <file_name> is the name of a *local* file which the program should
  open and read commands from.  This interface is useful for debugging
  since you can run the command from a shell or a debugger without
  the need for a parent process to set up a pipe.

  In any case, once the command stream processing is complete, main()
  will be called in such a way that it appears the above described
  arguments never existed.

  Commands are:

	iwd <pathname>
		Change working directory to <pathname>.  This is intended to
		get the process into the working directory specified in the
		user's job description file.

	fd <n> <pathname> <open_mode>
		Open the file <pathname> with the mode <open_mode>.  Set things up
		(using dup2()), so that the file is available at file descriptor
		number <n>.  This is intended for redirection of the standard
		files 0, 1, and 2.

	ckpt <pathname>
		The process should write its state information to the file
		<pathname> so that it can be restarted at a later time.
		We don't actually do a checkpoint here, we just set things
		up so that when we checkpoint, the given file name will
		be used.  The actual checkpoint is triggered by recipt of
		the signal SIGTSTP, or by the user code calling the ckpt()
		routine.

	restart <pathname>
		The process should read its state information from the file
		<pathname> and do a restart.  (In this case, main() will not
		get called, as we are jumping back to wherever the process
		left off at checkpoint time.)

	migrate_to <host_name> <port_number>
		A process on host <host_name> is listening at <port_number> for
		a TCP connection.  This process should connect to the given
		port, and write its state information onto the TCP connection
		exactly as it would for a ckpt command.  It is intended that
		the other process is running the same a.out file that this
		process is, and will use the state information to effect
		a restart on the new machine (a migration).

	migrate_from <fd>
		This process's parent (the condor_starter) has passed it an open
		file descriptor <fd> which is in reality a TCP socket.  The
		remote process will write its state information onto the TCP
		socket, which this process will use to effect a restart.  Since
		the restart is on a different machine, this is a migration.

	exit <status>
		The process should exit now with status <status>.  This
		is intended to be issued after a "ckpt" or "migrate_to"
		command.  We don't want to assume that the process
		should always exit after one of these commands because
		we want the flexibility to create interim checkpoints.
		(It's not clear why we would want to want to send a
		copy of a process to another machine and continue
		running on the current machine, but we have that
		flexibility too if we want it...)

	end
		We are temporarily at the end of commands.  Now it is time to
		call main() or effect the restart as requested.  Note that
		the process should not close the file descriptor at this point.
		Instead a signal handler will be set up to handle the signal
		SIGTSTP.  The signal handler will invoke the command stream
		interpreter again.  This is done so that the condor_starter can
		send a "ckpt" or a "migrate_to" command to the process after it
		has been running for some time.  In the case of the "ckpt"
		command the name of the file could have been known in advance, but
		in the case of the "migrate_to" command the name and port
		number of the remote host are presumably not known until
		the migration becomes necessary.
*/


#include "condor_common.h"
#include "condor_syscall_mode.h"
#include "syscall_numbers.h"
#include "condor_debug.h"
#include "condor_file_info.h"
static char *_FileName_ = __FILE__;

enum result { NOT_OK = 0, OK = 1, END };

enum command {
	IWD,
	FD,
	CKPT,
	RESTART,
	MIGRATE_TO,
	MIGRATE_FROM,
	EXIT,
	END_MARKER,
	NO_COMMAND
};

typedef struct {
	enum command id;
	char *name;
} COMMAND;

COMMAND CmdTable[] = {
	IWD,			"iwd",
	FD,				"fd",
	CKPT,			"ckpt",
	RESTART,		"restart",
	MIGRATE_TO,		"migrate_to",
	MIGRATE_FROM,	"migrate_from",
	EXIT,			"exit",
	END_MARKER,		"end",
	NO_COMMAND,		"",
};

#if 0
	static int		DebugFd;
	void debug_msg( const char *msg );
#endif

void _condor_interp_cmd_stream( int fd );
static void scan_cmd( char *buf, int *argc, char *argv[] );
static enum result do_cmd( int argc, char *argv[] );
static enum command find_cmd( const char *name );
static void display_cmd( int argc, char *argv[] );
static BOOLEAN condor_iwd( const char *path );
static BOOLEAN condor_fd( const char *num, const char *path, const char *open_mode );
static BOOLEAN condor_ckpt( const char *path );
static BOOLEAN condor_restart( );
static BOOLEAN condor_migrate_to( const char *host_addr, const char *port_num );
static BOOLEAN condor_migrate_from( const char *fd_no );
static BOOLEAN condor_exit( const char *status );
static int open_tcp_stream( unsigned int ip_addr, unsigned short port );
void unblock_signals();
void display_ip_addr( unsigned int addr );
void open_std_file( int which );
void set_iwd();
int open_file_stream( const char *local_path, int flags, size_t *len );
int open_ckpt_file( const char *name, int flags, size_t n_bytes );
void get_ckpt_name();
extern volatile int InRestart;
extern void InitFileState();
void _condor_disable_uid_switching();

int improved_condor_fd_arg ()
{
    int fd;
    char *fdp, *extra;

    fdp = getenv ("CONDOR_CMDFD");
    if (! fdp) return -1;
    fd = strtol (fdp, &extra, 0);
    if (*extra) return -1;
    pre_open (fd, TRUE, FALSE);
    return fd;
}


typedef unsigned long RAW_ADDR;

/* We don't know how to read the ckpt file from here, so we'll just
   look for the easter egg that vic left for us in /common/tmp. */
static RAW_ADDR
get_ckpt_low_shlib_start ()
{
     int fd;
     RAW_ADDR lss;

     fd = open ("/common/tmp/zandy/ckptinfo", O_RDONLY);
     if (fd < 0) {
	  fprintf (stderr, "Can't open ckptinfo file.  Bye\n");
	  exit (1);
     }
     if (sizeof (RAW_ADDR) != read (fd, &lss, sizeof (RAW_ADDR))) {
	  fprintf (stderr, "Can't read ckptinfo file.  Bye\n");
	  exit (1);
     }
     return lss;
}


/* Return the start address of the lowest dynamic object in the
   process.  When the process is restarted, any code loaded below this
   address will be safe, in that it will not be clobbered when the
   checkpoint image is restored.

   This is peculiar to Sparc Solaris 2.5.1 and should be moved to
   machdep.

   Assumptions:

   1. Dynamic libraries are loaded into descending addresses
   2. 0xe0000000 is a lower bound on dynamic segments and an upper
      bound on the junk below dynamic segments, like the text, data,
      and heap
   3. The segment map returned by PIOCMAP on /proc is sorted in
      ascending pr_vaddr order
                                                    -zandy 6/19/1998
 */
void zandy_continue ();

#include <sys/types.h> 
#include <sys/signal.h>
#include <sys/fault.h> 
#include <sys/syscall.h>
#include <sys/procfs.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <dlfcn.h>
static caddr_t
start_of_lowest_shlib ()
{
     int pfd;
     prmap_t *pmap;
     int npmap;
     int i;
     char proc[32];
     int scm;

     caddr_t segbound = (caddr_t) 0xe0000000;

     scm = SetSyscalls (SYS_LOCAL | SYS_UNMAPPED);

     /* Open /proc device for this process */
     sprintf (proc, "/proc/%d", getpid ());
     pfd = open (proc, O_RDONLY);
     if (pfd < 0) {
	  dprintf (D_ALWAYS, "Can't open /proc for this process\n");
	  Suicide ();
     }

     /* Get segment map for this process */
     if (ioctl (pfd, PIOCNMAP, &npmap) < 0) {
	  dprintf (D_ALWAYS, "Can't PIOCNMAP myself\n");
	  Suicide ();
     }
     pmap = (prmap_t *) malloc ((npmap + 1) * sizeof (prmap_t));
     if (! pmap) {
	  dprintf (D_ALWAYS, "Out of memory\n");
	  Suicide ();
     }
     if (ioctl (pfd, PIOCMAP, pmap) < 0) {
	  dprintf (D_ALWAYS, "Can't PIOCMAP myself\n");
	  Suicide ();
     }

     /* Find the first segment likely to be (to have once been, in a
        restarted process) a shared object. */
     for (i = 0; i < npmap && pmap[i].pr_vaddr < segbound; i++)
	  ;
     close (pfd);
     free (pmap);
     SetSyscalls (scm);
     if (i < npmap)
	  return pmap[i].pr_vaddr;
     else 
	  return NULL;
}


void
zandy_continue ()
{
     int cmd_fd = -1;
     char *cmd_name;
     int scm;
     int x = 1;

     dprintf (D_ALWAYS, "ZANDY: Entering zandy_continue2 (%#010x)\n",
	      zandy_continue);

#if 0
     dprintf (D_ALWAYS, "ZANDY: Zeekin' it\n");
     zeek ();
     dprintf (D_ALWAYS, "ZANDY: Zeeked it\n");
#endif

#if 0
     /* Standalone */
     SetSyscalls( SYS_LOCAL | SYS_UNMAPPED );
     DebugFlags = 0;		/* disable dprintf messages */
     InitFileState();  /* to create a file_state table so no SEGV */
     return;
#endif

     _condor_prestart (SYS_REMOTE);
     dprintf (D_ALWAYS, "ZANDY: Did prestart\n");     
#define USE_PIPES 0

#if USE_PIPES
     init_syscall_connection( TRUE );
#else
     init_syscall_connection( FALSE );
#endif
     dprintf (D_ALWAYS, "ZANDY: Did syscall_connection\n");     
     cmd_fd = improved_condor_fd_arg ();
     if (cmd_fd < 0) {
	  fprintf (stderr, "Can't get a cmd fd!\n");
	  Suicide ();
     }
     dprintf (D_ALWAYS, "ZANDY: Did elcheapo\n");     
     _condor_interp_cmd_stream( cmd_fd );
     dprintf (D_ALWAYS, "ZANDY: Did cmd_stream\n");     
     unblock_signals ();
     dprintf (D_ALWAYS, "ZANDY: Did unblock\n");     
     SetSyscalls( SYS_REMOTE | SYS_MAPPED );
     dprintf (D_ALWAYS, "ZANDY: Did set syscalls\n");     
     set_iwd();
     dprintf (D_ALWAYS, "ZANDY: Did iwd\n");     
     get_ckpt_name();
     dprintf (D_ALWAYS, "ZANDY: Did ckptname\n");     
     open_std_file( 0 );
     open_std_file( 1 );
     open_std_file( 2 );
     InRestart = FALSE;
     dprintf (D_ALWAYS, "ZANDY: Finished zandy_continue\n");
}


int
#if defined(HPUX)
_START( int argc, char *argv[], char **envp )
#else
MAIN( int argc, char *argv[], char **envp )
#endif
{
	int		cmd_fd = -1;
	char	*cmd_name;
	char	*extra;
	int		scm;
	char	*argv0;
	char 	*argv1;
	
		/* 
		   Since we're the user job, we want to just disable any
		   priv-state switching.  We would, eventually, anyway, since
		   we'd realize we weren't root, but then we hit the somewhat
		   expensive init_condor_ids() code in uids.c, which is
		   segfaulting on HPUX10 on a machine using YP.  So, we just
		   call a helper function here that sets the flags in uids.c
		   how we want them so we don't do switching, period.
		   -Derek Wright 7/21/98
		*/
	_condor_disable_uid_switching();

#undef WAIT_FOR_DEBUGGER
#if defined(WAIT_FOR_DEBUGGER)
	int		do_wait = 1;
	while( do_wait )
		;
#endif

	/* Some platforms have very picky strcmp()'s which like
	 * to coredump (IRIX) so make certain argv[1] points to something 
	 * by using an intermediate argv1 variable where neccesary */
	 if ( argc < 2 ) 
		argv1 = "\0";
	 else
		argv1 = argv[1];

		/*
		We must be started by a parent providing a command stream,
		therefore there must be at least 3 arguments.
		So here we check and see if we have been started properly
		with a command stream, as a condor_starter would do.  If
		not, we set syscalls to local, disable dprintf debug messages,
		print out a warning, and attempt to run normally "outside"
		of condor (with no checkpointing, remote syscalls, etc).  This
		allows users to have one condor-linked binary which can be run
		either inside or outside of condor.  -Todd, 5/97.
		*/
	if ( (argc < 3) ||
		 ( (strcmp("-_condor_cmd_fd",argv1) != MATCH) &&
		   (strcmp("-_condor_cmd_file",argv1) != MATCH) ) ) {
				/* Run the job "normally" outside of condor */
				SetSyscalls( SYS_LOCAL | SYS_UNMAPPED );
				DebugFlags = 0;		/* disable dprintf messages */
				InitFileState();  /* to create a file_state table so no SEGV */
				if ( strcmp("-_condor_nowarn",argv1) != MATCH ) {
					fprintf(stderr,"WARNING: This binary has been linked for Condor.\nWARNING: Setting up to run outside of Condor...\n");
				} else {
					/* compensate argument vector for flag */
					argv0 = argv[0];
					argv++;
					argc--;
					argv[0] = argv0;
				}

				/* Now start running user code and forget about condor */
#if defined(HPUX)
				exit(_start( argc, argv, envp ));
#else
				exit( main( argc, argv, envp ));
#endif
	}

	
	/* now setup signal handlers, etc */
	_condor_prestart( SYS_REMOTE );


#define USE_PIPES 0

#if USE_PIPES
	init_syscall_connection( TRUE );
#else
	init_syscall_connection( FALSE );
#endif

#if 0
	dprintf( D_ALWAYS, "User process started\n" );
	dprintf( D_ALWAYS, "\nOriginal\n" );
	DumpOpenFds();
	dprintf( D_ALWAYS, "END\n\n" );
	delay();
#endif

	if( strcmp("-_condor_cmd_fd",argv[1]) == MATCH ) {
#if 0
		dprintf( D_ALWAYS, "Found condor_cmd_fd\n" );
		delay();
#endif
		cmd_fd = strtol( argv[2], &extra, 0 );
		if( extra[0] ) {
			dprintf( D_ALWAYS, "Can't parse cmd stream fd (%s)\n", argv[2]);
			exit( 1 );
		}
#if 0
		dprintf( D_ALWAYS, "fd number is %d\n", cmd_fd );
		delay();
#endif
		/* scm = SetSyscalls( SYS_LOCAL | SYS_MAPPED ); */
		pre_open( cmd_fd, TRUE, FALSE );

#if 0
		dprintf( D_ALWAYS, "\nBefore reading commands\n" );
		DumpOpenFds();
		dprintf( D_ALWAYS, "END\n\n" );
		delay();
#endif


	} else if( strcmp("-_condor_cmd_file",argv[1]) == MATCH ) {

		dprintf( D_FULLDEBUG, "Found condor_cmd_file\n" );
		/* scm = SetSyscalls( SYS_LOCAL | SYS_MAPPED ); */
		cmd_fd = open( argv[2], O_RDONLY);
		if( cmd_fd < 0 ) {
			dprintf( D_ALWAYS, "Can't read cmd file \"%s\"\n", argv[2] );
			Suicide();
		}

		/* Some error in the command line syntax */
	} else {
		dprintf( D_ALWAYS, "Error in command line syntax\n" );
		Suicide();
	}

#if 0
	dprintf( D_ALWAYS, "\nCalling cmd stream processor\n" );
	delay();
#endif
	_condor_interp_cmd_stream( cmd_fd );
#if 0
	dprintf( D_ALWAYS, "Done\n\n" );
	delay();
#endif
	cmd_name = argv[0];
	argv += 2;
	argc -= 2;

   /*
   The flag '-_condor_debug_wait' can be set to setup an infinite loop so
   that we can attach to the user job.  This argument can be specified in
   the submit file, but must be the first argument to the job.  The argv
   is compensated so that the job gets the vector it expected.  --RR
   */
   if ( (argc > 1) && (strcmp(argv[1], "-_condor_debug_wait") == MATCH) )
   {
       int i = 1;
       argv ++;
       argc --;
       while (i);
   }

	argv[0] = cmd_name;

	unblock_signals();
	SetSyscalls( SYS_REMOTE | SYS_MAPPED );

#if 0
	dprintf( D_ALWAYS, "\nBefore calling main()\n" );
	DumpOpenFds();
	dprintf( D_ALWAYS, "END\n\n" );
#endif

#if 0
	DebugFd = syscall( SYS_open, "/tmp/mike", O_WRONLY | O_CREAT | O_TRUNC, 0664 );
	syscall( SYS_dup2, DebugFd, 23 );
	DebugFd = 23;
	debug_msg( "Hello World!\n" );
#endif

	set_iwd();
	get_ckpt_name();
	open_std_file( 0 );
	open_std_file( 1 );
	open_std_file( 2 );

	InRestart = FALSE;
		/* Now start running user code */
#if defined(HPUX)
	exit(_start( argc, argv, envp ));
#else
	exit( main( argc, argv, envp ));
#endif
}

void
_condor_interp_cmd_stream( int fd )
{
	BOOLEAN	at_end = FALSE;
	FILE	*fp = fdopen( fd, "r" );
	char	buf[1024];
	int		argc;
	char	*argv[256];
	int		scm;

	while( fgets(buf,sizeof(buf),fp) ) {
		scan_cmd( buf, &argc, argv );
		switch( do_cmd(argc,argv) ) {
		  case OK:
			break;
		  case NOT_OK:
			dprintf( D_ALWAYS, "?\n" );
			break;
		  case END:
			return;
		}
	}
	dprintf( D_ALWAYS, "ERROR: EOF on command stream\n" );
	Suicide();
}

static void
scan_cmd( char *buf, int *argc, char *argv[] )
{
	int		i;

	argv[0] = strtok( buf, " \n" );
	if( argv[0] == NULL ) {
		*argc = 0;
		return;
	}

	for( i = 1; argv[i] = strtok(NULL," \n"); i++ )
		;
	*argc = i;
}


static enum result
do_cmd( int argc, char *argv[] )
{
	if( argc == 0 ) {
		return FALSE;
	}

	switch( find_cmd(argv[0]) ) {
	  case END_MARKER:
		return END;
	  case IWD:
		if( argc != 2 ) {
			return FALSE;
		}
		return condor_iwd( argv[1] );
	  case FD:
		assert( argc == 4 );
		return condor_fd( argv[1], argv[2], argv[3] );
	  case RESTART:
		if( argc != 1 ) {
			return FALSE;
		}
		return condor_restart();
	  case CKPT:
		if( argc != 2 ) {
			return FALSE;
		}
		return condor_ckpt( argv[1] );
	  case MIGRATE_TO:
		if( argc != 3 ) {
			return FALSE;
		}
		return condor_migrate_to( argv[1], argv[2] );
	  case MIGRATE_FROM:
		if( argc != 2 ) {
			return FALSE;
		}
		return condor_migrate_from( argv[1] );
	  case EXIT:
		if( argc != 2 ) {
			return FALSE;
		}
		return condor_exit( argv[1] );
	  default:
		return FALSE;
	}
}

static enum command
find_cmd( const char *str )
{
	COMMAND	*ptr;

	for( ptr = CmdTable; ptr->id != NO_COMMAND; ptr++ ) {
		if( strcmp(ptr->name,str) == MATCH ) {
			return ptr->id;
		}
	}
	return NO_COMMAND;
}

static BOOLEAN
condor_iwd( const char *path )
{
#if 0
	dprintf( D_ALWAYS, "condor_iwd: path = \"%s\"\n", path );
	delay();
#endif
	REMOTE_syscall( CONDOR_chdir, path );
	Set_CWD( path );
	return TRUE;
}

static BOOLEAN
condor_fd( const char *num, const char *path, const char *open_mode )
{
	/* no longer used  - ignore */
	return TRUE;
}

static BOOLEAN
condor_ckpt( const char *path )
{
	dprintf( D_ALWAYS, "condor_ckpt: filename = \"%s\"\n", path );
	init_image_with_file_name( path );

	return TRUE;
}


static BOOLEAN
condor_restart()
{
	int		fd;
	size_t	n_bytes;

	dprintf( D_ALWAYS, "condor_restart:\n" );

#if 0
	fd = open_ckpt_file( "", O_RDONLY, n_bytes );
	init_image_with_file_descriptor( fd );
#else
	get_ckpt_name();
#endif
	restart();

		/* Can never get here - restart() jumps back into user code */
	return FALSE;
}

static BOOLEAN
condor_migrate_to( const char *host_name, const char *port_num )
{
	char 	*extra;
	long	port;

	port = strtol( port_num, &extra, 0 );
	if( extra[0] ) {
		return FALSE;
	}

	dprintf( D_FULLDEBUG,
		"condor_migrate_to: host = \"%s\", port = %d\n", host_name, port
	);
	return TRUE;
}

static BOOLEAN
condor_migrate_from( const char *fd_no )
{
	long	fd;
	char	*extra;

	fd = strtol( fd_no, &extra, 0 );
	if( extra[0] ) {
		return FALSE;
	}
	
	dprintf( D_FULLDEBUG, "condor_migrate_from: fd = %d\n", fd );
	return TRUE;
}

static BOOLEAN
condor_exit( const char *status )
{
	long	st;
	char	*extra;

	st = strtol( status, &extra, 0 );
	if( extra[0] ) {
		return FALSE;
	}
	
	dprintf( D_FULLDEBUG, "condor_exit: status = %d\n", st );
	return TRUE;
}


/*
  Open a stream for writing our checkpoint information.  Since we are in
  the "remote" startup file, this is the remote version.  We do it with
  a "pseudo system call" to the shadow.
*/
int
open_ckpt_file( const char *name, int flags, size_t n_bytes )
{
	char			file_name[ _POSIX_PATH_MAX ];
	int				status;

	return open_file_stream( name, flags, &n_bytes );
}

void
report_image_size( int kbytes )
{
	dprintf( D_ALWAYS, "Sending Image Size Report of %d kilobytes\n", kbytes );
	REMOTE_syscall( CONDOR_image_size, kbytes );
}

void
unblock_signals()
{
	sigset_t	sig_mask;
	int			scm;

	scm = SetSyscalls( SYS_LOCAL | SYS_UNMAPPED );

		/* unblock signals */
	sigfillset( &sig_mask );
	if( sigprocmask(SIG_UNBLOCK,&sig_mask,0) < 0 ) {
		dprintf( D_ALWAYS, "sigprocmask failed in unblock_signals: %s",
				 strerror(errno));
		Suicide();
	}

	SetSyscalls( scm );

	dprintf( D_ALWAYS, "Unblocked all signals\n" );
}

#define UNIT 10000

#if defined(ALPHA)
#	define LIM (2000 * UNIT)
#elif defined(AIX32)
#	define LIM (225 * UNIT)
#elif defined(SPARC)
#	define LIM (260 * UNIT)
#elif defined(ULTRIX43)
#	define LIM (170 * UNIT)
#elif defined(HPPAR)
#	define LIM (260 * UNIT)
#elif defined(LINUX)
#	define LIM (200 * UNIT)
#elif defined(Solaris)
#	define LIM (260 * UNIT)
#elif defined(SGI)
#	define LIM (200 * UNIT)
#endif

#if 1
	delay()
	{
		int		i;

		for( i=0; i<LIM; i++ )
			;
	}
#else
	delay(){}
#endif
#define B_NET(x) (((long)(x)&IN_CLASSB_NET)>>IN_CLASSB_NSHIFT)
#define B_HOST(x) ((long)(x)&IN_CLASSB_HOST)
#define HI(x) (((long)(x)&0xff00)>>8)
#define LO(x) ((long)(x)&0xff)


void
display_ip_addr( unsigned int addr )
{
	int		net_part;
	int		host_part;

	if( IN_CLASSB(addr) ) {
		net_part = B_NET(addr);
		host_part = B_HOST(addr);
		dprintf( D_FULLDEBUG, "%d.%d", HI(B_NET(addr)), LO(B_NET(addr)) );
		dprintf( D_FULLDEBUG, ".%d.%d\n", HI(B_HOST(addr)), LO(B_HOST(addr)) );
	} else {
		dprintf( D_FULLDEBUG, "0x%x\n", addr );
	}
}

/*
  Open a standard file (0, 1, or 2), given its fd number.
*/
void
open_std_file( int which )
{
	char	name[ _POSIX_PATH_MAX ];
	char	buf[ _POSIX_PATH_MAX + 50 ];
	int		pipe_fd;
	int		answer;
	int		status;

		/* The ckpt layer assumes the process is attached to a terminal,
		   so these are "pre_opened" in our open file table.  Here we must
		   get rid of those entries so we can open them properly for
		   remotely running jobs.
		*/
	close( which );

	status =  REMOTE_syscall( CONDOR_std_file_info, which, name, &pipe_fd );
	if( status == IS_PRE_OPEN ) {
		answer = pipe_fd;			/* it's a pipe */
	} else {
		switch( which ) {			/* it's an ordinary file */
		  case 0:
			answer = open( name, O_RDONLY, 0 );
			break;
		  case 1:
		  case 2:
			answer = open( name, O_WRONLY, 0 );
			break;
		}
	}
	if( answer < 0 ) {
		sprintf( buf, "Can't open \"%s\"", name );
		REMOTE_syscall(CONDOR_perm_error, buf );
		dprintf( D_ALWAYS, buf );
		Suicide();
	} else {
		if( answer != which ) {
			dup2( answer, which );
		}
	}
}

void
set_iwd()
{
	char	iwd[ _POSIX_PATH_MAX ];
	char	buf[ _POSIX_PATH_MAX + 50 ];

	if( REMOTE_syscall(CONDOR_get_iwd,iwd) < 0 ) {
		REMOTE_syscall(
			CONDOR_perm_error,
			"Can't determine initial working directory"
		);
		dprintf( D_ALWAYS, "Can't determine initial working directory\n" );
		Suicide();
	}
	if( REMOTE_syscall(CONDOR_chdir,iwd) < 0 ) {
		sprintf( buf, "Can't open working directory \"%s\"", iwd );
		REMOTE_syscall( CONDOR_perm_error, buf );
		dprintf( D_ALWAYS, "Can't chdir(%s)\n", iwd );
		Suicide();
	}
	Set_CWD( iwd );
}

void
get_ckpt_name()
{
	char	ckpt_name[ _POSIX_PATH_MAX ];
	int		status;

	status = REMOTE_syscall( CONDOR_get_ckpt_name, ckpt_name );
	if( status < 0 ) {
		dprintf( D_ALWAYS, "Can't get checkpoint file name!\n" );
		Suicide(0);
	}
	dprintf( D_ALWAYS, "Checkpoint file name is \"%s\"\n", ckpt_name );
	init_image_with_file_name( ckpt_name );
}

#if 0
void
debug_msg( const char *msg )
{
	int		status;

	status = syscall( SYS_write, DebugFd, msg, strlen(msg) );
	if( status < 0 ) {
		exit( errno );
	}
}
#endif
