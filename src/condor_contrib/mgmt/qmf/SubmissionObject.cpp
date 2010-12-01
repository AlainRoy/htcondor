/*
 * Copyright 2008 Red Hat, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "condor_common.h"
#include "condor_config.h"

#include "SubmissionObject.h"
#include "ArgsSubmissionGetJobSummaries.h"

#include "MgmtScheddPlugin.h"

#include "set_user_priv_from_ad.h"
#include "../condor_schedd.V6/scheduler.h"

#include "stdint.h"

#include "JobUtils.h"
#include "Utils.h"

#include "MgmtConversionMacros.h"

#include <sstream>

extern MgmtScheddPlugin *scheddPluginInstance;

using namespace std;
using namespace com::redhat::grid;

using namespace qpid::management;
using namespace qmf::com::redhat::grid;

SubmissionObject::SubmissionObject(ManagementAgent *agent,
					JobServerObject *_jobServer,
					const char *_name,
					const char *_owner): m_name(_name)
{
	mgmtObject = new Submission(agent, this, _jobServer);

	mgmtObject->set_Name(string(_name));
	mgmtObject->set_Owner(string(_owner));

	// By default the submission will be persistent.
	bool _lifetime = param_boolean("QMF_IS_PERSISTENT", true);
	agent->addObject(mgmtObject, _name, _lifetime);
}


SubmissionObject::~SubmissionObject()
{
	if ( mgmtObject )
	{
		mgmtObject->resourceDestroy();
	}
}


ManagementObject *
SubmissionObject::GetManagementObject(void) const
{
	return mgmtObject;
}


void
SubmissionObject::update(const PROC_ID &id,
						 const char *attr,
						 int value)
{
	dprintf(D_FULLDEBUG, "Submission[%s]::update(%d.%d, %s, %d)\n",
			mgmtObject->get_Name().c_str(),
			id.cluster, id.proc, attr, value);

	// Note: UNEXPANDED means no previous state

	if (strcasecmp(attr, ATTR_LAST_JOB_STATUS) == 0) {
		switch (value) {
		case UNEXPANDED:
				// Nothing to do for UNEXPANDED, it's the initial state
			break;
		case IDLE:
			mgmtObject->dec_Idle();
			break;
		case RUNNING:
			mgmtObject->dec_Running();
			break;
		case REMOVED:
			mgmtObject->dec_Removed();
			break;
		case COMPLETED:
			mgmtObject->dec_Completed();
			break;
		case HELD:
			mgmtObject->dec_Held();
			break;
		case SUBMISSION_ERR:
				// Nothing to do for SUBMISSION_ERR, should never see it
			dprintf(D_FULLDEBUG,
					"WARNING: Encountered a %s of SUBMISSION_ERR on %d.%d\n",
					ATTR_LAST_JOB_STATUS, id.cluster, id.proc);		
			break;
		default:
			dprintf(D_ALWAYS, "error: Unknown %s of %d on %d.%d\n",
					ATTR_LAST_JOB_STATUS, value, id.cluster, id.proc);
			break;
		}
	} else if (strcasecmp(attr, ATTR_JOB_STATUS) == 0) {
		switch (value) {
		case UNEXPANDED:
				// Nothing to do for UNEXPANDED, should never see it
			dprintf(D_FULLDEBUG,
					"WARNING: Encountered a %s of UNEXPANDED on %d.%d\n",
					ATTR_JOB_STATUS, id.cluster, id.proc);		
			break;
		case IDLE:
			mgmtObject->inc_Idle();
			break;
		case RUNNING:
			mgmtObject->inc_Running();
			break;
		case REMOVED:
			mgmtObject->inc_Removed();
			break;
		case COMPLETED:
			mgmtObject->inc_Completed();
			break;
		case HELD:
			mgmtObject->inc_Held();
			break;
		case SUBMISSION_ERR:
				// Nothing to do for SUBMISSION_ERR, should never see it
			dprintf(D_FULLDEBUG,
					"WARNING: Encountered a %s of SUBMISSION_ERR on %d.%d\n",
					ATTR_JOB_STATUS, id.cluster, id.proc);		
			break;
		default:
			dprintf(D_ALWAYS, "error: Unknown %s of %d on %d.%d\n",
					ATTR_JOB_STATUS, value, id.cluster, id.proc);
			break;
		}
	}
}

Manageable::status_t
SubmissionObject::GetJobSummaries ( Variant::List &jobs,
                            std::string &text )
{
	ClassAd *ad = NULL;
	MyString constraint;
	// id, timestamp (which?), command, args, ins, outs, state, message
	const char *ATTRS[] = {ATTR_CLUSTER_ID,
			ATTR_PROC_ID,
			ATTR_GLOBAL_JOB_ID,
			ATTR_Q_DATE,
			ATTR_ENTERED_CURRENT_STATUS,
			ATTR_JOB_STATUS,
			ATTR_JOB_CMD,
			ATTR_JOB_ARGUMENTS1,
			ATTR_JOB_ARGUMENTS2,
			ATTR_RELEASE_REASON,
			ATTR_HOLD_REASON,
			NULL
			};

	constraint.sprintf("%s == \"%s\"",
					   ATTR_JOB_SUBMISSION, this->m_name.c_str());

	dprintf(D_FULLDEBUG,"GetJobSummaries for submission: %s\n",constraint.Value());

	Variant::Map job;
	int init_scan = 1;
	while (NULL != (ad = GetNextJobByConstraint(constraint.Value(), init_scan))) {

		// debug
//		if (DebugFlags & D_FULLDEBUG) {
//			ad->dPrint(D_FULLDEBUG|D_NOHEADER);
//		}

		for (int i = 0; NULL != ATTRS[i]; i++) {
			if (!AddAttribute(*ad, ATTRS[i], job)) {
				dprintf(D_FULLDEBUG,"Warning: %s attribute not found for job of %s\n",ATTRS[i],constraint.Value());
			}
		}

		jobs.push_back(job);
		init_scan = 0;

		// debug
//		if (DebugFlags & D_FULLDEBUG) {
//			std::ostringstream oss;
//			oss << jobs;
//			dprintf(D_FULLDEBUG|D_NOHEADER, "%s\n",oss.str().c_str());
//		}

		FreeJobAd(ad);
		ad = NULL;
	}

	return STATUS_OK;
}

Manageable::status_t
SubmissionObject::ManagementMethod ( uint32_t methodId,
                                     Args &args,
                                     std::string &text )
{
    switch ( methodId )
    {
	case qmf::com::redhat::grid::Submission::METHOD_ECHO:
            return STATUS_OK;
        case qmf::com::redhat::grid::Submission::METHOD_GETJOBSUMMARIES:
            return GetJobSummaries ( ( ( ArgsSubmissionGetJobSummaries & ) args ).o_Jobs,
                             text );
    }

    return STATUS_NOT_IMPLEMENTED;
}
