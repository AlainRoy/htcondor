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
#ifndef _STARTD_RESOURCE_H
#define _STARTD_RESOURCE_H

#include "ResAttributes.h"
#include "ResState.h"
#include "Starter.h"
#include "claim.h"
#include "Reqexp.h"
#include "LoadQueue.h"
#include "AvailStats.h"
#include "cod_mgr.h"


class Resource : public Service
{
public:
	Resource( CpuAttributes*, int );
	~Resource();

		// Public methods that can be called from command handlers
	int		release_claim( void );	// Gracefully kill starter and release claim
	int		kill_claim( void );		// Quickly kill starter and release claim
	int		got_alive( void );		// You got a keep alive command
	int 	periodic_checkpoint( void );	// Do a periodic checkpoint

		// Multi-shadow wants to run more processes.  Send a SIGHUP to
		// the starter
	int		request_new_proc( void );

		// Gracefully kill starter but keep claim	
	int		deactivate_claim( void );	

		// Quickly kill starter but keep claim
	int		deactivate_claim_forcibly( void );

		// Shutdown methods that deal w/ opportunistic *and* COD claims
	int		shutdownAllClaims( bool graceful );
	int		releaseAllClaims( void );
	int		killAllClaims( void );

		// Resource state methods
	void	set_destination_state( State s ) { r_state->set_destination(s);};
	State	destination_state( void ) {return r_state->destination();};
	int		change_state( State s ) {return r_state->change(s);};
	int		change_state( Activity a) {return r_state->change(a);};
	int		change_state( State s , Activity a ) {return r_state->change(s, a);};
	State		state( void )		{return r_state->state();};
	Activity	activity( void )	{return r_state->activity();};
	int		eval_state( void )		{return r_state->eval();};
	bool	hasOppClaim( void );
	bool	hasAnyClaim( void );
	bool	isDeactivating( void )	{return r_cur->isDeactivating();};

		// Methods for computing and publishing resource attributes 
	void	compute( amask_t mask);
	void	publish( ClassAd*, amask_t );

		// Load Average related methods
	float	condor_load( void ) {return r_attr->condor_load();};
	float	compute_condor_load( void );
	void	resize_load_queue( void ); 
	float	owner_load( void ) {return r_attr->owner_load();};
	void	set_owner_load( float val ) {r_attr->set_owner_load(val);};
	void	compute_cpu_busy( void );
	time_t	cpu_busy_time( void );

	void	display( amask_t m ) {r_attr->display(m);}

		// dprintf() functions add the CPU id to the header of each
		// message for SMP startds (single CPU machines get no special
		// header and it works just like regular dprintf())
	void	dprintf( int, char*, ... );
	void	dprintf_va( int, char*, va_list );

		// Helper functions to log that we're ignoring a command that
		// came in while we were in an unexpected state, or while
		// we're shutting down.
	void	log_ignore( int cmd, State s );
	void	log_ignore( int cmd, State s, Activity a );
	void	log_shutdown_ignore( int cmd );

		// Return a pointer to the Claim object whose starter has the
		// given pid.  
	Claim*	findClaimByPid( pid_t starter_pid );

		// Return a pointer to the Claim object with the given ClaimId
	Claim*	findClaimById( const char* id );

	bool	claimIsActive( void ); 

	Claim*	newCODClaim( void );

		// Called when the starter of one of our claims exits
	void	starterExited( Claim* cur_claim );	

		// Since the preempting state is so weird, and when we want to
		// leave it, we need to decide where we want to go, and we
		// have to do lots of funky twiddling with our claim objects,
		// we put all the actions and logic in one place that gets
		// called whenever we're finally ready to leave the preempting
		// state. 
	void	leave_preempting_state( void );

		// Methods to initialize and refresh the resource classads.
	int		init_classad( void );		
	void	refresh_classad( amask_t mask );	
	int		force_benchmark( void );

	int		update( void );		// Schedule to update the central manager.
	int		do_update( void );			// Actually update the CM
	int		eval_and_update( void );	// Evaluate state and update CM. 
	void	final_update( void );		// Send a final update to the CM
									    // with Requirements = False.

 		// Helper functions to evaluate resource expressions
	int		wants_vacate( void );		// EXCEPT's on undefined
	int		wants_suspend( void );		// EXCEPT's on undefined
	int		wants_pckpt( void );		// Defaults to FALSE on undefined
	int		eval_kill( void );			// EXCEPT's on undefined
	int		eval_preempt( void );		// EXCEPT's on undefined
	int		eval_suspend( void );		// EXCEPT's on undefined
	int		eval_continue( void );		// EXCEPT's on undefined
	int		eval_is_owner( void );		// EXCEPT's on undefined
	int		eval_start( void );			// returns -1 on undefined
	int		eval_cpu_busy( void );		// returns FALSE on undefined

		// Data members
	ResState*		r_state;	// Startd state object, contains state and activity
	ClassAd*		r_classad;	// Resource classad (contains everything in config file)
	Claim*			r_cur;		// Info about the current claim
	Claim*			r_pre;		// Info about the possibly preempting claim
	CODMgr*			r_cod_mgr;	// Object to manage COD claims
	Reqexp*			r_reqexp;   // Object for the requirements expression
	CpuAttributes*	r_attr;		// Attributes of this resource
	LoadQueue*		r_load_queue;  // Holds 1 minute avg % cpu usage
	char*			r_name;		// Name of this resource
	int				r_id;		// CPU id of this resource (int form)
	char*			r_id_str;	// CPU id of this resource (string form)
	AvailStats		r_avail_stats; // computes resource availability stats

	int				type( void ) { return r_attr->type(); };

private:
	int			update_tid;	// DaemonCore timer id for update delay
	unsigned	update_sequence;	// Update sequence number

	int		fast_shutdown;	// Flag set if we're in fast shutdown mode.
	void	remove_pre( void );	// If r_pre is set, refuse and delete it.
	int		r_cpu_busy;
	time_t	r_cpu_busy_start_time;
};

#endif /* _STARTD_RESOURCE_H */
