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

#ifndef CONDOR_PROC_INCLUDED
#define CONDOR_PROC_INCLUDED

#include "condor_universe.h"
#include "condor_header_features.h"

#define NEW_PROC 1

typedef struct {
	int		cluster;
	int		proc;
} PROC_ID;

typedef struct {
	int		writer;	/* index into cmd array */
	int		reader;	/* index into cmd array */
	char	*name;	/* for named pipes, NULL otherwise */
} P_DESC;

#if !defined(WIN32) // hopefully we won't need PROC structures in V6 so don't bother porting to NT
/*
** Version 3
*/
typedef struct {
	int				version_num;		/* version of this structure */
	PROC_ID			id;					/* job id */
	int				universe;			/* STANDARD, PIPE, LINDA, PVM, etc */
	int				checkpoint;			/* Whether checkpointing is wanted */
	int				remote_syscalls;	/* Whether to provide remote syscalls */
	char			*owner;				/* login of person submitting job */
	int				q_date;				/* UNIX time job was submitted */
	int				completion_date;	/* UNIX time job was completed */
	int				status;				/* Running, unexpanded, completed, .. */
	int				prio;				/* Job priority */
	int				notification;		/* Notification options */
	int				image_size;			/* Size of the virtual image in K */
	char			*env;				/* environment */

		/* Number of commands and per/command items.  Each of these items
		** is declared as a pointer, but should be allocated as an array
		** at run time.
		*/
	int				n_cmds;				/* Number of executable files */
	char			**cmd;				/* Names of executable files */
	char			**args;				/* command line args */
	char			**in;				/* file for stdin */
	char			**out;				/* file for stdout */
	char			**err;				/* file for stderr */
	struct rusage	*remote_usage;		/* accumulated usage on remote hosts */
	int				*exit_status;		/* final exit status */

		/* Number of pipes and their descriptions.  Each of these items
		** is declared as a pointer, but should be allocated as an array
		** at run time.
		*/
	int				n_pipes;			/* Number of pipes */
	P_DESC			*pipe;				/* Descriptions of pipes */

	/* These specify the min and max number of machines needed for
	** a PVM job
	*/
	int				min_needed;
	int				max_needed;

	char			*rootdir;			/* Root directory for chroot() */
	char			*iwd;				/* Initial working directory   */
	char			*requirements;		/* job requirements */
	char			*preferences;		/* job preferences */
	struct rusage	local_usage;		/* accumulated usage by shadows */
	char			pad[66];			/* make at least as big as V2 proc */
#if defined(NEW_PROC)
} PROC;
#else
} FUTURE_PROC;
#endif

/*
** Version 2
*/
typedef struct {
	int				version_num;		/* version of this structure */
	PROC_ID			id;					/* job id */
	char			*owner;				/* login of person submitting job */
	int				q_date;				/* UNIX time job was submitted */
	int				completion_date;	/* UNIX time job was completed */
	int				status;				/* Running, unexpanded, completed, .. */
	int				prio;				/* Job priority */
	int				notification;		/* Notification options */
	int				image_size;			/* Size of the virtual image in K */
	char			*cmd;				/* a.out file */
	char			*args;				/* command line args */
	char			*env;				/* environment */
	char			*in;				/* file for stdin */
	char			*out;				/* file for stdout */
	char			*err;				/* file for stderr */
	char			*rootdir;			/* Root directory for chroot() */
	char			*iwd;				/* Initial working directory   */
	char			*requirements;		/* job requirements */
	char			*preferences;		/* job preferences */
	struct rusage	local_usage;		/* accumulated usage by shadows */
	struct rusage	remote_usage;		/* accumulated usage on remote hosts */
#if defined(NEW_PROC)
} V2_PROC;
#else
} PROC;
#endif

#if defined(NEW_PROC)
#define PROC_VERSION	3
typedef PROC V3_PROC;
typedef union {
	V2_PROC v2_proc;
	V3_PROC v3_proc;
} GENERIC_PROC;
#else
#define PROC_VERSION	2
#endif

typedef struct {
	PROC_ID			id;		
	char			*owner;
	int				q_date;
	int				status;
	int				prio;
	int				notification;		/* Notification options */
	char			*cmd;
	char			*args;
	char			*env;
	char			*in;
	char			*out;
	char			*err;
	char			*rootdir;			/* Root directory for chroot() */
	char			*iwd;				/* Initial working directory   */
	char			*requirements;
	char			*preferences;
	struct rusage	local_usage;
	struct rusage	remote_usage;
} OLD_PROC;

#endif

/*
**	Possible notification options
*/
#define NOTIFY_NEVER		0
#define NOTIFY_ALWAYS		1
#define	NOTIFY_COMPLETE		2
#define NOTIFY_ERROR		3

typedef struct {
	int		n_ids;
	int		array_len;
	int		next_id;
	int		id[1];	/* dummy, space for actual array to be malloc'd as needed */
} CLUSTER_LIST;

#define READER	1
#define WRITER	2
#define	UNLOCK	8

/*
** Warning!  Keep these consistent with the strings defined below
** in "JobStatusNames".
*/
#define UNEXPANDED	0
#define IDLE		1
#define RUNNING		2
#define REMOVED		3
#define COMPLETED	4
#define	HELD		5
#define SUBMISSION_ERR	6

#ifdef INCLUDE_STATUS_NAME_ARRAY
char    *JobStatusNames[] = {
    "UNEXPANDED",
    "IDLE",
    "RUNNING",
    "REMOVED",
    "COMPLETED",
	"HELD",
    "SUBMISSION_ERR",
};
#else
extern char *JobStatusNames[];
#endif

BEGIN_C_DECLS

PROC_ID getProcByString( const char* str );

END_C_DECLS

#define ICKPT -1

#endif /* CONDOR_PROC_INCLUDED */
