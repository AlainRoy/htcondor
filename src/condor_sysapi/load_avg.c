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

/* This file contains each implementation of load average that we need. */

/* The essence of the format looks like this for each type of machine:
	#if defined(HPUX) 
		code
	#endif

	The ------- line separates the load average calculating code from a
	function that gets the load average through the unix command 'uptime'.
	It is conditionally compiled as some architectures need it and others don't
*/

/* header files everyone needs */
#include "condor_common.h"
#include "condor_debug.h"
#include "sysapi.h"

/* the cooked version */
float
sysapi_load_avg(void)
{
	sysapi_internal_reconfig();

	return sysapi_load_avg_raw();
}

#if defined(HPUX)
/** Nicely ask the system what the one minute load average is.
    For more info, see the pstat manpage and sys/pstat.h
*/

#include "condor_uid.h"

#include <sys/pstat.h>

float
sysapi_load_avg_raw(void)
{
  struct pst_dynamic d;
  	/* make numcpus static so we do not have to recompute
	 * numcpus every time the load average is requested.
	 * after all, the number of cpus is not going to change!
	 * we need to multiply the value the HPUX kerenel gives
	 * us by the number of CPUs, because on SMP HPUX the kernel
	 * "distributes" the load average across all CPUs.  But
	 * no other Unix does that, so our startd assumes otherwise.
	 * So we multiply by the number of CPUs so HPUX SMP load avg
	 * is reported the same way as other Unixes. -Todd
	 */
  static int numcpus = 0;  

  if ( numcpus == 0 ) {
    numcpus = sysapi_ncpus();
	if ( numcpus < 1 ) {
		numcpus = 1;
	}
  }

  sysapi_internal_reconfig();
  if ( pstat_getdynamic ( &d, sizeof(d), (size_t)1, 0) != -1 ) {
    return (d.psd_avg_1_min * numcpus);
  }
  else {
    return -1.0;
  }
}

#elif defined(IRIX53) || defined(IRIX62) || defined(IRIX65)

#include "condor_uid.h"

#include <sys/sysmp.h>
#include <sys/sysinfo.h>

static int KernelLookupFailed = 0;

static float kernel_load_avg();

extern float lookup_load_avg_via_uptime();

/*
** Where is FSCALE defined on IRIX ?  Apparently nowhere...
** The SGI folks apparently use a scaled integer, but where is the scale
** factor defined?  This is a wild guess that seems to work.
*/
#ifndef FSCALE
#define FSCALE 1000
#endif

float
sysapi_load_avg_raw(void)
{

	float val;

	sysapi_internal_reconfig();
	if (!KernelLookupFailed)
		val = kernel_load_avg();
	if (KernelLookupFailed)
		val = lookup_load_avg_via_uptime();

	dprintf( D_FULLDEBUG, "Load avg: %f\n", val );
	return val;
}


#undef RETURN
#define RETURN \
    dprintf( D_ALWAYS, "Getting load avg from uptime.\n" );\
	KernelLookupFailed = 1;\
	set_priv(priv);\
	return 0.0

float
kernel_load_avg(void)
{
	static int addr_sysmp = -1;
	static int Kmem = -1;
	priv_state priv;
	long avenrun[3];


	/* get root access if we can */
	priv = set_root_priv();

	errno = 0;
	if ( addr_sysmp < 0 ) {
		addr_sysmp = sysmp(MP_KERNADDR,MPKA_AVENRUN);
		if ( !addr_sysmp || errno ) {
			dprintf(D_ALWAYS,"sysmp(MP_KERNADDR) failed, errno=%d\n",errno);
			RETURN;
		}
	}

	if ( Kmem < 0 ) {
		if ( (Kmem=open("/dev/kmem",O_RDONLY,0)) < 0 ) {
			dprintf(D_ALWAYS,"open /dev/kmem failed, errno=%d\n",errno);
			RETURN;
		}
	}

	if ( lseek(Kmem,addr_sysmp,0) != addr_sysmp ) {
		dprintf(D_ALWAYS,"lseek(/dev/kmem,%d,0) failed, errno=%d\n",addr_sysmp,errno);
		RETURN;
	}

	if ( read(Kmem,(char *)avenrun,sizeof(avenrun)) != sizeof(avenrun) ) {
		dprintf(D_ALWAYS,"read on /dev/kmem failed, errno=%d\n",errno);
		RETURN;
	}

	/* Disable root access */
	set_priv(priv);

	return ( (float)avenrun[0] / FSCALE );
}

/* just adding get_k_vars to avoid runtime errors */
get_k_vars()
{
}

#elif defined(LINUX)

