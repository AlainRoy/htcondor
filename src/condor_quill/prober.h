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
#ifndef _PROBER_H_
#define _PROBER_H_

#include "condor_common.h"

class JobQueueDBManager;

//! Prober
/*! this polls and probes Job Qeueue Log (job_queue.log) file.
 *  So, it returns the result of polling: INIT_DB, ADDITION, 
 *  COMPRESSION, and so on.
 */
class Prober
{
public:
	//! constructor	
	Prober();
	//! constructor	
	Prober(JobQueueDBManager *jqDBManager);
	//! destructor	
	~Prober();
	
	//! initialization
	void			Init();

	//! probe job_queue.log file
	int 			probe();

	//! store polling information into DBMS
	void			setProbeInfo();
	//! get polling information from DBMS
	int			    getProbeInfo();

		//
		// accessors
		//
	void			setJobQueueName(char* jqn);
	char*			getJobQueueName();
	void			setJQFile_Last_MTime(time_t t);
	time_t			getJQFile_Last_MTime();
	void			setJQFile_Last_Size(size_t s);
	size_t			getJQFile_Last_Size();


	enum PROBE_RESULT_TYPE {FILE_OPEN_ERROR = -1, ERROR = 0, NO_CHANGE = 1, 
					INIT_QUILL = 2, ADDITION = 3, 
					COMPRESSED = 4};

private:
		// information about a job_queuq.log file and polling state
	char			job_queue_name[_POSIX_PATH_MAX]; //!< job queue file path
		// stored metadata in DB
	long int		jqfile_last_mod_time;	 //!< last modification time
	long int		jqfile_last_size;		 //!< last size
		// currenlty probed...
	long int		cur_probed_jqfile_last_mod_time; //!< last modification time
	long int		cur_probed_jqfile_last_size;	 //!< last size

	ClassAdLogEntry	lastCALogEntry;		//!< last command (ClassAd Log Entry)

	JobQueueDBManager	*jqDBManager;	/*!< reference pointer 	
										 *   to JobQueueDBManager object
										 */
};

#endif /* _PROBER_H_ */
