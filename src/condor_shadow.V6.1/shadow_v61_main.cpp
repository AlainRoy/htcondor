/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/


#include "condor_common.h"
#include "condor_daemon_core.h"
#include "subsystem_info.h"
#include "baseshadow.h"
#include "shadow.h"
#include "mpishadow.h"
#include "parallelshadow.h"
#include "exit.h"
#include "condor_debug.h"
#include "condor_version.h"
#include "condor_attributes.h"
#include "dc_schedd.h"
#include "spool_version.h"
#include "shadow_wrangler.h"

ShadowWrangler GlobalWrangler;

// settings we're given on the command-line
static const char* schedd_addr = NULL;
const char* public_schedd_addr = NULL;
static const char* job_ad_file = NULL;
bool is_reconnect = false;
static int is_multi = false;
static int cluster = -1;
static int proc = -1;
static const char * xfer_queue_contact_info = NULL;
static time_t shadow_worklife_expires = 0;

static void
usage( int argc, char* argv[] )
{
	// TODO: Usage doesn't appear correct?
	dprintf( D_ALWAYS, "Usage: %s cluster.proc schedd_addr file_name\n",
			 argv[0] );
	for (int i=0; i < argc; i++) {
		dprintf(D_ALWAYS, "argv[%d] = %s\n", i, argv[i]);
	}
	exit( JOB_SHADOW_USAGE );
}


extern "C" {
int
ExceptCleanup(int, int, const char *buf)
{
	BaseShadow* Shadow = GlobalWrangler.getShadow(NULL);
	if (Shadow) {
		Shadow->log_except(buf);
	}
	return 0;
}
}

/* DaemonCore interface implementation */

DECL_SUBSYSTEM( "SHADOW", SUBSYSTEM_TYPE_SHADOW );

int
dummy_reaper(Service *,int pid,int)
{
	dprintf(D_ALWAYS,"dummy-reaper: pid %d exited; ignored\n",pid);
	return TRUE;
}


void
parseArgs( int argc, char *argv[] )
{
	char *opt;

	char** tmp = argv;
	for( tmp++; *tmp; tmp++ ) {
		opt = tmp[0];

		if( sscanf(opt, "%d.%d", &cluster, &proc) == 2 ) {
			if( cluster < 0 || proc < 0 ) {
				dprintf(D_ALWAYS, 
						"ERROR: invalid cluster.proc specified: %s\n", opt);
				usage(argc, argv);
			}
			continue;
		}
		
		if( opt[0] == '<' ) { 
				// might be the schedd's address
			if( is_valid_sinful(opt)) {
				schedd_addr = opt;
				continue;
			} else {
				dprintf(D_ALWAYS, 
						"ERROR: invalid shadow-private schedd_addr specified: %s\n", opt);
				usage(argc, argv);
			}
		}

		if( !strcmp(opt, "--reconnect") || !strcmp(opt, "-reconnect") ) {
			is_reconnect = true;
			continue;
		}

		if( !strcmp(opt, "--multi") || !strcmp(opt, "-multi") ) {
			is_multi = true;
			continue;
		}

		if (strncmp(opt, "--schedd", 8) == 0) {
			char *ptr = strchr(opt, '<');
			if (ptr && is_valid_sinful(ptr)) {
				public_schedd_addr = ptr;
				continue;
			}
			else {
				dprintf(D_ALWAYS, 
						"ERROR: invalid public schedd_addr specified: %s\n",
						opt);
				usage(argc, argv);
			}
		}

		if (strncmp(opt, "--xfer-queue=", 13) == 0) {
			xfer_queue_contact_info = opt+13;
			continue;
		}

		if (strcmp(opt, "--no-schedd-updates") == 0) {
			GlobalWrangler.setSendUpdatesToSchedd(false);
			continue;
		}

			// the only other argument we understand is the
			// filename we should read our ClassAd from, "-" for
			// STDIN.  There's no further checking we need to do 
		if( job_ad_file ) {
				// already were here, bail out
			dprintf( D_ALWAYS, "ERROR: unrecognized option (%s)\n", opt );
			usage(argc, argv);
		}
		job_ad_file = opt;
	}

		// A proper model of arguments should be presented here and
		// used to validate the provided arguments. It would be
		// something like:
		// if no cluster/proc, who cares
		// if no job_ad_file, fail
		// And that might be it.
		// The validation used to count arguments processed, which was
		// easily fooled.
}


