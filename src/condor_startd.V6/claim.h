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
/*  
	This file defines the following classes:

	Claim, Capability, and Client

	A Claim object contains all of the information a startd needs
    about a given claim, such as the capability, the client of this
    claim, etc.  The capability in the claim is just a pointer to a
    Capability object.  The client is also just a pointer to a Client
    object.  The startd maintains two Claim objects in the "rip", the
    per-resource information structure.  One for the current claim,
    and one for the possibly preempting claim that is pending.
 
	A Capability object contains the capability string, and some
	functions to manipulate and compare against this string.  The
	constructor generates a new capability with the following form:
	<ip:port>#random_integer

	A Client object contains all the info about a given client of a
	startd.  In particular, the client name (a.k.a. "user"), the
	address ("<www.xxx.yyy.zzz:pppp>" formed ip/port), and the
	hostname.

	Originally written 9/29/97 by Derek Wright <wright@cs.wisc.edu> 

	Decided the Match object should really be called "Claim" (the
	files were renamed in cvs from Match.[Ch] to claim.[Ch], and
	renamed everything on 1/10/03 - Derek Wright
*/

#ifndef _CLAIM_H
#define _CLAIM_H

#include "enum_utils.h"
#include "Starter.h"
class CODMgr;


class Capability
{
public:
	Capability( bool is_cod = false );
	~Capability();

	char*	id() {return c_id;};
	char*	capab() {return c_id;};
	bool	matches( const char* capab );

private:
	char*	c_id;	// capability string
};


class Client
{
public:
	Client();
	~Client();

	char*	name()	{return c_user;};	// For compatibility only
	char*	user()	{return c_user;};
	char*	owner()	{return c_owner;};
	char*	host()	{return c_host;};
	char*	addr() 	{return c_addr;};

	void	setuser(const char* user);
	void	setowner(const char* owner);
	void	setaddr(const char* addr);
	void	sethost(const char* host);

		// send a message to the client and accountant that the claim
		// is a being vacated
	void	vacate(char* cap);
private:
	char	*c_owner;	// name of the owner
	char	*c_user;	// name of the user
	char	*c_host;	// hostname of the clientmachine
	char	*c_addr;	// <ip:port> of the client
};


class Claim : public Service
{
public:
	Claim( Resource*, bool is_cod = false );
	~Claim();

		// Operations you can perform on a Claim
	void vacate();	// Send a vacate command to the client of this claim
	void alive();	// Process a keep alive for this claim

	void publish( ClassAd*, amask_t );

	void dprintf( int, char* ... );

	void refuseClaimRequest();

		// Timer functions
	void start_match_timer();
	void cancel_match_timer();
	int  match_timed_out();		// We were matched, but not claimed in time
	void start_claim_timer();
	void cancel_claim_timer();
	int  claim_timed_out(); 	// We were claimed, but didn't get a
								// keep alive in time from the schedd

		// Functions that return data
	float		rank()			{return c_rank;};
	float		oldrank()		{return c_oldrank;};
	char*		id();
	char*		capab();
	Client* 	client() 		{return c_client;};
	Capability* cap()			{return c_cap;};
	ClassAd*	ad() 			{return c_ad;};
	int			universe()		{return c_universe;};
	Stream*		requestStream()	{return c_request_stream;};
	int			cluster()		{return c_cluster;};
	int			proc()			{return c_proc;};
	int			job_start() 	{return c_job_start;};
	int			last_pckpt() 	{return c_last_pckpt;};
	int			getaliveint()	{return c_aliveint;};
	ClaimState	state()			{return c_state;};
	float		percentCpuUsage( void );
	unsigned long	imageSize( void );
	CODMgr*		getCODMgr( void );

		// Functions that set the values of data
	void setrank(float rank)	{c_rank=rank;};
	void setoldrank(float rank) {c_oldrank=rank;};
	void setad(ClassAd *ad);		// Set our ad to the given pointer
	void deletead(void);
	void setuniverse(int universe)	{c_universe=universe;};
	void setRequestStream(Stream* stream);	
	void setaliveint(int alive)		{c_aliveint=alive;};
	void setproc(int proc) 			{c_proc=proc;};
	void setcluster(int cluster)	{c_cluster=cluster;};
	void setlastpckpt(int lastckpt) {c_last_pckpt=lastckpt;};
	void setjobstart(int jobstart) 	{c_job_start=jobstart;};

		// starter-related functions
	int	 spawnStarter( start_info_t*, time_t );
	void setStarter( Starter* s );
	void starterExited( void );
	bool starterPidMatches( pid_t starter_pid );
	bool isDeactivating( void );
	bool isActive( void );
	bool deactivateClaim( bool graceful );
	bool suspendClaim( void );
	bool resumeClaim( void );
	bool starterKill( int sig );
	bool starterKillPg( int sig );
	bool starterKillSoft( void );
	bool starterKillHard( void );

	bool periodicCheckpoint( void );

	bool ownerMatches( const char* owner );

	bool wantsRelease( void )			{ return c_wants_release; };
	void setWantsRelease( bool val )	{ c_wants_release=val; }; 


private:
	Resource	*c_rip;
	Client 		*c_client;
	Capability 	*c_cap;
	ClassAd*	c_ad;
	Starter*	c_starter;
	float		c_rank;
	float		c_oldrank;
	int			c_universe;
	int			c_proc;
	int			c_cluster;
	int			c_job_start;
	int			c_last_pckpt;
	Stream*		c_request_stream; // cedar sock that a remote request
                                  // is waiting for a response on

	int			c_match_tid;	// DaemonCore timer id for this
								// match.  If we're matched but not
								// claimed within 5 minutes, throw out
								// the match.
	int			c_claim_tid;	// DeamonCore timer id for this
								// claim.  If we don't get a keep
								// alive in 3 alive intervals, the
								// schedd has died and we need to
								// release the claim.
	int			c_aliveint;		// Alive interval for this claim
	bool		c_is_cod;       // are we a COD claim or not?

	ClaimState	c_state;		// the state of this claim
	bool		c_wants_release;

};


#endif /* _CLAIM_H */

