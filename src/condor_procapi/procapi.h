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

#ifndef WIN32 // all the below is for UNIX

#include <strings.h>       // sprintf, atol
#include <dirent.h>        // get /proc entries (directory stuff)
#include <ctype.h>         // isdigit
#include <errno.h>         // for perror
#include <fcntl.h>         // open()
#include <unistd.h>        // getpagesize()
#include <sys/types.h>     // various types needed.
#include <time.h>          // use of time() for process age. 

#ifndef HPUX               // neither of these are in hpux.

#ifndef Solaris26          
#include <sys/procfs.h>    // /proc stuff for all but Solaris 2.6
#else
#include <procfs.h>        // /proc stuff for Solaris 2.6 
#endif

#endif

#ifdef HPUX                // hpux has to be different, of course.
#include <sys/param.h>     // used in pstat().
#include <sys/pstat.h>
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

#include "ntsysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <windows.h>
#include <winperf.h>
#include <tchar.h>
#include <largeint.h>

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
  /// the resident set size, in k.
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
  /// the last time (secs) this data was retrieved.
  double lasttime;
  /// old time usage number
  double oldtime;
  /// the old value for cpu usage.
  double oldusage;
  /// the old value for minor page faults.
  long oldminf;
  /// the old value for major page faults.
  long oldmajf;
  /// The "time of birth" of this process (in sec)
  long creation_time;
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

    @author Mike Yoder
    @see procInfo
 */

class ProcAPI {
 public:
  /** Default constructor. */
  ProcAPI();
  /** Destructor */
  ~ProcAPI();

  /** getProcInfo returns information about one process.  Information 
      can be retrieved on any process owned by the same user, and on some
      systems (Linux, HPUX, Sol2.6) information can be gathered from all 
      processes.  

      @param pid The pid of the process you want info on.
      @param pi  A pointer to a procInfo structure.
      @return A -1 is returned on failure, 0 otherwise.
      @see procInfo
  */
  int getProcInfo ( pid_t pid, piPTR& pi );

  /** getProcSetInfo gets information on a set of pids.  These pids are 
      specified by an array of pids that has 'numpids' elements.  The
      information is summed for all the pids ( except for age ) and returned
      in pi.

      @param pids An array of pids that you want info on.
      @param pi  A pointer to a procInfo structure.
      @return A -1 is returned if a pid's info can't be returned, 0 otherwise.
      @see procInfo
  */
  int getProcSetInfo ( pid_t *pids, int numpids, piPTR& pi );

  /** getFamilyInfo returns a procInfo struct for a process and all
      processes which are descended from that process ( children, children
      of children, etc ).  

      @param pid The pid of the 'elder' process you want info on.
      @param pi  A pointer to a procInfo structure.
      @return A -1 is returned on failure, 0 otherwise.
      @see procInfo
  */
  int getFamilyInfo ( pid_t pid, piPTR& pi );

  /** Feed this function a procInfo struct and it'll print it out for you. 

      @param pi A pointer to a procInfo structure.
      @return nothing
      @see procInfo
   */
  void printProcInfo ( piPTR pi );

  /** The next function, getMemInfo, has a different implementation for 
      each *&^%$#@! OS.  The numbers returned for total & free mem are 
      consistent with the numbers reported by top.

      @param totalmem Total system memory.
      @param freemem  Free memory
      @return A -1 is returned on failure, 0 otherwise.
  */
  int getMemInfo ( int& totalmem, int& freemem );

  /** This function returns a list of pids that are 'descendents' of that pid.
      I call this a 'family' of pids.  This list is put into pidFamily, which
      I assume is an already-allocated array.  This array will be terminated
      with a 0 at its end.  This array will never exceed 512 elemets.

      @param pid The 'elder' pid of the family you want a list of pids for.
      @param pidFamily An array for holding pids in the family
      @return A -1 is returned on failure, 0 otherwise.
  */
  int getPidFamily( pid_t pid, pid_t *pidFamily );

#ifdef WANT_STANDALONE_DEBUG
  /** This is a menu driver for tests 1-7.  Please don't try and break it. */
  void runTests();

  /** This first test demonstrates the creation and monitoring of
      a simple family of processes.  This process forks off copies
      of itself, and each of these children allocate some memory,
      touch each page of it, do some cpu-intensive work, and then
      deallocate the memory they allocated. They then sleep for a
      minute and exit.  These children are monitored individually
      and as a family of processes. */
  void test1();

  /** This test is similar to test 1, except that the children are
      forked off in a different way.  In this case, the parent forks
      a child, which itself forks a child, which forks a child, etc.  It
      is still possible to monitor this group by getting the
      family info for the parent pid.  */
  void test2();
  
