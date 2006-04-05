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

#include "condor_common.h"
#include "condor_config.h"
#include "condor_debug.h"
#include "sysapi.h"
#include "sysapi_externs.h"

char *_sysapi_kernel_version = NULL;

/* the raw version */
/* Do not free the returned pointer */
const char *
sysapi_kernel_version_raw(void)
{
#if !defined(WIN32)
	struct utsname buf;

	if( uname(&buf) < 0 ) {
		_sysapi_kernel_version = strdup("N/A");
		return _sysapi_kernel_version;
	}
#endif

#if defined(LINUX)
	if (strncmp(buf.release, "2.2.", 4) == MATCH) {
		_sysapi_kernel_version = strdup("2.2.x");
	} else if (strncmp(buf.release, "2.3.", 4) == MATCH) {
		_sysapi_kernel_version = strdup("2.3.x");
	} else if (strncmp(buf.release, "2.4.", 4) == MATCH) {
		_sysapi_kernel_version = strdup("2.4.x");
	} else if (strncmp(buf.release, "2.5.", 4) == MATCH) {
		_sysapi_kernel_version = strdup("2.5.x");
	} else if (strncmp(buf.release, "2.6.", 4) == MATCH) {
		_sysapi_kernel_version = strdup("2.6.x");
	} else if (strncmp(buf.release, "2.7.", 4) == MATCH) {
		_sysapi_kernel_version = strdup("2.7.x");
	} else if (strncmp(buf.release, "2.8.", 4) == MATCH) {
		_sysapi_kernel_version = strdup("2.8.x");
	} else {
		_sysapi_kernel_version = strdup(buf.release);
	}
#elif defined(Solaris)
	if (strcmp(buf.release, "2.10") == MATCH ||
		strcmp(buf.release, "5.10") == MATCH)
	{
		_sysapi_kernel_version = strdup("2.10");

	} else if (strcmp(buf.release, "2.9") == MATCH ||
			strcmp(buf.release, "5.9") == MATCH)
	{
		_sysapi_kernel_version = strdup("2.9");

	} else if (strcmp(buf.release, "2.8") == MATCH ||
				strcmp(buf.release, "5.8") == MATCH)
	{
		_sysapi_kernel_version = strdup("2.8");

	} else if (strcmp(buf.release, "2.7") == MATCH ||
				strcmp(buf.release, "5.7") == MATCH)
	{
		_sysapi_kernel_version = strdup("2.7");

	} else if(strcmp(buf.release, "5.6") == MATCH ||
				strcmp(buf.release, "2.6") == MATCH)
	{
		_sysapi_kernel_version = strdup("2.6");

	} else if (strcmp(buf.release, "5.5.1") == MATCH ||
				strcmp(buf.release, "2.5.1") == MATCH)
	{
		_sysapi_kernel_version = strdup("2.5.1");

	} else if (strcmp(buf.release, "5.5") == MATCH ||
				strcmp(buf.release, "2.5") == MATCH)
	{
		_sysapi_kernel_version = strdup("2.5");

	} else {
		_sysapi_kernel_version = strdup(buf.release);

	}

#elif defined(IRIX)
	_sysapi_kernel_version = strdup(buf.release);
#elif defined(OSF1)
	_sysapi_kernel_version = strdup(buf.release);
#elif defined(Darwin) || defined(CONDOR_FREEBSD4) || defined(CONDOR_FREEBSD5) || defined(CONDOR_FREEBSD6) || defined(CONDOR_FREEBSD7)
	_sysapi_kernel_version = strdup(buf.release);
#elif defined(AIX)
	_sysapi_kernel_version = strdup(buf.release);
#elif defined(HPUX)
	_sysapi_kernel_version = strdup(buf.release);
#elif defined(WIN32)
	_sysapi_kernel_version = strdup("Unknown");
#else
#	error Please port sysapi_kernel_version_raw() to this platform!
#endif
	
	return _sysapi_kernel_version;
}

/* the cooked version */
/* Do not free the returned pointer */
const char *
sysapi_kernel_version(void)
{	
	sysapi_internal_reconfig();

	if( _sysapi_kernel_version != NULL ) {
		return _sysapi_kernel_version;
	} else {
		return sysapi_kernel_version_raw();
	}
}


