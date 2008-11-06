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


#include "condor_common.h"
#include "condor_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <assert.h>
#include "user_log.c++.h"
#include "string_list.h"
#include "read_multiple_logs.h"

static const char * VERSION = "0.9.2";

MULTI_LOG_HASH_INSTANCE; // For the multi-log-file code...

enum Status { STATUS_OK, STATUS_CANCEL, STATUS_ERROR };

static ReadMultipleUserLogs	reader;
static int		verbosity = 0;

Status CheckArgs(int argc, char **argv);
bool CompareStringLists(StringList &reference, StringList &test);
bool GetGoodLogFiles(StringList &logFiles);
bool GetBadLogFiles();
bool ReadEvents(StringList &logFiles);
bool ReadAndTestEvent(ULogEvent *expectedEvent);
void PrintEvent(ULogEvent *event);

int main(int argc, char **argv)
{
		// Set up the dprintf stuff...
	Termlog = true;
	dprintf_config("test_multi_log");
	DebugFlags = D_ALWAYS;

	int		result = 0;

	Status tmpStatus = CheckArgs(argc, argv);
	if ( tmpStatus == STATUS_CANCEL ) {
		return 0;
	} else if ( tmpStatus == STATUS_ERROR ) {
		return 1;
	}

	printf("Testing multi-log reading code\n");
	fflush(stdout);

	if ( !GetBadLogFiles() ) {
		result = 1;
	}

	StringList		logFiles;

	if ( !GetGoodLogFiles(logFiles) ) {
		result = 1;
	}

	if ( !ReadEvents(logFiles) ) {
		result = 1;
	}

	if ( result == 0 ) {
		printf("\nTest succeeded\n");
		fflush(stdout);
	} else {
		fprintf(stderr, "\nTest FAILED !!!!!!!!!!!!!!!!!!!!!!!!\n");
		fflush(stdout);
	}

	return result;
}



Status
CheckArgs(int argc, char **argv)
{
	Status	status = STATUS_OK;

	const char *	usage = "Usage: test_multi_log [options]\n"
			"  -debug <level>: debug level (e.g., D_FULLDEBUG)\n"
			"  -usage: print this message and exit\n"
			"  -verbosity <number>: set verbosity level (default is 0)\n"
			"  -version: print the version number and compile date\n";

	for ( int index = 1; index < argc; ++index ) {
		if ( !strcmp(argv[index], "-debug") ) {
			if ( ++index >= argc ) {
				fprintf(stderr, "Value needed for -debug argument\n");
				printf("%s", usage);
				status = STATUS_ERROR;
			} else {
				set_debug_flags( argv[index] );
			}

		} else if ( !strcmp(argv[index], "-usage") ) {
			printf("%s", usage);
			status = STATUS_CANCEL;

		} else if ( !strcmp(argv[index], "-verbosity") ) {
			if ( ++index >= argc ) {
				fprintf(stderr, "Value needed for -verbosity argument\n");
				printf("%s", usage);
				status = STATUS_ERROR;
			} else {
				verbosity = atoi(argv[index]);
			}
		} else if ( !strcmp(argv[index], "-version") ) {
			printf("test_multi_log: %s, %s\n", VERSION, __DATE__);
			status = STATUS_CANCEL;

		} else {
			fprintf(stderr, "Unrecognized argument: <%s>\n", argv[index]);
			printf("%s", usage);
			status = STATUS_ERROR;
		}
	}

	return status;
}

// true == okay; false == error
bool
GetGoodLogFiles(StringList &logFiles)
{
	bool		isOkay = true;

	printf("\nGetting log files...\n");
	fflush(stdout);
	MyString		errorMsg;
	MultiLogFiles	mlf;
	errorMsg = mlf.getJobLogsFromSubmitFiles(
			"test_multi_log1.dag", "job", "dir", logFiles);
	if ( errorMsg != "" ) {
		printf("...error getting log files: %s", errorMsg.Value());
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	} else {
		printf("...succeeded\n");
		fflush(stdout);
	}

	printf("\nGetting log files...\n");
	fflush(stdout);
	errorMsg = mlf.getJobLogsFromSubmitFiles(
			"test_multi_log1.dag", "data", "dir", logFiles);
	if ( errorMsg != "" ) {
		printf("...error getting log files: %s", errorMsg.Value());
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	} else {
		printf("...succeeded\n");
		fflush(stdout);
	}

	if ( verbosity >= 1 ) {
		printf("\nLog files:\n");
		logFiles.print();
		fflush(stdout);
	}

	char currentDir[PATH_MAX];
	if ( !getcwd(currentDir, PATH_MAX) ) {
		fprintf(stderr, "Buffer is too short for getcwd()");
		fprintf(stderr, " (at %s: %d)\n", __FILE__, __LINE__);
		isOkay = false;
	}
	MyString	logPath;
	StringList	reference;
	logPath = MyString(currentDir) + "/test_multi_log.log1";
	reference.append(logPath.Value());
	logPath = MyString(currentDir) + "/../condor_c++_util/test_multi_log.log2";
	reference.append(logPath.Value());
	logPath = MyString(currentDir) + "/test_multi_log.log3";
	reference.append(logPath.Value());
	logPath = MyString(currentDir) + "/../condor_c++_util/test_multi_log.log3";
	reference.append(logPath.Value());
	logPath = MyString(currentDir) + "/test_multi_log.log4";
	reference.append(logPath.Value());

	if ( !CompareStringLists(reference, logFiles) ) {
		isOkay = false;
	}

	return isOkay;
}


