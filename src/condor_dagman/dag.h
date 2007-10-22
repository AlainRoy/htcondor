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

#ifndef DAG_H
#define DAG_H

#include "condor_common.h"
#include "list.h"
#include "job.h"
#include "scriptQ.h"
#include "user_log.c++.h"          /* from condor_c++_util/ directory */
#include "condor_constants.h"      /* from condor_includes/ directory */
#include "HashTable.h"
#include "../condor_daemon_core.V6/condor_daemon_core.h"
#include "read_multiple_logs.h"
#include "check_events.h"
#include "condor_id.h"
#include "prioritysimplelist.h"
#include "throttle_by_category.h"

// NOTE: must be kept in sync with Job::job_type_t
enum Log_source{
  CONDORLOG,
  DAPLOG
};

class Dagman;

//------------------------------------------------------------------------
/** A Dag instance stores information about a job dependency graph,
    and the state of jobs as they are running. Jobs are run by
    submitting them to condor using the UserLog API<p>
    
    The public interface to a Dag allows you to add jobs, add
    dependencies, submit jobs, control the locations of log files,
    query the state of a job, and process any new events that have
    appeared in the condor log file.
*/
class Dag {
  public:
  
    /** Create a DAG
		@param dagFile the DAG file name
		@param condorLogName the log file name specified by the -Condorlog
		       command line argument
        @param maxJobsSubmitted the maximum number of jobs to submit to Condor
               at one time
        @param maxPreScripts the maximum number of PRE scripts to spawn at
		       one time
        @param maxPostScripts the maximum number of POST scripts to spawn at
		       one time
		@param dapLogName the name of the Stork (DaP) log file
		@param allowLogError whether to allow the DAG to run even if we
			   have an error determining the job log files
		@param useDagDir run DAGs in directories from DAG file paths
		       if true
		@param maxIdleJobProcs the maximum number of idle job procss to
			   allow at one time (0 means unlimited)
		@param retrySubmitFirst whether, when a submit fails for a node's
		       job, to put the node at the head of the ready queue
		@param retryNodeFirst whether, when a node fails and has retries,
			   to put the node at the head of the ready queue
		@param condorRmExe executable to remove Condor jobs
		@param storkRmExe executable to remove Stork jobs
		@param DAGManJobId Condor ID of this DAGMan process
		@param prohibitMultiJobs whether submit files queueing multiple
			   job procs are prohibited
		@param submitDepthFirst whether ready nodes should be submitted
			   in depth-first (as opposed to breadth-first) order
    */

    Dag( /* const */ StringList &dagFiles, char *condorLogName,
		 const int maxJobsSubmitted,
		 const int maxPreScripts, const int maxPostScripts, 
		 const char *dapLogName, bool allowLogError,
		 bool useDagDir, int maxIdleJobProcs, bool retrySubmitFirst,
		 bool retryNodeFirst, const char *condorRmExe,
		 const char *storkRmExe, const CondorID *DAGManJobId,
		 bool prohibitMultiJobs, bool submitDepthFirst );

    ///
    ~Dag();

	/** Set the _abortOnScarySubmit value -- controls whether we abort
		the DAG on "scary" submit events.
		@param The abortOnScarySubmit value
	*/
	void SetAbortOnScarySubmit( bool abortOnScarySubmit ) {
		_abortOnScarySubmit = abortOnScarySubmit;
	}

	/** Initialize log files, lock files, etc.
	*/
	void InitializeDagFiles( bool deleteOldLogs );

    /** Prepare DAG for initial run.  Call this function ONLY ONCE.
        @param recovery specifies if this is a recovery
        @return true: successful, false: failure
    */
    bool Bootstrap (bool recovery);

    /// Add a job to the collection of jobs managed by this Dag.
    bool Add( Job& job );
  
    /** Specify a dependency between two jobs. The child job will only
        run after the parent job has finished.
        @param parent The parent job
        @param child The child job (depends on the parent)
        @return true: successful, false: failure
    */
    bool AddDependency (Job * parent, Job * child);
  
