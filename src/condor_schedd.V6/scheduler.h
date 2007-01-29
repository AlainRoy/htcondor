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
// //////////////////////////////////////////////////////////////////////
//
// condor_sched.h
//
// Define class Scheduler. This class does local scheduling and then negotiates
// with the central manager to obtain resources for jobs.
//
// //////////////////////////////////////////////////////////////////////

#ifndef _CONDOR_SCHED_H_
#define _CONDOR_SCHED_H_

#include "dc_collector.h"
#include "daemon.h"
#include "daemon_list.h"
#include "condor_classad.h"
#include "condor_io.h"
#include "proc.h"
#include "prio_rec.h"
#include "HashTable.h"
#include "string_list.h"
#include "list.h"
#include "classad_hashtable.h"	// for HashKey class
#include "Queue.h"
#include "user_log.c++.h"
#include "autocluster.h"
#include "shadow_mgr.h"
#include "enum_utils.h"
#include "self_draining_queue.h"
#include "schedd_cronmgr.h"
#include "condor_classad_namedlist.h"
#include "env.h"
//#include "condor_crontab.h"

extern  int         STARTD_CONTACT_TIMEOUT;
const	int			NEGOTIATOR_CONTACT_TIMEOUT = 30;

extern	DLL_IMPORT_MAGIC char**		environ;

class match_rec;

struct shadow_rec
{
    int             pid;
    PROC_ID         job_id;
	int				universe;
    match_rec*	    match;
    int             preempted;
    int             conn_fd;
	int				removed;
	bool			isZombie;	// added for Maui by stolley
	bool			is_reconnect;
}; 

struct OwnerData {
  char* Name;
  char* Domain;
  int JobsRunning;
  int JobsIdle;
  int JobsHeld;
  int JobsFlocked;
  int FlockLevel;
  int OldFlockLevel;
  time_t NegotiationTimestamp;
  OwnerData() { Name=NULL; Domain=NULL;
  JobsRunning=JobsIdle=JobsHeld=JobsFlocked=FlockLevel=OldFlockLevel=0; }
};

class match_rec
{
 public:
    match_rec(char*, char*, PROC_ID*, const ClassAd*, char*, char* pool);
	~match_rec();
    char*    		id;
    char*   		peer;
	
		// cluster of the job we used to obtain the match
	int				origcluster; 

		// if match is currently active, cluster and proc of the
		// running job associated with this match; otherwise,
		// cluster==origcluster and proc==-1
		// NOTE: use SetMrecJobID() to change cluster and proc,
		// because this updates the index of matches by job id.
    int     		cluster;
    int     		proc;

    int     		status;
	shadow_rec*		shadowRec;
	int				alive_countdown;
	int				num_exceptions;
	int				entered_current_status;
	ClassAd*		my_match_ad;
	char*			user;
	char*			pool;		// negotiator hostname if flocking; else NULL
	bool			sent_alive_interval;
	bool			allocated;	// For use by the DedicatedScheduler
	bool			scheduled;	// For use by the DedicatedScheduler

		// Set the mrec status to the given value (also updates
		// entered_current_status)
	void	setStatus( int stat );
};

class UserIdentity {
	public:
			// The default constructor is not recommended as it
			// has no real identity.  It only exists so
			// we can put UserIdentities in various templates.
		UserIdentity() : m_username(""), m_domain("") { }
		UserIdentity(const char * user, const char * domainname)
			: m_username(user), m_domain(domainname) { }
		UserIdentity(const UserIdentity & src)
			: m_username(src.m_username), m_domain(src.m_domain) { }
		const UserIdentity & operator=(const UserIdentity & src) {
			m_username = src.m_username;
			m_domain = src.m_domain;
			return *this;
		}
		const bool operator==(const UserIdentity & rhs) {
			return m_username == rhs.m_username && m_domain == rhs.m_domain;
		}
		MyString username() const { return m_username; }
		MyString domain() const { return m_domain; }

			// For use in HashTables
		static int HashFcn(const UserIdentity & index, int numBuckets);
	private:
		MyString m_username;
		MyString m_domain;
};