static FILE* fp = NULL;
ClassAd* 
readJobAd( void )
{
	ClassAd* ad = NULL;
	bool is_stdin = false;
	bool read_something = false;

	ASSERT( job_ad_file );

	if( job_ad_file[0] == '-' && job_ad_file[1] == '\0' ) {
		fp = stdin;
		is_stdin = true;
	} else {
		if (fp == NULL) {
			fp = safe_fopen_wrapper( job_ad_file, "r" );
			if( ! fp ) {
				EXCEPT( "Failed to open ClassAd file (%s): %s (errno %d)",
						job_ad_file, strerror(errno), errno );
			}
		}
	}

	dprintf( D_FULLDEBUG, "Reading job ClassAd from %s\n",
			 is_stdin ? "STDIN" : job_ad_file );

	ad = new ClassAd;
	MyString line;
	while( line.readLine(fp) ) {
        read_something = true;
		line.chomp();
		if( line[0] == '#' ) {
			dprintf( D_JOB, "IGNORING COMMENT: %s\n", line.Value() );
			continue;
		}
		if( line == "***" ) {
			dprintf( D_JOB, "Saw ClassAd delimitor, stopping\n" );
			break;
		}
        if( ! ad->Insert(line.Value()) ) {
			EXCEPT( "Failed to insert \"%s\" into ClassAd!", line.Value() );
        }
    }
	if( ! read_something ) {
		EXCEPT( "reading ClassAd from (%s): file is empty",
				is_stdin ? "STDIN" : job_ad_file );
	}
	if( (DebugFlags & D_JOB) && (DebugFlags & D_FULLDEBUG) ) {
		ad->dPrint( D_JOB );
	} 

	// For debugging, see if there's a special attribute in the
	// job ad that sends us into an infinite loop, waiting for
	// someone to attach with a debugger

	int shadow_should_wait = 0;
	ad->LookupInteger( ATTR_SHADOW_WAIT_FOR_DEBUG,
					   shadow_should_wait );
	if( shadow_should_wait ) {
		dprintf( D_ALWAYS, "Job requested shadow should wait for "
			"debugger with %s=%d, going into infinite loop\n",
			ATTR_SHADOW_WAIT_FOR_DEBUG, shadow_should_wait );
		while( shadow_should_wait ) {}
	}

	return ad;
}


void
ShadowWrangler::initShadow( ClassAd* ad )
{
	int universe; 
	if( ! ad->LookupInteger(ATTR_JOB_UNIVERSE, universe) ) {
			// change to the right universes when everything works.
		dprintf( D_ALWAYS, "WARNING %s not specified, assuming VANILLA\n", 
				 ATTR_JOB_UNIVERSE );
		universe = CONDOR_UNIVERSE_VANILLA;
	}

	dprintf( D_ALWAYS, "Initializing a %s shadow for job %d.%d\n", 
			 CondorUniverseName(universe), cluster, proc );

	int wantPS = 0;
	ad->LookupBool("WantParallelScheduling", wantPS);
	if (wantPS) {
		universe = CONDOR_UNIVERSE_PARALLEL;
	}

	BaseShadow* Shadow = NULL;

	switch ( universe ) {
	case CONDOR_UNIVERSE_PARALLEL:
		Shadow = new ParallelShadow();
		break;
	case CONDOR_UNIVERSE_VANILLA:
	case CONDOR_UNIVERSE_JAVA:
	case CONDOR_UNIVERSE_VM:
		Shadow = new UniShadow();
		break;
	case CONDOR_UNIVERSE_MPI:
		Shadow = new MPIShadow();
		break;
	default:
		dprintf( D_ALWAYS, "This version of the shadow cannot support "
				 "universe %d (%s)\n", universe,
				 CondorUniverseName(universe) );
		EXCEPT( "Universe not supported" );
	}
	if (Shadow) {
		Shadow->init( ad, schedd_addr, xfer_queue_contact_info );
		putShadow(ad, Shadow);
	}
}


