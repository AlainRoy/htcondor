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
#ifndef CONDOR_HPUX_64BIT_TYPES_H
#define CONDOR_HPUX_64BIT_TYPES_H

/* This is pretty ugly, but it's how the system header files define
   struct stat64 and struct rlimit64 when _LARGEFILE64_SOURCE is
   defined.  We don't want that always defined on HPUX, so we just do
   this directly here. */

#if defined(HPUX10)
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef int64_t off64_t;
typedef uint64_t rlim64_t;
typedef int64_t blkcnt64_t;
#endif

#define __stat stat64
#if defined(HPUX10)
#define blkcnt_t blkcnt64_t
#define off_t off64_t
#define ad_long_t long
#elif defined(HPUX11)
#define _T_INO_T      ino_t
#define _T_OFF_T      off64_t
#define _T_BLKCNT_T   blkcnt64_t
#else
#error Inspect the 64 bit stat type specification!
#endif
#include <sys/_stat_body.h>
#if defined(HPUX10)
#undef ad_long_t
#undef off_t
#undef blkcnt_t
#elif defined(HPUX11)
#undef _T_INO_T
#undef _T_OFF_T
#undef _T_BLKCNT_T
#else
#error Inspect the 64 bit stat type specification!
#endif
#undef __stat

#define __rlimit  rlimit64
#if defined(HPUX10)
#define rlim_t rlim64_t
#elif defined(HPUX11)
#define _T_RLIM_T rlim64_t
#else
#error Inspect the 64 bit rlimit type specification!
#endif
#include <sys/_rlimit_body.h>
#if defined(HPUX10)
#undef rlim_t
#elif defined(HPUX11)
#undef _T_RLIM_T
#else
#error Inspect the 64 bit rlimit type specification!
#endif
#undef __rlimit

#endif /* CONDOR_HPUX_64BIT_TYPES_H */
