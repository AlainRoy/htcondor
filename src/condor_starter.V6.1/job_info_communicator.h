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

#if !defined(_CONDOR_JOB_INFO_COMMUNICATOR_H)
#define _CONDOR_JOB_INFO_COMMUNICATOR_H

#include "../condor_daemon_core.V6/condor_daemon_core.h"
#include "condor_classad.h"
#include "user_proc.h"
#include "local_user_log.h"

/** 
	This class is a base class for the various ways a starter can
	recieve and send information about the underlying job.  For now,
	there are two main ways to do this: 1) to talk to a condor_shadow
	and 2) the local filesystem, command line args, etc.
*/

class JobInfoCommunicator : public Service {
public:
		/// Constructor
	JobInfoCommunicator();

		/// Destructor
	virtual ~JobInfoCommunicator();

		/// Pure virtual functions:

		// // // // // // // // // // // //
		// Initialization
		// // // // // // // // // // // //

		/** Initialize ourselves.  This should perform the following
			actions, no matter what kind of job controller we're
			dealing with:
			- aquire the job classad
			- call registerStarterInfo()
			- call initUserPriv()
			- call initJobInfo()
			@return true if we successfully initialized, false if we
			   failed and need to abort
		*/
	virtual bool init( void ) = 0;

		/// Read anything relevent from the config file
	virtual void config( void ) = 0;

		/** Setup the execution environment for the job.  
		 */
	virtual void setupJobEnvironment( void ) = 0;

		// // // // // // // // // // // //
		// Information about the job 
		// // // // // // // // // // // //
	
		/** Return a pointer to the filename to use for the job's
			standard input file.
		*/
	virtual const char* jobInputFilename( void );	

		/** Return a pointer to the filename to use for the job's
			standard output file.
		*/
	virtual const char* jobOutputFilename( void );	

		/** Return a pointer to the filename to use for the job's
			standard error file.
		*/
	virtual const char* jobErrorFilename( void );	

		/** Return a pointer to the job's initial working directory. 
		*/
	virtual const char* jobIWD( void );

		/** true if the starter is using a different iwd than the one
			in the job ad, false if not.
		*/
	virtual bool iwdIsChanged( void ) { return change_iwd; };

		/// Return a pointer to the original name for the job.
	virtual const char* origJobName( void );

		/// Return a pointer to the ClassAd for our job.
	virtual ClassAd* jobClassAd( void );

		/// Return the job's universe integer.
	int jobUniverse( void );

	int jobCluster( void );
	int jobProc( void );
	int jobSubproc( void );

		/// Total bytes sent by this job 
	virtual float bytesSent( void ) = 0;

		/// Total bytes received by this job 
	virtual float bytesReceived( void ) = 0;


		// // // // // // // // // // // //
		// Job execution and state changes
		// // // // // // // // // // // //

		/** All jobs have been spawned by the starter.
		 */
	virtual void allJobsSpawned( void ) = 0;

		/** The starter has been asked to suspend.  Take whatever
			steps make sense for the JIC, and notify our job
			controller what happend.
		*/
	virtual void Suspend( void ) = 0;

		/** The starter has been asked to continue.  Take whatever
			steps make sense for the JIC, and notify our job
			controller what happend.
		*/
	virtual void Continue( void ) = 0;

		/** The last job this starter is controlling has exited.  Do
			whatever we have to do to cleanup and notify our
			controller. 
		*/
	virtual void allJobsDone( void ) = 0;

		/** The last job this starter is controlling has been
			completely cleaned up.  Do whatever final work we want to
			do to shutdown, notify others, etc.
		*/
	virtual void allJobsGone( void ) = 0;

		/** The starter has been asked to shutdown fast.
		 */
	virtual void gotShutdownFast( void );

		/** The starter has been asked to shutdown gracefully.
		 */
	virtual void gotShutdownGraceful( void );


		// // // // // // // // // // // //
		// Notfication to our controller
		// // // // // // // // // // // //

