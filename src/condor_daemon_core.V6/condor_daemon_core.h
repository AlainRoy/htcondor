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
// //////////////////////////////////////////////////////////////////////////
//
// This file contains the definition for class DaemonCore. This is the
// central structure for every daemon in condor. The daemon core triggers
// preregistered handlers for corresponding events. class Service is the base
// class of the classes that daemon_core can serve. In order to use a class
// with the DaemonCore, it has to be a derived class of Service.
//
//
// //////////////////////////////////////////////////////////////////////////

#ifndef _CONDOR_DAEMON_CORE_H_
#define _CONDOR_DAEMON_CORE_H_

#include "condor_common.h"
#include "condor_uid.h"
#include "condor_io.h"
#include "condor_timer_manager.h"
#include "condor_ipverify.h"
#include "condor_commands.h"
#include "condor_classad.h"
#include "HashTable.h"
#include "list.h"
#include "extArray.h"
#ifdef WIN32
#include "ntsysinfo.h"
#endif

static const int KEEP_STREAM = 100;
static const int MAX_SOCKS_INHERITED = 4;
static char* EMPTY_DESCRIP = "<NULL>";

// typedefs for callback procedures
typedef int		(*CommandHandler)(Service*,int,Stream*);
typedef int		(Service::*CommandHandlercpp)(int,Stream*);

typedef int		(*SignalHandler)(Service*,int);
typedef int		(Service::*SignalHandlercpp)(int);

typedef int		(*SocketHandler)(Service*,Stream*);
typedef int		(Service::*SocketHandlercpp)(Stream*);

typedef int		(*ReaperHandler)(Service*,int pid,int exit_status);
typedef int		(Service::*ReaperHandlercpp)(int pid,int exit_status);

// other macros and protos needed on WIN32 for exit status
#ifdef WIN32
#define WEXITSTATUS(stat) ((int)(stat))
#define WTERMSIG(stat) ((int)(stat))
#define WIFSTOPPED(stat) ((int)(0))
int WIFEXITED(DWORD stat);
int WIFSIGNALED(DWORD stat);
#endif  // of ifdef WIN32

// some constants for HandleSig().
#define _DC_RAISESIGNAL 1
#define _DC_BLOCKSIGNAL 2
#define _DC_UNBLOCKSIGNAL 3

// If WANT_DC_PM is defined, it means we want DaemonCore Process Management.
// We _always_ want it on WinNT; on Unix, some daemons still do their own 
// Process Management (just until we get around to changing them to use 
// daemon core).
#if defined(WIN32) && !defined(WANT_DC_PM)
#define WANT_DC_PM
#endif

/** helper function for finding available port for both 
	TCP and UDP command socket */
int BindAnyCommandPort(ReliSock *rsock, SafeSock *ssock);

extern "C" char *mySubSystem;

/** This class badly needs documentation, doesn't it? 
	This file contains the definition for class DaemonCore. This is the
	central structure for every daemon in condor. The daemon core triggers
	preregistered handlers for corresponding events. class Service is the base
	class of the classes that daemon_core can serve. In order to use a class
	with the DaemonCore, it has to be a derived class of Service.
*/
class DaemonCore : public Service
{
	friend class TimerManager; 
#ifdef WIN32
	friend dc_main( int argc, char** argv );
	friend DWORD pidWatcherThread(void*);
	friend BOOL CALLBACK DCFindWindow(HWND, LPARAM);
#else
	friend main(int, char**);
#endif

	public:
		DaemonCore(int PidSize = 0, int ComSize = 0, int SigSize = 0, int SocSize = 0, int ReapSize = 0);
		~DaemonCore();

		void	Driver();
		int		ServiceCommandSocket();
		int		InServiceCommandSocket() 	{ return inServiceCommandSocket_flag; }

		int		ReInit();

		int		Verify(DCpermission perm, const struct sockaddr_in *sin );
		
		int		Register_Command(int command, char *com_descrip, CommandHandler handler, 
					char *handler_descrip, Service* s = NULL, DCpermission perm = ALLOW, int dprintf_flag = D_COMMAND);
		int		Register_Command(int command, char *com_descript, CommandHandlercpp handlercpp, 
					char *handler_descrip, Service* s, DCpermission perm = ALLOW, int dprintf_flag = D_COMMAND);
		int		Cancel_Command( int command );
		int		InfoCommandPort();
		char*	InfoCommandSinfulString(int pid=-1);