float
sysapi_load_avg_raw(void)
{
    FILE	*proc;
	struct utsname buf;
	int		major, minor, patch;
	float	short_avg, medium_avg, long_avg;

	sysapi_internal_reconfig();

	// Obtain the kernel version so that we know what /proc looks like..
	if( uname(&buf) < 0 )  return -1;
	sscanf(buf.release, "%d.%d.%d", &major, &minor, &patch);

	// /proc/loadavg looks like:

	// Kernel Version 2.0.0:
	// 0.03 0.03 0.09 2/42 15582

    proc=fopen("/proc/loadavg","r");
    if(!proc)
	return -1;

	switch(major) {
		case 1:
		case 2:
    		fscanf(proc, "%f %f %f", &short_avg, &medium_avg, &long_avg);
			break;

		default:
			dprintf(D_ALWAYS, "/proc format unknown for kernel version %d.%d.%d",
				major, minor, patch);
    		fclose(proc);
			return -1;
			break;
	}

    fclose(proc);

    dprintf(D_FULLDEBUG, "Load avg: %f %f %f\n", short_avg, medium_avg, long_avg);

	return short_avg;
}


void
get_k_vars() {}


#elif defined(OSF1)

#include <sys/table.h>

float
sysapi_load_avg_raw(void)
{
	struct tbl_loadavg	avg;
	float short_avg, medium_avg, long_avg;

	sysapi_internal_reconfig();

	if( table(TBL_LOADAVG,0,(char *)&avg,1,sizeof(avg)) < 0 ) {
		dprintf( D_ALWAYS, "Can't get load average info from kernel\n" );
		exit( 1 );
	}

	if( avg.tl_lscale ) {
		short_avg = avg.tl_avenrun.l[0] / (float)avg.tl_lscale;
		medium_avg = avg.tl_avenrun.l[1] / (float)avg.tl_lscale;
		long_avg = avg.tl_avenrun.l[2] / (float)avg.tl_lscale;
	} else {
		short_avg = avg.tl_avenrun.d[0];
		medium_avg = avg.tl_avenrun.d[1];
		long_avg = avg.tl_avenrun.d[2];
	}

	return short_avg;
}

#elif defined(Solaris)

#include "condor_uid.h"
#include <kstat.h>

static int KernelLookupFailed = 0;

double kstat_load_avg();
float lookup_load_avg_via_uptime();

float
sysapi_load_avg_raw(void)
{

	float val;

	sysapi_internal_reconfig();

	if (!KernelLookupFailed)
		val = (float)kstat_load_avg();
	if (KernelLookupFailed)
		val = lookup_load_avg_via_uptime();

	dprintf( D_FULLDEBUG, "Load avg: %f\n", val );
	return val;
}


#undef RETURN
#define RETURN \
    dprintf( D_ALWAYS, "Getting load avg from uptime.\n" );\
	KernelLookupFailed = 1;\
	return 0.0

double
kstat_load_avg(void)
{
	static kstat_ctl_t	*kc = NULL;		/* libkstat cookie */
	static kstat_t		*ksp = NULL;	/* kstat pointer */
	kstat_named_t 		*ksdp = NULL;  	/* kstat data pointer */

	if( ! kc ) {
		if( (kc = kstat_open()) == NULL ) {
			dprintf( D_ALWAYS, "kstat_open() failed, errno = %d\n", errno );
			RETURN;
		}
	}

	if( ! ksp ) {
		if( (ksp = kstat_lookup(kc, "unix", 0, "system_misc")) == NULL ) {
			dprintf( D_ALWAYS, "kstat_lookup() failed, errno = %d\n", errno );
			RETURN;
		}
	}

	if( kstat_read(kc, ksp, NULL) == -1 ) {
		dprintf( D_ALWAYS, "kstat_read() failed, errno = %d\n", errno );
		RETURN;
	}

	ksdp = (kstat_named_t *) kstat_data_lookup(ksp, "avenrun_1min");
	if( ksdp ) {
		return (double) ksdp->value.l / FSCALE;
	} else {
		dprintf( D_ALWAYS, "kstat_data_lookup() failed, errno = %d\n",
				 errno);
		RETURN;
	}		
}

/* just adding get_k_vars to avoid runtime errors */
get_k_vars()
{
}

#elif defined(WIN32)

/* 
** Load average on WIN32 is calculated by sampling the System
** Processor Queue Length and the % Processor Time of each processor
** in the system.  The first time it is called, calc_load_avg(),
** starts a thread which samples the counter periodically.  The sample
** interval is set by constants below.
*/

#include <stdio.h>
#include <winperf.h>
#include <pdh.h>
#include <pdhmsg.h>

#define SAMPLE_INTERVAL 1000		/* take a sample every 1000 ms */
#define NUM_SAMPLES 60000/SAMPLE_INTERVAL	/* num samples per 1 minute */

#undef TESTING

static CRITICAL_SECTION cs;		/* protects samples array */
static struct {
	double load;
	time_t sampletime;
} samples[NUM_SAMPLES];
static int ncpus;

