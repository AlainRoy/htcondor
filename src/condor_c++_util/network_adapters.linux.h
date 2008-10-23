/***************************************************************
*
* Copyright (C) 1990-2008, Condor Team, Computer Sciences Department,
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

#ifndef _NETWORK_ADAPTERS_LINUX_H_
#define _NETWORK_ADAPTERS_LINUX_H_

#include "extArray.h"
#include "network_adapters.h"
#include "network_adapters.unix.h"


/***************************************************************
* LinuxNetworkAdapters class
* Get list of all network adapters
***************************************************************/

class LinuxNetworkAdapters : public UnixNetworkAdapters
{
	LinuxNetworkAdapters( void );

	~LinuxNetworkAdapters( void );

	/// Get list of all adapters
	static int getAllNames( ExtArray<const char *> names );
};

#define NETWORK_ADAPTERS_TYPE_DEFINED	1
typedef LinuxNetworkAdapters	NetworkAdapters;

#endif // _NETWORK_ADAPTERS_LINUX_H_
