/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
 * CONDOR Copyright Notice
 *
 * See LICENSE.TXT for additional notices and disclaimers.
 *
 * Copyright (c)1990-1998 CONDOR Team, Computer Sciences Department,
 *University of Wisconsin-Madison, Madison, WI.  All Rights Reserved.
 *No use of the CONDOR Software Program Source Code is authorized
 *without the express consent of the CONDOR Team.  For more
 *information contact: CONDOR Team, Attention: Professor Miron Livny,
 *7367 Computer Sciences, 1210 W. Dayton St., Madison, WI 53706-1685,
 *(608) 262-0856 or miron@cs.wisc.edu.
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

#ifndef CONDOR_DAEMON_H
#define CONDOR_DAEMON_H

#include "condor_common.h"
#include "condor_io.h"
#include "condor_classad.h"
#include "condor_collector.h"
#include "condor_secman.h"
#include "daemon_types.h"
#include "KeyCache.h"


/** 
  Class used to pass around and store information about a given
  daemon.  You instantiate one of these objects and pass in the type
  of daemon you care about, and optionally, the name of the daemon and
  pool that it reports to.  The pool and name arguments both default
  to NULL, which means the local pool (the pool that the machine where
  the Daemon object was instantiated reports to) and the local  
  daemon, respectively.  

  <p>Once you have the object, you call the locate() method to have
  it actually try to find all the information about the daemon it can
  provide.  If you call member functions for info before you call
  locate(), the object will call locate() for you, though that can
  obscure errors, so it's generally discouraged.

  <p>If there are any errors, the error() method will stop returning
  NULL and start returning a string that describes the error, which
  can then be used for dprintf(), etc.

  <p>"The local daemon" means different things, depending on the type
  of daemon.  For central manager daemons (negotiator, collector,
  view_collector), it means the CM this machine is configured to use
  (which would normally be a remote host, unless you were
  instantiating the Daemon object on the CM itself).  For all other
  daemons (master, startd, schedd), it means the daemon running on the
  local host.

  <p>Information provided by the Daemon class includes the daemon's
  name (which could be a "condor" name such as
  "vm1@raven.cs.wisc.edu", or a full hostname), hostname, fully
  qualified hostname, address (sinful string), port, the type of the
  daemon (daemon_t enum), the pool it reports to (if it's a remote
  pool) and a bool that describes if the daemon is "local" (as
  described above).

  <p>We use this class to localize all the logic for finding daemons
  in one place, and all the rest of Condor now uses this class.  This
  way, when we add support for a new way of finding daemons (like
  address files, multiple network interface config parameters, etc),
  we can add the code to support it in one place, instead of peppered
  throughout the entire source tree.  */
class Daemon {
public:
		/** Constructor.  Takes the type of the daemon you want
		  (basically, the subsystem, though we use a daemon_t enum.
		  See daemon_types.h for details).  Also, you can pass in the
		  name (a hostname, or Condor-name, such as
		  "vm1@raven.cs.wisc.edu") of the particular daemon you care
		  about.  If you pass in a NULL (the default) for the name, we
		  assume you want the "local" daemon.  Alternatively, you can
		  pass in the "sinful string" of the daemon you care about.  
		  Finally, you can pass in the name of the pool you want to
		  query.  If you pass in a NULL (the default), we assume you
		  want the local pool.
		  @param type The type of the daemon, specified in a daemon_t
		  @param name The name (or sinful string) of the daemon, NULL if you want local
		  @param pool The name of the pool, NULL if you want local */
	Daemon( daemon_t type, const char* name = NULL, 
				const char* pool = NULL );

		/** Constructor.
		  @param addr_string This can be a sinful string.  If not, it should be an
		    IP address or hostname and the port parameter must be set as well.
		  @param port        The port to connect to.  This parameter is ignored if
		    the addr_string parameter is a sinful string (i.e. already has a port).
		*/
	Daemon( const char* addr_string, int port = 0 );

		/// Destructor.
	virtual ~Daemon();

		/** Find all information about the daemon.  This is virtual,
		  since the way you find info for a daemon depends on the type
		  of daemon.  But, all the different methods you might use to
		  find the info are supposed to be handled through here: fully
		  resolving hostnames, address files, querying the collector,
		  well-known addresses, DNS lookups, whatever it takes.  If
		  this fails, you can call error() to get a string describing
		  what went wrong.
		  @return Success or failure of getting all the info.
		*/
	virtual bool locate( void );

		/** Return the error string.  If there's ever a problem
		  enountered in the Daemon object, this will start returning a
		  string desribing it.  Returns NULL if there's no error.
		  */
	char* error( void )	{ return _error; }

		// //////////////////////////////////////////////////////////
		/// Methods for getting information about the daemon.
		// //////////////////////////////////////////////////////////