static int WINAPI
sample_load(void *thr_data)
{
	HQUERY hQuery = NULL;
	HCOUNTER hCounterQueueLength, *hCounterCpuLoad;
	int nextsample = 0, i;
	PDH_STATUS pdhStatus;
	PDH_FMT_COUNTERVALUE counterval;
	long queuelen;
	double cpuload;
	char counterpath[35];

	EnterCriticalSection(&cs);
	for (i=0; i < NUM_SAMPLES; i++) {
		samples[i].load = 0;
		samples[i].sampletime = (time_t)0;
	}
	LeaveCriticalSection(&cs);

	pdhStatus = PdhOpenQuery(NULL, 0, &hQuery);
	if (pdhStatus != ERROR_SUCCESS) {
		/* dprintf(D_ALWAYS, "PdhOpenQuery returns 0x%x\n", 
			    (int)pdhStatus); */
		return 1;
	}
	pdhStatus = PdhAddCounter(hQuery, 
							  "\\System\\Processor Queue Length", 
							  0, &hCounterQueueLength);
	if (pdhStatus != ERROR_SUCCESS) {
		/* dprintf(D_ALWAYS, "PdhAddCounter returns 0x%x\n", 
						   (int)pdhStatus); */
		return 2;
	}
	hCounterCpuLoad = malloc(sizeof(HCOUNTER)*ncpus);
	for (i=0; i < ncpus; i++) {
		sprintf(counterpath, "\\Processor(%d)\\%% Processor Time", i);
		pdhStatus = PdhAddCounter(hQuery, counterpath, 0, 
								  hCounterCpuLoad+i);
		if (pdhStatus != ERROR_SUCCESS) {
			/* dprintf(D_ALWAYS, "PdhAddCounter returns 0x%x\n", 
							   (int)pdhStatus); */
			return 3;
		}
	}

	while (1) {

		pdhStatus = PdhCollectQueryData(hQuery);
		if (pdhStatus != ERROR_SUCCESS) {
			/* dprintf(D_ALWAYS, "PdhCollectQueryData returns 0x%x\n", 
							   (int)pdhStatus); */
			return 4;
		}

		pdhStatus = PdhGetFormattedCounterValue(hCounterQueueLength, 
												PDH_FMT_LONG,
												NULL, &counterval);
		if (pdhStatus != ERROR_SUCCESS) {
			/* dprintf(D_ALWAYS, "PdhGetFormattedCounterValue returns 0x%x\n",
							   (int)pdhStatus); */
			return 5;
		}
		queuelen = counterval.longValue;
		cpuload = 0.0;
		for (i=0; i < ncpus; i++) {
			pdhStatus = PdhGetFormattedCounterValue(hCounterCpuLoad[i], 
													PDH_FMT_DOUBLE,
													NULL, &counterval);
			if (pdhStatus != ERROR_SUCCESS) {
				/* dprintf(D_ALWAYS, "PdhGetFormattedCounterValue returns 0x%x\n",
								   (int)pdhStatus); */
				return 6;
			}
			cpuload += counterval.doubleValue/100.0;
		}

		EnterCriticalSection(&cs);
		/* 
		** Here is the code to simulate Unix style load average on Win32.
		** If the system is not fully utilized, the length of the processor
		** queue should be near 0.  When the system is fully utilized,
		** we must discount two items on the processor queue: the system
		** thread and the thread which we displaced to take our measurement.
		** If there are more than 2 items on the queue, we want to add this
		** to our load average to show the additional load on the system.
		*/
		if (queuelen > 2) {
			samples[nextsample].load = cpuload + queuelen - 2;
		} else {
			samples[nextsample].load = cpuload;
		}
		samples[nextsample].sampletime = time(NULL);
		LeaveCriticalSection(&cs);
		nextsample++;
		nextsample %= NUM_SAMPLES;

		Sleep(SAMPLE_INTERVAL);

	}

	return 0;	/* should never reach this point */
}