    /** Blocks until the Condor Log file grows.
        @return true: log file grew, false: timeout or shrinkage
    */

    
    //    bool DetectLogGrowth();
    bool DetectCondorLogGrowth();         //<--DAP 
    bool DetectDaPLogGrowth();            //<--DAP

    /** Force the Dag to process all new events in the condor log file.
        This may cause the state of some jobs to change.

		@param logsource The type of log from which events should be read.
        @param recover Process Log in Recover Mode, from beginning to end
        @return true on success, false on failure
    */
    bool ProcessLogEvents (int logsource, bool recovery = false); //<--DAP

	/** Process a single event.  Note that this is called every time
			we attempt to read the user log, so we may or may not have
			a valid event here.
		@param The type of log which is the source of the event.
		@param The outcome from the attempt to read the user log.
	    @param The event.
		@param Whether we're in recovery mode.
		@param Whether we're done trying to read events (set by this
			function).
		@return True if the DAG should continue, false if we should abort.
	*/
	bool ProcessOneEvent (int logsource, ULogEventOutcome outcome, const ULogEvent *event,
			bool recovery, bool &done);

	/** Process an abort or executable error event.
	    @param The event.
		@param The job corresponding to this event.
		@param Whether we're in recovery mode.
	*/
	void ProcessAbortEvent(const ULogEvent *event, Job *job,
			bool recovery);

	/** Process a terminated event.
	    @param The event.
		@param The job corresponding to this event.
		@param Whether we're in recovery mode.
	*/
	void ProcessTerminatedEvent(const ULogEvent *event, Job *job,
			bool recovery);

	/** Remove the batch system job for the given DAG node.
		@param The node for which to remove the job.
	*/
	void RemoveBatchJob(Job *node);

	/** Processing common to all "end-of-job proc" events.
		@param The node the event is associated with.
		@param Whether we're in recovery mode.
		@param Whether the job proc failed
	*/
	void ProcessJobProcEnd(Job *node, bool recovery, bool failed);

	/** Process a post script terminated event.
	    @param The event.
		@param The job corresponding to this event.
		@param Whether we're in recovery mode.
	*/
	void ProcessPostTermEvent(const ULogEvent *event, Job *job,
			bool recovery);

	/** Process a submit event.
	    @param The event.
		@param The job corresponding to this event.
		@param Whether we're in recovery mode.
		@param Whether the submit event is considered "sane".
	*/
	void ProcessSubmitEvent(Job *job, bool recovery, bool &submitEventIsSane);

	/** Process an event indicating that a job is in an idle state.
	    Note that this method only does processing relating to keeping
		the count of idle jobs -- any other processing should be done in
		a different method.
		@param The job corresponding to this event.
	*/
	void ProcessIsIdleEvent(Job *job);

	/** Process an event that indicates that a job is NOT in an idle state.
	    Note that this method only does processing relating to keeping
		the count of idle jobs -- any other processing should be done in
		a different method.
		@param The job corresponding to this event.
	*/
	void ProcessNotIdleEvent(Job *job);

    /** Get pointer to job with id jobID
        @param the handle of the job in the DAG
        @return address of Job object, or NULL if not found
    */
    Job * FindNodeByNodeID (const JobID_t jobID) const;

    /** Get pointer to job with name jobName
        @param jobName the name of the job in the DAG
        @return address of Job object, or NULL if not found
    */
    Job * FindNodeByName (const char * jobName) const;

    /** Get pointer to job with condor or stork ID condorID
		@param logsource The type of log from which events should be read.
        @param condorID the CondorID of the job in the DAG
        @return address of Job object, or NULL if not found
    */
    Job * FindNodeByEventID (int logsource, const CondorID condorID) const;

    /** Ask whether a node name exists in the DAG
        @param nodeName the name of the node in the DAG
        @return true if the node exists, false otherwise
    */
    bool NodeExists( const char *nodeName ) const;

    /** Set the event checking level.
		@param allowEvents what "bad" events to treat as non-fatal (as
			   opposed to fatal) errors; see check_events.h for values.
    */
	void SetAllowEvents( int allowEvents);

    /// Print the list of jobs to stdout (for debugging).
    void PrintJobList() const;
    void PrintJobList( Job::status_t status ) const;