void ShadowWrangler::startShadow( ClassAd *ad )
{
		// see if the SchedD punched a DAEMON-level authorization
		// hole for this job. if it did, we'll do the same here
		//
	BaseShadow* Shadow = getShadow(ad);

	MyString auth_hole_id;
	if (ad->LookupString(ATTR_STARTD_PRINCIPAL, auth_hole_id)) {
		IpVerify* ipv = daemonCore->getIpVerify();
		if (!ipv->PunchHole(DAEMON, auth_hole_id)) {
			dprintf(D_ALWAYS,
			        "WARNING: IpVerify::PunchHole error for %s: "
			            "job may fail to execute\n",
			        auth_hole_id.Value());
		}
	}

	initShadow( ad );

	int wantClaiming = 0;
	ad->LookupBool(ATTR_CLAIM_STARTD, wantClaiming);

	if( is_reconnect ) {
		Shadow->reconnect();
	} else {
			// if the shadow is going to claim the startd,
			// we need to asynchrously claim it.
			
			// Otherwise, in the usual case under the sched,
			// call spawn here, which will activate the pre-claimed
			// startd
		if (!wantClaiming) {
			Shadow->spawn();
		}
	}		
}


int handleJobRemoval(Service*,int sig)
{
	BaseShadow* Shadow = GlobalWrangler.getShadow(NULL);
	if( Shadow ) {
		return Shadow->handleJobRemoval(sig);
	}
	return 0;
}


int handleUpdateJobAd(Service*,int sig)
//int handleUpdateJobAd(Service*,int sig, Stream *sock)
{
	BaseShadow* Shadow = GlobalWrangler.getShadow(NULL);
	if( Shadow ) {
		return Shadow->handleUpdateJobAd(sig);
	}
	return 0;
}


void
main_init(int argc, char *argv[])
{
	_EXCEPT_Cleanup = ExceptCleanup;

		/* Start up with condor.condor privileges. */
	set_condor_priv();

        parseArgs( argc, argv );

		// Register a do-nothing reaper.  This is just because the
		// file transfer object, which could be instantiated later,
		// registers its own reaper and does an EXCEPT if it gets
		// a reaper ID of 1 (since lots of other daemons have a reaper
		// ID of 1 hard-coded as special... this is bad).
	daemonCore->Register_Reaper("dummy_reaper",
							(ReaperHandler)&dummy_reaper,
							"dummy_reaper",NULL);


		// register SIGUSR1 (condor_rm) for shutdown...
	daemonCore->Register_Signal( SIGUSR1, "SIGUSR1", 
		(SignalHandler)&handleJobRemoval,"handleJobRemoval");

		// ragister UPDATE_JOBAD for qedit changes
	daemonCore->Register_Signal( UPDATE_JOBAD, "UPDATE_JOBAD", 
		(SignalHandler)&handleUpdateJobAd,"handleUpdateJobAd");

	// Configure the command handling from the schedd
	if (is_multi) {
		daemonCore->Register_Command( REMOVE_SHADOW_JOB, "REMOVE_SHADOW_JOB",
			(CommandHandlercpp)&ShadowWrangler::handleRemoveJob,
			"handleRemoveJob", &GlobalWrangler, DAEMON, D_COMMAND, true  );
		daemonCore->Register_Command( UPDATE_JOBAD, "UPDATE_JOBAD",
			(CommandHandlercpp)&ShadowWrangler::handleUpdateJob, "handleUpdateJobAd",
			&GlobalWrangler, DAEMON, D_COMMAND, true);
		daemonCore->Register_Command( CREATE_SHADOW_JOB, "CREATE_SHADOW_JOB",
			(CommandHandlercpp)&ShadowWrangler::handleCreateJob, "createJob",
			&GlobalWrangler, DAEMON, D_COMMAND, true);
	}

	int shadow_worklife = param_integer( "SHADOW_WORKLIFE", 3600 );
	if( shadow_worklife > 0 ) {
		shadow_worklife_expires = time(NULL) + shadow_worklife;
	}
	else if( shadow_worklife == 0 ) {
			// run one job and then exit
		shadow_worklife_expires = time(NULL)-1;
	}
	else {
		shadow_worklife_expires = 0;
	}

	CheckSpoolVersion(SPOOL_MIN_VERSION_SHADOW_SUPPORTS,SPOOL_CUR_VERSION_SHADOW_SUPPORTS);

	ClassAd* ad = readJobAd();
	if( ! ad ) {
		EXCEPT( "Failed to read job ad!" );
	}

	GlobalWrangler.startShadow( ad );
}

