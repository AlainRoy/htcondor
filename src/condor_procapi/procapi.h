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
#ifndef PROCAPI_H
#define PROCAPI_H

/* The following written by Mike Yoder */
//@author Mike Yoder

/* Here are the necessary #includes */
#include "condor_common.h"
#include "condor_uid.h"
#include "HashTable.h"
#include "extArray.h"

#ifndef WIN32 // all the below is for UNIX

#include <strings.h>       // sprintf, atol
#include <dirent.h>        // get /proc entries (directory stuff)
#include <ctype.h>         // isdigit
#include <errno.h>         // for perror
#include <fcntl.h>         // open()
#include <unistd.h>        // getpagesize()
#include <sys/types.h>     // various types needed.
#include <time.h>          // use of time() for process age. 

#if (!defined(HPUX) && !defined(CONDOR_DARWIN))     // neither of these are in hpux.

#if defined(Solaris26) || defined(Solaris27) || defined(Solaris28) || defined(Solaris29)
#include <procfs.h>        // /proc stuff for Solaris 2.6, 2.7, 2.8, 2.9
#else
#include <sys/procfs.h>    // /proc stuff for everything else and
#endif

#endif /* ! HPUX && CONDOR_DARWIN */

#ifdef HPUX                // hpux has to be different, of course.
#include <sys/param.h>     // used in pstat().
#include <sys/pstat.h>
#endif

#ifdef CONDOR_DARWIN
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/bootstrap.h>
#include <mach/mach_error.h>
#include <mach/mach_types.h>
#endif 

#ifdef OSF1                // this is for getting physical/available
#include <sys/table.h>     // memory in OSF1
#endif

#ifdef IRIX              // this is the include for memory info in Irix.
#include <sys/sysmp.h>
#endif

#else // It's WIN32...
// Warning: WIN32 stuff below.

typedef DWORD pid_t;

#define UNICODE 1
#define _UNICODE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <windows.h>
#include <winperf.h>
#include <tchar.h>

#define INITIAL_SIZE    40960L    // init. size for getting pDataBlock
#define EXTEND_SIZE	     4096L    // incremental addition to pDataBlock

//LPTSTR is a point to a null-terminated windows or Unicode string.
//TEXT() basically puts a string into unicode.
// Here are some Windows specific strings.
const LPTSTR NamesKey = 
      TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib");
const LPTSTR DefaultLangId = TEXT("009");  //english!
const LPTSTR Counters = TEXT("Counters");
const LPTSTR Help = TEXT("Help");
const LPTSTR LastHelp = TEXT("Last Help");
const LPTSTR LastCounter = TEXT("Last Counter");
const LPTSTR Slash = TEXT("\\");

struct Offset {       // There will be one instance of this structure in
	DWORD imgsize;      // the ProcAPI class - it will hold the offsets of
	DWORD rssize;       // the data that reside in the pDataBlock
	DWORD faults;
	DWORD pctcpu;       // this info is gotten by grabOffsets()
	DWORD utime;
	DWORD stime;
	DWORD elapsed;
	DWORD procid;
};


#endif // WIN32

/** This is the structure that is returned from the getProcInfo() 
    member of ProcAPI.  It is returned all at once so that multiple
    calls don't have to be made.  All OS's support the given information,
    WITH THE EXCEPTION OF MAJ/MIN FAULTS:
    <ul>
     <li> Linux, Irix, and Hpux all return a reasonable-looking number.
     <li> They are completely unsupported by OSF/1.
     <li> Solaris 2.5.1 and 2.6 *Sometimes* returns small number of major 
        faults, and I've only seen a 0 for minor faults.  The documentation
        claims that they are inexact values, anyway.
    </ul>
    
    In the case of a 'family' of pids ( given a pid, return info on that
    pid + all its children ), the values returned are a sum of the process'
    values.  The exception to this is 'age', which is the wall clock age of
    the oldest process.
*/
struct procInfo {

  /// the image size (aka virtual memory), in k.
  unsigned long imgsize;
  /// the resident set size, in k.  on WinNT, it is peak working set size.
  unsigned long rssize;       

  /** The number of minor page faults generated per second.  
      The definition of "minor" is "those which have not required 
      loading a memory page from disk"  Therefore, the page was
      still floating around somewhere in the free list.  */
  unsigned long minfault;

  /** The number of major page faults generated per second.
      The definition of "major" is "those which require loading
      a memory page from disk". */
  unsigned long majfault;