    /** @return the total number of nodes in the DAG
     */
    inline int NumNodes() const { return _jobs.Number(); }

    /** @return the number of nodes completed
     */
    inline int NumNodesDone() const { return _numNodesDone; }

    /** @return the number of nodes that failed in the DAG
     */
    inline int NumNodesFailed() const { return _numNodesFailed; }

    /** @return the number of jobs currently submitted to batch system(s)
     */
    inline int NumJobsSubmitted() const { return _numJobsSubmitted; }

    /** @return the number of nodes ready to submit job to batch system
     */
    inline int NumNodesReady() const { return _readyQ->Number(); }

    /** @return the number of PRE scripts currently running
     */
    inline int NumPreScriptsRunning() const
		{ return _preScriptQ->NumScriptsRunning(); }

    /** @return the number of POST scripts currently running
     */
    inline int NumPostScriptsRunning() const
		{ return _postScriptQ->NumScriptsRunning(); }

    /** @return the total number of PRE/POST scripts currently running
     */
    inline int NumScriptsRunning() const
		{ return NumPreScriptsRunning() + NumPostScriptsRunning(); }

	/** @return the number of nodes currently in the status
	 *          Job::STATUS_PRERUN.
	 */
	inline int PreRunNodeCount() const
		{ return _preRunNodeCount; }

	/** @return the number of nodes currently in the status
	 *          Job::STATUS_POSTRUN.
	 */
	inline int PostRunNodeCount() const
		{ return _postRunNodeCount; }

	/** @return the number of nodes currently in the status
	 *          Job::STATUS_PRERUN or Job::STATUS_POSTRUN.
	 */
	inline int ScriptRunNodeCount() const
		{ return _preRunNodeCount + _postRunNodeCount; }

	inline bool Done() const { return NumNodesDone() == NumNodes(); }

		/** Submit all ready jobs, provided they are not waiting on a
			parent job or being throttled.
			@param the appropriate Dagman object
			@return number of jobs successfully submitted
		*/
    int SubmitReadyJobs(const Dagman &dm);

    /** Remove all jobs (using condor_rm) that are currently running.
        All jobs currently marked Job::STATUS_SUBMITTED will be fed
        as arguments to condor_rm via popen.  This function is called
        when the Dagman Condor job itself is removed by the user via
        condor_rm.  This function <b>is not</b> called when the schedd
        kills Dagman.
    */
    void RemoveRunningJobs ( const Dagman & ) const;

    /** Remove all pre- and post-scripts that are currently running.
	All currently running scripts will be killed via daemoncore.
	This function is called when the Dagman Condor job itself is
	removed by the user via condor_rm.  This function <b>is not</b>
	called when the schedd kills Dagman.
    */
    void RemoveRunningScripts ( ) const;

    /** Creates a DAG file based on the DAG in memory, except all
        completed jobs are premarked as DONE.
        @param rescue_file The name of the rescue file to generate
        @param datafile The original DAG config file to read from
		@param useDagDir run DAGs in directories from DAG file paths 
			if true
    */
    void Rescue (const char * rescue_file, const char * datafile,
    			bool useDagDir) /* const */;

	int PreScriptReaper( const char* nodeName, int status );
	int PostScriptReaper( const char* nodeName, int status );

	void PrintReadyQ( debug_level_t level ) const;

	bool RemoveNode( const char *name, MyString &whynot );

	bool RemoveDependency( Job *parent, Job *child );
	bool RemoveDependency( Job *parent, Job *child, MyString &whynot );
	
    /* Detects cycle within dag submitted by user
	   @return true if there is cycle
	*/
	bool isCycle ();

	// max number of PRE & POST scripts to run at once (0 means no limit)
    const int _maxPreScripts;
    const int _maxPostScripts;

	void SetDotFileName(char *dot_file_name);
	void SetDotIncludeFileName(char *include_file_name);
	void SetDotFileUpdate(bool update_dot_file)       { _update_dot_file    = update_dot_file; }
	void SetDotFileOverwrite(bool overwrite_dot_file) { _overwrite_dot_file = overwrite_dot_file; }
	bool GetDotFileUpdate(void)                       { return _update_dot_file; }
	void DumpDotFile(void);

