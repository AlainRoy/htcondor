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
	killkids( int pid) 
	Author: Todd Tannenbaum
	
	pass killkids a process id, and killkids will kill all of the
	descendents (children, grand-children, great-grand, etc) of that
	process.  it calls /bin/ps; this is lame, but it is portable.
	if you have a "berkeley" style ps (such as SunOS 4.x or BSD), then
	you should #define BERKELEY_PS somewhere.
*/

#include "condor_common.h"
#include "condor_debug.h"
#include "condor_uid.h"

#define MAX_NUM_PIDS 2000

#if defined(SUNOS41) || defined(LINUX)
#define BERKELEY_PS
#endif

#if defined(BERKELEY_PS)
#define PS_CMD "/bin/ps -jax"
#elif defined(ULTRIX42) || defined(ULTRIX43)
#define PS_CMD "/bin/ps -axl"
#else
#define PS_CMD "/bin/ps -ef "
#endif 

typedef struct mypstree {
	pid_t pid;
	unsigned int left,right,child;
} MYPSTREE;
	
static MYPSTREE mypstree[MAX_NUM_PIDS];
static treei = 1;

static
int
treescan(pid_t findpid)
{
	int i = 1;

	while ( mypstree[i].pid != 0 ) {
		if ( findpid < mypstree[i].pid ) {
			if ( mypstree[i].left > 0 )
				i = mypstree[i].left;
			else {
				mypstree[i].left = ++treei;
				if ( treei == MAX_NUM_PIDS )
					treei--;
				i = treei;
				break;
			}
		} else 
		if ( findpid > mypstree[i].pid ) {
			if ( mypstree[i].right > 0 )
				i = mypstree[i].right;
			else {
				mypstree[i].right = ++treei;
				if ( treei == MAX_NUM_PIDS )
					treei--;
				i = treei;
				break;
			}
		} else {
			break;
		}
	}
	mypstree[i].pid = findpid;
	return(i);
}
		
void
killkids(pid_t inpid, int sig)
{
   FILE *ps;
   char line[250];
   pid_t pid, ppid;
   unsigned int parent,child,temp;
   priv_state	priv;

   treei = 1;
   memset(mypstree,0,sizeof(MYPSTREE)*MAX_NUM_PIDS);

   ps = popen(PS_CMD,"r");
   if ( ps == NULL )
      exit(-1);

   fgets(line,249,ps);  /* skip the column header line */

   while ( fgets(line,249,ps) != NULL ) {
#if defined(BERKELEY_PS)
	if ( sizeof(pid_t) == sizeof(long) )
		sscanf(line,"%ld %ld ",&ppid,&pid);
	else
		sscanf(line,"%d %d ",&ppid,&pid);
#elif defined(ULTRIX42) || defined(ULTRIX43)
	if ( sizeof(pid_t) == sizeof(long) )
		sscanf(line,"%*s %*s %ld %ld ",&ppid,&pid);
	else
		sscanf(line,"%*s %*s %d %d ",&ppid,&pid);
#else
	if ( sizeof(pid_t) == sizeof(long) )
		sscanf(line,"%*s %ld %ld ",&pid,&ppid);
	else
		sscanf(line,"%*s %d %d ",&pid,&ppid);
#endif
	if ( (ppid == 0) || (ppid == 1) )
		continue;
	child = treescan(pid);
	parent = treescan(ppid);

	if ( pid != inpid && mypstree[parent].child != child ) {
		temp = mypstree[parent].child;
		mypstree[parent].child = child;
		while ( mypstree[child].child != 0 ) {
			child = mypstree[child].child;
		}
		mypstree[child].child = temp;
	}
   }

        /* don't use 'pclose()' here, it does its own wait, and messes
           with our handling of SIGCHLD! */
        /* except on HPUX pclose() is both safe & required - Todd */
#if   defined(HPUX) || defined(Solaris) 
   pclose(ps);
#else
   (void)fclose(ps);
#endif
   
   priv = set_root_priv();

	temp = treescan(inpid);
	if ( mypstree[temp].child != 0 ) {
		temp = mypstree[temp].child;
		while ( mypstree[temp].pid > 0 ) {

    		if( sig != SIGCONT ) {
				dprintf( D_ALWAYS, "About to kill pid %d with signal %d\n", 
						 mypstree[temp].pid, SIGCONT );
		         kill( mypstree[temp].pid, SIGCONT );
			} 
			dprintf( D_ALWAYS, "About to kill pid %d with signal %d\n", 
					 mypstree[temp].pid, sig );
   		    kill( mypstree[temp].pid, sig );

            temp = mypstree[temp].child;
		}
	}
	set_priv( priv );
}
