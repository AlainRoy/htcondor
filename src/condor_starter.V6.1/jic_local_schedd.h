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

#if !defined(_CONDOR_JIC_LOCAL_SCHEDD_H)
#define _CONDOR_JIC_LOCAL_SCHEDD_H

#include "jic_local_file.h"
#include "qmgr_job_updater.h"
#include "starter_user_policy.h"

/** 
	This is the child class of JICLocalFile (and therefore JICLocal
	and JobInfoCommunicator) that deals with running "local universe"
	jobs directly under a condor_schedd.  This JIC gets the job
	ClassAd info from a file (a pipe to STDIN, in fact).  Instead of
	simply reporting everything to a file, it reports info back to the
	schedd via special exit status codes.
*/

class JICLocalSchedd : public JICLocalFile {
public:

		/** Constructor 
			@param classad_filename Full path to the ClassAd, "-" if STDIN
			@param schedd_address Sinful string of the schedd's qmgmt port
			@param cluster Cluster ID number (if any)
			@param proc Proc ID number (if any)
			@param subproc Subproc ID number (if any)
		*/
	JICLocalSchedd( const char* classad_filename,
					const char* schedd_address,
					int cluster, int proc, int subproc );

		/// Destructor
	virtual ~JICLocalSchedd();

		/// Initialize ourselves
		/// This first will call JICLocal::init() then
		/// initialize the user policy object
	virtual bool init( void );
	
		/**
		 * Puts the job on hold. The Starter actually does the 
		 * dirty work, we just add the reason to the ad and email the
		 * user. We set the exit_code to JOB_SHOULD_HOLD so that
		 * schedd will put the job on hold in the queue. We do NOT need
		 * to write an EVICT log event because JICLocalSchedd::notifyJobExit()
		 * will take care of that for us.
		 * 
		 * @param reason - why the job is going on hold
		 * @return true if the jobs were told to be put on hold
		 */
	virtual bool holdJob( const char* );
	
		/**
		 * The job needs to be removed from the Starter. This
		 * is more than just calling Starter->Remove() because we need
		 * to stuff the remove reason in the job ad and update the job queue.
		 * We also set the proper exit code for the Starter. We do NOT need
		 * to write an EVICT log event because JICLocalSchedd::notifyJobExit()
		 * will take care of that for us.
		 * 
		 * @param reason - why the job is being removed
		 * @return true if the job was set to be removed
		 **/
	virtual bool removeJob( const char* );
	
		/*
		 * The job exited on its own accord and its not to be requeued,
		 * so we need to update the job and write a TERMINATE event 
		 * into the user log. It is important that we do NOT set the
		 * exit_code in here like we do in the other methods.
		 * 
		 * @param reason - why the job is being terminated
		 * @return true if the job was set to be terminated
		 **/
	virtual bool terminateJob( const char* );
	
		/**
		 * The job needs to be requeued back on the schedd, so 
		 * we need to update the job ad, the queue, and write 
		 * a REQUEUE event into the user log. We have to set
		 * the proper exit code for the Starter so that the schedd
		 * knows to put the job back on hold.
		 * 
		 * @param reason - why the job is being requeued
		 * @return true if the job was set to be requeued
		 **/
	virtual bool requeueJob( const char* );

	virtual void allJobsGone( void );

		/// The starter has been asked to shutdown fast.
	virtual void gotShutdownFast( void );

		/// The starter has been asked to shutdown gracefully.
	virtual void gotShutdownGraceful( void );

		/// The starter has been asked to evict for condor_rm
	virtual void gotRemove( void );

		/// The starter has been asked to evict for condor_hold
	virtual void gotHold( void );

		/** Get the local job ad.  We need a JICLocalSchedd specific
			version of this so that after we grab the ad (using the
			JICLocal version), we need to initialize our
			QmgrJobUpdater object with a pointer to the ad.
		*/
	virtual bool getLocalJobAd( void );

		/**
		 * A job is exiting the Starter and we need to take necessary
		 * actions. First we will update the job's ad file with various
		 * information about what the job did. Next, if the job completed on
		 * its own, we'll want to call the StarterUserPolicy's checkAtExit(),
		 * which handles writing any user log events and updating the job
		 * queue back on the schedd. If the job is being killed from "unnatural"
		 * causes, such as a condor_rm, then we will figure out the right
		 * update type is for the job and write an EVICT event to the user log.
		 * 
		 * @param exit_status - the exit status of the job from wait()
		 * @param reason - the Condor-defined reason why the job is exiting
		 * @param user_proc - the Proc object for this job
		 * @return true if the job was set to exit properly
		 * @see h/exit.h
		 **/
	virtual bool notifyJobExit( int exit_status, int reason, 
								UserProc* user_proc );  
			
		/**
		 * An error occured with this job on the Starter. We need to update
		 * the job ad with the appropriate information about the error, and
		 * then report to whomever we need to.
		 * 
		 * @param default_action - the action that should be taken if the user doesn't say
		 * @param reason - the error message
		 * @param code - the error code
		 * @param subcode - the error subcode
		 * @return true if the job ad was updated succesfully
		 * @see user_error_policy.h
		 **/
	virtual bool notifyJobError( int default_action, const char *reason,
								 int code, int subcode = 0 );

protected:

		/// This version confirms we're handling a "local" universe job. 
	virtual bool getUniverse( void );

		/// Initialize our local UserLog-writing code.
	virtual bool initLocalUserLog( void );

		/// The value we will exit with to tell our schedd what happened
	int exit_code;

		/// The sinful string of the schedd's qmgmt command port
	char* schedd_addr;

		/// object for managing updates to the schedd's job queue for
		/// dynamic attributes in this job.
	QmgrJobUpdater* job_updater;
	
		/// The UserPolicy object wrapper for doing periodic and check
		/// at exit policy evaluations
	StarterUserPolicy *starter_user_policy;

};

#endif /* _CONDOR_JIC_LOCAL_SCHEDD_H */