	void CheckAllJobs();

		/** Returns a delimited string listing the node names of all
			of the given node's parents.
			@return delimited string of parent node names
		*/
	const MyString ParentListString( Job *node, const char delim = ',' ) const;

	int NumIdleJobProcs() const { return _numIdleJobProcs; }

		/** Get the number of Stork (nee DaP) log files.
			@return The number of Stork log files (can be 0).
		*/
	int GetStorkLogCount() const { return _storkLogFiles.number(); }

		/** Print the number of deferrals during the run (caused
		    by MaxJobs, MaxIdle, MaxPre, or MaxPost).
			@param level: debug level for output.
			@param force: whether to force output even if there have
				been no deferrals.
		*/
	void PrintDeferrals( debug_level_t level, bool force ) const;

    	/** Print information about the nodes that are currently
		    pending for.
		*/
    void PrintPendingNodes() const;

		// non-exe failure codes for return value integers -- we
		// represent DAGMan, batch-system, or other external errors
		// with the integer return-code space *below* -64.  So, 0-255
		// are normal exe return codes, -1 to -64 represent catching
		// exe signals 1 to 64, and -1000 and below represent DAGMan,
		// batch-system, or other external errors
	const int DAG_ERROR_CONDOR_SUBMIT_FAILED;
	const int DAG_ERROR_CONDOR_JOB_ABORTED;

		// The maximum signal we can deal with in the error-reporting
		// code.
	const int MAX_SIGNAL;

	bool ProhibitMultiJobs() const { return _prohibitMultiJobs; }

		/** Set the config file used for this DAG.
			@param configFile The configuration file for this DAG.
		*/
	void SetConfigFile( const char *configFile ) { _configFile = configFile; }

		/** Set the interval for "pending node" reports.
			@param interval The number of seconds without an event we
			wait before reporting the pending nodes.
		*/
	void SetPendingNodeReportInterval( int interval );
	
		// Node category throttle information for the DAG.
	ThrottleByCategory		_catThrottles;

  protected:

	/** Print a numbered list of the DAG files.
	    @param The list of DAG files being run.
	*/
	void PrintDagFiles( /* const */ StringList &dagFiles );

	/** Find all Condor (not DaP) log files associated with this DAG.
	    @param The list of DAG files being run.
		@param useDagDir run DAGs in directories from DAG file paths
		       if true
	*/
	void FindLogFiles( /* const */ StringList &dagFiles, bool useDagDir );

    /* Prepares to submit job by running its PRE Script if one exists,
       otherwise adds job to _readyQ and calls SubmitReadyJobs()
	   @param The job to start
	   @param Whether this is a retry
	   @return true on success, false on failure
    */
    bool StartNode( Job *node, bool isRetry );

	typedef enum {
		SUBMIT_RESULT_OK,
		SUBMIT_RESULT_FAILED,
		SUBMIT_RESULT_NO_SUBMIT,
	} submit_result_t;

	/** Submit the Condor or Stork job for a node, including doing
		some higher-level work such as sleeping before the actual submit
		if necessary.
		@param the appropriate Dagman object
		@param the node for which to submit a job
		@param reference to hold the Condor ID the job is assigned
		@return submit_result_t (see above)
	*/
	submit_result_t SubmitNodeJob( const Dagman &dm, Job *node,
				CondorID &condorID );

	/** Do the post-processing of a successful submit of a Condor or
		Stork job.
		@param the node for which the job was just submitted
		@param the Condor ID of the associated job
	*/	
	void ProcessSuccessfulSubmit( Job *node, const CondorID &condorID );

	/** Do the post-processing of a failed submit of a Condor or
		Stork job.
		@param the node for which the job was just submitted
		@param the maximum number of submit attempts allowed for a job.
	*/
	void ProcessFailedSubmit( Job *node, int max_submit_attempts );

	/** Decrement the job counts for this node.
		@param The node for which to decrement the job counts.
	*/
	void DecrementJobCounts( Job *node );

	// Note: there's no IncrementJobCounts method because the code isn't
	// exactly duplicated when incrementing.