struct GridJobCounts {
	GridJobCounts() : GridJobs(0), UnmanagedGridJobs(0) { }
	unsigned int GridJobs;
	unsigned int UnmanagedGridJobs;
};

enum MrecStatus {
    M_UNCLAIMED,
	M_CONNECTING,
	M_STARTD_CONTACT_LIMBO,  // after contacting startd; before recv'ing reply
	M_CLAIMED,
    M_ACTIVE
};

	
typedef enum {
	NO_SHADOW_STD,
	NO_SHADOW_JAVA,
	NO_SHADOW_WIN32,
	NO_SHADOW_DC_VANILLA,
	NO_SHADOW_OLD_VANILLA,
	NO_SHADOW_RECONNECT,
	NO_SHADOW_MPI,
} NoShadowFailure_t;


// These are the args to contactStartd that get stored in the queue.
class ContactStartdArgs
{
public:
	ContactStartdArgs( char* the_claim_id, char* sinful, bool is_dedicated );
	~ContactStartdArgs();

	char*		claimId( void )		{ return csa_claim_id; };
	char*		sinful( void )		{ return csa_sinful; }
	bool		isDedicated()		{ return csa_is_dedicated; }

private:
	char *csa_claim_id;
	char *csa_sinful;
	bool csa_is_dedicated;
};


class Scheduler : public Service
{
  public:
	
	Scheduler();
	~Scheduler();
	
	// initialization
	void			Init();
	void			Register();
	void			RegisterTimers();

	// maintainence
	void			timeout(); 
	void			reconfig();
	void			shutdown_fast();
	void			shutdown_graceful();
	void			schedd_exit();
	void			invalidate_ads();
	void			update_local_ad_file();
	
	// negotiation
	int				doNegotiate(int, Stream *);
	int				negotiatorSocketHandler(Stream *);
	int				delayedNegotiatorHandler(Stream *);
	int				negotiate(int, Stream *);
	int				reschedule_negotiator(int, Stream *);
	int				reschedule_negotiator_timer() { return reschedule_negotiator(0, NULL); }
	void			vacate_service(int, Stream *);
	AutoCluster		autocluster;
	void			sendReschedule( bool checkRecent = true );

	// job managing
	int				abort_job(int, Stream *);
	void			send_all_jobs(ReliSock*, struct sockaddr_in*);
	void			send_all_jobs_prioritized(ReliSock*, struct sockaddr_in*);
	friend	int		count(ClassAd *);
	friend	void	job_prio(ClassAd *);
	friend  int		find_idle_local_jobs(ClassAd *);
	//friend	int		calculateCronSchedule( ClassAd* );
	void			display_shadow_recs();
	int				actOnJobs(int, Stream *);
	int				updateGSICred(int, Stream* s);
	int				spoolJobFiles(int, Stream *);
	static int		spoolJobFilesWorkerThread(void *, Stream *);
	static int		transferJobFilesWorkerThread(void *, Stream *);
	static int		generalJobFilesWorkerThread(void *, Stream *);
	int				spoolJobFilesReaper(int,int);	
	int				transferJobFilesReaper(int,int);
	void			PeriodicExprHandler( void );