		/** Notifyour controller that the job is about to spawn
		 */
	virtual void notifyJobPreSpawn( void ) = 0;

		/** Notify our controller that the job exited
			@param exit_status The exit status from wait()
			@param reason The Condor-defined exit reason
			@param user_proc The UserProc that was running the job
		*/
	virtual bool notifyJobExit( int exit_status, int reason, 
								UserProc* user_proc ) = 0;


	virtual bool notifyStarterError( const char* err_msg, bool critical ) = 0;


	void setOutputAdFile( const char* path );
	const char* getOutputAdFile( void ) { return job_output_ad_file; };
	bool writeOutputAdFile( ClassAd* ad );
	void initOutputAdFile( void );

		// // // // // // // // // // // //
		// Misc utilities
		// // // // // // // // // // // //

		/** Make sure the given filename will be included in the
			output files of the job that are sent back to the job
			submitter.  
			@param filename File to add to the job's output list 
		*/
	virtual void addToOutputFiles( const char* filename ) = 0;

		/// Has user_priv been initialized yet?
	bool userPrivInitialized( void ); 


protected:

		// // // // // // // // // // // //
		// Protected helper methods
		// // // // // // // // // // // //

		/** Register some important information about ourself that the
			job controller might needs.
			@return true on success, false on failure
		*/
	virtual	bool registerStarterInfo( void ) = 0;

		/** Initialize the priv_state code with the appropriate user
			for this job.
			@return true on success, false on failure
		*/
	virtual bool initUserPriv( void ) = 0;

		/** Initialize the priv_state code on Windows.  For now, this
			is identical code, no matter what kind of JIC we're using,
			so put it in 1 place to avoid duplication
		*/
	bool initUserPrivWindows( void );

		/** See if we can initialize user_priv without ATTR_OWNER.  if
			we can, do it and return true.  If not, return false.
		*/
	bool initUserPrivNoOwner( void ); 

		/** Publish information into the given classad for updates to
			our job controller
			@param ad ClassAd pointer to publish into
			@return true if success, false if failure
		*/ 
	virtual bool publishUpdateAd( ClassAd* ad ) = 0;

		/** Initialize our version of important information for this
			job which the starter will want to know.  This should
			init the following: orig_job_name, job_input_name, 
			job_output_name, job_error_name, job_iwd, 
			job_universe, job_cluster, job_proc, and job_subproc
			@return true on success, false on failure */
	virtual	bool initJobInfo( void ) = 0;

		/** Since we want to support the ATTR_STARTER_WAIT_FOR_DEBUG,
			as soon as we have the job ad, each JIC subclass will want
			to do this work at a different time.  However, since the
			code is the same in all cases, we use this helper in the
			base class to do the work, which looks up the attr in the
			job ad, and if it's defined as true, we go into the
			infinite loop, waiting for someone to attach with a
			debugger.  This also handles printing out the job classad
			to D_JOB if that's in our DebugFlags.
		 */
	virtual void checkForStarterDebugging( void );


		// // // // // // // // // // // //
		// Protected data members
		// // // // // // // // // // // //

	LocalUserLog* u_log;

		/** The real job executable name (after ATTR_JOB_CMD
			is switched to condor_exec).
		*/
	char* orig_job_name;

	char* job_input_name;

	char* job_output_name;

	char* job_error_name;

	char* job_iwd;

	char* job_output_ad_file;
	bool job_output_ad_is_stdout;
	
		/// The ClassAd for our job.  We control the memory for this.
	ClassAd* job_ad;

		/// The universe of the job.
	int job_universe;

	int job_cluster;
	int	job_proc;
	int	job_subproc;

		/// if true, we were asked to shutdown
	bool requested_exit;

		/** true if we're using a different iwd for the job than what
			the job ad says.
		*/
	bool change_iwd;

	bool user_priv_is_initialized;

};


#endif /* _CONDOR_JOB_INFO_COMMUNICATOR_H */