		int		Register_Signal(int sig, char *sig_descrip, SignalHandler handler, 
					char *handler_descrip, Service* s = NULL, DCpermission perm = ALLOW);
		int		Register_Signal(int sig, char *sig_descript, SignalHandlercpp handlercpp, 
					char *handler_descrip, Service* s, DCpermission perm = ALLOW);
		int		Cancel_Signal( int sig );
		int		Block_Signal(int sig) { return(HandleSig(_DC_BLOCKSIGNAL,sig)); }
		int		Unblock_Signal(int sig) { return(HandleSig(_DC_UNBLOCKSIGNAL,sig)); }

		int		Register_Reaper(char *reap_descrip, ReaperHandler handler, 
					char *handler_descrip, Service* s = NULL);
		int		Register_Reaper(char *reap_descript, ReaperHandlercpp handlercpp, 
					char *handler_descrip, Service* s);
		int		Reset_Reaper(int rid, char *reap_descrip, ReaperHandler handler, 
					char *handler_descrip, Service* s = NULL);
		int		Reset_Reaper(int rid, char *reap_descript, ReaperHandlercpp handlercpp, 
					char *handler_descrip, Service* s);
		int		Cancel_Reaper( int rid );
		
		int		Register_Socket(Stream* iosock, char *iosock_descrip, SocketHandler handler,
					char *handler_descrip, Service* s = NULL, DCpermission perm = ALLOW);
		int		Register_Socket(Stream* iosock, char *iosock_descrip, SocketHandlercpp handlercpp,
					char *handler_descrip, Service* s, DCpermission perm = ALLOW);
		int		Register_Command_Socket( Stream* iosock, char *descrip = NULL ) {
					return(Register_Socket(iosock,descrip,(SocketHandler)NULL,
										   (SocketHandlercpp)NULL,"DC Command Handler",NULL,ALLOW,0)); 
				}
		int		Cancel_Socket( Stream* );

		int		Lookup_Socket( Stream *iosock );

		int		Register_Timer(unsigned deltawhen, Event event, char *event_descrip, 
					Service* s = NULL);
		int		Register_Timer(unsigned deltawhen, unsigned period, Event event, 
					char *event_descrip, Service* s = NULL);
		int		Register_Timer(unsigned deltawhen, Eventcpp event, char *event_descrip, 
					Service* s);
		int		Register_Timer(unsigned deltawhen, unsigned period, Eventcpp event, 
					char *event_descrip, Service* s);
		int		Cancel_Timer( int id );
		int		Reset_Timer( int id, unsigned when, unsigned period = 0 );
		
		void	Dump(int, char* = NULL );

		inline	pid_t getpid() { return mypid; };
		inline	pid_t getppid() { return ppid; };
		int		Is_Pid_Alive(pid_t pid);

			/** Send_Signal to daemonCore processes or non-DC process */
		int		Send_Signal(pid_t pid, int sig);

			/** Methods for process management.  these work
				on any process, not just daemon core processes.
			*/
			//@{
			/// Shut down fast.  Please doc++ me.
		int		Shutdown_Fast(pid_t pid);
			/// Shut down gracefully.  Please doc++ me.
		int		Shutdown_Graceful(pid_t pid);
			/// Suspend a process.  Please doc++ me.
		int		Suspend_Process(pid_t pid);
			/// Continue a process.  Please doc++ me.
		int		Continue_Process(pid_t pid);
			//@}

			/** Data pointer functions.  These functions deal with
				associating a pointer to data with a registered callback.
			*/
			//@{
			/** Set the data pointer when you're <b>inside</b> the handler
				function.  It will not work outside.
				@param data The desired pointer to set...
			*/
		int		SetDataPtr( void *data );

			/** "Register" a data pointer.  You want to do this immediately
				after you register a Command/Socket/Timer/etc.  When you enter 
				the handler for this registered function, the pointer you 
				specify will be magically available to (G|S)etDataPtr().
				@param data The desired pointer to set...
			*/
		int		Register_DataPtr( void *data );

			/** Get the data pointer when you're <b>inside</b> the handler
				function.  It will not work outside.  You must have done a 
				Register_DataPtr after the function was registered for this
				to work.
				@return The desired pointer to set...
			*/
		void	*GetDataPtr();
			//@}

