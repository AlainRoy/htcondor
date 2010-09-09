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

/*
	This code tests the sin_to_string() function implementation.
 */

#include "condor_common.h"
#include "condor_debug.h"
#include "condor_config.h"
#include "internet.h"
#include "function_test_driver.h"
#include "unit_test_utils.h"
#include "emit.h"

static bool test_normal_case(void);

bool FTEST_sin_to_string(void) {
		// beginning junk for getPortFromAddr(() {
	emit_function("sin_to_string(sockaddr_in)");
	emit_comment("Converts a sockaddr_in to a sinful string.");
	emit_problem("None");
	
		// driver to run the tests and all required setup
	FunctionDriver driver;
	driver.register_function(test_normal_case);
	
		// run the tests
	return driver.do_all_functions();
}

static bool test_normal_case() {
	emit_test("Is normal input converted correctly?");
	in_addr ip;
	if(inet_aton("192.168.0.2", &ip) == 0) {
		emit_alert("inet_aton() returned failure.");
		ABORT;
	}
	unsigned int port = 47;
	emit_input_header();
	emit_param("IP", "192.168.0.2");
	emit_param("PORT", "47");
	emit_output_expected_header();
	char* expected = strdup("<192.168.0.2:47>");
	emit_retval(expected);
	emit_output_actual_header();
	sockaddr_in sa_in;
	sa_in.sin_addr.s_addr = ip.s_addr;
	sa_in.sin_port = htons(port);
	char* sinstring = sin_to_string(&sa_in);
	emit_retval(sinstring);
	if(strcmp(expected, sinstring) != 0) {
		free(expected);
		free(sinstring);
		FAIL;
	}
	free(expected);
	PASS;
}