	/** Update the job counts for the given node.
		@param The amount by which to change the job counts.
	*/
	void UpdateJobCounts( Job *node, int change );

    // add job to termination queue and report termination to all
    // child jobs by removing job ID from each child's waiting queue
    void TerminateJob( Job* job, bool bootstrap = false );
  
	void PrintEvent( debug_level_t level, const ULogEvent* event,
					 Job* node );

	void RestartNode( Job *node, bool recovery );

	/* DFS number the jobs in the DAG in order to detect cycle*/
	void DFSVisit (Job * job);

		/** Check whether we got an exit value that should abort the DAG.
			@param The job associated with either the PRE script, POST
				script, or "main" job that just finished.
			@param The "type" of script or job we're dealing with (PRE
				script, POST script, or node).
			@return True iff aborting the DAG (it really should not
			    return in that case)
		*/
	static bool CheckForDagAbort(Job *job, const char *type);

		// takes a userlog event and returns the corresponding node
	Job* LogEventNodeLookup( int logsource, const ULogEvent* event,
				bool &submitEventIsSane );

		// check whether a userlog event is sane, or "impossible"

	bool EventSanityCheck( int logsource, const ULogEvent* event,
						const Job* node, bool* result );

		// compares a submit event's job ID with the one that appeared
		// earlier in the submit command's stdout (which we stashed in
		// the Job object)

	bool SanityCheckSubmitEvent( const CondorID condorID, const Job* node );

		/** Get the appropriate hash table for event ID->node mapping,
			according to whether this is a Condor or Stork node.
			@param the node type/logsource (Condor or Stork) (see
				Log_source and Job::job_type_t)
			@return a pointer to the appropriate hash table
		*/
	HashTable<int, Job *> *		GetEventIDHash(int jobType);

		/** Get the appropriate hash table for event ID->node mapping,
			according to whether this is a Condor or Stork node.
			@param the node type/logsource (Condor or Stork) (see
				Log_source and Job::job_type_t)
			@return a pointer to the appropriate hash table
		*/
	const HashTable<int, Job *> *		GetEventIDHash(int jobType) const;

		// The log file name specified by the -Condorlog command line
		// argument (not used for much anymore).
	char *		_condorLogName;

    // name of consolidated condor log
	StringList _condorLogFiles;

    // Documentation on ReadUserLog is present in condor_c++_util
	ReadMultipleUserLogs _condorLogRdr;

    //
    bool          _condorLogInitialized;

    //-->DAP
		// The log file name specified by the -Storklog command line
		// argument.
    const char* _dapLogName;

		// The list of all Stork log files (generated from the relevant
		// submit files).
	StringList	_storkLogFiles;

		// Object to read events from Stork logs.
	ReadMultipleUserLogs	_storkLogRdr;

		// Whether the Stork (nee DaP) log(s) have been successfully
		// initialized.
    bool          _dapLogInitialized;
    //<--DAP

		/** Get the total number of node job user log files we'll be
			accessing.
			@return The total number of log files.
		*/
	int TotalLogFileCount() { return _condorLogFiles.number() +
				_storkLogFiles.number(); }

    /// List of Job objects
    List<Job>     _jobs;

	HashTable<MyString, Job *>		_nodeNameHash;

	HashTable<JobID_t, Job *>		_nodeIDHash;

	// Hash by CondorID (really just by the cluster ID because all
	// procs in the same cluster map to the same node).
	HashTable<int, Job *>			_condorIDHash;

	// Hash by StorkID (really just by the cluster ID because all
	// procs in the same cluster map to the same node).
	HashTable<int, Job *>			_storkIDHash;

    // Number of nodes that are done (completed execution)
    int _numNodesDone;
    
    // Number of nodes that failed (job or PRE or POST script failed)
    int _numNodesFailed;

    // Number of batch system jobs currently submitted
    int _numJobsSubmitted;

    /*  Maximum number of jobs to submit at once.  Non-negative.  Zero means
        unlimited
    */
    const int _maxJobsSubmitted;

		// Number of DAG job procs currently idle.
	int _numIdleJobProcs;

    	// Maximum number of idle job procs to allow (stop submitting if the
		// number of idle job procs hits this limit).  Non-negative.  Zero
		// means unlimited.
    const int _maxIdleJobProcs;

