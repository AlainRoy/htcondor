/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2004, Condor Team, Computer Sciences Department,
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
#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#ifdef X509_AUTHENTICATION

#define SYSTEM 1
#define USER 2

#include "reli_sock.h"
#include "globus_gss_assist.h"
#include "sslutils.h"

enum Cred_status {
	ENV_NOT_SET = 1,
	CRED_NOT_SET,
	CRED_EXPIRED,
	CRED_SUCCESS
};

#define MAX_BUF_LENGTH 1024



typedef int CREDENTIAL_TYPE; 
typedef int Process_Type;

class X509_Credential{
	private :
		
		char *crd_name; //path to the credential file
		Process_Type my_type; //SYSTEM or USER
		Cred_status c_stat; //Valid or if failed why ?
		
	public :
			
		X509_Credential();
		X509_Credential(Process_Type,char*);
		X509_Credential(char*);
		int forward_credential(ReliSock*);
		int receive_credential(ReliSock*,char*);
		bool isvalid();
		int SetupEnv();
 		void erase_env();
};


#endif

#endif /* CREDENTIAL_H */
