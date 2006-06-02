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
#if !defined(_QMGMT_LOG_H)
#define _QMGMT_LOG_H

/*
   The ClassAdLog class is used throughout Condor for persistent storage
   of ClassAds.  ClassAds are stored in memory using the ClassAdHashTable
   class (see classad_hashtable.h) and stored on disk in ascii format 
   using the LogRecord class hierarchy (see log.h).  The Transaction
   class (see log_transaction.h) is used for simple transactions on
   the hash table.  Currently, only one transaction can be active.  When
   no transaction is active, the log operation is immediately written
   to disk and performed on the hash table.

   Log operations are not performed on the hash table until the transaction
   is committed.  To see updates to the table before they are committed,
   use the LookupInTransaction method.  To provide strict transaction
   semantics, an interface which uses the ClassAdLog should always call
   LookupInTransaction before looking up a value in the hash table when
   a transaction is active.

   The constructor will ignore any incomplete transactions written to the
   log.  The LogBeginTransaction and LogEndTransaction classes are used
   internally by ClassAdLog to delimit transactions in the on-disk log.
*/

#include "condor_classad.h"
#include "log.h"
#include "classad_hashtable.h"
#include "log_transaction.h"

#define CondorLogOp_NewClassAd			101
#define CondorLogOp_DestroyClassAd		102
#define CondorLogOp_SetAttribute		103
#define CondorLogOp_DeleteAttribute		104
#define CondorLogOp_BeginTransaction	105
#define CondorLogOp_EndTransaction		106
#define CondorLogOp_LogHistoricalSequenceNumber 107

typedef HashTable <HashKey, ClassAd *> ClassAdHashTable;

class ClassAdLog {
public:
	ClassAdLog();
	ClassAdLog(const char *filename,int max_historical_logs=0);
	~ClassAdLog();

	void AppendLog(LogRecord *log);	// perform a log operation
	void TruncLog();				// clean log file on disk

	void BeginTransaction();
	bool AbortTransaction();
	void CommitTransaction();

	bool AdExistsInTableOrTransaction(const char *key);

	// returns 1 and sets val if corresponding SetAttribute found
	// returns 0 if no SetAttribute found
	// return -1 if DeleteAttribute or DestroyClassAd found
	int LookupInTransaction(const char *key, const char *name, char *&val);

	ClassAdHashTable table;

	// When the log is truncated (i.e. compacted), old logs
	// may be saved, up to some limit.  The default is not
	// to save any history (max = 0).
	void SetMaxHistoricalLogs(int max);
	int GetMaxHistoricalLogs();

protected:
	/** Returns handle to active transaction.  Upon return of this
		method, any active transaction is forgotten.  It is the caller's
		responsibility to eventually delete the handle returned.
		@return Pointer to the transaction state, or NULL if no transaction
		currently active.
		@see setActiveTransaction
	*/
	Transaction *getActiveTransaction();

	/** Sets the active transaction to the provided handle.  Will fail
		if there is currently a transaction already active.  Upon successful return,
		the callers handle will be reset to NULL, as the caller should no 
		longer touch the handle (including delete it).
		@param transaction A pointer to a transaction object previously
		returned by getActiveTransaction().
		@return True on success, else false.
		@see getActiveTransaction
	*/
	bool setActiveTransaction(Transaction* & transaction);

private:
	void LogState(FILE* fp);
	FILE* log_fp;

	char log_filename[_POSIX_PATH_MAX];
	Transaction *active_transaction;
	int max_historical_logs;
	unsigned long historical_sequence_number;

	bool SaveHistoricalLogs();
};

class LogHistoricalSequenceNumber : public LogRecord {
public:
	LogHistoricalSequenceNumber(unsigned long historical_sequence_number, time_t timestamp);
	int Play(void *data_structure);

	unsigned long get_historical_sequence_number() {return historical_sequence_number;}
	time_t get_timestamp() {return timestamp;}

private:
	virtual int WriteBody(FILE *fp);
	virtual int ReadBody(FILE *fp);

	unsigned long historical_sequence_number;
	time_t timestamp; //time is logged for purely informational purposes
};

class LogNewClassAd : public LogRecord {
public:
	LogNewClassAd(const char *key, const char *mytype, const char *targettype);
	virtual ~LogNewClassAd();
	int Play(void *data_structure);
	char *get_key() { return strdup(key); }
	char *get_mytype() { return strdup(mytype); }
	char *get_targettype() { return strdup(targettype); }

private:
	virtual int WriteBody(FILE *fp);
	virtual int ReadBody(FILE* fp);

	char *key;
	char *mytype;
	char *targettype;
};


class LogDestroyClassAd : public LogRecord {
public:
	LogDestroyClassAd(const char *key);
	virtual ~LogDestroyClassAd();
	int Play(void *data_structure);
	char *get_key() { return strdup(key); }

private:
	virtual int WriteBody(FILE* fp) { int r=fwrite(key, sizeof(char), strlen(key), fp); return r < strlen(key) ? -1 : r;}
	virtual int ReadBody(FILE* fp);

	char *key;
};


class LogSetAttribute : public LogRecord {
public:
	LogSetAttribute(const char *key, const char *name, const char *value);
	virtual ~LogSetAttribute();
	int Play(void *data_structure);
	char *get_key() { return strdup(key); }
	char *get_name() { return strdup(name); }
	char *get_value() { return strdup(value); }

private:
	virtual int WriteBody(FILE* fp);
	virtual int ReadBody(FILE* fp);

	char *key;
	char *name;
	char *value;
};

class LogDeleteAttribute : public LogRecord {
public:
	LogDeleteAttribute(const char *key, const char *name);
	virtual ~LogDeleteAttribute();
	int Play(void *data_structure);
	char *get_key() { return strdup(key); }
	char *get_name() { return strdup(name); }

private:
	virtual int WriteBody(FILE* fp);
	virtual int ReadBody(FILE* fp);

	char *key;
	char *name;
};

class LogBeginTransaction : public LogRecord {
public:
	LogBeginTransaction() { op_type = CondorLogOp_BeginTransaction; }
	virtual ~LogBeginTransaction(){};
private:

	virtual int WriteBody(FILE* fp) {return 0;}
	virtual int ReadBody(FILE* fp);

};

class LogEndTransaction : public LogRecord {
public:
	LogEndTransaction() { op_type = CondorLogOp_EndTransaction; }
	virtual ~LogEndTransaction(){};
private:
	virtual int WriteBody(FILE* fp) {return 0;}
	virtual int ReadBody(FILE* fp);

};

LogRecord *InstantiateLogEntry(FILE* fp, int type);

#endif
