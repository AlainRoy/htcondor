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

#ifndef CONDORRESOURCE_H
#define CONDORRESOURCE_H

#include "condor_common.h"
#include "../condor_daemon_core.V6/condor_daemon_core.h"

#include "condorjob.h"
#include "baseresource.h"
#include "gahp-client.h"

#define ACQUIRE_DONE		0
#define ACQUIRE_QUEUED		1
#define ACQUIRE_FAILED		2

class CondorJob;
class CondorResource;

class CondorResource : public BaseResource
{
 public:

	CondorResource( const char *resource_name );
	~CondorResource();

	bool IsEmpty();

	void Reconfig();
	void RegisterJob( CondorJob *job, const char *submitter_id );
	void UnregisterJob( CondorJob *job );

	int DoScheddPoll();

	static CondorResource *FindOrCreateResource( const char *resource_name );
	static void setPollInterval( int new_interval )
		{ scheddPollInterval = new_interval; }

	static int scheddPollInterval;

	StringList submitter_ids;
	MyString submitter_constraint;
	int scheddPollTid;
	char *scheddName;
	bool scheddStatusActive;

 private:
	static HashTable <HashKey, CondorResource *> ResourcesByName;

	List<CondorJob> *registeredJobs;
	GahpClient *gahp;
};

#endif
