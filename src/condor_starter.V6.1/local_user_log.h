/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
 * CONDOR Copyright Notice
 *
 * See LICENSE.TXT for additional notices and disclaimers.
 *
 * Copyright (c)1990-2002 CONDOR Team, Computer Sciences Department, 
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

#if !defined(_CONDOR_LOCAL_USER_LOG_H)
#define _CONDOR_LOCAL_USER_LOG_H

#include "../condor_daemon_core.V6/condor_daemon_core.h"
#include "condor_classad.h"
#include "user_log.c++.h"

/** 
	This class is used by the starter to maintain a local user log
	file for a job that runs under its control.  Each
	JobInfoCommunicator has a pointer to one of these, and if the job
	wants it for whatever reason, a LocalUserLog object is
	instantiated, and anytime the shadow would be writing a userlog
	event for this job, we do so here, too.
*/

/*
  Since JobInfoCommunicator and LocalUserLog each have a pointer to
  each other, the include files can't both include the other without
  trouble.  So, we just declare the class here, instead of including
  job_info_communicator.h, and everything is happy.
*/ 
class JobInfoCommunicator;



class LocalUserLog : public Service {
public:

		/// Constructor
	LocalUserLog( JobInfoCommunicator* my_jic );

		/// Destructor
	~LocalUserLog();

		// // // // // // // // // // // //
		// Initialization
		// // // // // // // // // // // //

		/** Initialize ourselves with all the info we need already
			proccessed and handed to use directly
			@param filename Full path to userlog to write
		*/
	bool init( const char* filename, bool is_xml, 
			   int cluster, int proc, int subproc );

		/** Initialize ourselves with the info in the given job ad.
		*/
	bool initFromJobAd( ClassAd* ad, const char* iwd );


		// // // // // // // // // // // //
		// Writing Events
		// // // // // // // // // // // //

		/** Log an execute event for this job. */
	bool logExecute( ClassAd* ad );

		/** Log a suspend event for this job.
			@param ad ClassAd containing the info we need for the
			event (which is what the JIC would be sending to the
			controller in some way)
		*/
	bool logSuspend( ClassAd* ad );

		/** Log a continue event for this job.
		*/
	bool logContinue( ClassAd* ad );

		/** Log a terminate event for this job.
			@param ad ClassAd containing the info we need for the
			event (which is what the JIC would be sending to the
			controller in some way)
		*/
	bool logTerminate( ClassAd* ad, int exit_reason );

		/** Log an evict event for this job.
			@param ad ClassAd containing the info we need for the
			event (which is what the JIC would be sending to the
			controller in some way)
		*/
	bool logEvict( ClassAd* ad, int exit_reason );

private:

		// // // // // // // // // // // //
		// Private helper methods
		// // // // // // // // // // // //

		/** Since both logTerminate() and logEvict() want to include
			rusage information, we have a shared helper function to
			pull the relevent ClassAd attributes out of the ad and to
			initialize an rusage structure.
		*/ 
	struct rusage getRusageFromAd( ClassAd* ad );


		// // // // // // // // // // // //
		// Private data members
		// // // // // // // // // // // //

		/// Pointer to the jic for this LocalUserLog object. 
	JobInfoCommunicator* jic;

		/// Have we been initialized yet?
	bool is_initialized;

		/// The actual UserLog object we're using to write events. 
	UserLog u_log;

};


#endif /* _CONDOR_LOCAL_USER_LOG_H */
