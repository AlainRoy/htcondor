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

#ifndef CONDOR_GAHP_CLIENT_H
#define CONDOR_GAHP_CLIENT_H

#include "globus_utils.h"

		class Gahp_Args {
			public:
				Gahp_Args();
				~Gahp_Args();
				char ** read_argv(int readfd);
				void free_argv();
				char **argv;
				int argc;
				static int pipe_ready();
			private:
				static bool skip_next_r;
		};

		class Gahp_Buf {
			public:
				Gahp_Buf(int size);
				~Gahp_Buf();
				char *buffer;
		};

// Additional error values that GAHP calls can return
///
static const int GAHPCLIENT_COMMAND_PENDING = -100;
///
static const int GAHPCLIENT_COMMAND_NOT_SUPPORTED = -101;
///
static const int GAHPCLIENT_COMMAND_NOT_SUBMITTED = -102;
///
static const int GAHPCLIENT_COMMAND_TIMED_OUT = -103;

// Special values for what proxy to use with commands
//   Use default proxy (not any cached proxy)
static const int GAHPCLIENT_CACHE_DEFAULT_PROXY = -1;
//   Use last proxy (use whatever proxy was used for the previous command)
static const int GAHPCLIENT_CACHE_LAST_PROXY = -2;

///
class GahpClient : public Service {
	
	public:
		
		/** @name Instantiation. 
		 */
		//@{
	
			/// Constructor
		GahpClient();
			/// Destructor
		~GahpClient();
		
		//@}

		///
		bool Startup(const char * gahp_server_path = NULL);

		///
		bool Initialize(const char * proxy_path,
						const char * gahp_server_path = NULL);

		///
		void purgePendingRequests() { clear_pending(); }

		///
		void setMaxPendingRequests(int max) { max_pending_requests = max; }

		///
		int getMaxPendingRequests() { return max_pending_requests; }

		///
		static bool getUsePrefix() { return use_prefix; }

		/// Return -1 if gahp does not exist, else returns the gahp pid
		static int getPid() { return m_gahp_pid; }

		/** @name Mode methods.
		 * Methods to set/get the mode.
		 */
		//@{
	
		/// Enum used by setMode() method
		enum mode {
				/** */ normal,
				/** */ results_only,
				/** */ blocking
		};

		/**
		*/
		void setMode( mode m ) { m_mode = m; }

		/**
		*/
		mode getMode() { return m_mode; }
	
		//@}

		/** @name Timeout methods.
		 * Methods to set/get the timeout on pending async commands.
		 */
		//@{
	
		/** Set the timeout.
			@param t timeout in seconds, or zero to disable timeouts.
			@see getTimeout
		*/
		void setTimeout(int t) { m_timeout = t; }

		/** Get the currently timeout value.
			@return timeout in seconds, or zero if no timeout set.
			@see setTimeout
		*/
		unsigned int getTimeout() { return m_timeout; }
	
		//@}

		/** @name Async results methods.
		   Methods to control how to fetch and/or be notified about
		   pending asynchronous results.
		 */
		//@{
		
		/** Immediately poll the Gahp Server for results.  Normally,
			this method is invoked automatically either by a timer set
			via setPollInterval or by a Gahp Server async result
			notification.
			@return The number of pending commands which have completed
			since the last poll (note: could easily be zero).	
			@see setPollInterval
		*/
		static int poll();

		/** Reset the specified timer to go off immediately when a
			pending command has completed.
			@param tid The timer id to reset via DaemonCore's Reset_Timer 
			method, or a -1 to disable.
			@see Reset_Timer
			@see getNotificationTimerId
		*/
		void setNotificationTimerId(int tid) { user_timerid = tid; }

		/** Return the timer id previously set with method 
			setNotificationTimerId.
			@param tid The timer id which will be reset via DaemonCore's 
			Reset_Timer method, or a -1 if deactivated.
			@see setNotificationTimerId
			@see Reset_Timer
		*/
		int getNotificationTimerId() { return user_timerid; }

		/** Set interval to automatically poll the Gahp Server for results.
			If the Gahp server supports async result notification, then
			the poll interval defaults to zero (disabled).  Otherwise,
			it will default to 5 seconds.  
		 	@param interval Polling interval in seconds, or zero
			to disable polling all together.  
			@return true on success, false otherwise.
			@see getPollInterval
		*/
		static void setPollInterval(unsigned int interval);

		/** Retrieve the interval used to auto poll the Gahp Server 
			for results.  Also used to determine if async notification
			is in effect.
		 	@return Polling interval in seconds, or a zero
			to represent auto polling is disabled (likely if
			the Gahp server supports async notification).
			@see setPollInterval
		*/
		static unsigned int getPollInterval() { return m_pollInterval; }

		static void poll_real_soon();
		//@}
					