  /** percent of recent cpu time used by this process.  To maintain consistency
      between systems, this value must be sampled over time by hand.
      Here's how it works:  If this is the first reference to
      a pid, that pid's %cpu usage until that time is returned.  If the pid
      has been looked at before, the %cpu since the last reference is returned.
      So, if you look at a process every 10 seconds, you get the %cpu for
      those previous 10 seconds.<p>
  
      Note that this number is a PERCENT, i.e. range of 100.0 - 0.0, on
      a single processor machine.<p>
      
      On multiple processor machines, it works the same way.  However, say 
      you have a process with 2 threads.  If both of these threads are cpu 
      bound (and they each get their own processor), then you'll have a 
      usage of 200%.  For single threaded tasks, it's the same as above.

  */
  double cpuusage;

  /// time (secs) in user mode
  long user_time;
  /// time (secs) in system mode 
  long sys_time;
  /// seconds (wall clock) since process creation.
  long age; 

  /// this pid
  pid_t pid;
  /// parents pid.
  pid_t ppid;

  /// The time of birth of this pid
  long creation_time;

  /// pointer to next procInfo, if one exists.
  struct procInfo *next;

  // the owner of this process
  uid_t owner;
};

/// piPTR is typedef'ed as a pointer to a procInfo structure.
typedef struct procInfo * piPTR;

/* The pidlist struct is used for getting all the pids on a machine
   at once.  It is used by getpidlist() */

struct pidlist {
  pid_t pid;               // the pid of a process
  struct pidlist *next;    // the next pid.
};
typedef struct pidlist * pidlistPTR;

const int PHBUCKETS = 101;  // why 101?  Well...it's slightly greater than
                            // your average # of processes, and it's prime. 

/** procHashNode is used to hold information in the hashtable.  It is used
    to save the state of certain things that need to be sampled over time.
    For instance, the number of page faults is always given as a number 
    for the lifetime of a process.  I want to save this number and the
    time, and convert it into a page fault per second rate.  The other state
    want to save is the cpu uage times, for % cpu usage.
*/

struct procHashNode {
  /// Ctor
  procHashNode();
  /// the last time (secs) this data was retrieved.
  double lasttime;
  /// old cpu usage number (raw, not percent)
  double oldtime;
  /// the old value for cpu usage (the actual percent)
  double oldusage;
  /// the old value for minor page faults. (This is a raw # of total faults)
  long oldminf;
  /// the old value for major page faults.
  long oldmajf;
  /// the major page fault rate (per sec).  This is the last value sampled.
  long majfaultrate;
  /// the minor page fault rate.  Last value sampled.
  long minfaultrate;
  /// The "time of birth" of this process (in sec)
  long creation_time;
  /** A pretty good band... no, really, this flag is used by a simple garbage collection sheme
	  which controls when procHashNodes should be deleted.  First we set garbage to true.  
	  Then everytime we access this procHashNode we set it to false.  After an hour has passed,
	  any node which still has garbage set to true is deleted. */
  bool garbage;
};


/** The ProcAPI class returns information about running processes.  It is
    possible to get information for:
    <ul>
     <li> One pid.
     <li> A set of pids.
     <li> A pid and all of that pid's descendants.
    </ul>
    When you ask for information, a procInfo structure is returned to
    you containing the following:
    <ul>
     <li> Image size and resident set size.
     <li> Page faults (major & minor) per second.
     <li> % cpu usage.
     <li> Time spent in user and system modes.
     <li> The "wall clock" age of the process.
    </ul>
    Note that all of this information is returned at once.  This is because
    the operating system usually returns several of these values together
    in a structure, and it would be inefficient to repeatedly make system
    calls when only one call is really needed.  Also, 8 values * 3 types
    of things to get values for...equals 24 functions, which is way too many.
    <p>
    Note: memory for the procInfo structure can be allocated by the caller.
    If you pass a NULL pointer to one of the functions, space will be
    allocated for you.  Things will be bad if you pass a non-allocated, 
    non-NULL pointer into a member function.  Don't forget to deallocate
    the procInfo structure when done!

	<p>
	Note: Since there is no need to instantiate multiple copies of
	this class, the whole class is static, and should just never be
	instantiated at all.

    @author Mike Yoder
    @see procInfo
 */

class ProcAPI {
 public:

	 /** Destructor */
  ~ProcAPI();

  /** getProcInfo returns information about one process.  Information 
      can be retrieved on any process owned by the same user, and on some
      systems (Linux, HPUX, Sol2.6) information can be gathered from all 
      processes.  

      @param pid The pid of the process you want info on.
      @param pi  A pointer to a procInfo structure.
      @return A 0 on success, and number less than 0 on failure.
      @see procInfo
  */
  static int getProcInfo ( pid_t pid, piPTR& pi );

