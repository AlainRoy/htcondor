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
#ifndef CONDOR_MACROS_H
#define CONDOR_MACROS_H

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

	/* Don't prevent calls to open() or fopen() in the ckpt or 
	 * remote syscalls code.
	 */
#if defined(IN_CKPT_LIB) || defined(REMOTE_SYSCALLS)
#	if !defined(_CONDOR_ALLOW_OPEN_AND_FOPEN)
#		define _CONDOR_ALLOW_OPEN_AND_FOPEN 1
#	endif
#endif

	/* For security purposes, do not allow open() or fopen().
	 * Folks should use the wrapper functions in 
	 * src/condor_c++_util/condor_open.[hC] 
	 */
#ifdef _CONDOR_ALLOW_OPEN_AND_FOPEN
#  ifndef _CONDOR_ALLOW_OPEN
#     define _CONDOR_ALLOW_OPEN 1
#  endif
#  ifndef _CONDOR_ALLOW_FOPEN
#     define _CONDOR_ALLOW_FOPEN 1
#  endif
#endif
#ifndef _CONDOR_ALLOW_OPEN
   #include "../condor_c++_util/condor_open.h"
   #ifdef open
   #  undef open
   #endif
   #define open (Calls_to_open_must_use___safe_open_wrapper___instead)   
   #ifdef __GNUC__
   #   pragma GCC poison Calls_to_open_must_use___safe_open_wrapper___instead
   #endif
#endif
#ifndef _CONDOR_ALLOW_FOPEN
   #include "../condor_c++_util/condor_open.h"
   #ifdef fopen
   #  undef fopen
   #endif
   /* Our fopen macro does not play well with the new 
	  version of the Platform SDK for Vista	*/
   #ifndef WIN32
   #   define fopen (Calls_to_fopen_must_use___safe_fopen_wrapper___instead)   
   #endif
   #ifdef __GNUC__
   #   pragma GCC poison Calls_to_fopen_must_use___safe_fopen_wrapper___instead
   #endif
#endif 


#endif /* CONDOR_MACROS_H */
