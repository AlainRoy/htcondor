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

#ifndef READ_MULTIPLE_LOGS_H
#define READ_MULTIPLE_LOGS_H

// This class allows reading more than one user log file, as if they
// were merged into one big log file, sorted by event time.  It returns
// ULogEvent objects just like the ReadUserLog class.  It tries to 
// return the events sorted by time, but cannot distinguish between 
// events in different files that occurred in the same second. 

#include "condor_common.h"
#include "user_log.c++.h"
#include "MyString.h"
#include "string_list.h"

class ReadMultipleUserLogs
{
public:
	ReadMultipleUserLogs();
	explicit ReadMultipleUserLogs(StringList &listLogFileNames);

	~ReadMultipleUserLogs();

	    /** Sets the list of log files to monitor; throws away any
		    previous list of files to monitor.
		 */
    bool initialize(StringList &listLogFileNames);

    	/** Returns the "next" event from any log file.  The event pointer 
        	is set to point to a newly instatiated ULogEvent object.
    	*/
    ULogEventOutcome readEvent (ULogEvent * & event);

	    /** Returns true iff any of the logs we're monitoring grew since
		    the last time this method was called.
		 */
	bool detectLogGrowth();

	    /** Deletes the given log files.
		 */
	static void DeleteLogs(StringList &logFileNames);

	    /** Gets the userlog files used by a dag
		    on success, the return value will be ""
		    on failure, it will be an appropriate error message
	    */
    static MyString getJobLogsFromSubmitFiles(const MyString &strDagFileName,
			const MyString &jobKeyword, StringList &listLogFilenames);

	    /** Gets the log file from a Condor submit file.
		    on success, the return value will be the log file name
		    on failure, it will be ""
		 */
    static MyString loadLogFileNameFromSubFile(const MyString &strSubFilename);

private:
	void cleanup();

	struct LogFileEntry
	{
		bool isInitialized;
		MyString strFilename;
		ReadUserLog readUserLog;
		ULogEvent *pLastLogEvent;
		off_t logSize;
	};

	int iLogFileCount;
	LogFileEntry *pLogFileEntries;

	    /** Goes through the list of logs and tries to initialize (open
		    the file of) any that aren't initialized yet.
			Returns true iff it successfully initialized *any* previously-
			uninitialized log.
		 */
	bool initializeUninitializedLogs();

		/** Returns true iff the given log grew since the last time
		    we called this method on it.
		 */
	static bool LogGrew(LogFileEntry &log);

	    /** Read the entire contents of the given file into a MyString.
		 */
    static MyString readFileToString(const MyString &strFilename);

		/**
		 * Get the given parameter if it is defined in the given submit file
		 * line.
		 * @param The submit file line.
		 * @param The name of the parameter to get.
		 * @return The parameter value defined in that line, or "" if the
		 *   parameter is not defined.
		 */
	static MyString getParamFromSubmitLine(MyString &submitLine,
			const char *paramName);

		/**
		 * Combine input ("physical") lines that end with the given
		 * continuation character into "logical" lines.
		 * @param Input string list of "physical" lines.
		 * @param Continuation character.
		 * @param Output string list of "logical" lines.
		 */
	static MyString CombineLines(StringList &listIn, char continuation,
			StringList &listOut);
};

#endif