	// match managing
	int 			publish( ClassAd *ad );
    match_rec*      AddMrec(char*, char*, PROC_ID*, const ClassAd*, char*, char*);
    int         	DelMrec(char*);
    int         	DelMrec(match_rec*);
	match_rec*      FindMrecByJobID(PROC_ID);
	void            SetMrecJobID(match_rec *rec, int cluster, int proc);
	void            SetMrecJobID(match_rec *match, PROC_ID job_id);
	shadow_rec*		FindSrecByPid(int);
	shadow_rec*		FindSrecByProcID(PROC_ID);
	void			RemoveShadowRecFromMrec(shadow_rec*);
	void            sendSignalToShadow(pid_t pid,int sig,PROC_ID proc);
	int				AlreadyMatched(PROC_ID*);
	void			StartJobs();
	void			StartLocalJobs();
	void			sendAlives();
	void			RecomputeAliveInterval(int cluster, int proc);
	void			StartJobHandler();
	void			addRunnableJob( shadow_rec* );
	void			spawnShadow( shadow_rec* );
	void			spawnLocalStarter( shadow_rec* );
	bool			claimLocalStartd();
	bool			isStillRunnable( int cluster, int proc, int &status ); 
	UserLog*		InitializeUserLog( PROC_ID job_id );
	bool			WriteAbortToUserLog( PROC_ID job_id );
	bool			WriteHoldToUserLog( PROC_ID job_id );
	bool			WriteReleaseToUserLog( PROC_ID job_id );
	bool			WriteExecuteToUserLog( PROC_ID job_id, const char* sinful = NULL );
	bool			WriteEvictToUserLog( PROC_ID job_id, bool checkpointed = false );
	bool			WriteTerminateToUserLog( PROC_ID job_id, int status );
	bool			WriteRequeueToUserLog( PROC_ID job_id, int status, const char * reason );
#ifdef WANT_NETMAN
	void			RequestBandwidth(int cluster, int proc, match_rec *rec);
#endif

		// Public startd socket management functions
	void			addRegContact( void ) { num_reg_contacts++; };
	void			delRegContact( void ) { num_reg_contacts--; };
	int				numRegContacts( void ) { return num_reg_contacts; };
	void            checkContactQueue();

		/** Used to enqueue another set of information we need to use
			to contact a startd.  This is called by both
			Scheduler::negotiate() and DedicatedScheduler::negotiate()
			once a match is made.  This function will also start a
			timer to check the contact queue, if needed.
			@param args A structure that holds all the information
			@return true on success, false on failure (to enqueue).
		*/
	bool			enqueueStartdContact( ContactStartdArgs* args );

		/** Registered in contactStartdConnected, this function is called when
			the startd replies to our request.  If it replies in the 
			positive, the mrec->status is set to M_ACTIVE, else the 
			mrec gets deleted.  The 'sock' is de-registered and deleted
			before this function returns.
			@param sock The sock with the startd's reply.
			@return FALSE on denial/problems, TRUE on success
		*/
	int             startdContactSockHandler( Stream *sock );

		/** Registered in contactStartd, this function is called when
			the non-blocking connection is opened.
			@param sock The sock with the connection to the startd.
			@return FALSE on denial/problems, KEEP_STREAM on success
		*/
	int startdContactConnectHandler( Stream *sock );

		/** Used to enqueue another disconnected job that we need to
			spawn a shadow to attempt to reconnect to.
		*/
	bool			enqueueReconnectJob( PROC_ID job );
	void			checkReconnectQueue( void );
	void			makeReconnectRecords( PROC_ID* job, const ClassAd* match_ad );

	bool	spawnJobHandler( int cluster, int proc, shadow_rec* srec );
	bool 	enqueueFinishedJob( int cluster, int proc );


		// Useful public info
	char*			shadowSockSinful( void ) { return MyShadowSockName; };
	char*			dcSockSinful( void ) { return MySockName; };
	int				aliveInterval( void ) { return alive_interval; };
	char*			uidDomain( void ) { return UidDomain; };
	int				getJobsTotalAds() { return JobsTotalAds; };
	int				getMaxJobsSubmitted() { return MaxJobsSubmitted; };

		// Used by the DedicatedScheduler class
	void 			swap_space_exhausted();
	void			delete_shadow_rec(int);
	void			delete_shadow_rec(shadow_rec*);
	shadow_rec*     add_shadow_rec( int pid, PROC_ID*, int univ, match_rec*,
									int fd );
	shadow_rec*		add_shadow_rec(shadow_rec*);
	void			add_shadow_rec_pid(shadow_rec*);
	void			HadException( match_rec* );

		// Used to manipulate the "extra ads" (read:Hawkeye)
	int adlist_register( const char *name );
	int adlist_replace( const char *name, ClassAd *newAd );
	int adlist_delete( const char *name );
	int adlist_publish( ClassAd *resAd );