void
main_config()
{
	GlobalWrangler.config();
}


void
main_shutdown_fast()
{
	GlobalWrangler.shutDown( JOB_NOT_CKPTED );
}

void
main_shutdown_graceful()
{
	GlobalWrangler.gracefulShutDown();
}


void
dumpClassad( const char* header, ClassAd* ad, int debug_flag )
{
	if( ! header  ) {
		dprintf( D_ALWAYS, "ERROR: called dumpClassad() w/ NULL header\n" ); 
		return;
	}
	if( ! ad  ) {
		dprintf( D_ALWAYS, "ERROR: called dumpClassad(\"%s\") w/ NULL ad\n", 
				 header );   
		return;
	}
	if( DebugFlags & debug_flag ) {
		dprintf( debug_flag, "*** ClassAd Dump: %s ***\n", header );  
		ad->dPrint( debug_flag );
		dprintf( debug_flag, "--- End of ClassAd ---\n" );
	}
}


void
printClassAd( void )
{
	printf( "%s = True\n", ATTR_IS_DAEMON_CORE );
	printf( "%s = True\n", ATTR_HAS_FILE_TRANSFER );
	printf( "%s = True\n", ATTR_HAS_PER_FILE_ENCRYPTION );
	printf( "%s = True\n", ATTR_HAS_MPI );
	printf( "%s = True\n", ATTR_HAS_JAVA );
	printf( "%s = True\n", ATTR_HAS_RECONNECT );
	printf( "%s = True\n", ATTR_HAS_JOB_AD_FROM_FILE );
	printf( "%s = True\n", ATTR_HAS_VM );
	printf( "%s = \"%s\"\n", ATTR_VERSION, CondorVersion() );
}


void
main_pre_dc_init( int argc, char* argv[] )
{
	if( argc == 2 && strncasecmp(argv[1],"-cl",3) == MATCH ) {
		printClassAd();
		exit(0);
	}
}


void
main_pre_command_sock_init( )
{
}

bool
ShadowWrangler::recycleShadow(BaseShadow* Shadow, int previous_job_exit_reason)
{
	if( previous_job_exit_reason != JOB_EXITED ) {
		return false;
	}
	if( shadow_worklife_expires && time(NULL) > shadow_worklife_expires ) {
		return false;
	}

	dprintf(D_ALWAYS,"Reporting job exit reason %d and attempting to fetch new job.\n",
			previous_job_exit_reason );

	ClassAd *new_job_ad = NULL;
	if (m_sendUpdatesToSchedd) {
		// If we're running under a schedd, get the next job ad
		// from the schedd
		ASSERT( schedd_addr );

		DCSchedd schedd(schedd_addr);
		MyString error_msg;
		if( !schedd.recycleShadow( previous_job_exit_reason, &new_job_ad, error_msg ) )
		{
			dprintf(D_ALWAYS,"recycleShadow() failed: %s\n",error_msg.Value());
			delete new_job_ad;
			return false;
		}
	} else {
		// if we are a free-running shadow
		new_job_ad = readJobAd();
	}

	if( !new_job_ad ) {
		dprintf(D_FULLDEBUG,"No new job found to run under this shadow.\n");
		return false;
	}

	new_job_ad->LookupInteger(ATTR_CLUSTER_ID,cluster);
	new_job_ad->LookupInteger(ATTR_PROC_ID,proc);
	dprintf(D_ALWAYS,"Switching to new job %d.%d\n",cluster,proc);

	delete Shadow;
	Shadow = NULL;
	is_reconnect = false;

	startShadow( new_job_ad );
	return true;
}