			/** Put the {key, value} pair into the environment */
		int SetEnv(char *key, char *value);
			/** Put env_var into the environment
				@param env_var Desired string to go into environment;
				   must be of the form 'name=value' 
			*/
		int SetEnv(char *env_var);
		
			/** Create a process.  Works for NT and Unix.  On Unix, a
				fork and exec are done.  Read the source for ugly 
				details - it takes care of most everything.
				@param name The full path name of the executable.  If this 
				   is a relative path name AND cwd is specified, then we
				   prepend the result of getcwd() to 'name' and pass 
				   that to exec().
				@param args The list of args, separated by spaces.  The 
				   first arg is argv[0], the name you want to appear in 
				   'ps'.  If you don't specify agrs, then 'name' is 
				   used as argv[0].
				@param priv The priv state to change into right before
				   the exec.  Default = no action.
				@param reaper_id The reaper number to use.  Default = 1.
				@param want_command_port Well, do you?  Default = TRUE
				@param env A colon-separated list of stuff to be put into
				   the environment of the new process
				@param cwd Current Working Directory
				@param new_process_group Do you want to make one?
				   Default = FALSE
				@param sock_inherit_list A list of socks to inherit.
				@param std An array of three file descriptors to map
				   to stdin, stdout, stderr respectively.  If this array
				   is NULL, don't perform remapping.
                @param nice_inc The value to be passed to nice() in the
                   child.  0 < nice < 20, and greater numbers mean
                   less priority.  This is an addition to the current
                   nice value.
				@return The pid of the newly created process.
			*/
		int	Create_Process(
			char		*name,
			char		*args,
			priv_state	priv = PRIV_UNKNOWN,
			int			reaper_id = 1,
			int			want_commanand_port = TRUE,
			char		*env = NULL,
			char		*cwd = NULL,
			int			new_process_group = FALSE,
			Stream		*sock_inherit_list[] = NULL,
			int			std[] = NULL,
            int         nice_inc = 0
			);

		// NULL terminated array of inherited sockets
		Stream **GetInheritedSocks() { return (Stream **)inheritedSocks; }

		// see if process was killed because it was hung
		int Was_Not_Responding(pid_t pid);


#ifdef FUTURE		
		int		Create_Thread()
		int		Kill_Thread()
#endif


		
	private:		
		int		HandleSigCommand(int command, Stream* stream);
		int		HandleReq(int socki);
		int		HandleSig(int command, int sig);
		void	Inherit( ReliSock* &rsock, SafeSock* &ssock );  // called in main()
		int		HandleProcessExitCommand(int command, Stream* stream);
		int		HandleProcessExit(pid_t pid, int exit_status);
		int		HandleDC_SIGCHLD(int sig);

		int		Register_Command(int command, char *com_descip, CommandHandler handler, 
					CommandHandlercpp handlercpp, char *handler_descrip, Service* s, 
					DCpermission perm, int dprintf_flag, int is_cpp);
		int		Register_Signal(int sig, char *sig_descip, SignalHandler handler, 
					SignalHandlercpp handlercpp, char *handler_descrip, Service* s, 
					DCpermission perm, int is_cpp);
		int		Register_Socket(Stream* iosock, char *iosock_descrip, SocketHandler handler, 
					SocketHandlercpp handlercpp, char *handler_descrip, Service* s, 
					DCpermission perm, int is_cpp);
		int		Register_Reaper(int rid, char *reap_descip, ReaperHandler handler, 
					ReaperHandlercpp handlercpp, char *handler_descrip, Service* s, 
					int is_cpp);


		struct CommandEnt
		{
		    int				num;
		    CommandHandler	handler;
			CommandHandlercpp	handlercpp;
			int				is_cpp;
			DCpermission	perm;
			Service*		service; 
			char*			command_descrip;
			char*			handler_descrip;
			void*			data_ptr;
			int				dprintf_flag;
		};
		void				DumpCommandTable(int, const char* = NULL);
		int					maxCommand;		// max number of command handlers
		int					nCommand;		// number of command handlers used
		CommandEnt*			comTable;		// command table

		struct SignalEnt 
		{
			int				num;
		    SignalHandler	handler;
			SignalHandlercpp	handlercpp;
			int				is_cpp;
			DCpermission	perm;
			Service*		service; 
			int				is_blocked;
			// Note: is_pending must be volatile because it could be set inside
			// of a Unix asynchronous signal handler (such as SIGCHLD).
			volatile int	is_pending;	
			char*			sig_descrip;
			char*			handler_descrip;
			void*			data_ptr;
		};
		void				DumpSigTable(int, const char* = NULL);
		int					maxSig;		// max number of signal handlers
		int					nSig;		// number of signal handlers used
		SignalEnt*			sigTable;		// signal table
		volatile int		sent_signal;	// TRUE if a signal handler sends a signal