		// Used by both the Scheduler and DedicatedScheduler during
		// negotiation
	bool canSpawnShadow( int started_jobs, int total_jobs );  
	void addActiveShadows( int num ) { CurNumActiveShadows += num; };

	int				shadow_prio_recs_consistent();
	void			mail_problem_message();
	
	// info about our central manager
	CollectorList* 	Collectors;

		// ckireyev: we can no longer stash the Negotiator object
		// since the negotiator host may change on the fly
	//Daemon*			Negotiator;

		// object to manage our various shadows and their ClassAds
	ShadowMgr shadow_mgr;

		// hashtable used to hold matching ClassAds for Globus Universe
		// jobs which desire matchmaking.
	HashTable <PROC_ID, ClassAd *> *resourcesByProcID;
  
private:
	
	// information about this scheduler
	ClassAd*		ad;
	char*			MySockName;		// dhaval
	Scheduler*		myself;

	// information about the command port which Shadows use
	char*			MyShadowSockName;
	ReliSock*		shadowCommandrsock;
	SafeSock*		shadowCommandssock;

	// The "Cron" manager (Hawkeye) & it's classads
	ScheddCronMgr	*CronMgr;
	NamedClassAdList extra_ads;

	// parameters controling the scheduling and starting shadow
	int				SchedDInterval;
	int				SchedDMinInterval;
	int				QueueCleanInterval;
	int				PeriodicExprInterval;
	int             RequestClaimTimeout;
	int				JobStartDelay;
	int				JobStartCount;
	int				JobsThisBurst;
	int				MaxJobsRunning;
	char*			StartLocalUniverse; // expression for local jobs
	char*			StartSchedulerUniverse; // expression for scheduler jobs
	int				MaxJobsSubmitted;
	bool			NegotiateAllJobsInCluster;
	int				JobsStarted; // # of jobs started last negotiating session
	int				SwapSpace;	 // available at beginning of last session
	int				ShadowSizeEstimate;	// Estimate of swap needed to run a job
	int				SwapSpaceExhausted;	// True if job died due to lack of swap space
	int				ReservedSwap;		// for non-condor users
	int				MaxShadowsForSwap;
	int				CurNumActiveShadows;
	int				JobsIdle; 
	int				JobsRunning;
	int				JobsHeld;
	int				JobsTotalAds;
	int				JobsFlocked;
	int				JobsRemoved;
	int				SchedUniverseJobsIdle;
	int				SchedUniverseJobsRunning;
	int				LocalUniverseJobsIdle;
	int				LocalUniverseJobsRunning;
	char*			LocalUnivExecuteDir;
	int				BadCluster;
	int				BadProc;
	ExtArray<OwnerData> Owners; // May be tracking AccountingGroup instead of owner username/domain
	HashTable<UserIdentity, GridJobCounts> GridJobOwners;
	int				N_Owners;
	time_t			NegotiationRequestTime;
	int				ExitWhenDone;  // Flag set for graceful shutdown
	Queue<shadow_rec*> RunnableJobQueue;
	int				StartJobTimer;
	int				timeoutid;		// daemoncore timer id for timeout()
	int				startjobsid;	// daemoncore timer id for StartJobs()

	int				shadowReaperId; // daemoncore reaper id for shadows

	int             startJobsDelayBit;  // for delay when starting jobs.

		// used so that we don't register too many Sockets at once & fd panic
	int             num_reg_contacts;  
		// Here we enqueue calls to 'contactStartd' when we can't just 
		// call it any more.  See contactStartd and the call to it...
	Queue<ContactStartdArgs*> startdContactQueue;
	int				checkContactQueue_tid;	// DC Timer ID to check queue

		// If we we need to reconnect to disconnected starters, we
		// stash the proc IDs in here while we read through the job
		// queue.  Then, we can spawn all the shadows after the fact. 
	SimpleList<PROC_ID> jobsToReconnect;
	int				checkReconnectQueue_tid;

	SelfDrainingQueue job_is_finished_queue;
	int jobIsFinishedHandler( ServiceData* job_id );

