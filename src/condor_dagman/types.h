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

#ifndef _TYPES_H_
#define _TYPES_H_

#include "condor_common.h"    /* for <stdio.h> */
#include "condor_debug.h"

//---------------------------------------------------------------------------
#ifndef __cplusplus
typedef int bool;
#define true  1
#define false 0
#endif

///
typedef int JobID_t;

//---------------------------------------------------------------------------
/** An object to represent the Condor ID of a job.  Condor uses three integers
    (cluster, proc, subproc) to identify jobs.  This structure will be used to
    store those three numbers.
*/
class CondorID {
  public:
    /// Constructor
    CondorID () : _cluster(-1), _proc(-1), _subproc(-1) {}

    /// Copy Constructor
    CondorID (int cluster, int proc, int subproc):
        _cluster(cluster), _proc(proc), _subproc(subproc) {}
    
    ///
    inline void Set (int cluster, int proc, int subproc) {
        _cluster = cluster;
        _proc    = proc;
        _subproc = subproc;
    }

    /** Compares this condorID's with another.
        @param condorID the other CondorID to compare
        @return zero if they match
    */
    int Compare (const CondorID condorID) const;
    
    /** Test for equality between two CondorID's.
        @param the other CondorID object
        @return true if equal, false if not
    */
    inline bool operator == (const CondorID condorID) const {
        return Compare (condorID) == 0;
    }
    
    /// The job cluster
    int _cluster;

    /// The job process number
    int _proc;

    /// The job subprocess number
    int _subproc;
};


#endif /* #ifndef _TYPES_H_ */