		/** Return the daemon's name.  This will return the name of
		  the daemon, which is usually the fully qualified hostname,
		  but might be something else.  For schedds (submit-only
		  schedds, in particular), there might be a user name in front
		  of the hostname, like "wright@raven.cs.wisc.edu".  For SMP
		  startds, you might have a virtual machine id in front, like
		  "vm2@raven.cs.wisc.edu".  Even if you instantiate the Daemon
		  object with a NULL as the argument for the name (to get info
		  on the local daemon), this function will always return the
		  name of the specified daemon (it never returns NULL).
		  @return The name of this daemon (not necessarily the
		  hostname).
		  */
	char* name( void );

		/** Return the hostname where the daemon is running.  This is
		  just the hostname, without the domain.  For example,
		  "raven".  This function will return NULL only if there was
		  an error in the locate() method.
		  @return Just the hostname where the daemon is running.
		  */
	char* hostname( void );

		/** 
			@return The version string of this daemon, or NULL if we
			don't know it.
		*/
	char* version( void );

		/** 
			@return The platform string of this daemon, or NULL if we
			don't know it.
		*/
	char* platform( void );

		/** Return the full hostname where the daemon is running.
		  This is the fully qualified hostname, including the domain
		  name.  For example, "raven.cs.wisc.edu".  This function will
		  return NULL only if there was an error in the locate()
		  method. 
		  @return The fully-qualified hostname where the daemon is running.
		  */
	char* fullHostname( void );

		/** Return the address of the daemon.  This is given as a
		  "sinful string", which is used throughout Condor to specify
		  IP/port pairs.  It is just a character string, of the form:
		  "<numeric.ip.addr:port>", for example,
		  "<128.105.101.17:4363>".  This function will return NULL
		  only if there was an error in the locate() method.
		  @return The sinful string of the daemon.
		  */
	char* addr( void );

		/** Return the remote pool this daemon reports to.  If the
		  requested daemon is reporting to a remote pool (not the
		  local pool for the machine that instantiated the daemon
		  object), this full hostname of the collector for the pool is
		  given here.  This function returns NULL if the daemon is
		  reporting to the local pool.
		  @return The name of the collector for the daemon.
		  */
	char* pool( void );

		/** Return the port the daemon is listening on.  This is the
		  integer port number that daemon has bound to.  It will
		  always be a positive, non-zero integer, unless there was an
		  error in locate(), in which case we return -1.
		  @return The port the daemon is listening on.
		  */
	int port( void );

		/** Return the type of the daemon.  This returns a daemon_t
		  enum that specifies the type of the requested daemon (which
		  is a required parameter to the constructor).  See
		  daemon_types.h for details on this enum.
		  @return The type of the daemon.
		  */
	daemon_t type( void )		{ return _type; }

		/** Return whether this daemon is local.  See the top-level
		  documentation for this class for details on exactly what
		  "local" means for the different types of daemons.
		  */
	bool isLocal( void )			{ return _is_local; }

		/** Returns a descriptive string for error messages.  This has
		  all the logic about printing out an appropriate string to
		  describe the daemon, it's type, and it's location.  For
		  example: "the local master", "the startd on
		  raven.cs.wisc.edu", etc.
		  */
	const char* idStr( void );

		/** Dump all info about this daemon to the given debug
		  level.
		  */
	void display( int debugflag );

		/** Dump all info about this daemon to the given FILE*
		  */
	void display( FILE* fp );

		// //////////////////////////////////////////////////////////
		/// Methods for communicating with the daemon.
		// //////////////////////////////////////////////////////////

		/**	Create a new ReliSock object, connected to the daemon.
		  Callers can optionally specify a timeout to use for the
		  connect().  If there was a failure in connect(), we delete
		  the object and return NULL.
		  @param sec Number of seconds for the timeout on connect().
		  @return A new ReliSock object connected to the daemon.  
		  */
	ReliSock* reliSock( int sec = 0 );

		/**	Create a new SafeSock object, connected to the daemon.
		  Callers can optionally specify a timeout to use for the
		  connect().  If there was a failure in connect(), we delete 
		  the object and return NULL.
		  @param sec Number of seconds for the timeout on connect().
		  @return A new SafeSock object connected to the daemon.  
		  */
	SafeSock* safeSock( int sec = 0 );

public:
		/** Send the given command to the daemon.  The caller gives
		  the command they want to send, the type of Sock they
		  want to use to send it over, and an optional timeout.  
		  We then instantiate a new Sock of the right type and
		  timeout, send the command, and finally, the eom().  The Sock
		  is then destroyed.
		  @param cmd The command you want to send.
		  @param st The type of the Sock you want to use.
		  @param sec The timeout you want to use on your Sock.
		  @return Success or failure.
		  */
	bool sendCommand( int cmd, 
					   Stream::stream_type st = Stream::reli_sock,
					   int sec = 0 );
	