		// Get the associated GridJobCounts object for a given
		// user identity.  If necessary, will create a new one for you.
		// You can read or write the values, but don't go
		// deleting the pointer!
	GridJobCounts * GetGridJobCounts(UserIdentity user_identity);

	// useful names
	char*			CondorAdministrator;
	char*			Mail;
	char*			filename;					// save UpDown object
	char*			AccountantName;
    char*			UidDomain;

	time_t last_reschedule_request;

	// connection variables
	struct sockaddr_in	From;
	int					Len; 

	// utility functions
	int				count_jobs();
	void   			check_claim_request_timeouts( void );
	int				insert_owner(char*);
	void			child_exit(int, int);
	void			scheduler_univ_job_exit(int pid, int status, shadow_rec * srec);
	void			scheduler_univ_job_leave_queue(PROC_ID job_id, int status, shadow_rec * srec);
	void			clean_shadow_recs();
	void			preempt( int n, bool force_sched_jobs = false );
	void			attempt_shutdown();
	static void		refuse( Stream* s );
	void			tryNextJob( int secs = -1 );
	int				jobThrottle( void );
	void			initLocalStarterDir( void );
	void	noShadowForJob( shadow_rec* srec, NoShadowFailure_t why );
		//
		// This method will insert the next runtime for a 
		// job into its requirements
		//
	//bool calculateCronSchedule( ClassAd*, bool force = false );


		/** We begin the process of opening a non-blocking ReliSock
			connection to the startd.

			We push the ClaimId and the jobAd, then register
			the Socket with startdContactSockHandler and put the new mrec
			into the daemonCore data pointer.  
			@param args An object that holds all the info we care about 
			@return false on failure and true on success
		 */
	void	contactStartd( ContactStartdArgs* args );

	shadow_rec*		StartJob(match_rec*, PROC_ID*);
	shadow_rec*		start_std(match_rec*, PROC_ID*, int univ);
	shadow_rec*		start_pvm(match_rec*, PROC_ID*);
	shadow_rec*		start_sched_universe_job(PROC_ID*);
	shadow_rec*		start_local_universe_job(PROC_ID*);
	bool			spawnJobHandlerRaw( shadow_rec* srec, const char* path,
										ArgList const &args,
										Env const *env, 
										const char* name, bool is_dc,
										bool wants_pipe );
	void			Relinquish(match_rec*);
	void			check_zombie(int, PROC_ID*);
	void			kill_zombie(int, PROC_ID*);
	int				is_alive(shadow_rec* srec);
	shadow_rec*     find_shadow_rec(PROC_ID*);
	void			NotifyUser(shadow_rec*, char*, int, int);
	
#ifdef CARMI_OPS
	shadow_rec*		find_shadow_by_cluster( PROC_ID * );
#endif

	void			expand_mpi_procs(StringList *, StringList *);

	HashTable <HashKey, match_rec *> *matches;
	HashTable <PROC_ID, match_rec *> *matchesByJobID;
	HashTable <int, shadow_rec *> *shadowsByPid;
	HashTable <PROC_ID, shadow_rec *> *shadowsByProcID;
	HashTable <int, ExtArray<PROC_ID> *> *spoolJobFileWorkers;
	int				numMatches;
	int				numShadows;
	DaemonList		*FlockCollectors, *FlockNegotiators;
	int				MaxFlockLevel;
	int				FlockLevel;
    int         	alive_interval;  // how often to broadcast alive
		// leaseAliveInterval is the minimum interval we need to send
		// keepalives based upon ATTR_JOB_LEASE_DURATION...
	int				leaseAliveInterval;  
	int				aliveid;	// timer id for sending keepalives to startd
	int				MaxExceptions;	 // Max shadow excep. before we relinquish
	bool			ManageBandwidth;