  /** This test determines if you can get information on processes
      other than those you own.  If you get a summary of all the
      processes in the system ( parent=1 ) then you can. */
  void test3();

  /** This test checks the getProcSetInfo() function.  The user is 
      asked for pids, and the sum of their information is returned. */
  void test4();
  
  /** This is a quick test of getPidFamily().  It forks off some
      processes and it'll print out the pids associated with these children */
  void test5();

  /** Here's the test of cpu usage monitoring over time.  I'll
      fork off a process, which will alternate between sleeping
      and working.  I'll return info on that process. */
  void test6();

  /** This is the nifty test.  Given a name of an executable, this test
      does and fork() and then an exec(), basically starting up a child
      process that is that program.  This child program and its descendents
      are monitored once every 10 seconds and the results printed out.
      I have had success with all sorts of programs, even a monster like
      Netscape ( /s/netscape-4/bin/netscape ) */
  void test_monitor ( char * jobname );
#endif  // of WANT_STANDALONE_DEBUG

 private:
  void initpi ( piPTR& );                  // initialization of pi.
  int isinfamily ( pid_t *, int, pid_t );  // used by buildFamily & NT equiv.
#ifndef WIN32
	  // works with the hashtable; finds cpuusage, maj/min page faults.
  void do_usage_sampling( piPTR&, double, long, long);
  int buildPidList();                      // just what it says
  int buildProcInfoList();                 // ditto.
  int buildFamily( pid_t );                // builds + sums procInfo list
  int getNumProcs();                       // guess.
  long secsSinceEpoch();                   // used for wall clock age
  double convertTimeval ( struct timeval );// convert timeval to double
  pid_t getAndRemNextPid();                // used in pidList deconstruction
  void deallocPidList();                   // these deallocate their
  void deallocAllProcInfos();              // respective lists.
  void deallocProcFamily();
#endif // not defined WIN32

#ifdef WANT_STANDALONE_DEBUG
  void sprog1 ( pid_t *, int, int );       // used by test1.
  void sprog2 ( int, int );                // used by test2.
#endif  // of WANT_STANDALONE_DEBUG

#ifdef WIN32 // some windoze-specific thingies:
	//Todd's system info class, for getting parent:
  CSysinfo ntSysInfo;

  void makeFamily( pid_t dadpid, pid_t *allpids, int numpids, 
				 pid_t* &fampids, int &famsize );
  void getAllPids( pid_t* &pids, int &numpids );
  int multiInfo ( pid_t *pidlist, int numpids, piPTR &pi );
  int isinlist( pid_t pid, pid_t *pidlist, int numpids ); 
  DWORD GetSystemPerfData ( LPTSTR pValue );
  double LI_to_double ( LARGE_INTEGER );
  PPERF_OBJECT_TYPE firstObject( PPERF_DATA_BLOCK pPerfData );
  PPERF_OBJECT_TYPE nextObject( PPERF_OBJECT_TYPE pObject );
  PERF_COUNTER_DEFINITION *firstCounter( PERF_OBJECT_TYPE *pObjectDef );
  PERF_COUNTER_DEFINITION *nextCounter( PERF_COUNTER_DEFINITION *pCounterDef);
  PERF_INSTANCE_DEFINITION *firstInstance( PERF_OBJECT_TYPE *pObject );
  PERF_INSTANCE_DEFINITION *nextInstance(PERF_INSTANCE_DEFINITION *pInstance);
  
  void grabOffsets ( PPERF_OBJECT_TYPE );
      // pointer to perfdata block - where all the performance data lives.
  PPERF_DATA_BLOCK pDataBlock;
  
  struct Offset *offsets;
  
#endif // WIN32 poop.

  /* Using condor's HashTable template class.  I'm storing a procHashNode, 
     hashed on a pid. */
  HashTable <pid_t, procHashNode *> *procHash;
  friend int hashFunc ( const pid_t& pid, int numbuckets );

#ifndef WIN32 // these aren't used by the NT version...

  // private data structures:
  pidlistPTR pidList;      // this will be a linked list of all processes
                           // in the system.  Built by buildpidlist()

  piPTR      allProcInfos; // this will be a linked list of 
                           // procinfo structures, one for each process
                           // whose /proc information is available.

  piPTR      procFamily;   // a linked list ( using links in the procInfo
                           // struct ) of processes in the desired 'family'
                           // (a process + its descendants)

  int pagesize;            // pagesize is the size of memory pages, in k.
                           // It's here so the call getpagesize() only needs
                           // to be called once.

#ifdef LINUX
  long unsigned boottime;  // this is used only in linux.  It represents the 
                           // number of seconds after the epoch that the
                           // machine was booted.  Used in age calculation
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
