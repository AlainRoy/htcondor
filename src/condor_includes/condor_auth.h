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

#ifndef CONDOR_AUTHENTICATOR
#define CONDOR_AUTHENTICATOR

#include "reli_sock.h"             // ReliSock
#include "condor_debug.h"
#include "internet.h"
#include "condor_uid.h"
#include "my_username.h"
#include "string_list.h"

const int CAUTH_NONE                    = 0;
const int CAUTH_ANY                     = 1;
const int CAUTH_CLAIMTOBE               = 2;
const int CAUTH_FILESYSTEM              = 4;
const int CAUTH_FILESYSTEM_REMOTE       = 8;
const int CAUTH_NTSSPI                  = 16;
const int CAUTH_X509                    = 32;
const int CAUTH_KERBEROS                = 64;
const int CAUTH_ANONYMOUS               = 128;

const char STR_DEFAULT_CONDOR_USER[]    = "condor";    // Default condor user
const char STR_DEFAULT_CONDOR_SPOOL[]   = "SPOOL";
const char STR_DEFAULT_CACHE_DIR[]      = "/tmp";      // Not portable!!!
const char STR_CONDOR_CACHE_DIR[]       = "CONDOR_CACHE_DIR";

//----------------------------------------------------------------------
// This is the base class for authenticators. All supported 
// authentication methods should derive from this class
//----------------------------------------------------------------------

class Condor_Auth_Base {
 public:

    Condor_Auth_Base(ReliSock * sock, int mode);
    //------------------------------------------
    // PURPOSE: Constructor for Condor_Auth_Base
    // REQUIRE: mode indicates the authenticator
    //          type. i.e. Kerberos, X.509, etc.
    // RETURNS: A authenticator object
    //------------------------------------------

    virtual ~Condor_Auth_Base();
    //------------------------------------------
    // PURPOSE: Destructor
    // REQUIRE: None
    // RETURNS: None
    //------------------------------------------

    virtual int authenticate(const char * remoteHost) = 0;
    //------------------------------------------
    // PURPOSE: authenticate with the other side 
    // REQUIRE: 1. Remote host
    //          2. Call setRemoteHost after successful authentication
    // RETURNS: TRUE -- success; FALSE -- failure
    //------------------------------------------

    virtual int isValid() const = 0;
    //------------------------------------------
    // PURPOSE: whether the authenticator is in
    //          valid state.
    // REQUIRE: None
    // RETURNS: TRUE or FALSE
    //------------------------------------------

    virtual int endTime() const;
    //------------------------------------------
    // PURPOSE: Return the expiration time
    // REQUIRE: None
    // RETURNS: -1 (default), unless overwritten
    //          by derived class such as Kerberos
    //------------------------------------------

    virtual int wrap(char*  input, 
                     int    input_len, 
                     char*& output,
                     int&   output_len);
    //------------------------------------------
    // PURPOSE: Wrap the buffer. The base class
    //          will provide some default
    //          encryption method
    // REQUIRE: A successful authentication
    // RETUNRS: TRUE -- success, FALSE -- falure
    //          May need more code later on
    //------------------------------------------
    
    virtual int unwrap(char*  input, 
                       int    input_len, 
                       char*& output, 
                       int&   output_len);
    //------------------------------------------
    // PURPOSE: Unwrap the buffer
    // REQUIRE: A successful authentication
    // RETURNS: TRUE -- success, FALSE -- failure
    //------------------------------------------

    const int isAuthenticated() const;
    //------------------------------------------
    // PURPOSE: Check out the status 
    // REQUIRE: None
    // RETURNS: Authentication status
    //          >= 1 -- authenticated
    //          0    -- not yet authenticated
    //------------------------------------------

    const int getMode() const;
    //------------------------------------------
    // PURPOSE: retrieve the mode of the authenticator
    // REQUIRE: None
    // RETURNS: mode -- see the enumeration above
    //------------------------------------------

    const char * getRemoteFQU();
    //------------------------------------------
    // PURPOSE: get the user in fully qualifed form
    //          i.e. who@somewhere.edu
    // REQUIRE: authenticated
    // RETURNS: pointer to fqu
    //------------------------------------------

    const char * getRemoteUser() const;
    //------------------------------------------
    // PURPOSE: Return user name of the remote client
    //          For example, userA
    // REQUIRE: successful authentication 
    // RETURSN: null -- if not yet authenticated
    //          const char * -- name of the remote user
    //------------------------------------------

    const char * getRemoteHost() const;
    //------------------------------------------
    // PURPOSE: Find out what remote host is 
    // REQUIRE: A successful authentication
    // RETURNS: A pointer to the remote peer
    //------------------------------------------

    const char * getRemoteDomain() const;
    const char * getLocalDomain() const;
    //------------------------------------------
    // PURPOSE: getRemoteDomain returns domain of 
    //          the remote client For example, cs.wisc.edu. 
    //          getLocalDomain returns domain for 
    //          itself
    // REQUIRE: successful authentication 
    // RETURSN: null -- if not yet authenticated
    //          const char * -- name of the remote domain
    //------------------------------------------

    Condor_Auth_Base& setAuthenticated(int authenticated);
    //------------------------------------------
    // PURPOSE: Set the state of authentication
    // REQUIRE: whether it's authenticated or not
    // RETUNRS: None (this)
    //------------------------------------------

    Condor_Auth_Base& setRemoteUser(const char * user);
    //------------------------------------------
    // PURPOSE: Set the remote user name
    // REQUIRE: Name of the remote user
    // RETUNRS: None (this)
    //------------------------------------------
    
 protected:

    Condor_Auth_Base& setRemoteHost(const char * hostAddr);
    //------------------------------------------
    // PURPOSE: Set the remote address
    // REQUIRE: address of the remote host
    // RETUNRS: None (this)
    //------------------------------------------

    Condor_Auth_Base& setRemoteDomain(const char * domain);
    //------------------------------------------
    // PURPOSE: Set the remote domain
    // REQUIRE: Name of the remote domain
    // RETUNRS: None (this)
    //------------------------------------------

    Condor_Auth_Base& setFullyQualifiedUser(const char * fqu);

    const bool isDaemon() const;
    //------------------------------------------
    // PURPOSE: Whether it's a deamon or not
    // REQUIRE: None
    // RETUNRS: 1 -- daemon; 0 -- no
    //------------------------------------------

    ReliSock *      mySock_;         // Sock to communicate

 private:
    Condor_Auth_Base();              
    //------------------------------------------
    // Private constructor which no one should
    // ever call.
    //------------------------------------------

    //------------------------------------------
    // Data (private)
    //------------------------------------------
    int             authenticated_;  // status
    int             mode_;           // Which mode are we in
    bool            isDaemon_;       // Whether it's a daemon or not
    char *          remoteUser_;     // Remote user
    char *          remoteDomain_;   // Remote domain
    char *          remoteHost_;     // Remote host
    char *          localDomain_;    // Local user domain
    char *          fqu_;            // Fully qualified
};

#endif