		bool cacheProxyFromFile( int id, const char *proxy_path );
		bool uncacheProxy( int id );
		bool useCachedProxy( int id, bool force = false );

		void setNormalProxyCacheId( int id ) { normal_proxy_cache_id = id; }
		int getNormalProxyCacheId() { return normal_proxy_cache_id; }

		void setDelegProxyCacheId( int id ) { deleg_proxy_cache_id = id; }
		int getDelegProxyCacheId() { return deleg_proxy_cache_id; }

		//-----------------------------------------------------------
		
		/**@name Globus Methods
		 * These methods have the exact same API as their native Globus
		 * Toolkit counterparts.  
		 */
		//@{

		/// cache it from the gahp
		const char *
		globus_gram_client_error_string(int error_code);

		///
		int 
		globus_gram_client_callback_allow(
			globus_gram_client_callback_func_t callback_func,
			void * user_callback_arg,
			char ** callback_contact);

		///
		int 
		globus_gram_client_job_request(const char * resource_manager_contact,
			const char * description,
			const int job_state_mask,
			const char * callback_contact,
			char ** job_contact);

		///
		int 
		globus_gram_client_job_cancel(const char * job_contact);

		///
		int
		globus_gram_client_job_status(const char * job_contact,
			int * job_status,
			int * failure_code);

		///
		int
		globus_gram_client_job_signal(const char * job_contact,
			globus_gram_protocol_job_signal_t signal,
			const char * signal_arg,
			int * job_status,
			int * failure_code);

		///
		int
		globus_gram_client_job_callback_register(const char * job_contact,
			const int job_state_mask,
			const char * callback_contact,
			int * job_status,
			int * failure_code);

		///
		int 
		globus_gram_client_ping(const char * resource_manager_contact);

		///
		int 
		globus_gram_client_job_contact_free(char *job_contact) 
			{ free(job_contact); return 0; }


		///
		int
		globus_gram_client_set_credentials(const char *proxy_path);

		///
		int
		globus_gram_client_job_refresh_credentials(const char *job_contact);

		///
		int
		globus_gass_server_superez_init( char **gass_url, int port );


#ifdef CONDOR_GLOBUS_HELPER_WANT_DUROC
	// Not yet ready for prime time...
	globus_duroc_control_barrier_release();
	globus_duroc_control_init();
	globus_duroc_control_job_cancel();
	globus_duroc_control_job_request();
	globus_duroc_control_subjob_states();
	globus_duroc_error_get_gram_client_error();
	globus_duroc_error_string();
#endif /* ifdef CONDOR_GLOBUS_HELPER_WANT_DUROC */

		//@}

	private:

			// Various Private Methods
		const char* escape(const char *input);
		int new_reqid();
		void clear_pending();
		bool is_pending(const char *command, const char *buf);
		void now_pending(const char *command,const char *buf,
						 int cache_id = GAHPCLIENT_CACHE_DEFAULT_PROXY);
		Gahp_Args* get_pending_result(const char *,const char *);
		bool check_pending_timeout(const char *,const char *);
		int reset_user_timer(int tid);
		int reset_user_timer_alarm();

			// Methods for private GAHP commands
		bool command_version(bool banner_string = false);
		bool command_initialize_from_file(const char *proxy_path,
			const char *command=NULL);
		bool command_commands();
		bool command_async_mode_on();
		bool command_response_prefix(const char *prefix);

			// Private Data Members
		static unsigned int m_reference_count;
		unsigned int m_timeout;
		mode m_mode;
		char pending_command[150];
		char *pending_args;
		int pending_reqid;
		Gahp_Args* pending_result;
		time_t pending_timeout;
		int pending_timeout_tid;
		bool pending_submitted_to_gahp;
		static HashTable<int,GahpClient*> *requestTable;
		static Queue<int> waitingToSubmit;
		int user_timerid;
		int normal_proxy_cache_id;
		int deleg_proxy_cache_id;
		int pending_cache_id;

			// These data members all deal with the GAHP
			// server.  Since there is only one instance of the GAHP
			// server, all the below data members are static.
		static int m_gahp_pid;
		static int m_reaperid;
		static int m_gahp_readfd;
		static int m_gahp_writefd;
		static char m_gahp_version[150];
		static StringList * m_commands_supported;
		static bool use_prefix;
		static void write_line(const char *command);
		static void write_line(const char *command,int req,const char *args);
		static void Reaper(Service*,int pid,int status);
		static unsigned int m_pollInterval;
		static int poll_tid;
		static bool poll_pending;
		static char* m_callback_contact;	
		static void* m_user_callback_arg;
		static globus_gram_client_callback_func_t m_callback_func;
		static int m_callback_reqid;
		static int max_pending_requests;
		static int num_pending_requests;
		static int current_cache_id;

};	// end of class GahpClient

	
#endif /* ifndef CONDOR_GAHP_CLIENT_H */
