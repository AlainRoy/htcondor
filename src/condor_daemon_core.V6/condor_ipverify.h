/**************************************************************
 * CONDOR Copyright Notice
 *
 * See LICENSE.TXT for additional notices and disclaimers.
 *
 * Copyright � 1990-1998 CONDOR Team, Computer Sciences Department, 
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
**************************************************************/

/*
 * Description:  Condor IP Verify: Implementation to enforce a given
 * IP address-based security policy.  An IP address can be given a mask
 * that specifies level of access, such as READ, WRITE, ADMINISTRATOR,
 * whatever.  Methods specify the level of access for a given IP address,
 * or subnet, or domain name.  
 */

#ifndef _CONDOR_IPVERIFY_H_
#define _CONDOR_IPVERIFY_H_

#include "condor_common.h"
#include "condor_debug.h"
#include "HashTable.h"
#include "string_list.h"

// enum for Daemon Core socket/command/signal permissions
enum DCpermission { ALLOW, READ, WRITE, NEGOTIATOR, IMMEDIATE_FAMILY, ADMINISTRATOR,
					OWNER };


static const int IPVERIFY_ALLOW = 0;
static const int IPVERIFY_USE_TABLE = 1;
static const int IPVERIFY_ONLY_DENIES = 2;
static const int IPVERIFY_MAX_PERM_TYPES = 15;	// 15 is max for 32-bit mask
		
class IpVerify {

public:
	// Constructor/Destructor
	IpVerify();
	~IpVerify();

	// Init() method params information out of the condor_config file and
	// sets up the initial permission hash table
	int Init();

	// Verify() method returns TRUE if connection should be allowed, and
	// FALSE if this connection should be refused.
	int Verify( DCpermission perm, const struct sockaddr_in *sin );

	// CacheDnsResults(TRUE) means cache resolver lookups in our
	// hashtable cache; CacheDnsResults(FALSE) means do a 
	// gethostbyaddr() lookup every time.  
	void CacheDnsResults(int flag) { cache_DNS_results = flag; }


private:

	// perm_names and perm_ints must match up.  this is to map
	// permission enums (ints) with their human-readable string name.
	static const char* perm_names[];
	static const int perm_ints[];

	int is_ipaddr(const char *, struct in_addr *);
	int add_hash_entry(const struct in_addr & sin_addr,int new_mask);
	int fill_table(StringList *slist, int mask);
	int cache_DNS_results;

	inline int allow_mask(int perm) { return (1 << (1+2*perm)); }
	inline int deny_mask(int perm) { return (1 << (2+2*perm)); }
	
	int did_init;

	class PermTypeEntry {
	public:
		int behavior;
		StringList* allow_hosts;
		StringList* deny_hosts;
		PermTypeEntry() {
			allow_hosts = NULL;
			deny_hosts = NULL;
			behavior = IPVERIFY_USE_TABLE;
		}
		~PermTypeEntry() {
			if (allow_hosts)
				delete allow_hosts;
			if (deny_hosts)
				delete deny_hosts;
		}
	};
	
	PermTypeEntry* PermTypeArray[IPVERIFY_MAX_PERM_TYPES];

	typedef HashTable <struct in_addr, int> PermHashTable_t;
	PermHashTable_t* PermHashTable;

};
	


#endif // of ifndef _CONDOR_IPVERIFY_H_
