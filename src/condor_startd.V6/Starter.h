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
  This file defines the Starter class, used by the startd to keep
  track of a resource's starter process.  

  Written 10/6/97 by Derek Wright <wright@cs.wisc.edu>
*/

#ifndef _CONDOR_STARTD_STARTER_H
#define _CONDOR_STARTD_STARTER_H

typedef struct jobstartinfo {
	char *ji_hname;
	int ji_sock1;
	int ji_sock2;
} start_info_t;

class Starter
{
public:
	Starter( Resource* );
	~Starter();

	void	dprintf( int, char* ... );

	char*	name() {return s_name;};
	void	setname(char*);
	int		kill(int);
	int		killpg(int);
	void	killkids(int);
	int		spawn(start_info_t*);
	void	exited();
	pid_t	pid() {return s_pid;};
	bool	active();
	pid_t*	pidfamily() {return s_pidfamily;};
	int		pidfamily_size() {return s_family_size;};
	void	recompute_pidfamily();

private:
	Resource*	rip;
	pid_t	s_pid;
	char*	s_name;
	int		reallykill(int, int);
	int		exec_starter(char*, char*, int, int);
	pid_t*	s_pidfamily;
	int		s_family_size;
	ProcFamily*	s_procfam;
};

#endif _CONDOR_STARTD_STARTER_H