		/** Send the given command to the daemon.  The caller gives
		  the command they want to send, a pointer to the Sock they
		  want us to use to send it over, and an optional timeout.
		  This method will then put the desired timeout on that sock,
		  place it in encode() mode, send the command, and finally,
		  the eom().  The sock is otherwise left alone (i.e. not
		  destroyed)
		  @param cmd The command you want to send.
		  @param sock The Sock you want to use.
		  @param sec The timeout you want to use on your Sock.
		  @return Success or failure.
		  */
	bool sendCommand( int cmd, Sock* sock, int sec = 0 );

		/** Start sending the given command to the daemon.  The caller
		  gives the command they want to send, and the type of Sock
		  they want to use to send it over.  This method will then
		  allocate a new Sock of the right type, send the command, and
		  return a pointer to the Sock while it is still in encode()
		  mode.  If there is a failure, it will return NULL.
		  THE CALLER IS RESPONSIBLE FOR DELETING THE SOCK.
		  @param cmd The command you want to send.
		  @param st The type of the Sock you want to use.
		  @param sec The timeout you want to use on your Sock.
		  @return NULL on error, or the Sock object to use for the
		  rest of the command on success.
		  */
	Sock* startCommand( int cmd, 
				Stream::stream_type st = Stream::reli_sock,
				int sec = 0 );
	
		/** Start sending the given command to the daemon.  The caller
		  gives the command they want to send, and a pointer to the
		  Sock they want us to use to send it over.  This method will
		  then place that Sock in encode() mode, send the command, and
		  return true on success, false on failure.
		  @param cmd The command you want to send.
		  @param sock The Sock you want to use.
		  @param sec The timeout you want to use on your Sock.
		  @return NULL on error, or the Sock object to use for the
		  rest of the command on success.
		*/
	bool startCommand( int cmd, Sock* sock, int sec = 0 );

protected:
	// Data members

	char* _name;
	char* _hostname;
	char* _full_hostname;
	char* _addr;
	char* _version;
	char* _platform;
	char* _pool;
	char* _error;
	char* _id_str;
	int _port;
	daemon_t _type;
	bool _is_local;
	bool _tried_locate;
	SecMan _sec_man;


		// //////////////////////////////////////////////////////////
		/// Helper methods.
		// //////////////////////////////////////////////////////////

		/** Initializes the object.
		  */
	void common_init();

		/** Helper for regular daemons (schedd, startd, master).
		  This does all the real work of finding the right address,
		  port, ip_addr, etc.  We check for address files, and query
		  the appropriate collector if that doesn't work.
		  @param subsys The subsystem string for this daemon
		  @param adtype The type of ClassAd we'll query.
		  */
	bool getDaemonInfo( const char* subsys, AdTypes adtype );

		/** Helper for central manager daemons (collector and
		  negotiator).  These are a special case since they have
		  well-known ports, instead of needing to query the central
		  manager (of course).   Also, we have to deal with the fact
		  that the CM might have multiple network interfaces, and we
		  need to be sure to connect to the right one.  We return
		  success or failure on whether we found any of the parameters
		  we were looking for that describe where the CM is.  This is
		  useful when we're trying to find the condor_view collector,
		  b/c if we can't find condor_view-specific entries, we fall
		  back and try to just find the default collector.  
		  @param subsys The subsystem string for this daemon
		  @param port The fixed port for this daemon
		  @return Whether or not we found the info we want
		  */
	bool getCmInfo( const char* subsys, int port );

		/** Set a new value for the error string.  If the error string
		  is already set, deallocate the existing string.  Then, make
		  a copy of the given string and store that in _error.
		  */
	void newError( const char* );

		/** Returns a string containing the local daemon's name.  If
		  the <subsys>_NAME parameter is set in the config file, we
		  use that, and pass it to build_valid_daemon_name() to make
		  sure we have a fully-qualified hostname.  If not, we just
		  use my_full_hostname().  The string we return is newly
		  allocated and should be deallocated with delete []. 
		  */
	char* localName( void );

		/* 
		   These helpers prevent memory leaks.  Whenever we want to
		   set one of these strings, you just use the helper, which
		   will delete any existing value of the string, and set it to
		   the value you pass in.  Unlike newError(), this DOES NOT
		   make a copy of what you pass (since so many of our util lib
		   functions already allocate a string), so the string you
		   pass in should be a strnewp()'ed or equivalent string.  
		   We simply return the value you pass in.
		*/
	char* New_full_hostname( char* );
	char* New_hostname( char* );
	char* New_name( char* );
	char* New_version( char* );
	char* New_platform( char* );
	char* New_addr( char* );
	char* New_pool( char* );

 private:

		// I can't be copied (yet)
	Daemon( const Daemon& );
	Daemon& operator = ( const Daemon& );

};

#endif /* CONDOR_DAEMON_H */