float
sysapi_load_avg_raw(void)
{
	static HANDLE threadHandle = NULL;
	static int threadID = -1;
	time_t currentTime;
	double totalLoad=0.0;
	int numSamples=0, i;

	sysapi_internal_reconfig();

	if (threadHandle == NULL) {
		ncpus = sysapi_ncpus();
		InitializeCriticalSection(&cs);
		threadHandle = CreateThread(NULL, 0, sample_load, 
									NULL, 0, &threadID);
		if (threadHandle == NULL) {
#ifndef TESTING
			dprintf(D_ALWAYS, "failed to create loadavg thread, errno = %d\n",
					GetLastError());
#endif
			return 0.0;
		}
		Sleep(SAMPLE_INTERVAL*5);	/* wait for ~5 samples */
	}

	currentTime = time(NULL);

	EnterCriticalSection(&cs);
	for (i=0; i < NUM_SAMPLES; i++) {
		/* if this sample occurred within the minute, then add to total */
		if ((currentTime-60) <= samples[i].sampletime) {
			totalLoad += samples[i].load;
			numSamples++;
		}
	}
	LeaveCriticalSection(&cs);

	if (numSamples == 0) {
#ifndef TESTING
		dprintf(D_ALWAYS, "no loadavg samples this minute, maybe thread died???\n");
#endif
		return 0.0;
	}

#ifndef TESTING
	dprintf(D_LOAD, "loadavg=%f with %d samples\n",
			((float)totalLoad)/((float)numSamples), numSamples);
#endif

	return ((float)totalLoad)/((float)numSamples);
}

#if defined(TESTING)
int main()
{
	while (1) {
		printf("%f\n", sysapi_load_avg());
		Sleep(5000);
	}

	return 0;
}
#endif

/* END WIN32 */
#endif /* #if defined(WIN32) */


/*----------------------------------------------------------------------*/
/* only include this helper function on these architectures */

#if defined(Solaris) || defined(IRIX53) || defined(IRIX62) || defined(IRIX65)

/*
 *  We will use uptime(1) to get the load average.  We will return the one
 *  minute load average by parsing its output.  This is fairly portable and
 *  doesn't require root permission.  Sample output from a mips-dec-ultrix4.3
 *
 *  example% /usr/ucb/uptime
 *    8:52pm  up 1 day, 22:28,  4 users,  load average: 0.23, 0.08, 0.01
 *
 *  The third last number is the required load average.
 */

#define DEFAULT_LOADAVG         0.10

static char            *uptime_path;

/*
 *  path_to_uptime
 *
 *  Check for executable uptime is /usr/ucb and /usr/bin in that order.
 *  If uptime is found in either of these, return the full path of uptime,
 *  e.g., /usr/ucb/uptime.  Otherwise return NULL.
 */

char *path_to_uptime(void)
{
        static char upt_path[16];

        if (access("/usr/ucb/uptime", X_OK) == 0)
        {
                strcpy(upt_path, "/usr/ucb/uptime");
                return upt_path;
    }
        else if (access("/usr/bin/uptime", X_OK) == 0)
        {
                strcpy(upt_path, "/usr/bin/uptime");
                return upt_path;
    }
        else if (access("/usr/bsd/uptime", X_OK) == 0)
        {
                strcpy(upt_path, "/usr/bsd/uptime");
                return upt_path;
    }
        else
                return NULL;
}

float
lookup_load_avg_via_uptime()
{
	
	float    loadavg;
	FILE *output_fp;
	int counter;
	char word[20];

	if (uptime_path == NULL) {
		uptime_path = path_to_uptime();
	}

	/*  We start uptime and pipe its output to ourselves.
	 *  Then we read word by word till we get "load average".  We read the
	 *  next number.  This is the number we want.
	 */
	if (uptime_path != NULL) {
		if ((output_fp = popen(uptime_path, "r")) == NULL) {
//			dprintf(D_ALWAYS, "popen(\"uptime\")");
			return DEFAULT_LOADAVG;
		}
		
		do { 
			if (fscanf(output_fp, "%s", word) == EOF) {
				dprintf(D_ALWAYS,"can't get \"average:\" from uptime\n");
				pclose(output_fp);
				return DEFAULT_LOADAVG;
			}
//			dprintf(D_FULLDEBUG, "%s ", word);
			
			if (strcmp(word, "average:") == 0) {
				/*
				 *  We are at the required position.  Read in the next
				 *  floating
				 *  point number.  That is the required average.
				 */
				if (fscanf(output_fp, "%f", &loadavg) != 1) {
					dprintf(D_ALWAYS, "can't read loadavg from uptime\n");
					pclose(output_fp);
					return DEFAULT_LOADAVG;
				} else {
//					dprintf(D_FULLDEBUG,"The loadavg is %f\n",loadavg);
				}
				
				/*
				 *  Some callers of this routine may have a SIGCHLD handler.
				 *  If this is so, calling pclose will interfere withthat.
				 *  We check if this is the case and use fclose instead.
				 *  -- Ajitk
				 */
				pclose(output_fp);
//				dprintf(D_FULLDEBUG, "\n");
				return loadavg;
			}
		} while (!feof(output_fp)); 
		
		/*
		 *  Reached EOF before getting at load average!  -- Ajitk
		 */
        
		pclose(output_fp);
        
		/*EXCEPT("uptime eof");*/
	}
	
	/* not reached */
	return DEFAULT_LOADAVG;
}

#endif /* #if defined(Solaris) || defined(IRIX53) || defined(IRIX62) || defined(IRIX(65) */