// true == okay; false == error
bool
GetBadLogFiles()
{
	bool		isOkay = true;

	StringList	logFiles;

	printf("\nGetting log files...\n");
	fflush(stdout);
	MyString		errorMsg;
	MultiLogFiles	mlf;
	errorMsg = mlf.getJobLogsFromSubmitFiles(
			"test_multi_log2.dag", "job", "dir", logFiles);
	if ( errorMsg != "" ) {
		printf("...got expected error (%s)\n", errorMsg.Value());
	} else {
		printf("...should have gotten an error (test failed)");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	if ( verbosity >= 1 ) {
		printf("\nLog files:\n");
		logFiles.print();
		fflush(stdout);
	}

	printf("\nGetting log files...\n");
	fflush(stdout);
	errorMsg = mlf.getJobLogsFromSubmitFiles(
			"test_multi_log3.dag", "job", "dir", logFiles);
	if ( errorMsg != "" ) {
		printf("...got expected error (%s)\n", errorMsg.Value());
		fflush(stdout);
	} else {
		printf("...should have gotten an error (test failed)");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	if ( verbosity >= 1 ) {
		printf("\nLog files:\n");
		logFiles.print();
		fflush(stdout);
	}

	return isOkay;
}

// true == okay; false == error
bool
CompareStringLists(StringList &reference, StringList &test)
{
	bool	isOkay = true;

	if ( reference.number() != test.number() ) {
		printf("ERROR: expected %d strings, got %d", reference.number(),
				test.number());
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	reference.rewind();
	char	*str;
	while ( (str = reference.next()) != NULL ) {
		if ( !test.contains(str) ) {
			printf("ERROR: test list should contain <%s> but does not", str);
			printf(" (at %s: %d)\n", __FILE__, __LINE__);
			fflush(stdout);
			isOkay = false;
		}
	}

	return isOkay;
}

// true == okay; false == error
bool
ReadEvents(StringList &logFiles)
{
	bool	isOkay = true;

	printf("\nReading events\n");
	fflush(stdout);

	MultiLogFiles::TruncateLogs(logFiles);

	// Note: return value of false is okay here because log files are
	// empty.
	if ( !reader.initialize(logFiles) ) {
		if ( verbosity >= 1 ) printf("reader.initialize() returns false\n");
	} else {
		if ( verbosity >= 1 ) printf("reader.initialize() returns true\n");
	}

	printf("Testing detectLogGrowth() on empty files...\n");
	if ( !reader.detectLogGrowth() ) {
		printf("...succeeded\n");
		fflush(stdout);
	} else {
		printf("...failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	ULogEvent	*event;
	printf("Testing readEvent() on empty files...\n");
	if ( reader.readEvent(event) != ULOG_NO_EVENT ) {
		printf("...failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	} else {
		printf("...succeeded\n");
		fflush(stdout);
	}

	printf("Testing writing and reading events...\n");
	fflush(stdout);

	UserLog		log1("test", "test_multi_log.log1", 1, 0, 0, false);
	UserLog		log2("test", "test_multi_log.log2", 2, 0, 0, true);
	UserLog		log3("test", "test_multi_log.log3", 3, 0, 0, false);
	UserLog		log4("test", "test_multi_log.log3", 4, 0, 0, false);
	UserLog		log5("test", "test_multi_log.log4", 5, -1, -1, false);

	SubmitEvent	subE;
	strcpy(subE.submitHost, "<128.105.165.12:32779>");
	if ( !log1.writeEvent(&subE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	if ( !reader.detectLogGrowth() ) {
		printf("Error: should have gotten log growth");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	subE.cluster = 1;
	if ( !ReadAndTestEvent(&subE) ) {
		isOkay = false;
	}

	if ( reader.detectLogGrowth() ) {
		printf("Error: should NOT have gotten log growth");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	JobTerminatedEvent	termE;
	termE.normal = true;
	termE.returnValue = 0;
	if ( !log1.writeEvent(&termE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	termE.cluster = 1;
	if ( !ReadAndTestEvent(&termE) ) {
		isOkay = false;
	}

	if ( !log2.writeEvent(&subE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}
		// Sleep to make event order deterministic, to simplify the test.
	sleep(2);
	if ( !log3.writeEvent(&subE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	if ( !reader.detectLogGrowth() ) {
		printf("Error: should have gotten log growth");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	subE.cluster = 2;
	if ( !ReadAndTestEvent(&subE) ) {
		isOkay = false;
	}

	subE.cluster = 3;
	if ( !ReadAndTestEvent(&subE) ) {
		isOkay = false;
	}

	if ( !ReadAndTestEvent(NULL) ) {
		isOkay = false;
	}

	if ( !log2.writeEvent(&termE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}
		// Sleep to make event order deterministic, to simplify the test.
	sleep(2);
	if ( !log3.writeEvent(&termE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	termE.cluster = 2;
	if ( !ReadAndTestEvent(&termE) ) {
		isOkay = false;
	}

	termE.cluster = 3;
	if ( !ReadAndTestEvent(&termE) ) {
		isOkay = false;
	}

	if ( !ReadAndTestEvent(NULL) ) {
		isOkay = false;
	}

	if ( !log4.writeEvent(&subE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	subE.cluster = 4;
	if ( !ReadAndTestEvent(&subE) ) {
		isOkay = false;
	}

	if ( !ReadAndTestEvent(NULL) ) {
		isOkay = false;
	}

	if ( !log4.writeEvent(&termE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	termE.cluster = 4;
	if ( !ReadAndTestEvent(&termE) ) {
		isOkay = false;
	}

	if ( !ReadAndTestEvent(NULL) ) {
		isOkay = false;
	}

	subE.cluster = 5;
	subE.proc = -1;
	subE.subproc = -1;
	if ( !log5.writeEvent(&subE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	ExecuteEvent	execE;
	strcpy(execE.executeHost, "<128.105.666.99:12345>");
	if ( !log5.writeEvent(&execE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}

	if ( !ReadAndTestEvent(&subE) ) {
		isOkay = false;
	}
	if ( !ReadAndTestEvent(&execE) ) {
		isOkay = false;
	}

	GenericEvent	genE;
	strcpy(genE.info, "job type: transfer");
	if ( !log5.writeEvent(&genE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}
	if ( !ReadAndTestEvent(&genE) ) {
		isOkay = false;
	}

	strcpy(genE.info, "src_url: file:/dev/null");
	if ( !log5.writeEvent(&genE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}
	if ( !ReadAndTestEvent(&genE) ) {
		isOkay = false;
	}

	strcpy(genE.info, "dest_url: file:/dev/null");
	if ( !log5.writeEvent(&genE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}
	if ( !ReadAndTestEvent(&genE) ) {
		isOkay = false;
	}

	if ( !log5.writeEvent(&termE) ) {
		printf("Error: writeEvent() failed");
		printf(" (at %s: %d)\n", __FILE__, __LINE__);
		fflush(stdout);
		isOkay = false;
	}
	if ( !ReadAndTestEvent(&termE) ) {
		isOkay = false;
	}

	if ( isOkay ) {
		printf("...succeeded\n");
		fflush(stdout);
	} else {
		printf("...failed\n");
		fflush(stdout);
	}

	return isOkay;
}

bool
ReadAndTestEvent(ULogEvent *expectedEvent)
{
	bool	isOkay = true;

	ULogEvent	*event;
	if ( reader.readEvent(event) != ULOG_OK ) {
		if ( expectedEvent ) {
			printf("Error reading event");
			printf(" (at %s: %d)\n", __FILE__, __LINE__);
			fflush(stdout);
			isOkay = false;
		}
	} else {
		if ( verbosity >= 1 ) PrintEvent(event);

		if ( expectedEvent ) {
			if ( expectedEvent->eventNumber != event->eventNumber ||
					expectedEvent->cluster != event->cluster ||
					expectedEvent->proc != event->proc ||
					expectedEvent->subproc != event->subproc ) {
				printf("Event read does not match expected event");
				printf(" (at %s: %d)\n", __FILE__, __LINE__);
				printf("  Expected event: ");
				PrintEvent(expectedEvent);
				printf("  Event read: ");
				PrintEvent(event);
				fflush(stdout);
				isOkay = false;
			}
		} else {
			printf("Error: should NOT have gotten an event");
			printf(" (at %s: %d)\n", __FILE__, __LINE__);
			fflush(stdout);
			isOkay = false;
		}
	}

	if ( !isOkay ) {
		printf("  Expecting ");
		PrintEvent(expectedEvent);
		fflush(stdout);
	}

	return isOkay;
}

void
PrintEvent(ULogEvent *event)
{
	if ( event ) {
		printf("Event: %d.%d.%d: %s\n", event->cluster, event->proc,
				event->subproc,ULogEventNumberNames[event->eventNumber]);
		fflush(stdout);
	} else {
		printf("Event: NULL\n");
		fflush(stdout);
	}
}