		// Whether to allow the DAG to run even if we have an error
		// determining the job log files.
	bool		_allowLogError;

		// If this is true, nodes for which the job submit fails are retried
		// before any other ready nodes; otherwise a submit failure puts
		// a node at the back of the ready queue.  (Default is true.)
	bool		m_retrySubmitFirst;

		// If this is true, nodes for which the node fails (and the node
		// has retries) are retried before any other ready nodes;
		// otherwise a node failure puts a node at the back of the ready
		// queue.  (Default is false.)
	bool		m_retryNodeFirst;

		// Executable to remove Condor jobs.
	const char *	_condorRmExe;

		// Executable to remove Stork jobs.
	const char *	_storkRmExe;

		// Condor ID of this DAGMan process.
	const CondorID *	_DAGManJobId;

	// queue of jobs ready to be submitted to Condor
	PrioritySimpleList<Job*>* _readyQ;

	// queue of submitted jobs not yet matched with submit events in
	// the Condor job log
    Queue<Job*>* _submitQ;

	ScriptQ* _preScriptQ;
	ScriptQ* _postScriptQ;

		// Number of nodes currently in status Job::STATUS_PRERUN.
	int		_preRunNodeCount;

		// Number of nodes currently in status Job::STATUS_POSTRUN.
	int		_postRunNodeCount;
	
	int DFS_ORDER; 

	// Information for producing dot files, which can be used to visualize
	// DAG files. Dot is part of the graphviz package, which is available from
	// http://www.research.att.com/sw/tools/graphviz/
	char *_dot_file_name;
	char *_dot_include_file_name;
	bool  _update_dot_file;
	bool  _overwrite_dot_file;
	int   _dot_file_name_suffix;

	void IncludeExtraDotCommands(FILE *dot_file);
	void DumpDotFileNodes(FILE *temp_dot_file);
	void DumpDotFileArcs(FILE *temp_dot_file);
	void ChooseDotFileName(MyString &dot_file_name);

		// Separate event checkers for Condor and Stork here because
		// IDs could collide.
	CheckEvents	_checkCondorEvents;
	CheckEvents	_checkStorkEvents;

		// Total count of jobs deferred because of MaxJobs limit (note
		// that a single job getting deferred multiple times is counted
		// multiple times).
	int		_maxJobsDeferredCount;

		// Total count of jobs deferred because of MaxIdle limit (note
		// that a single job getting deferred multiple times is counted
		// multiple times).
	int		_maxIdleDeferredCount;

		// Total count of jobs deferred because of node category throttles.
	int		_catThrottleDeferredCount;

		// whether or not to prohibit multiple job proc submitsn (e.g.,
		// node jobs that create more than one job proc)
	bool		_prohibitMultiJobs;

		// Whether to submit ready nodes in depth-first order (as opposed
		// to breadth-first order).
	bool		_submitDepthFirst;

	bool		_abortOnScarySubmit;

		// The next time we're allowed to try submitting a job -- 0 means
		// go ahead and submit right away.
	time_t		_nextSubmitTime;

		// The delay we use the next time a submit fails -- _nextSubmitTime
		// becomes the current time plus _nextSubmitDelay (this is in
		// seconds).
	int			_nextSubmitDelay;

		// Whether we're in recovery mode.  We only need this here for
		// the PR 554 fix in PostScriptReaper -- otherwise it gets passed
		// down thru the call stack.
	bool		_recovery;

		// The config file (if any) specified for this DAG.  As of 2007-06-08,
		// this is only used when writing a rescue DAG -- we already
		// parse the given config file (if any) before the Dag object
		// is created.
	const char *	_configFile;

		// The time interval (in seconds) at which to print pending
		// node reports.
	int			_pendingReportInterval;

		// The last time we got a log event for a node job.
	time_t		_lastEventTime;

		// The last time we printed a pending node report.
	time_t		_lastPendingNodePrintTime;

		// Default Condor ID to use in reseting a node's Condor ID on
		// retry.
	static const CondorID	_defaultCondorId;

};

#endif /* #ifndef DAG_H */
