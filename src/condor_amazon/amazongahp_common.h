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

#ifndef __AMAZONGAHP_COMMON_H__
#define __AMAZONGAHP_COMMON_H__

#include "gahp_common.h"
#include "MyString.h"
#include "string_list.h"
#include "amazonCommands.h"

#define AMAZON_SCRIPT_INTERPRETER "perl"
#define AMAZON_SCRIPT_NAME "condor_amazon.pl"

typedef bool (*ioCheckfn)(char **argv, int argc);
typedef bool (*workerfn)(char **argv, int argc, MyString &output_string);

class AmazonGahpCommand 
{
	public:
		AmazonGahpCommand(const char*, ioCheckfn, workerfn);
		MyString command;
		ioCheckfn iocheckfunction;
		workerfn workerfunction;
};

void
registerAmazonGahpCommand(const char* command, ioCheckfn iofunc, workerfn workerfunc);
bool registerAllAmazonCommands(void);
int numofAmazonCommands(void);
int allAmazonCommands(StringList &output);
bool executeIOCheckFunc(const char* cmd, char **argv, int argc);
bool executeWorkerFunc(const char* cmd, char **argv, int argc, MyString &output_string);

void set_working_dir(const char*);
const char* get_working_dir(void);
void set_amazon_lib_path(const char*);
const char* get_amazon_lib_path(void);

int parse_gahp_command (const char* raw, Gahp_Args* args);

bool check_read_access_file(const char *file);
bool check_create_file(const char *file);

bool find_amazon_lib(MyString &lib_name);
MyString delete_quotation_marks(const char *value);
void parse_param_string(const char *line, MyString &name, MyString &value, bool del_quotes);

int get_int (const char *, int *);
int get_ulong (const char *, unsigned long *);

int verify_number (const char*);
int verify_request_id(const char *);
int verify_string_name(const char *);
int verify_ami_id(const char *);
int verify_instance_id(const char *);
int verify_number_args (const int, const int);
int verify_min_number_args (const int, const int);

bool check_access_and_secret_key_file(const char* accesskeyfile, const char* secretkeyfile, MyString &err_msg);

MyString create_failure_result( int req_id, const char *err_msg);
MyString create_success_result( int req_id, StringList *result_list);

#endif
