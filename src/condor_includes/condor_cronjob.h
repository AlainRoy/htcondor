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
#ifndef _CONDOR_CRONJOB_H
#define _CONDOR_CRONJOB_H

#include "condor_common.h"
#include "../condor_daemon_core.V6/condor_daemon_core.h"
#include "linebuffer.h"
#include "Queue.h"

// Cron's StdOut Line Buffer
class CronJobOut : public LineBuffer
{
  public:
	CronJobOut( class CondorCronJob *job );
	virtual ~CronJobOut( void ) {};
	virtual int Output( const char *buf, int len );
	int GetQueueSize( void );
	char *GetLineFromQueue( void );
	int FlushQueue( void );
  private:
	Queue<char *>		lineq;
	class CondorCronJob	*job;
};

// Cron's StdErr Line Buffer
class CronJobErr : public LineBuffer
{
  public:
	CronJobErr( class CondorCronJob *job );
	virtual ~CronJobErr( void ) {};
	virtual int Output( const char *buf, int len );
  private:
	class CondorCronJob	*job;
};

// Job's state
typedef enum {
	CRON_NOINIT,
	CRON_IDLE, 
	CRON_RUNNING,
	CRON_TERMSENT,
	CRON_KILLSENT,
	CRON_DEAD
} CronJobState;

// Job's "run" mode
typedef enum 
{ 
	CRON_EXIT_TIME,			// Timing from exit
	CRON_PERIODIC, 			// Run it periodically
	CRON_KILL,				// Job has been killed
	CRON_ILLEGAL
} CronJobMode;

// Notification events..
typedef enum {
	CONDOR_CRON_JOB_START,
	CONDOR_CRON_JOB_DIED,
} CondorCronEvent;

// Cronjob event
class CondorCronJob;
typedef int     (Service::*CronEventHandler)(CondorCronJob*,CondorCronEvent);

// Define a Condor 'Cron' job
class CondorCronJob : public Service
{
  public:
	CondorCronJob( const char *mgrName, const char *jobName );
	virtual ~CondorCronJob( );

	// Finish initialization
	virtual int Initialize( void );

	// Manipulate the job
	const char *GetName( void ) { return name.GetCStr(); };
	const char *GetPrefix( void ) { return prefix.GetCStr(); };
	const char *GetPath( void ) { return path.GetCStr(); };
	const char *GetArgs( void ) { return args.GetCStr(); };
	const char *GetEnv( void ) { return env.GetCStr(); };
	const char *GetCwd( void ) { return cwd.GetCStr(); };
	unsigned GetPeriod( void ) { return period; };

	// State information
	CronJobState GetState( void ) { return state; };
	bool IsRunning( void ) 
		{ return ( CRON_RUNNING == state ? true : false ); };
	bool IsIdle( void )
		{ return ( CRON_IDLE == state ? true : false ); };
	bool IsDead( void ) 
		{ return ( CRON_DEAD == state ? true : false ); };
	bool IsAlive( void ) 
		{ return ( (CRON_IDLE == state)||(CRON_DEAD == state)
				   ? false : true ); };

	int Reconfig( void );

	int SetName( const char *name );
	int SetPrefix( const char *prefix );
	int SetPath( const char *path );	
	int SetArgs( const char *args );
	int SetEnv( const char *env );
	int SetCwd( const char *cwd );
	int SetPeriod( CronJobMode mode, unsigned period );
	int SetKill( bool );
	int SetReconfig( bool );
	int AddPath( const char *path );	
	int AddArgs( const char *args );
	int AddEnv( const char *env );

	virtual int KillJob( bool );

	void Mark( void ) { marked = true; };
	void ClearMark( void ) { marked = false; };
	bool IsMarked( void ) { return marked; };

	int ProcessOutputQueue( void );
	virtual int ProcessOutput( const char *line );

	int SetEventHandler( CronEventHandler handler, Service *s );

  private:
	MyString		name;			// Logical name of the job
	MyString		prefix;			// Publishing prefix
	MyString		path;			// Path to the executable
	MyString		args;			// Arguments to pass it
	MyString		env;			// Environment variables
	MyString		cwd;			// Process's initial CWD
	unsigned		period;			// The configured period
	int				runTimer;		// It's DaemonCore "run" timerID
	CronJobMode		mode;			// Is is a periodic
	CronJobState	state;			// Is is currently running?
	int				pid;			// The process's PID
	int				stdOut;			// Process's stdout file descriptor
	int				stdErr;			// Process's stderr file descriptor
	int				childFds[3];	// Child process FDs
	int				reaperId;		// ID Of the child reaper
	CronJobOut		*stdOutBuf;		// Buffer for stdout
	CronJobErr		*stdErrBuf;		// Buffer for stderr
	bool			marked;			// Is this one marked?
	int				killTimer;		// Make sure it dies

	// Event handler stuff
	CronEventHandler	eventHandler;	// Handle cron events
	Service				*eventService;	// Associated service

	// Options
	bool			optKill;		// Kill the job if before next run?
	bool			optReconfig;	// Send the job a HUP for reconfig

	// Private methods; these can be replaced
	virtual int Schedule( void );
	virtual int RunJob( void );
	virtual int StartJob( void );
	virtual int KillHandler( void );
	virtual int StdoutHandler( int pipe );
	virtual int StderrHandler( int pipe );
	virtual int Reaper( int exitPid, int exitStatus );
	virtual int RunProcess( void );

	// No reason to replace these
	int OpenFds( void );
	int TodoRead( int, int );
	void CleanAll( void );
	void CleanFile( FILE **file );
	void CleanFd( int *fd );
	const char *StateString( void );
	const char *StateString( CronJobState state );

	// Timer maintainence
	int SetTimer( unsigned first, unsigned seconds );
	int KillTimer( unsigned seconds );

	// Debug / TODO
# if 0
	char	*TodoBuffer;
	int		TodoBufSize;
	int		TodoBufWrap;
	int		TodoBufOffset;
	int		TodoWriteNum;
	public:
	void	TodoWrite( void );
# endif
};

#endif /* _CONDOR_CRONJOB_H */
