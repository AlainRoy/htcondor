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
#ifndef _CONDOR_TDMAN_H_
#define _CONDOR_TDMAN_H_

#include "condor_daemon_core.h"
#include "../condor_transferd/condor_td.h"
#include "HashTable.h"

// After a transferd registers successfully, this will be called back.
typedef int (Service::*TDRegisterCallback)(TransferDaemon *td);

// When a transferd dies, this will be called back
typedef int (Service::*TDReaperCallback)(int pid, int status, 
	TransferDaemon *td);

// This holds the status for a particular transferd
enum TDMode {
	// The representative object of the transfer daemon has been made, but 
	// the work to invoke it hasn't been completed yet.
	TD_PRE_INVOKED,
	// the transferd has been invoked, but hasn't registered
	TD_INVOKED,
	// the transferd has been registered and is available for use
	TD_REGISTERED,
	// someone has come back to the schedd and said the registered transferd
	// is not connectable or wasn't there, or whatever.
	TD_MIA
};

// smart structure;
// identification of a transferd for continuation purposes across registered
// callback funtions.
class TDUpdateContinuation
{
	public:
		TDUpdateContinuation(MyString s, MyString f, MyString i, MyString n)
		{
			sinful = s;
			fquser = f;
			id = i;
			name = n;
		}

		TDUpdateContinuation(char *s, char *f, char *i, char *n)
		{
			sinful = s;
			fquser = f;
			id = i;
			name = n;
		}

		// sinful string of the td
		MyString sinful;
		// fully qualified user the td is running as
		MyString fquser;
		// the id string the schedd gave to the ransferd
		MyString id;
		// the registration name for the socket handler
		MyString name;
};

// This represents the invocation, and current status, of a transfer daemon
class TransferDaemon
{
	public:
		TransferDaemon(MyString fquser, MyString id, TDMode status);
		/*
		TransferDaemon(MyString fquser, MyString id, TDMode status,
						TDRegisterCallback reg_callback, 
						TDReaperCallback reap_callback);
		*/
		~TransferDaemon();

		// This transferd had been started on behalf of a fully qualified user
		// This records who that user is.
		void set_fquser(MyString fquser);
		MyString get_fquser(void);

		// Set the specific ID string associated with this td.
		void set_id(MyString id);
		MyString get_id(void);

		// The schedd changes the state of this object according to what it
		// knows about the status of the daemon itself.
		void set_status(TDMode s);
		TDMode get_status(void);

		// To whom should this td report?
		void set_schedd_sinful(MyString sinful);
		MyString get_schedd_sinful(void);

		// Who is this transferd (after it registers)
		void set_sinful(MyString sinful);
		void set_sinful(char *sinful);
		MyString get_sinful(void);

		// The socket the schedd uses to listen to updates from the td.
		// This is also what was the original registration socket.
		void set_update_sock(ReliSock *update_sock);
		ReliSock* get_update_sock(void);

		// The socket one must use to send to a new transfer request to
		// this daemon.
		void set_treq_sock(ReliSock *treq_sock);
		ReliSock* get_treq_sock(void);

		// If I happen to have a transfer request when making this object,
		// store them here until the transferd registers and I can deal 
		// with it then. This object assumes ownership of the memory unless
		// false is returned.
		bool add_transfer_request(TransferRequest *treq);

		// If there are any pending requests, do them, and respond to the
		// client paired with those requests.
		bool push_transfer_requests(void);

		// When a transferd daemon produces an update, the manager will 
		// give it to the td object for it to do what it will with it.
		// This function does NOT own the memory passed to it.
		bool update_transfer_request(ClassAd *update);

		// If I want to restart a real transferd associated with this object,
		// then clear out the parts that represent a living daemon.
		void clear(void);

	private:
		// The sinful string of this transferd after registration
		MyString m_sinful;

		// the owner of the transferd
		MyString m_fquser;

		// The id string associated with this td
		MyString m_id;

		// The schedd to which this td must report
		MyString m_schedd_sinful;

		// current status about this transferd I requested
		TDMode m_status;

		// Storage of Transfer Requests when first enqueued
		SimpleList<TransferRequest*> m_treqs;

		// Storage of Transfer Requests when transferd is doing its work
		// Key: capability, Value: TransferRequest
		HashTable<MyString, TransferRequest*> m_treqs_in_progress;

		// Storage of Transfer Requests when transferd had its say about them
		SimpleList<TransferRequest*> m_treqs_finished;

		// The registration socket that the schedd also receives updates on.
		ReliSock *m_update_sock; 

		// The socket the schedd initiated to send treqs to the td
		ReliSock *m_treq_sock;

		// When the tranferd wakes up an registers, call this when the
		// registration process in complete
		TDRegisterCallback m_reg_func;

		// If the transferd dies, invoke this callback with its identification
		// and how it died.
		TDReaperCallback m_reap_func;
};

class TDMan : public Service
{
	public:
		TDMan();
		~TDMan();

		// returns NULL if no td is currently available. Returns the
		// transfer daemon object invoked for this user. If no such transferd
		// exists, then return NULL;
		TransferDaemon* find_td_by_user(MyString fquser);
		TransferDaemon* find_td_by_user(char *fquser);

		// when the td registers itself, figure out to which of my objects its
		// identity string pairs.
		TransferDaemon* find_td_by_ident(MyString id);
		TransferDaemon* find_td_by_ident(char *ident);

		// I've determined that I have to create a transfer daemon, so have the
		// caller set up a TransferDaemon object, and then I'll be responsible
		// for starting it.
		// The caller has specified the fquser and id in the object.
		// This function will dig around in the td object and fire up a 
		// td according to what it finds.
		// Returns true if everything went ok, false if the transferd could
		// not be started.
		bool invoke_a_td(TransferDaemon *td);

		// install some daemon core handlers to deal with transferd's wanting
		// to talk to me.
		void register_handlers(void);

		// what to do when a td dies or exits
		int transferd_reaper(int pid, int status);

		// deal with a td that comes back to register itself to me.
		int transferd_registration(int cmd, Stream *sock);

		// handle updates from a transferd
		int transferd_update(Stream *sock);

		// same thing like in Scheduler object.
		void refuse(Stream *s);

	private:
		// This is where I store the table of transferd objects, each
		// representing the status and connection to a transferd.
		// Key: fquser, Value: transferd
		HashTable<MyString, TransferDaemon*> *m_td_table;

		// Store an association between an id given to a specific transferd
		// and the user that id ultimately identifies.
		// Key: id, Value: fquser
		HashTable<MyString, MyString> *m_id_table;

		// a table of pids associated with running transferds so reapers
		// can do their work, among other things. This is a hash table
		// of alias pointers into m_td_table.
		HashTable<long, TransferDaemon*> *m_td_pid_table;
	
	// NOTE: When we get around to implementing multiple tds per user with
	// different id strings, then find_any_td must return a list of tds and
	// the hash tables value must be that corresponding list.
};

#endif /* _CONDOR_TDMAN_H_ */








