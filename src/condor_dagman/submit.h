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

#ifndef CONDOR_SUBMIT_H
#define CONDOR_SUBMIT_H

#include "condor_id.h"

/** Submits a job to condor using popen().  This is a very primitive method
    to submitting a job, and SHOULD be replacable by a Condor Submit API.

    In the mean time, this function executes the condor_submit command
    via popen() and parses the output, sniffing for the CondorID assigned
    to the submitted job.

    Parsing the condor_submit output successfully depends on the current
    version of Condor, and how it's condor_submit outputs results on the
    command line.
   
	@param dm the appropriate Dagman object
    @param cmdFile the job's Condor command file.
    @param condorID will hold the ID for the submitted job (if successful)
    @param DAGNodeName the name of the job's DAG node
	@param DAGParentNodeNames a delimited string listing the node's parents
	@param names the names of any parameters for this node
	@param vals the values of any parameters for this node
    @param directory the directory in which to run this job
    @return true on success, false on failure
*/

bool condor_submit( const Dagman &dm, const char* cmdFile, CondorID& condorID,
					const char* DAGNodeName, MyString DAGParentNodeNames,
					List<MyString>* names, List<MyString>* vals,
					const char* directory);

bool dap_submit( const char* cmdFile, CondorID& condorID,
		 const char* DAGNodeName, const char* directory );  //--> DAP

#endif /* #ifndef CONDOR_SUBMIT_H */