		//
		// This table is used for scheduling cron jobs
		// If a ClassAd has an entry in this table then we need
		// to query the CronTab object to ask it what the next
		// runtime is for job is
		//
	//HashTable<PROC_ID, CronTab*> *cronTabs;
		//
		// We also keep a list of job's that do not need a cronTab
		// This way we can quickly look at this to see if we 
		// should skip it when trying to figure out cron schedules
		// Hopefully, a lookup in this table should be faster
		// then checking to see if a CronTab is needed
		//
	//Queue<PROC_ID> *cronTabsExclude;

		// put state into ClassAd return it.  Used for condor_squawk
	int	dumpState(int, Stream *);

		// A bit that says wether or not we've sent email to the admin
		// about a shadow not starting.
	int sent_shadow_failure_email;

	// some stuff about Quill that should go into the ad
#if WANT_QUILL
	int quill_enabled;
	int quill_is_remotely_queryable;
	char *quill_name;
	char *quill_db_name;
	char *quill_db_ip_addr;
	char *quill_db_query_password;
#endif

};


// Other prototypes
int		get_job_prio(ClassAd *ad, bool compute_autoclusters = false);
extern void set_job_status(int cluster, int proc, int status);
extern bool claimStartd( match_rec* mrec, ClassAd* job_ad, bool is_dedicated );
extern bool claimStartdConnected( Sock *sock, match_rec* mrec, ClassAd* job_ad, bool is_dedicated );
extern bool sendAlive( match_rec* mrec );
extern void fixReasonAttrs( PROC_ID job_id, JobAction action );
extern bool moveStrAttr( PROC_ID job_id, const char* old_attr,  
						 const char* new_attr, bool verbose );
extern bool moveIntAttr( PROC_ID job_id, const char* old_attr,  
						 const char* new_attr, bool verbose );
extern bool abortJob( int cluster, int proc, const char *reason, bool use_transaction );
extern bool abortJobsByConstraint( const char *constraint, const char *reason, bool use_transaction );
extern bool holdJob( int cluster, int proc, const char* reason = NULL, 
					 bool use_transaction = false, 
					 bool notify_shadow = true,  
					 bool email_user = false, bool email_admin = false,
					 bool system_hold = true);
extern bool releaseJob( int cluster, int proc, const char* reason = NULL, 
					 bool use_transaction = false, 
					 bool email_user = false, bool email_admin = false,
					 bool write_to_user_log = true);


/** Hook to call whenever we're going to give a job to a "job
	handler", be that a shadow, starter (local univ), or gridmanager.
	it takes the optional shadow record as a void* so this can be
	seamlessly used for Create_Thread_With_Data().  In fact, we don't
	need the srec at all for the function itself, but we'll need it as
	our data pointer in the reaper (so we can actually spawn the right
	handler), so we ignore it here, but will need it later...
*/
int aboutToSpawnJobHandler( int cluster, int proc, void* srec=NULL );


/** For use as a reaper with Create_Thread_With_Data(), or to call
	directly after aboutToSpawnJobHandler() if there's no thread. 
*/
int aboutToSpawnJobHandlerDone( int cluster, int proc, void* srec=NULL,
								int exit_status = 0 );


/** A helper function that wraps the call to jobPrepNeedsThread() and
	invokes aboutToSpawnJobHandler() as appropriate, either in its own
	thread using Create_Thread_Qith_Wata(), or calling it and then
	aboutToSpawnJobHandlerDone() directly.
*/
void callAboutToSpawnJobHandler( int cluster, int proc, shadow_rec* srec );


/** Hook to call whenever a job enters a "finished" state, something
	it can never get out of (namely, COMPLETED or REMOVED).  Like the
	aboutToSpawnJobHandler() hook above, this might be expensive, so
	if jobPrepNeedsThread() returns TRUE for this job, we should call
	this hook within a seperate thread.  Therefore, it takes the
	optional void* so it can be used for Create_Thread_With_Data().
*/
int jobIsFinished( int cluster, int proc, void* vptr = NULL );


/** For use as a reaper with Create_Thread_With_Data(), or to call
	directly after jobIsFinished() if there's no thread. 
*/
int jobIsFinishedDone( int cluster, int proc, void* vptr = NULL,
					   int exit_status = 0 );


#endif /* _CONDOR_SCHED_H_ */
