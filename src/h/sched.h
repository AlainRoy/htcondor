/* 
** Copyright 1986, 1987, 1988, 1989, 1990, 1991 by the Condor Design Team
** 
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted,
** provided that the above copyright notice appear in all copies and that
** both that copyright notice and this permission notice appear in
** supporting documentation, and that the names of the University of
** Wisconsin and the Condor Design Team not be used in advertising or
** publicity pertaining to distribution of the software without specific,
** written prior permission.  The University of Wisconsin and the Condor
** Design Team make no representations about the suitability of this
** software for any purpose.  It is provided "as is" without express
** or implied warranty.
** 
** THE UNIVERSITY OF WISCONSIN AND THE CONDOR DESIGN TEAM DISCLAIM ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE UNIVERSITY OF
** WISCONSIN OR THE CONDOR DESIGN TEAM BE LIABLE FOR ANY SPECIAL, INDIRECT
** OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
** OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
** OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
** OR PERFORMANCE OF THIS SOFTWARE.
** 
** Authors:  Allan Bricker and Michael J. Litzkow,
** 	         University of Wisconsin, Computer Sciences Dept.
** 
*/ 

#ifndef _SCHED_H_
#define _SCHED_H_

#if defined(HPUX9)
#include "fake_flock.h"
#endif

typedef struct {
	int		p_disk_avail;
	char	*p_host;
} PERM;

typedef struct {
	int		s_users;
	int		s_wanting;
	int		s_running;
	int		s_state;
	int		s_disk_avail;
	int		s_disk_wanted;
} STATUS;

typedef struct {
	int		port1;
	int		port2;
} PORTS;

#ifdef JUNK
typedef struct {
	int		c_cmd;
} COMMAND;

typedef struct
{
	time_t	when;
	int		state;
} STATE_CHANGE;

typedef struct {
	int		user_time;
	int		ru_time;
	int		accept_time;
	int		n_changes;
	STATE_CHANGE	*changes;
} STATISTICS;

typedef struct {
	int		add_flags;
	int		del_flags;
} FLAGS;
#endif /* JUNK */

typedef struct {
	int		pid;
	int		job_id;
	int		umbilical;
} SHADOW_REC;

#if !defined(WIN32)	// no time_t in WIN32

typedef struct {
	int		clusterId;
	int		procId;
	char*	host;
	char*  	owner;
	int		q_date;
	time_t	finishTime;
	time_t	cpuTime;
} COMPLETED_JOB_INFO;

typedef struct {
	char*       capability;
	char*       server;
} FIN_MATCHES;

typedef struct {
	int		clusterID;
	int		procID;
	time_t	startTime;
	char*	host;
} RUNNING_JOB_INFO;

#endif // if !defined(WIN32)

typedef struct {        /* record sent by startd to shadow */
	int		version_num;/* always negative */
	PORTS	ports;
	int     ip_addr;    /* internet addressof executing machine */
	char*   server_name;/* name of executing machine */
} StartdRec;
	/* Startd version numbers : always negative  */
#define VERSION_FOR_FLOCK   -1

	/* Scheduler Commands */
/*
**	Scheduler version number
*/
#define SCHED_VERS			400
#define ALT_STARTER_BASE 70

/*
**	In the following definitions 'FRGN' does not
**	stand for "friggin'"...
*/
#define CONTINUE_FRGN_JOB	(SCHED_VERS+1)
#define SUSPEND_FRGN_JOB	(SCHED_VERS+2)
#define CKPT_FRGN_JOB		(SCHED_VERS+3)
#define KILL_FRGN_JOB		(SCHED_VERS+4)

#define LOCAL_STATUS		(SCHED_VERS+5)
#define LOCAL_STATISTICS	(SCHED_VERS+6)

#define PERMISSION			(SCHED_VERS+7)
#define SET_DEBUG_FLAGS		(SCHED_VERS+8)
#define PREEMPT_LOCAL_JOBS	(SCHED_VERS+9)

#define RM_LOCAL_JOB		(SCHED_VERS+10)
#define START_FRGN_JOB		(SCHED_VERS+11)

#define AVAILABILITY		(SCHED_VERS+12)		/* Not used */
#define NUM_FRGN_JOBS		(SCHED_VERS+13)
#define STARTD_INFO			(SCHED_VERS+14)
#define SCHEDD_INFO			(SCHED_VERS+15)
#define NEGOTIATE			(SCHED_VERS+16)
#define SEND_JOB_INFO		(SCHED_VERS+17)
#define NO_MORE_JOBS		(SCHED_VERS+18)
#define JOB_INFO			(SCHED_VERS+19)
#define GIVE_STATUS			(SCHED_VERS+20)
#define RESCHEDULE			(SCHED_VERS+21)
#define PING				(SCHED_VERS+22)
#define NEGOTIATOR_INFO		(SCHED_VERS+23)
#define GIVE_STATUS_LINES	(SCHED_VERS+24)
#define END_NEGOTIATE		(SCHED_VERS+25)
#define REJECTED			(SCHED_VERS+26)
#define X_EVENT_NOTIFICATION (SCHED_VERS+27)
#define RECONFIG 			(SCHED_VERS+28)
#define GET_HISTORY              (SCHED_VERS+29)
#define UNLINK_HISTORY_FILE      (SCHED_VERS+30)
#define UNLINK_HISTORY_FILE_DONE (SCHED_VERS+31)
#define DO_NOT_UNLINK_HISTORY_FILE      (SCHED_VERS+32)
#define SEND_ALL_JOBS		(SCHED_VERS+33)
#define SEND_ALL_JOBS_PRIO	(SCHED_VERS+34)
#define REQ_NEW_PROC		(SCHED_VERS+35)
#define PCKPT_FRGN_JOB                (SCHED_VERS+36)
#define SEND_RUNNING_JOBS     (SCHED_VERS+37)
#define CHECK_CAPABILITY    (SCHED_VERS+38)
#define GIVE_PRIORITY		(SCHED_VERS+39)
#define	MATCH_INFO			(SCHED_VERS+40)
#define	ALIVE				(SCHED_VERS+41)
#define	REQUEST_SERVICE		(SCHED_VERS+42)
#define	RELINQUISH_SERVICE	(SCHED_VERS+43)
#define	VACATE_SERVICE		(SCHED_VERS+44)
#define PRIORITY_INFO       (SCHED_VERS+45)     /* negotiator to accountant */

#include "condor_network.h"	/* for port numbers */

#define XDR_BLOCKSIZ (1024*4)
#define DEFAULT_MEMORY 3

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef MATCH
#define MATCH 0		/* "Equality" return for strcmp() */
#endif

#ifndef ACCEPTED
#define ACCEPTED 1
#endif

#ifndef REJECTED
#define REJECTED 0
#endif

#define OK		TRUE
#define NOT_OK	FALSE

#include "condor_mach_status.h"

#define LOCK_JOB_QUEUE(q,m) GuardQueue(q,m,__FILE__,__LINE__)
#define CLOSE_JOB_QUEUE(q) GuardQueue(q,LOCK_UN,__FILE__,__LINE__)

/* new stuff for capability management */
#define SIZE_OF_CAPABILITY_STRING 11
#define SIZE_OF_FINAL_STRING 40					/* ipaddr:port + capability */

#define	EXITSTATUS_OK		0					/* exit status of the agent */
#define EXITSTATUS_NOTOK	1
#define EXITSTATUS_FAIL		2

#define	MATCH_ACTIVE		1					/* match record status */
#define	NOTMATCHED			0

#endif