  /** getProcSetInfo gets information on a set of pids.  These pids are 
      specified by an array of pids that has 'numpids' elements.  The
      information is summed for all the pids ( except for age ) and returned
      in pi.

      @param pids An array of pids that you want info on.
      @param pi  A pointer to a procInfo structure.
      @return A -1 is returned if a pid's info can't be returned, 0 otherwise.
      @see procInfo
  */
  static int getProcSetInfo ( pid_t *pids, int numpids, piPTR& pi );

  /** getFamilyInfo returns a procInfo struct for a process and all
      processes which are descended from that process ( children, children
      of children, etc ).  

      @param pid The pid of the 'elder' process you want info on.
      @param pi  A pointer to a procInfo structure.
      @return A -1 is returned on failure, 0 otherwise.
      @see procInfo
  */
  static int getFamilyInfo ( pid_t pid, piPTR& pi );

  /** Feed this function a procInfo struct and it'll print it out for you. 

      @param pi A pointer to a procInfo structure.
      @return nothing
      @see procInfo
   */
  static void printProcInfo ( piPTR pi );

  /** The next function, getMemInfo, has a different implementation for 
      each *&^%$#@! OS.  The numbers returned for total & free mem are 
      consistent with the numbers reported by top.

      @param totalmem Total system memory.
      @param freemem  Free memory
      @return A -1 is returned on failure, 0 otherwise.
  */
  static int getMemInfo ( int& totalmem, int& freemem );

  /** This function returns a list of pids that are 'descendents' of that pid.
      I call this a 'family' of pids.  This list is put into pidFamily, which
      I assume is an already-allocated array.  This array will be terminated
      with a 0 at its end.  This array will never exceed 512 elemets.

      @param pid The 'elder' pid of the family you want a list of pids for.
      @param pidFamily An array for holding pids in the family
      @return A -1 is returned on failure, 0 otherwise.
  */
  static int getPidFamily( pid_t pid, pid_t *pidFamily );

  /* returns all pids owned by a specific user.
   	
	 @param pidFamily An array for holding pids in the family
	 @param searchLogin A string specifying the owner who's pids we want
	 @return A -1 is returned on failure, 0 otherwise.
  */
  static int getPidFamilyByLogin( const char *searchLogin, pid_t *pidFamily );

#ifdef WANT_STANDALONE_DEBUG
  /** This is a menu driver for tests 1-7.  Please don't try and break it. */
  static void runTests();

  /** This first test demonstrates the creation and monitoring of
      a simple family of processes.  This process forks off copies
      of itself, and each of these children allocate some memory,
      touch each page of it, do some cpu-intensive work, and then
      deallocate the memory they allocated. They then sleep for a
      minute and exit.  These children are monitored individually
      and as a family of processes. */
  static void test1();

  /** This test is similar to test 1, except that the children are
      forked off in a different way.  In this case, the parent forks
      a child, which itself forks a child, which forks a child, etc.  It
      is still possible to monitor this group by getting the
      family info for the parent pid.  */
  static void test2();
  
  /** This test determines if you can get information on processes
      other than those you own.  If you get a summary of all the
      processes in the system ( parent=1 ) then you can. */
  static void test3();

  /** This test checks the getProcSetInfo() function.  The user is 
      asked for pids, and the sum of their information is returned. */
  static void test4();
  
  /** This is a quick test of getPidFamily().  It forks off some
      processes and it'll print out the pids associated with these children */
  static void test5();

  /** Here's the test of cpu usage monitoring over time.  I'll
      fork off a process, which will alternate between sleeping
      and working.  I'll return info on that process. */
  static void test6();

  /** This is the nifty test.  Given a name of an executable, this test
      does and fork() and then an exec(), basically starting up a child
      process that is that program.  This child program and its descendents
      are monitored once every 10 seconds and the results printed out.
      I have had success with all sorts of programs, even a monster like
      Netscape ( /s/netscape-4/bin/netscape ) */
  static void test_monitor ( char * jobname );
#endif  // of WANT_STANDALONE_DEBUG

 private:

  /** Default constructor.  It's private so that no one really
	  instantiates a ProcAPI object anywhere. */
  ProcAPI() {};		

  static void initpi ( piPTR& );                  // initialization of pi.
  static int isinfamily ( pid_t *, int, pid_t );  // used by buildFamily & NT equiv.

