/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
 * CONDOR Copyright Notice
 *
 * See LICENSE.TXT for additional notices and disclaimers.
 *
 * Copyright (c)1990-1998 CONDOR Team, Computer Sciences Department, 
 * University of Wisconsin-Madison, Madison, WI.  All Rights Reserved.  
 * No use of the CONDOR Software Program Source Code is authorized 
 * without the express consent of the CONDOR Team.  For more information 
 * contact: CONDOR Team, Attention: Professor Miron Livny, 
 * 7367 Computer Sciences, 1210 W. Dayton St., Madison, WI 53706-1685, 
 * (608) 262-0856 or miron@cs.wisc.edu.
 *
 * U.S. Government Rights Restrictions: Use, duplication, or disclosure 
 * by the U.S. Government is subject to restrictions as set forth in 
 * subparagraph (c)(1)(ii) of The Rights in Technical Data and Computer 
 * Software clause at DFARS 252.227-7013 or subparagraphs (c)(1) and 
 * (2) of Commercial Computer Software-Restricted Rights at 48 CFR 
 * 52.227-19, as applicable, CONDOR Team, Attention: Professor Miron 
 * Livny, 7367 Computer Sciences, 1210 W. Dayton St., Madison, 
 * WI 53706-1685, (608) 262-0856 or miron@cs.wisc.edu.
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

typedef HashTable <HashKey, ClassAd *> ClassAdHashTable;

class ClassAdLog {
public:
	ClassAdLog();
	ClassAdLog(const char *filename);
	~ClassAdLog();

	void AppendLog(LogRecord *log);	// perform a log operation
	void TruncLog();				// clean log file on disk

	void BeginTransaction();
	void AbortTransaction();
	void CommitTransaction();

	// returns 1 and sets val if corresponding SetAttribute found
	// returns 0 if no SetAttribute found
	// return -1 if DeleteAttribute or DestroyClassAd found
	int LookupInTransaction(const char *key, const char *name, ExprTree *&expr);

	ClassAdHashTable table;

private:
	void 	LogState(FILE *fp);
	char 	log_filename[_POSIX_PATH_MAX];
	FILE 	*log_fp;
	Source	src;
	Sink	snk;
	bool 	EmptyTransaction;
	Transaction *active_transaction;
};

class LogNewClassAd : public LogRecord {
public:
	LogNewClassAd(const char *key);
	~LogNewClassAd();
	int Play(void *data_structure);
	char *get_key() { return strdup(key); }

private:
	virtual int WriteBody(FILE*);
	virtual int ReadBody(FILE*);

	char *key;
};


class LogDestroyClassAd : public LogRecord {
public:
	LogDestroyClassAd(const char *key);
	~LogDestroyClassAd();
	int Play(void *data_structure);
	char *get_key() { return strdup(key); }

private:
	virtual int WriteBody(FILE *fp) { return fwrite(key, 1, strlen(key), fp); }
	virtual int ReadBody(FILE *);

	char *key;
};


class LogSetAttribute : public LogRecord {
public:
	LogSetAttribute(const char *key, const char *name, ExprTree *expr);
	~LogSetAttribute();
	int Play(void *data_structure);
	char *get_key() { return strdup(key); }
	char *get_name() { return strdup(name); }
	ExprTree *get_expr() { return (expr ? expr->Copy( ) : NULL) ; }

private:
	virtual int WriteBody(FILE *);
	virtual int ReadBody(FILE *);

	char *key;
	char *name;
	ExprTree *expr;
};

class LogDeleteAttribute : public LogRecord {
public:
	LogDeleteAttribute(const char *key, const char *name);
	~LogDeleteAttribute();
	int Play(void *data_structure);
	char *get_key() { return strdup(key); }
	char *get_name() { return strdup(name); }

private:
	virtual int WriteBody(FILE *);
	virtual int ReadBody(FILE *);

	char *key;
	char *name;
};

class LogBeginTransaction : public LogRecord {
public:
	LogBeginTransaction() { op_type = CondorLogOp_BeginTransaction; }
private:
	virtual int WriteBody(FILE *) { return 0; }
	virtual int ReadBody(FILE *) { return 0; }
};

class LogEndTransaction : public LogRecord {
public:
	LogEndTransaction() { op_type = CondorLogOp_EndTransaction; }
private:
	virtual int WriteBody(FILE *) { return 0; }
	virtual int ReadBody(FILE *) { return 0; }
};

LogRecord *InstantiateLogEntry(FILE *fp, int type);
#endif
