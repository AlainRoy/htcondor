/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2006, Condor Team, Computer Sciences Department,
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
#ifndef _CONDOR_STARTER_HOOK_MGR_H
#define _CONDOR_STARTER_HOOK_MGR_H

#include "condor_common.h"
#include "HookClientMgr.h"
#include "HookClient.h"
#include "enum_utils.h"

class HookPrepareJobClient;
class HookJobExitClient;
class HookEvictJobClient;


/**
   The StarterHookMgr manages all the hooks that the starter invokes.
*/
class StarterHookMgr : public HookClientMgr
{
public:
	StarterHookMgr(); 
	~StarterHookMgr();

	bool initialize(ClassAd* job_ad);
	bool reconfig();

		/**
		   See if we're configured to use HOOK_PREPARE_JOB and spawn it.

		   @return 1 if we spawned the hook, 0 if we're not configured
		   to use this hook for the job's hook keyword, or -1 on error.
		*/
	int tryHookPrepareJob();

		/**
		   Invoke HOOK_UPDATE_JOB_INFO to provide a periodic update
		   for job information such as image size, CPU time, etc.
		   Also called on temporary state changes like suspend/resume.
		   @param job_info ClassAd of job info for the update.
		   @return True if a hook is spawned, otherwise false.
		*/
	bool hookUpdateJobInfo(ClassAd* job_info);

		/**
		   Invoke HOOK_JOB_EXIT to tell the outside world the
		   final status of a given job, including the exit status,
		   total CPU time, final image size, etc.  The starter will
		   wait until this hook returns before exiting, although all
		   output from the hook is ignored, including the exit status.
		 */
	bool tryHookJobExit(ClassAd* job_info);
	bool handleHookJobExit();

		/**
		   Invoke HOOK_EVICT_JOB to tell the outside world that a
		   specific job is being evicted. The hook should attempt to
		   save whatever work can be saved and gracefully cleanup the
		   job ASAP. The starter will wait to kill the job outright
		   until this hook returns, but there's no guarantee how much
		   time this hook has to run before it will be killed along
		   with the starter.
		 */
	void tryHookEvictJob(bool graceful);
	void handleHookEvictJob();

private:

		/// The hook keyword defined in the job, or NULL if not present.
	char* m_hook_keyword;

		/// The path to HOOK_PREPARE_JOB, if defined.
	char* m_hook_prepare_job;
		/// The path to HOOK_UPDATE_JOB_INFO, if defined.
	char* m_hook_update_job_info;
		/// The path to HOOK_JOB_EXIT, if defined.
	char* m_hook_job_exit;
		/// The path to HOOK_EVICT_JOB, if defined.
	char* m_hook_evict_job;

		/**
		   If the job we're running defines a hook keyword, find the
		   validate path to the given hook.
		   @param hook_type The hook you want the path for.
		   @return Path to the hook if defined and valid, otherwise NULL.
		*/
	char* getHookPath(HookType hook_type);

		/// Clears out all the hook paths we've validated and saved.
	void clearHookPaths( void );
};


/**
   Manages an invocation of HOOK_PREPARE_JOB.
*/
class HookPrepareJobClient : public HookClient
{
public:
	friend class StarterHookMgr;

	HookPrepareJobClient(const char* hook_path);

		/**
		   HOOK_PREPARE_JOB has exited.  If the hook exited with a
		   non-zero status, the job environment couldn't be prepared,
		   so the job is not spawned and the starter exits immediately.
		   Otherwise, we let the Starter know the job is ready to run.
		*/
	virtual void hookExited(int exit_status);
};


/**
   Manages an invocation of HOOK_JOB_EXIT.
*/
class HookJobExitClient : public HookClient
{
public:
	friend class StarterHookMgr;

	HookJobExitClient(const char* hook_path);
	virtual void hookExited(int exit_status);
};


/**
   Manages an invocation of HOOK_EVICT_JOB.
*/
class HookEvictJobClient : public HookClient
{
public:
	friend class StarterHookMgr;

	HookEvictJobClient(const char* hook_path);
	virtual void hookExited(int exit_status);
};


#endif /* _CONDOR_STARTER_HOOK_MGR_H */
