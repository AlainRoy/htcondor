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
    This file defines the ResAttributes class which stores various
	attributes about a given resource such as load average, available
	swap space, disk space, etc.

   	Written 10/9/97 by Derek Wright <wright@cs.wisc.edu>
*/

#ifndef _RES_ATTRIBUTES_H
#define _RES_ATTRIBUTES_H

#if !defined(WIN32)
#include "afs.h"
#endif

typedef char amask_t;

const amask_t A_PUBLIC	= 1;
const amask_t A_PRIVATE	= 2;
const amask_t A_STATIC	= 4;
const amask_t A_TIMEOUT	= 8;
const amask_t A_UPDATE	= 16;
const amask_t A_SHARED	= 32;
const amask_t A_SUMMED	= 64;
const amask_t A_ALL		= (A_UPDATE | A_TIMEOUT | A_STATIC | A_SHARED | A_SUMMED);

#define IS_PUBLIC(mask)		((mask) & A_PUBLIC)
#define IS_PRIVATE(mask)	((mask) & A_PRIVATE)
#define IS_STATIC(mask)		((mask) & A_STATIC)
#define IS_TIMEOUT(mask)	((mask) & A_TIMEOUT)
#define IS_UPDATE(mask)		((mask) & A_UPDATE)
#define IS_SHARED(mask)		((mask) & A_SHARED)
#define IS_SUMMED(mask)		((mask) & A_SUMMED)
#define IS_ALL(mask)		((mask) & A_ALL)


// Machine-wide attributes.  
class MachAttributes
{
public:
	MachAttributes();
	~MachAttributes();

	void init();

	void publish( ClassAd*, amask_t );  // Publish desired info to given CA
	void compute( amask_t );			  // Actually recompute desired stats

		// Compute kflops and mips on the given resource
	void benchmark( Resource*, int force = 0 );	

		// Functions to return the value of shared attributes
	int				num_cpus()	{ return m_num_cpus; };
	int				phys_mem()	{ return m_phys_mem; };
	unsigned long   virt_mem()	{ return m_virt_mem; };
	unsigned long   disk()		{ return m_disk; };

private:
		// Dynamic info
	float			m_load;
	float			m_condor_load;
	unsigned long   m_virt_mem;
	unsigned long   m_disk;
	time_t			m_idle;
	time_t			m_console_idle;
	int				m_mips;
	int				m_kflops;
	int				m_last_benchmark;   // Last time we computed benchmarks
	int				m_clock_day;
	int				m_clock_min;
		// Static info
	int				m_num_cpus;
	int				m_phys_mem;
	char*			m_arch;
	char*			m_opsys;
	char*			m_uid_domain;
	char*			m_filesystem_domain;
#if !defined(WIN32)
	AFS_Info*		m_afs_info;
#endif
};	


// CPU-specific attributes.  
class CpuAttributes
{
public:
	CpuAttributes( MachAttributes*, float, float, float );

	void attach( Resource* );	// Attach to the given Resource

	void publish( ClassAd*, amask_t );  // Publish desired info to given CA
	void compute( amask_t );			  // Actually recompute desired stats

	float condor_load() { return c_condor_load; };
	float compute_condor_load();

	void dprintf( int, char*, ... );

private:
	Resource*	 	rip;
	MachAttributes*	map;

		// Dynamic info
	float			c_condor_load;
	unsigned long   c_virt_mem;
	unsigned long   c_disk;

		// Static info
	int				c_phys_mem;
	int				c_num_cpus;

		// These hold the percentages of shared, dynamic resources
		// that are allocated to this CPU.
	float			c_phys_mem_percent;
	float			c_virt_mem_percent;
	float			c_disk_percent;
};	


void deal_with_benchmarks( Resource* );

#endif _RES_ATTRIBUTES_H

