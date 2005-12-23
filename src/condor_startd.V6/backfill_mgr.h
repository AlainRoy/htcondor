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
#ifndef _CONDOR_BACKFILL_MGR_H
#define _CONDOR_BACKFILL_MGR_H

#include "condor_common.h"
#include "condor_state.h"
#include "extArray.h"


/**
   BackfillVM base class

   Defines a consistent interface the startd will use to interact with
   a given computational instance of a backfill system on a single VM.
   Depending on the backfill backend, there might be multiple programs
   running, or a single program where resources are given and
   reclaimed dynamically.  In both cases, Condor will still think of
   these compute slots as separate entities, and the BackfillMgr class
   implemented for a given backend system will handle the details.
   Initially there will only be derived classes for BOINC, but
   someday, others might be added, too.
*/

class BackfillVM
{
public:
	BackfillVM( int vm_id );
	virtual ~BackfillVM() = 0;

	int getVMID() { return m_vm_id; };
	virtual bool init() = 0;
	virtual bool start() = 0;
	virtual bool suspend() = 0;
	virtual bool resume() = 0;
	virtual bool softkill() = 0;
	virtual bool hardkill() = 0;

protected:

		/// Protected data
	int        m_vm_id;
	Activity   m_activity;
	time_t     m_entered_current_activity;

		/// Protected methods
	bool enterActivity( Activity act );

};



/**
   BackfillMgr base class

   The BackfillMgr maintains a set of BackfillVM objects, and
   defines a consistent interface the startd will use to interact with
   a given backfill backend system.  Initially, there will only be
   derived classes for BOINC, but someday, others might be added.
*/

class BackfillMgr
{
public:
	BackfillMgr(); 
	virtual ~BackfillMgr() = 0;

	virtual bool init() = 0;
	virtual bool reconfig() = 0;

		// for any of these, using vm_id 0 means all VMs
	virtual bool rmVM( int vm_id ) = 0;
	virtual bool start( int vm_id ) = 0;
	virtual bool suspend( int vm_id ) = 0;
	virtual bool resume( int vm_id ) = 0;
	virtual bool softkill( int vm_id ) = 0;
	virtual bool hardkill( int vm_id ) = 0;

protected:

	ExtArray<BackfillVM*> m_vms;

};



#endif /* _CONDOR_BACKFILL_MGR_H */