  static uid_t getFileOwner(int fd); // used to get process owner from /proc
  static void closeFamilyHandles(); // closes all open handles for the family

  // works with the hashtable; finds cpuusage, maj/min page faults.
  static void do_usage_sampling( piPTR& pi, 
								 double ustime, 
								 long majf, 
								 long minf
#ifdef WIN32  /* Win32 also passes in the time. */
								 , double now
#endif
								 );

#ifndef WIN32
  static int buildPidList();                      // just what it says
  static int buildProcInfoList();                 // ditto.
  static int buildFamily( pid_t );                // builds + sums procInfo list
  static int getNumProcs();                       // guess.
  static long secsSinceEpoch();                   // used for wall clock age
  static double convertTimeval ( struct timeval );// convert timeval to double
  static pid_t getAndRemNextPid();                // used in pidList deconstruction
  static void deallocPidList();                   // these deallocate their
  static void deallocAllProcInfos();              // respective lists.
  static void deallocProcFamily();
#endif // not defined WIN32

#ifdef WANT_STANDALONE_DEBUG
  static void sprog1 ( pid_t *, int, int );       // used by test1.
  static void sprog2 ( int, int );                // used by test2.
#endif  // of WANT_STANDALONE_DEBUG

#ifdef WIN32 // some windoze-specific thingies:

  static void makeFamily( pid_t dadpid, pid_t *allpids, int numpids, 
						  pid_t* &fampids, int &famsize );
  static void getAllPids( pid_t* &pids, int &numpids );
  static int multiInfo ( pid_t *pidlist, int numpids, piPTR &pi );
  static int isinlist( pid_t pid, pid_t *pidlist, int numpids ); 
  static DWORD GetSystemPerfData ( LPTSTR pValue );
  static double LI_to_double ( LARGE_INTEGER );
  static PPERF_OBJECT_TYPE firstObject( PPERF_DATA_BLOCK pPerfData );
  static PPERF_OBJECT_TYPE nextObject( PPERF_OBJECT_TYPE pObject );
  static PERF_COUNTER_DEFINITION *firstCounter( PERF_OBJECT_TYPE *pObjectDef );
  static PERF_COUNTER_DEFINITION *nextCounter( PERF_COUNTER_DEFINITION *pCounterDef);
  static PERF_INSTANCE_DEFINITION *firstInstance( PERF_OBJECT_TYPE *pObject );
  static PERF_INSTANCE_DEFINITION *nextInstance(PERF_INSTANCE_DEFINITION *pInstance);
  
  static void grabOffsets ( PPERF_OBJECT_TYPE );
      // pointer to perfdata block - where all the performance data lives.
  static PPERF_DATA_BLOCK pDataBlock;
  
  static struct Offset *offsets;
  static ExtArray<HANDLE> familyHandles;

#endif // WIN32 poop.

  /* Using condor's HashTable template class.  I'm storing a procHashNode, 
     hashed on a pid. */
  static HashTable <pid_t, procHashNode *> *procHash;
  friend int hashFunc ( const pid_t& pid, int numbuckets );

#ifndef WIN32 // these aren't used by the NT version...

  // private data structures:
  static pidlistPTR pidList;      // this will be a linked list of all processes
                           // in the system.  Built by buildpidlist()

  static piPTR allProcInfos; // this will be a linked list of 
                           // procinfo structures, one for each process
                           // whose /proc information is available.

  static piPTR procFamily; // a linked list ( using links in the procInfo
                           // struct ) of processes in the desired 'family'
                           // (a process + its descendants)

  static int pagesize;     // pagesize is the size of memory pages, in k.
                           // It's here so the call getpagesize() only needs
                           // to be called once.

#ifdef LINUX

  static long unsigned boottime; // this is used only in linux.  It
		// represents the number of seconds after the epoch that the
		// machine was booted.  Used in age calculation.
  static long unsigned boottime_expiration; // the boottime value will
		// change if the time is adjusted on this machine (by ntpd or afs,
		// for example), so we recompute it when our value expires

#endif // LINUX

#endif // not defined WIN32

}; 


#ifdef WANT_STANDALONE_DEBUG
// The following are for the test programs.

// how many children to you want to create?  5 is a good number for me.
const int NUMKIDS = 5;

// when each child is created, it allocates some memory.  The first
// child allocates "MEMFACTOR" Megs.  The second allocates MEMFACTOR *
// 2 Megs.  etc, etc.  Be careful of thrashing...unless that's what
// you want.
const int MEMFACTOR = 3;

#endif // WANT_STANDALONE_DEBUG

#endif // PROCAPI_H
