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

#ifndef SYSAPI_H
#define SYSAPI_H


BEGIN_C_DECLS

/* How to get the physical memory on a machine, answer in Megs */
int sysapi_phys_memory_raw(void);
int sysapi_phys_memory(void);

/* How to get the free disk blocks from a full pathname, answer in KB */
int sysapi_disk_space_raw(const char *filename);
int sysapi_disk_space(const char *filename);

/* return the number of cpus there on a machine */
int sysapi_ncpus_raw(void);
int sysapi_ncpus(void);

/* calculate the number of mips the machine is. Even though this is a user
	thing and on platforms like NT it is done by hand, it goes in the sysapi.
	The reasoning is that you are still asking for a fundamental parameter
	from the machine, and even if there isn't OS support for it, it is
	still a lowlevel number. */
int sysapi_mips_raw(void);
int sysapi_mips(void);

/* as above, but for kflops */
int sysapi_kflops_raw(void);
int sysapi_kflops(void);

/* return the idle time on the machine in the arguments */
void sysapi_idle_time_raw(time_t *m_idle, time_t *m_console_idle);
void sysapi_idle_time(time_t *m_idle, time_t *m_console_idle);

/* some reconfigure functions that people need to know */
void sysapi_reconfig(void);
void sysapi_internal_reconfig(void);

/* if this is called, then the sysapi knows that a last_x_event has happend
	and records the time it happened, this is quite useful in idle_time.C */
void sysapi_last_xevent(void);

/* return the one minute load average on a machine */
float sysapi_load_avg_raw(void);
float sysapi_load_avg(void);

/* return information about the arch and operating system.
	The pointer returned points to a static buffer defined in the library,
	do not free it */
char* sysapi_condor_arch(void);
char* sysapi_uname_arch(void);
char* sysapi_opsys(void);

/* return information about how many 1K blocks of swap space there are.
	If there are more 1K blocks than INT_MAX, then INT_MAX is returned */
int sysapi_swap_space_raw(void);
int sysapi_swap_space(void);

/* these are functions that spit out data about the library, useful for
	testing or debugging purposes */
void sysapi_test_dump_all(void);
void sysapi_test_dump_internal_vars(void);
void sysapi_test_dump_functions(void);

END_C_DECLS

#endif
