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

#include "condor_common.h"
#include "condor_debug.h"
#include "sysapi.h"
#include "sysapi_externs.h"
 
#if defined(HPUX)

#include <sys/pstat.h>

int
sysapi_phys_memory_raw(void)
{
	struct pst_static s;
	unsigned long pages, pagesz;
	double size;
						   
	sysapi_internal_reconfig();
	if (pstat_getstatic(&s, sizeof(s), (size_t)1, 0) != -1) {
		pages = s.physical_memory;
		pagesz = s.page_size >> 10;
		size = (double)pages * (double)pagesz;
		size /= 1024.0;

		if (size > INT_MAX){
			return INT_MAX;
		}
		return (int)size;
	} else {
		return -1;
	}
}

#elif defined(IRIX62) || defined(IRIX65)

#include <unistd.h>
#include <sys/types.h>
#include <sys/sysmp.h>

int
sysapi_phys_memory_raw(void)
{
	struct rminfo rmstruct;
	long pages, pagesz;
	double size;

	sysapi_internal_reconfig();
	pagesz = (sysconf(_SC_PAGESIZE) >> 10);		// We want kbytes.
	
	if( (sysmp(MP_SAGET,MPSA_RMINFO,&rmstruct,sizeof(rmstruct)) < 0) ||
		(pagesz == -1) ) { 
		return -1;
	}
		/* Correct what appears to be some kind of rounding error */
	if( rmstruct.physmem % 2 ) {
		pages = rmstruct.physmem + 1;
	} else {
		pages = rmstruct.physmem;
	}

	/* Return the answer in megs */
	size = (double)pages * (double)pagesz;
	size /= 1024.0;
	if (size > INT_MAX){
		return INT_MAX;
	}
	return (int)size;
}

#elif defined(Solaris) 

/*
 * This works for Solaris >= 2.3
 */
#include <unistd.h>

int
sysapi_phys_memory_raw(void)
{
	long pages, pagesz;
	double hack, size;

	sysapi_internal_reconfig();
	pages = sysconf(_SC_PHYS_PAGES);
	pagesz = (sysconf(_SC_PAGESIZE) >> 10);		// We want kbytes.

	if (pages == -1 || pagesz == -1 ) {
		return -1;
	}

#if defined(X86)
		/* 
		   This is super-ugly.  For some reason, Intel Solaris seems
		   to have some kind of rounding error for reporting memory.
		   These values just came from a little trail and error and
		   seem to work pretty well.  -Derek Wright (1/29/98)
 	    */
	hack = (double)pages * (double)pagesz;
	
	/* I don't know if this divisor is right, but it'll do for now.
		Keller 05/20/99 */
	if ((hack / 1024.0) > INT_MAX)
	{
		return INT_MAX;
	}

	if( hack > 260000 ) {
		return (int) (hack / 1023.0);		
	} else if( hack > 130000 ) {
		return (int) (hack / 1020.0);
	} else if( hack > 65000 ) {
		return (int) (hack / 1010.0);
	} else {
		return (int) (hack / 1000.0);
	}
#else
	size = (double)pages * (double)pagesz;
	size /= 1024.0;

	if (size > INT_MAX) {
		return INT_MAX;
	}
	return (int)size;
#endif
}

#elif defined(LINUX)
#include <stdio.h>

int 
sysapi_phys_memory_raw(void) 
{	

	FILE	*proc;
	double	phys_mem;
	char	tmp_c[20];
	char	c;

	sysapi_internal_reconfig();
	proc=fopen("/proc/meminfo","r");
	if(!proc) {
		return -1;
	}

	  /*
	  // The /proc/meminfo looks something like this:

	  //       total:    used:    free:  shared: buffers:  cached:
	  //Mem:  19578880 19374080   204800  7671808  1191936  8253440
	  //Swap: 42831872  8368128 34463744
	  //MemTotal:     19120 kB
	  //MemFree:        200 kB
	  //MemShared:     7492 kB
	  //Buffers:       1164 kB
	  //Cached:        8060 kB
	  //SwapTotal:    41828 kB
	  //SwapFree:     33656 kB
	  */	  
	while((c=fgetc(proc))!='\n');
	fscanf(proc, "%s %lf", tmp_c, &phys_mem);
	fclose(proc);

	phys_mem /= (1024*1024);

	if (phys_mem > INT_MAX) {
		return INT_MAX;
	}

	return (int)phys_mem;
}

#elif defined(WIN32)

