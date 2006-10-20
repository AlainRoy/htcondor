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

/*
 * Description:	 Condor IP Verify: Implementation to enforce a given
 * IP address-based security policy.  An IP address can be given a mask
 * that specifies level of access, such as READ, WRITE, ADMINISTRATOR,
 * whatever.  Methods specify the level of access for a given IP address,
 * or subnet, or domain name.  
 *
 * Most of the access levels form a heirarchy.
 * DAEMON and ADMINISTRATOR levels imply WRITE level.
 * WRITE, NEGOTIATOR, and CONFIG levels imply READ level.
 * Thus, a client that has DAEMON level access will be authorized if
 * the required access level is READ.
 */

#ifndef _CONDOR_IPVERIFY_H_
#define _CONDOR_IPVERIFY_H_

#include "condor_common.h"
#include "condor_debug.h"
template <class Key, class value> class HashTable;
class MyString;
#include "string_list.h"
#include "net_string_list.h"
#include "condor_perms.h"

/// Not_Yet_Ducumented
static const int USERVERIFY_ALLOW = 0;
/// Not_Yet_Ducumented
static const int USERVERIFY_USE_TABLE = 1;
/// Not_Yet_Ducumented
static const int USERVERIFY_ONLY_DENIES = 2;
/// Not_Yet_Ducumented
static const int USERVERIFY_DENY = 3;
///	Not_Yet_Ducumented.  15 is max for 32-bit mask
static const int USERVERIFY_MAX_PERM_TYPES = 15;
		
	/** PermString() converts the given DCpermission into the
		human-readable string version of the name.
		@param perm The permission you want to convert
		@return The string version of it
	*/
const char* PermString( DCpermission perm );

static const int USER_AUTH_FAILURE = 0;
static const int USER_AUTH_SUCCESS = 1;
static const int USER_ID_REQUIRED  = 2;

/** Not_Yet_Ducumented
 */
class IpVerify {

public:

	///
	IpVerify();

	///
	~IpVerify();

	/** Params information out of the condor_config file and
		sets up the initial permission hash table
		@return Not_Yet_Ducumented
	*/
	int Init();

	/** Verify() method returns whether connection should be allowed or
		refused.
		@param perm		   Not_Yet_Ducumented
		@param sockaddr_in Not_Yet_Ducumented
		@return USER_AUTH_SUCCESS -- if success, USER_AUTH_FAILURE -- if failer
                USER_ID_REQUIRED -- if user id is required but the caller did not pass in
	*/
	int Verify( DCpermission perm, const struct sockaddr_in *sin, const char * user = NULL );

	/** Not_Yet_Ducumented
		@param flag TRUE or FALSE.	TRUE means cache resolver lookups in our
			   hashtable cache, FALSE means do a gethostbyaddr() lookup
			   every time.
	*/
	void CacheDnsResults(int flag) { cache_DNS_results = flag; }

	bool AddAllowHost( const char* host, DCpermission perm );
		/** AddAllowHost() method adds a new host to the hostallow
			table, for the given permission level.
			@param host IP address or hostname (no wildcards)
			@param perm Permission level to use
			@return TRUE if successful, FALSE on error
		*/
private:

    typedef HashTable <MyString, int> UserPerm_t;     // <userid, perm> pair
    /* This is for listing users per host */
    typedef HashTable <MyString, StringList *>    UserHash_t;


	class PermTypeEntry {
	public:
		int behavior;
		NetStringList* allow_hosts;
		NetStringList* deny_hosts;
		UserHash_t* allow_users;
		UserHash_t* deny_users;
		PermTypeEntry();
		~PermTypeEntry();
	};
	

	// perm_names and perm_ints must match up.	this is to map
	// permission enums (ints) with their human-readable string name.
	static const char* perm_names[];
	static const int perm_ints[];
    bool has_user(UserPerm_t * , const char *, int & , MyString &);
	int add_hash_entry(const struct in_addr & sin_addr, const char * user, int new_mask);
	void fill_table( PermTypeEntry * pentry, int mask, char * list, bool allow);
	int cache_DNS_results;
    void split_entry(const char * entry, char ** host, char ** user);
	inline int allow_mask(int perm) { return (1 << (1+2*perm)); }
	inline int deny_mask(int perm) { return (1 << (2+2*perm)); }
	bool lookup_user(StringList * list, const char * user);
	char * merge(char * newPerm, char * oldPerm);
	int did_init;

	bool add_host_entry( const char* addr, int new_mask );
	void process_allow_users( void );

	PermTypeEntry* PermTypeArray[USERVERIFY_MAX_PERM_TYPES];

	typedef HashTable <struct in_addr, UserPerm_t *> PermHashTable_t;
	PermHashTable_t* PermHashTable;

	typedef HashTable <MyString, int> HostHashTable_t; // <host, Perm_t> pair
	HostHashTable_t* AllowHostsTable;
};
	


#endif // of ifndef _CONDOR_USERVERIFY_H_