		struct SockEnt
		{
		    Stream*			iosock;
			SOCKET			sockd;
		    SocketHandler	handler;
			SocketHandlercpp	handlercpp;
			int				is_cpp;
			DCpermission	perm;
			Service*		service; 
			char*			iosock_descrip;
			char*			handler_descrip;
			void*			data_ptr;
		};
		void				DumpSocketTable(int, const char* = NULL);
		int					maxSocket;		// number of socket handlers to start with
		int					nSock;		// number of socket handlers used
		ExtArray<SockEnt>	*sockTable;		// socket table; grows dynamically if needed
		int					initial_command_sock;  

		struct ReapEnt
		{
		    int				num;
		    ReaperHandler	handler;
			ReaperHandlercpp	handlercpp;
			int				is_cpp;
			Service*		service; 
			char*			reap_descrip;
			char*			handler_descrip;
			void*			data_ptr;
		};
		void				DumpReapTable(int, const char* = NULL);
		int					maxReap;		// max number of reaper handlers
		int					nReap;			// number of reaper handlers used
		ReapEnt*			reapTable;		// reaper table

		struct PidEntry
		{
			pid_t pid;
			int new_process_group;
#ifdef WIN32
			HANDLE hProcess;
			HANDLE hThread;
			DWORD tid;
			HWND hWnd;
#endif
			char sinful_string[28];
			char parent_sinful_string[28];
			int is_local;
			int parent_is_local;
			int	reaper_id;
			int hung_tid;	// Timer to detect hung processes
			int was_not_responding;
		};
		typedef HashTable <pid_t, PidEntry *> PidHashTable;
		PidHashTable* pidTable;
		pid_t mypid;
		pid_t ppid;
		int Send_WM_CLOSE(pid_t pid);

#ifdef WIN32
		// note: as of WinNT 4.0, MAXIMUM_WAIT_OBJECTS == 64
		struct PidWatcherEntry
		{
			PidEntry* pidentries[MAXIMUM_WAIT_OBJECTS - 1];
			HANDLE event;
			HANDLE hThread;
			CRITICAL_SECTION crit_section;
			int nEntries;
		};

		List<PidWatcherEntry> PidWatcherList;

		int					WatchPid(PidEntry *pidentry);
#endif
			
		static				TimerManager t;
		void				DumpTimerList(int, char* = NULL);

		IpVerify			ipverify;	

		void				free_descrip(char *p) { if(p &&  p != EMPTY_DESCRIP) free(p); }
	
		fd_set				readfds; 

		struct in_addr		negotiator_sin_addr;	// used by Verify method

#ifdef WIN32
		DWORD	dcmainThreadId;		// the thread id of the thread running the main daemon core
		CSysinfo ntsysinfo;		// class to do undocumented NT process management
		int SetFDInheritFlag(int fd, int flag);	// set files inheritable or not on NT
#endif

#ifndef WIN32
		int async_pipe[2];  // 0 for reading, 1 for writing
		volatile int async_sigs_unblocked;
#endif

		Stream *inheritedSocks[MAX_SOCKS_INHERITED+1];

		// methods to detect and kill hung processes
		int HandleChildAliveCommand(int command, Stream* stream);
		int HungChildTimeout();
		int SendAliveToParent();
		unsigned int max_hang_time;
		int send_child_alive_timer;

		// these need to be in thread local storage someday
		void **curr_dataptr;
		void **curr_regdataptr;
		int inServiceCommandSocket_flag;
		// end of thread local storage
		
#ifdef WIN32
	static char *ParseEnvArgsString(char *env, char *inheritbuf);
#else
	static char **ParseEnvArgsString(char *env, bool env);
#endif
};

#ifndef _NO_EXTERN_DAEMON_CORE

	// Function to call when you want your daemon to really exit.
	// This allows DaemonCore a chance to clean up.
extern void DC_Exit( int status );	

extern DaemonCore* daemonCore;
#endif

#define MAX_INHERIT_SOCKS 10
#define _INHERITBUF_MAXSIZE 500


#endif /* _CONDOR_DAEMON_CORE_H_ */
