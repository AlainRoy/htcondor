/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
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

#ifndef AMAZON_COMMANDS_H
#define AMAZON_COMMANDS_H

#include "condor_common.h"
#include "condor_string.h"
#include "MyString.h"
#include "string_list.h"

#include <map>

#include <smdevp.h>

// EC2 Commands
#define AMAZON_COMMAND_VM_START             "AMAZON_VM_START"
#define AMAZON_COMMAND_VM_STOP              "AMAZON_VM_STOP"
#define AMAZON_COMMAND_VM_REBOOT            "AMAZON_VM_REBOOT"
#define AMAZON_COMMAND_VM_STATUS            "AMAZON_VM_STATUS"
#define AMAZON_COMMAND_VM_STATUS_ALL        "AMAZON_VM_STATUS_ALL"
#define AMAZON_COMMAND_VM_RUNNING_KEYPAIR   "AMAZON_VM_RUNNING_KEYPAIR"
#define AMAZON_COMMAND_VM_CREATE_GROUP      "AMAZON_VM_CREATE_GROUP"
#define AMAZON_COMMAND_VM_DELETE_GROUP      "AMAZON_VM_DELETE_GROUP"
#define AMAZON_COMMAND_VM_GROUP_NAMES       "AMAZON_VM_GROUP_NAMES"
#define AMAZON_COMMAND_VM_GROUP_RULES       "AMAZON_VM_GROUP_RULES"
#define AMAZON_COMMAND_VM_ADD_GROUP_RULE    "AMAZON_VM_ADD_GROUP_RULE"
#define AMAZON_COMMAND_VM_DEL_GROUP_RULE    "AMAZON_VM_DEL_GROUP_RULE"
#define AMAZON_COMMAND_VM_CREATE_KEYPAIR    "AMAZON_VM_CREATE_KEYPAIR"
#define AMAZON_COMMAND_VM_DESTROY_KEYPAIR   "AMAZON_VM_DESTROY_KEYPAIR"
#define AMAZON_COMMAND_VM_KEYPAIR_NAMES     "AMAZON_VM_KEYPAIR_NAMES"
#define AMAZON_COMMAND_VM_REGISTER_IMAGE    "AMAZON_VM_REGISTER_IMAGE"
#define AMAZON_COMMAND_VM_DEREGISTER_IMAGE  "AMAZON_VM_DEREGISTER_IMAGE"

// S3 Commands
#define AMAZON_COMMAND_S3_ALL_BUCKETS		"AMAZON_S3_ALL_BUCKETS"
#define AMAZON_COMMAND_S3_CREATE_BUCKET		"AMAZON_S3_CREATE_BUCKET"
#define AMAZON_COMMAND_S3_DELETE_BUCKET		"AMAZON_S3_DELETE_BUCKET"
#define AMAZON_COMMAND_S3_LIST_BUCKET		"AMAZON_S3_LIST_BUCKET"
#define AMAZON_COMMAND_S3_UPLOAD_FILE		"AMAZON_S3_UPLOAD_FILE"
#define AMAZON_COMMAND_S3_UPLOAD_DIR		"AMAZON_S3_UPLOAD_DIR"
#define AMAZON_COMMAND_S3_DELETE_FILE		"AMAZON_S3_DELETE_FILE"
#define AMAZON_COMMAND_S3_DOWNLOAD_FILE		"AMAZON_S3_DOWNLOAD_FILE"
#define AMAZON_COMMAND_S3_DOWNLOAD_BUCKET	"AMAZON_S3_DOWNLOAD_BUCKET"

#define GENERAL_GAHP_ERROR_CODE	"GAHPERROR"
#define GENERAL_GAHP_ERROR_MSG	"GAHP_ERROR"

class AmazonRequest {
	public:
		AmazonRequest();
		virtual ~AmazonRequest();

        virtual bool SendRequest();

    protected:
        typedef std::map< std::string, std::string > AttributeValueMap;
        AttributeValueMap query_parameters;
        
        std::string serviceURL;
        std::string accessKeyFile;
        std::string secretKeyFile;
        
        std::string errorMessage;
        std::string errorCode;
        
        std::string resultString;
};

// EC2 Commands

class AmazonVMStart : public AmazonRequest {
	public:
		AmazonVMStart();
		virtual ~AmazonVMStart();

        virtual bool SendRequest();

		static bool ioCheck(char **argv, int argc);
		static bool workerFunction(char **argv, int argc, std::string &result_string);

    protected:
        std::string instanceID;
};

class AmazonVMStop : public AmazonRequest {
	public:
		AmazonVMStop();
		virtual ~AmazonVMStop();

		static bool ioCheck(char **argv, int argc);
		static bool workerFunction(char **argv, int argc, std::string &result_string);
};

#define AMAZON_STATUS_RUNNING "running"
#define AMAZON_STATUS_PENDING "pending"
#define AMAZON_STATUS_SHUTTING_DOWN "shutting-down"
#define AMAZON_STATUS_TERMINATED "terminated"

class AmazonStatusResult {
	public:
		std::string instance_id;
		std::string status;
		std::string ami_id;
		std::string public_dns;
		std::string private_dns;
		std::string keyname;
		std::string instancetype;

        std::vector< std::string > securityGroups;
};

class AmazonVMStatusAll : public AmazonRequest {
	public:
		AmazonVMStatusAll();
		virtual ~AmazonVMStatusAll();

        virtual bool SendRequest();

		static bool ioCheck(char **argv, int argc);
		static bool workerFunction(char **argv, int argc, std::string &result_string);

    protected:
        std::vector< AmazonStatusResult > results;
};

class AmazonVMStatus : public AmazonVMStatusAll {
	public:
		AmazonVMStatus();
		virtual ~AmazonVMStatus();

		static bool ioCheck(char **argv, int argc);
		static bool workerFunction(char **argv, int argc, std::string &result_string);
};

class AmazonVMRunningKeypair : public AmazonVMStatusAll {
	public:
		AmazonVMRunningKeypair();
		virtual ~AmazonVMRunningKeypair();

		static bool ioCheck(char **argv, int argc);
		static bool workerFunction(char **argv, int argc, std::string &result_string);
};

class AmazonVMCreateKeypair : public AmazonRequest {
	public:
		AmazonVMCreateKeypair();
		virtual ~AmazonVMCreateKeypair();

        virtual bool SendRequest();

		static bool ioCheck(char **argv, int argc);
		static bool workerFunction(char **argv, int argc, std::string &result_string);

    protected:
    	std::string privateKeyFileName;
};

class AmazonVMDestroyKeypair : public AmazonRequest {
	public:
		AmazonVMDestroyKeypair();
		virtual ~AmazonVMDestroyKeypair();

		static bool ioCheck(char **argv, int argc);
		static bool workerFunction(char **argv, int argc, std::string &result_string);
};

class AmazonVMKeypairNames : public AmazonRequest {
	public:
		AmazonVMKeypairNames();
		virtual ~AmazonVMKeypairNames();

        virtual bool SendRequest();

		static bool ioCheck(char **argv, int argc);
		static bool workerFunction(char **argv, int argc, std::string &result_string);

    protected:
        StringList keyNames;
};


#endif