int
sysapi_phys_memory_raw(void)
{
	MEMORYSTATUS status;		
	sysapi_internal_reconfig();
	GlobalMemoryStatus(&status);
	return (int)( ceil(status.dwTotalPhys/(1024*1024)+.4 ) );
}

#elif defined(OSF1)


/* Need these two to avoid compiler warning since <sys/table.h>
   includes a stupid version of <net/if.h> that does forward decls of
   struct mbuf and struct rtentry for C++, but not C. -Derek 6/3/98 */
#include <sys/mbuf.h>
#include <net/route.h>

#include <sys/table.h>

int
sysapi_phys_memory_raw(void)
{
	struct tbl_pmemstats s;
	sysapi_internal_reconfig();
	if (table(TBL_PMEMSTATS,0,(void *)&s,1,sizeof(s)) < 0) {
		return -1;
	}
	return (int)(s.physmem/(1024*1024));
}

#elif defined(Darwin)
#include <sys/sysctl.h>
int
sysapi_phys_memory_raw(void)
{
	sysapi_internal_reconfig();
        int mib[2], physmem;
        size_t len;   
        mib[0] = CTL_HW;     
        mib[1] = HW_PHYSMEM;        
        len = sizeof(physmem);   
        sysctl(mib, 2, &physmem, &len, NULL, 0);   
	return physmem / ( 1024 * 1024);
}
#elif defined(AIX)
int
sysapi_phys_memory_raw(void)
{
	CLASS_SYMBOL cuat;
	struct CuAt mem_ent;
	struct CuAt *mret = NULL;
	sysapi_internal_reconfig();
	unsigned long memory_size = 0;
	char *path = NULL;

	/* will eventually use the ODMI to figure this out */
    if (odm_initialize() < 0)
    {
		/* This is quite terrible if it happens */
        dprintf(D_ALWAYS, 
			"sysapi_phys_memory_raw(): Could not initialize the ODM database: "
			"%d\n", odmerrno);
		return -1;
    }

	/* remember to free this memory just before I leave this function */
    path = odm_set_path("/etc/objrepos");
	if (path == (char*)-1) /* eewww */
	{
        dprintf(D_ALWAYS, "sysapi_phys_memory_raw(): Could not set class path! "
			"%d\n", odmerrno);
		return -1;
	}

	/* open up a predefined class symbol found in libcfg.a */
    cuat = odm_open_class(CuAt_CLASS);
    if (cuat == NULL)
    {
        dprintf(D_ALWAYS, "sysapi_phys_memory_raw(): Could not open CuAt! %d\n",
			odmerrno);
    	if (odm_terminate() < 0)
    	{
        	dprintf(D_ALWAYS, "Could not terminate using the ODM database: "
				"%d\n", odmerrno);
			free(path);
			return -1;
    	}
		free(path);
		return -1;
    }

    /* odm_get_list() is scary cause I can't tell if it is going to actually
        remove the entries from the ODM when it returns them to me or not.
        So I'm traversing the list in the safe way that I know how */

	/* get me the various memory entries that represent the full amount of 
		memory on the machine */
    mret = odm_get_obj(cuat, "name like mem? AND attribute='size'", 
		&mem_ent, ODM_FIRST);
    while(mret != NULL)
    {
		/* This value appears to be in Megabytes. */
		memory_size += atoi(mem_ent.value);

        mret = odm_get_obj(cuat, NULL, &mem_ent, ODM_NEXT);
    }

    if (odm_close_class(cuat) < 0)
    {
        dprintf(D_ALWAYS, "Could not close CuAt in the ODM database: %d\n",
			odmerrno);
		free(path);
		return -1;
    }

    if (odm_terminate() < 0)
    {
        dprintf(D_ALWAYS, "Could not terminate using the ODM database: %d\n",
			odmerrno);
		free(path);
		return -1;
    }

	free(path);

	return (int)memory_size;
}
#else
#error "sysapi.h: Please define a sysapi_phys_memory_raw() for this platform!"
#endif


/* This is the cooked version of sysapi_phys_memory_raw(). Where as the raw
	function gives you the raw number, this function can process the number
	for you for some reason and return that instead.
*/
int
sysapi_phys_memory(void)
{
	int mem;
	sysapi_internal_reconfig();
	mem = _sysapi_memory;
	if (!_sysapi_memory) {
		mem = sysapi_phys_memory_raw();
	}
	if (mem < 0) return mem;
	mem -= _sysapi_reserve_memory;
	if (mem < 0) return 0;
	return mem;
}
