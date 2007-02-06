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


// List of attributes used in ClassAds  If you find yourself using anything
// other than ATTR_<blah> to add/lookup expressions/variables *STOP*, add the
// new variable to this file and use the ATTR_<blah> symbolic constant.  --RR

#ifndef __CONDOR_ATTRIBUTES_H__
#define __CONDOR_ATTRIBUTES_H__

extern const char * ATTR_ACCOUNTING_GROUP;
extern const char * ATTR_ACTION_CONSTRAINT;
extern const char * ATTR_ACTION_IDS;
extern const char * ATTR_ACTION_RESULT;
extern const char * ATTR_ACTION_RESULT_TYPE;
extern const char * ATTR_ACTIVITY;
extern const char * ATTR_ALLOW_NOTIFICATION_CC;
extern const char * ATTR_ALL_REMOTE_HOSTS;
extern const char * ATTR_APPEND_FILES;
extern const char * ATTR_ARCH;
extern const char * ATTR_AVAIL_BANDWIDTH_TO_SUBMIT_MACHINE;
extern const char * ATTR_AVAIL_BANDWIDTH_TO_LAST_CKPT_SERVER;
extern const char * ATTR_AVAIL_BANDWIDTH_TO_CKPT_SERVER;
extern const char * ATTR_AVAIL_SINCE;
extern const char * ATTR_AVAIL_TIME;
extern const char * ATTR_AVAIL_TIME_ESTIMATE;
extern const char * ATTR_BANDWIDTH_TO_SUBMIT_MACHINE;
extern const char * ATTR_BANDWIDTH_TO_LAST_CKPT_SERVER;
extern const char * ATTR_BANDWIDTH_TO_CKPT_SERVER;
extern const char * ATTR_BUFFER_SIZE;
extern const char * ATTR_BUFFER_FILES;
extern const char * ATTR_BUFFER_BLOCK_SIZE;
extern const char * ATTR_BUFFER_BLOCKS_USED;
extern const char * ATTR_BUFFER_PREFETCH_SIZE;
extern const char * ATTR_BYTES_SENT;
extern const char * ATTR_BYTES_RECVD;
extern const char * ATTR_CAPABILITY;
extern const char * ATTR_CKPT_SERVER;
extern const char * ATTR_COD_CLAIMS;
extern const char * ATTR_COMMAND;
extern const char * ATTR_COMPRESS_FILES;
extern const char * ATTR_REQUESTED_CAPACITY;
extern const char * ATTR_CKPT_ARCH;
extern const char * ATTR_CKPT_LAST_READ;
extern const char * ATTR_CKPT_OPSYS;
extern const char * ATTR_CLAIM_ID;
extern const char * ATTR_CLAIM_STATE;
extern const char * ATTR_CLAIM_TYPE;
extern const char * ATTR_CLIENT_MACHINE;
extern const char * ATTR_CLOCK_DAY;
extern const char * ATTR_CLOCK_MIN;
extern const char * ATTR_CLUSTER_ID;
extern const char * ATTR_AUTO_CLUSTER_ID;
extern const char * ATTR_AUTO_CLUSTER_ATTRS;
extern const char * ATTR_COMPLETION_DATE;
#define ATTR_CONDOR_LOAD_AVG			AttrGetName( ATTRE_CONDOR_LOAD_AVG )
#define ATTR_CONDOR_ADMIN				AttrGetName( ATTRE_CONDOR_ADMIN )
extern const char * ATTR_CONSOLE_IDLE;
extern const char * ATTR_CONTINUE;
extern const char * ATTR_CORE_SIZE;
extern const char * ATTR_CRON_MINUTES;
extern const char * ATTR_CRON_HOURS;
extern const char * ATTR_CRON_DAYS_OF_MONTH;
extern const char * ATTR_CRON_MONTHS;
extern const char * ATTR_CRON_DAYS_OF_WEEK;
extern const char * ATTR_CRON_NEXT_RUNTIME;
extern const char * ATTR_CRON_CURRENT_TIME_RANGE;
extern const char * ATTR_CRON_PREP_TIME;
extern const char * ATTR_CPU_BUSY;
extern const char * ATTR_CPU_BUSY_TIME;
extern const char * ATTR_CPU_IS_BUSY;
extern const char * ATTR_CPUS;
extern const char * ATTR_CURRENT_HOSTS;
extern const char * ATTR_CURRENT_JOBS_RUNNING;
extern const char * ATTR_CURRENT_RANK;
extern const char * ATTR_CURRENT_TIME;
extern const char * ATTR_DAEMON_START_TIME;
extern const char * ATTR_DAG_NODE_NAME;
extern const char * ATTR_DAG_NODE_NAME_ALT;
extern const char * ATTR_DAGMAN_JOB_ID;
extern const char * ATTR_DEFERRAL_OFFSET;
extern const char * ATTR_DEFERRAL_TIME;
extern const char * ATTR_DEFERRAL_WINDOW;
extern const char * ATTR_DESTINATION;
extern const char * ATTR_DISK;
extern const char * ATTR_DISK_USAGE;
extern const char * ATTR_EMAIL_ATTRIBUTES;
extern const char * ATTR_ENTERED_CURRENT_ACTIVITY;
extern const char * ATTR_ENTERED_CURRENT_STATE;
extern const char * ATTR_ENTERED_CURRENT_STATUS;
extern const char * ATTR_ERROR_STRING;
extern const char * ATTR_EXCEPTION_HIERARCHY;
extern const char * ATTR_EXCEPTION_NAME;
extern const char * ATTR_EXCEPTION_TYPE;
extern const char * ATTR_EXECUTABLE_SIZE;
extern const char * ATTR_EXIT_REASON;
extern const char * ATTR_FETCH_FILES;
extern const char * ATTR_FILE_NAME;
extern const char * ATTR_FILE_SIZE;
extern const char * ATTR_FILE_SYSTEM_DOMAIN;
extern const char * ATTR_FILE_REMAPS;
extern const char * ATTR_FILE_READ_COUNT;
extern const char * ATTR_FILE_READ_BYTES;
extern const char * ATTR_FILE_WRITE_COUNT;
extern const char * ATTR_FILE_WRITE_BYTES;
extern const char * ATTR_FILE_SEEK_COUNT;
extern const char * ATTR_FLOCKED_JOBS;
extern const char * ATTR_FLAVOR;
extern const char * ATTR_FORCE;
extern const char * ATTR_GID;
extern const char * ATTR_GLOBAL_JOB_ID;
extern const char * ATTR_GZIP;
extern const char * ATTR_GLOBUS_CONTACT_STRING;
extern const char * ATTR_GLOBUS_DELEGATION_URI;
extern const char * ATTR_GLOBUS_GRAM_VERSION;
extern const char * ATTR_GLOBUS_RESOURCE;
extern const char * ATTR_GLOBUS_RESOURCE_UNAVAILABLE_TIME;
extern const char * ATTR_JOB_MUST_EXPAND;
extern const char * ATTR_GLOBUS_RSL;
extern const char * ATTR_GLOBUS_STATUS;
extern const char * ATTR_GLOBUS_XML;
extern const char * ATTR_X509_USER_PROXY;
extern const char * ATTR_X509_USER_PROXY_SUBJECT;
extern const char * ATTR_GLOBUS_JOBMANAGER_TYPE;
extern const char * ATTR_GLOBUS_SUBMIT_ID;
extern const char * ATTR_GRIDFTP_SERVER_JOB;
extern const char * ATTR_GRIDFTP_URL_BASE;
extern const char * ATTR_REQUESTED_GRIDFTP_URL_BASE;
extern const char * ATTR_GRID_RESOURCE;
extern const char * ATTR_GRID_RESOURCE_UNAVAILABLE_TIME;
extern const char * ATTR_GRID_JOB_ID;
// ckireyev myproxy
extern const char * ATTR_MYPROXY_SERVER_DN;
extern const char * ATTR_MYPROXY_HOST_NAME;
extern const char * ATTR_MYPROXY_PASSWORD;
extern const char * ATTR_MYPROXY_CRED_NAME;
extern const char * ATTR_MYPROXY_REFRESH_THRESHOLD;
extern const char * ATTR_MYPROXY_NEW_PROXY_LIFETIME;
// END ckireyev myproxy
extern const char * ATTR_HAS_CHECKPOINTING;
extern const char * ATTR_HAS_FILE_TRANSFER;
extern const char * ATTR_HAS_PER_FILE_ENCRYPTION;
extern const char * ATTR_HAS_IO_PROXY;
extern const char * ATTR_HAS_JAVA;
extern const char * ATTR_HAS_JIC_LOCAL_CONFIG;
extern const char * ATTR_HAS_JIC_LOCAL_STDIN;
extern const char * ATTR_HAS_JOB_AD;
extern const char * ATTR_HAS_JOB_AD_FROM_FILE;
extern const char * ATTR_HAS_JOB_DEFERRAL;
extern const char * ATTR_HAS_MPI;
extern const char * ATTR_HAS_OLD_VANILLA;
extern const char * ATTR_HAS_PVM;
extern const char * ATTR_HAS_RECONNECT;
extern const char * ATTR_HAS_REMOTE_SYSCALLS;
extern const char * ATTR_HAS_TDP;
extern const char * ATTR_HAS_SOAP_API;
extern const char * ATTR_HAS_WIN_RUN_AS_OWNER;
extern const char * ATTR_HELD_JOBS;
extern const char * ATTR_HOLD_KILL_SIG;
extern const char * ATTR_IDLE_JOBS;
extern const char * ATTR_IMAGE_SIZE;
extern const char * ATTR_IS_DAEMON_CORE;
extern const char * ATTR_IS_OWNER;
extern const char * ATTR_IS_QUEUE_SUPER_USER;
extern const char * ATTR_INACTIVE;
extern const char * ATTR_JAR_FILES;
extern const char * ATTR_JAVA_MFLOPS;
extern const char * ATTR_JAVA_VENDOR;
extern const char * ATTR_JAVA_VERSION;
extern const char * ATTR_JOB_ACTION;
extern const char * ATTR_JOB_ARGUMENTS1;
extern const char * ATTR_JOB_ARGUMENTS2;
extern const char * ATTR_JOB_CMD;
extern const char * ATTR_JOB_CMDEXT;
extern const char * ATTR_JOB_CORE_DUMPED;
extern const char * ATTR_JOB_CORE_FILENAME;
extern const char * ATTR_JOB_CURRENT_START_DATE;
extern const char * ATTR_JOB_DURATION;
extern const char * ATTR_JOB_ENVIRONMENT1;
extern const char * ATTR_JOB_ENVIRONMENT1_DELIM;
extern const char * ATTR_JOB_ENVIRONMENT2;
extern const char * ATTR_JOB_ERROR;
extern const char * ATTR_JOB_ERROR_ORIG;
extern const char * ATTR_JOB_ERROR_SIZE;
extern const char * ATTR_JOB_KEYWORD;
extern const char * ATTR_JOB_LEASE_DURATION;
extern const char * ATTR_JOB_LEASE_EXPIRATION;
extern const char * ATTR_JOB_SPOOL_EXECUTABLE;
extern const char * ATTR_JOB_EXIT_STATUS;
extern const char * ATTR_JOB_EXIT_REQUIREMENTS;
extern const char * ATTR_JOB_ID;
extern const char * ATTR_JOB_FINISHED_HOOK_DONE;
extern const char * ATTR_JOB_INPUT;
extern const char * ATTR_JOB_IWD;
extern const char * ATTR_JOB_JAVA_VM_ARGS1;
extern const char * ATTR_JOB_JAVA_VM_ARGS2;
extern const char * ATTR_JOB_REMOTE_IWD;
extern const char * ATTR_JOB_RUNAS_OWNER;
extern const char * ATTR_JOB_LOCAL_CPU;
extern const char * ATTR_JOB_LOCAL_SYS_CPU;
extern const char * ATTR_JOB_LOCAL_USER_CPU;
extern const char * ATTR_JOB_MANAGED;
extern const char * ATTR_JOB_MANAGED_MANAGER;
extern const char * ATTR_JOB_MATCHED;
extern const char * ATTR_JOB_NOOP;
extern const char * ATTR_JOB_NOOP_EXIT_SIGNAL;
extern const char * ATTR_JOB_NOOP_EXIT_CODE;
extern const char * ATTR_JOB_NOTIFICATION;
extern const char * ATTR_JOB_OUTPUT;
extern const char * ATTR_JOB_OUTPUT_ORIG;
extern const char * ATTR_JOB_OUTPUT_SIZE;
extern const char * ATTR_JOB_PID;
extern const char * ATTR_JOB_PRIO;
extern const char * ATTR_JOB_COMMITTED_TIME;
extern const char * ATTR_JOB_LANGUAGE;
extern const char * ATTR_JOB_LAST_START_DATE;
extern const char * ATTR_JOB_LEAVE_IN_QUEUE;
extern const char * ATTR_JOB_REMOTE_SYS_CPU;
extern const char * ATTR_JOB_REMOTE_USER_CPU;
extern const char * ATTR_JOB_REMOTE_WALL_CLOCK;
extern const char * ATTR_JOB_ROOT_DIR;
extern const char * ATTR_JOB_RUN_COUNT;
extern const char * ATTR_JOB_START;
extern const char * ATTR_JOB_START_DATE;
extern const char * ATTR_JOB_STATE;
extern const char * ATTR_JOB_STATUS;
extern const char * ATTR_JOB_STATUS_ON_RELEASE;
extern const char * ATTR_JOB_UNIVERSE;
extern const char * ATTR_JOB_GRID_TYPE;
extern const char * ATTR_JOB_WALL_CLOCK_CKPT;
extern const char * ATTR_KEYBOARD_IDLE;
extern const char * ATTR_KEYSTORE_FILE;
extern const char * ATTR_KEYSTORE_ALIAS;
extern const char * ATTR_KEYSTORE_PASSPHRASE_FILE;
extern const char * ATTR_KFLOPS;
extern const char * ATTR_KILL;
extern const char * ATTR_KILL_SIG;
extern const char * ATTR_LAST_AVAIL_INTERVAL;
extern const char * ATTR_LAST_BENCHMARK;
extern const char * ATTR_LAST_CKPT_SERVER;
extern const char * ATTR_LAST_CKPT_TIME;
extern const char * ATTR_LAST_CLAIM_ID;
extern const char * ATTR_LAST_CLAIM_STATE;
extern const char * ATTR_LAST_VACATE_TIME;
extern const char * ATTR_LAST_HEARD_FROM;
extern const char * ATTR_LAST_HOLD_REASON;
extern const char * ATTR_LAST_HOLD_REASON_CODE;
extern const char * ATTR_LAST_HOLD_REASON_SUBCODE;
extern const char * ATTR_LAST_JOB_LEASE_RENEWAL;
extern const char * ATTR_LAST_JOB_LEASE_RENEWAL_FAILED;
extern const char * ATTR_LAST_MATCH_TIME;
extern const char * ATTR_LAST_MATCH_LIST_PREFIX;
extern const char * ATTR_LAST_MATCH_LIST_LENGTH;
extern const char * ATTR_LAST_REJ_MATCH_TIME;
extern const char * ATTR_LAST_REJ_MATCH_REASON;
extern const char * ATTR_LAST_PERIODIC_CHECKPOINT;
extern const char * ATTR_LAST_RELEASE_REASON;
extern const char * ATTR_LAST_REMOTE_HOST;
extern const char * ATTR_LAST_UPDATE;
extern const char * ATTR_LOCAL_CREDD;
extern const char * ATTR_LOCAL_FILES;
extern const char * ATTR_LOAD_AVG;
extern const char * ATTR_MACHINE;
extern const char * ATTR_MASTER_IP_ADDR;
extern const char * ATTR_MAX_HOSTS;
extern const char * ATTR_MAX_JOB_RETIREMENT_TIME;
extern const char * ATTR_MAX_JOBS_RUNNING;
extern const char * ATTR_MEMORY;
extern const char * ATTR_MIN_HOSTS;
extern const char * ATTR_MIPS;
extern const char * ATTR_MIRROR_ACTIVE;
extern const char * ATTR_MIRROR_JOB_ID;
extern const char * ATTR_MIRROR_LEASE_TIME;
extern const char * ATTR_MIRROR_RELEASED;
extern const char * ATTR_MIRROR_REMOTE_LEASE_TIME;
extern const char * ATTR_MIRROR_SCHEDD;
extern const char * ATTR_MIRROR_SUBMITTER_ID;
extern const char * ATTR_MPI_IS_MASTER;
extern const char * ATTR_MPI_MASTER_ADDR;
extern const char * ATTR_PARALLEL_IS_MASTER;
extern const char * ATTR_PARALLEL_MASTER_ADDR;
extern const char * ATTR_MY_TYPE;
extern const char * ATTR_NAME;
extern const char * ATTR_NICE_USER;
extern const char * ATTR_NEGOTIATOR_REQUIREMENTS;
extern const char * ATTR_NEXT_CLUSTER_NUM;
extern const char * ATTR_NEXT_JOB_START_DELAY;
extern const char * ATTR_NODE;
extern const char * ATTR_NORDUGRID_RSL;
extern const char * ATTR_NOTIFY_USER;
extern const char * ATTR_NOTIFY_JOB_SCHEDULER;
extern const char * ATTR_NT_DOMAIN;
extern const char * ATTR_NUM_COD_CLAIMS;
extern const char * ATTR_NUM_CKPTS;
extern const char * ATTR_NUM_CKPTS_RAW;
extern const char * ATTR_NUM_GLOBUS_SUBMITS;
extern const char * ATTR_NUM_MATCHES;
extern const char * ATTR_NUM_HOPS_TO_SUBMIT_MACHINE;
extern const char * ATTR_NUM_HOPS_TO_LAST_CKPT_SERVER;
extern const char * ATTR_NUM_HOPS_TO_CKPT_SERVER;
extern const char * ATTR_NUM_PIDS;
extern const char * ATTR_NUM_RESTARTS;
extern const char * ATTR_NUM_SYSTEM_HOLDS;
extern const char * ATTR_NUM_USERS;
extern const char * ATTR_OPSYS;
extern const char * ATTR_ORIG_MAX_HOSTS;
extern const char * ATTR_OWNER;
extern const char * ATTR_PARALLEL_SCHEDULING_GROUP;
extern const char * ATTR_PARALLEL_SCRIPT_SHADOW;
extern const char * ATTR_PARALLEL_SCRIPT_STARTER;
extern const char * ATTR_PERIODIC_CHECKPOINT;
#define ATTR_PLATFORM					AttrGetName( ATTRE_PLATFORM )
extern const char * ATTR_PREEMPTING_ACCOUNTING_GROUP;
extern const char * ATTR_PREEMPTING_RANK;
extern const char * ATTR_PREEMPTING_OWNER;
extern const char * ATTR_PREEMPTING_USER;
extern const char * ATTR_PREFERENCES;
extern const char * ATTR_PREV_SEND_ESTIMATE;
extern const char * ATTR_PREV_RECV_ESTIMATE;
extern const char * ATTR_PRIO;
extern const char * ATTR_PROC_ID;
extern const char * ATTR_Q_DATE;
extern const char * ATTR_RANK;
extern const char * ATTR_REAL_UID;
extern const char * ATTR_RELEASE_REASON;
extern const char * ATTR_REMOTE_HOST;
extern const char * ATTR_REMOTE_HOSTS;
extern const char * ATTR_REMOTE_JOB_ID;
extern const char * ATTR_REMOTE_OWNER;
extern const char * ATTR_REMOTE_POOL;
extern const char * ATTR_REMOTE_SCHEDD;
extern const char * ATTR_REMOTE_SPOOL_DIR;
extern const char * ATTR_REMOTE_USER;
extern const char * ATTR_REMOTE_USER_PRIO;
extern const char * ATTR_REMOTE_VIRTUAL_MACHINE_ID;
extern const char * ATTR_REMOVE_KILL_SIG;
extern const char * ATTR_REMOVE_REASON;
extern const char * ATTR_REQUEUE_REASON;
extern const char * ATTR_REQUIREMENTS;
extern const char * ATTR_RESULT;
extern const char * ATTR_RSC_BYTES_SENT;
extern const char * ATTR_RSC_BYTES_RECVD;
extern const char * ATTR_RUNNING_JOBS;
extern const char * ATTR_RUNNING_COD_JOB;
extern const char * ATTR_RUN_BENCHMARKS;
extern const char * ATTR_SHADOW_IP_ADDR;
extern const char * ATTR_MY_ADDRESS;
extern const char * ATTR_SCHEDD_IP_ADDR;
extern const char * ATTR_SCHEDD_NAME;
extern const char * ATTR_SCHEDULER;
extern const char * ATTR_SHADOW_WAIT_FOR_DEBUG;
extern const char * ATTR_SOURCE;
extern const char * ATTR_STAGE_IN_START;
extern const char * ATTR_STAGE_IN_FINISH;
extern const char * ATTR_STAGE_OUT_START;
extern const char * ATTR_STAGE_OUT_FINISH;
extern const char * ATTR_START;
extern const char * ATTR_START_LOCAL_UNIVERSE;
extern const char * ATTR_START_SCHEDULER_UNIVERSE;
extern const char * ATTR_STARTD_IP_ADDR;
extern const char * ATTR_STATE;
extern const char * ATTR_STARTER_IP_ADDR;
extern const char * ATTR_STARTER_ABILITY_LIST;
extern const char * ATTR_STARTER_IGNORED_ATTRS;
extern const char * ATTR_STARTER_ULOG_FILE;
extern const char * ATTR_STARTER_ULOG_USE_XML;
extern const char * ATTR_STARTER_WAIT_FOR_DEBUG;
extern const char * ATTR_STATUS;
extern const char * ATTR_STREAM_INPUT;
extern const char * ATTR_STREAM_OUTPUT;
extern const char * ATTR_STREAM_ERROR;
extern const char * ATTR_SUBMITTER_ID;
extern const char * ATTR_SUBMITTOR_PRIO;
extern const char * ATTR_SUBNET;
extern const char * ATTR_SUSPEND;
extern const char * ATTR_SUSPEND_JOB_AT_EXEC;
extern const char * ATTR_TARGET_TYPE;
extern const char * ATTR_TIME_TO_LIVE;
extern const char * ATTR_TOTAL_CLAIM_RUN_TIME;
extern const char * ATTR_TOTAL_CLAIM_SUSPEND_TIME;
#define ATTR_TOTAL_CONDOR_LOAD_AVG			AttrGetName( ATTRE_TOTAL_LOAD )

extern const char * ATTR_TOOL_DAEMON_ARGS1;
extern const char * ATTR_TOOL_DAEMON_ARGS2;
extern const char * ATTR_TOOL_DAEMON_CMD;
extern const char * ATTR_TOOL_DAEMON_ERROR;
extern const char * ATTR_TOOL_DAEMON_INPUT;
extern const char * ATTR_TOOL_DAEMON_OUTPUT;

extern const char * ATTR_TOTAL_CPUS;
extern const char * ATTR_TOTAL_DISK;
extern const char * ATTR_TOTAL_FLOCKED_JOBS;
extern const char * ATTR_TOTAL_REMOVED_JOBS;
extern const char * ATTR_TOTAL_HELD_JOBS;
extern const char * ATTR_TOTAL_IDLE_JOBS;
extern const char * ATTR_TOTAL_JOB_ADS;
extern const char * ATTR_TOTAL_JOB_RUN_TIME;
extern const char * ATTR_TOTAL_JOB_SUSPEND_TIME;
extern const char * ATTR_TOTAL_LOAD_AVG;
extern const char * ATTR_TOTAL_MEMORY;
extern const char * ATTR_TOTAL_RUNNING_JOBS;
extern const char * ATTR_TOTAL_LOCAL_RUNNING_JOBS;
extern const char * ATTR_TOTAL_LOCAL_IDLE_JOBS;
extern const char * ATTR_TOTAL_SCHEDULER_RUNNING_JOBS;
extern const char * ATTR_TOTAL_SCHEDULER_IDLE_JOBS;
extern const char * ATTR_TOTAL_TIME_IN_CYCLE;
extern const char * ATTR_TOTAL_VIRTUAL_MACHINES;
extern const char * ATTR_TOTAL_VIRTUAL_MEMORY;
extern const char * ATTR_UID;
extern const char * ATTR_UID_DOMAIN;
extern const char * ATTR_ULOG_FILE;
extern const char * ATTR_ULOG_USE_XML;
extern const char * ATTR_UPDATE_INTERVAL;
extern const char * ATTR_CLASSAD_LIFETIME;
extern const char * ATTR_UPDATE_PRIO;
extern const char * ATTR_UPDATE_SEQUENCE_NUMBER;
extern const char * ATTR_USE_GRID_SHELL;
extern const char * ATTR_USER;
extern const char * ATTR_VACATE;
extern const char * ATTR_VACATE_TYPE;
extern const char * ATTR_VIRTUAL_MEMORY;
extern const char * ATTR_WANT_CHECKPOINT;
extern const char * ATTR_WANT_CLAIMING;
extern const char * ATTR_WANT_IO_PROXY;
extern const char * ATTR_WANT_MATCH_DIAGNOSTICS;
extern const char * ATTR_WANT_PARALLEL_SCHEDULING_GROUPS;
extern const char * ATTR_WANT_REMOTE_SYSCALLS;
extern const char * ATTR_WANT_REMOTE_IO;
extern const char * ATTR_WANT_SUBMIT_NET_STATS;
extern const char * ATTR_WANT_LAST_CKPT_SERVER_NET_STATS;
extern const char * ATTR_WANT_CKPT_SERVER_NET_STATS;
extern const char * ATTR_WANT_AD_REVAULATE;
extern const char * ATTR_COLLECTOR_IP_ADDR;
extern const char * ATTR_NEGOTIATOR_IP_ADDR;
extern const char * ATTR_CREDD_IP_ADDR;
extern const char * ATTR_NUM_HOSTS_TOTAL;
extern const char * ATTR_NUM_HOSTS_CLAIMED;
extern const char * ATTR_NUM_HOSTS_UNCLAIMED;
extern const char * ATTR_NUM_HOSTS_OWNER;
extern const char * ATTR_MAX_RUNNING_JOBS;
#define ATTR_VERSION					AttrGetName( ATTRE_VERSION )
extern const char * ATTR_SCHEDD_BIRTHDATE;
extern const char * ATTR_SHADOW_VERSION;
extern const char * ATTR_VIRTUAL_MACHINE_ID;
extern const char * ATTR_SHOULD_TRANSFER_FILES;
extern const char * ATTR_WHEN_TO_TRANSFER_OUTPUT;
extern const char * ATTR_TRANSFER_TYPE;
extern const char * ATTR_TRANSFER_FILES;
extern const char * ATTR_TRANSFER_KEY;
extern const char * ATTR_TRANSFER_EXECUTABLE;
extern const char * ATTR_TRANSFER_INPUT;
extern const char * ATTR_TRANSFER_OUTPUT;
extern const char * ATTR_TRANSFER_ERROR;
extern const char * ATTR_TRANSFER_INPUT_FILES;
extern const char * ATTR_TRANSFER_INTERMEDIATE_FILES;
extern const char * ATTR_TRANSFER_OUTPUT_FILES;
extern const char * ATTR_TRANSFER_OUTPUT_REMAPS;
extern const char * ATTR_ENCRYPT_INPUT_FILES;
extern const char * ATTR_ENCRYPT_OUTPUT_FILES;
extern const char * ATTR_DONT_ENCRYPT_INPUT_FILES;
extern const char * ATTR_DONT_ENCRYPT_OUTPUT_FILES;
extern const char * ATTR_TRANSFER_SOCKET;
extern const char * ATTR_SERVER_TIME;
extern const char * ATTR_SHADOW_BIRTHDATE;
extern const char * ATTR_HOLD_REASON;
extern const char * ATTR_HOLD_REASON_CODE;
extern const char * ATTR_HOLD_REASON_SUBCODE;
extern const char * ATTR_WANT_MATCHING;
extern const char * ATTR_WANT_RESOURCE_AD;
extern const char * ATTR_TOTAL_SUSPENSIONS;
extern const char * ATTR_LAST_SUSPENSION_TIME;
extern const char * ATTR_CUMULATIVE_SUSPENSION_TIME;

extern const char * ATTR_ON_EXIT_BY_SIGNAL;
extern const char * ATTR_ON_EXIT_CODE;
extern const char * ATTR_ON_EXIT_HOLD_CHECK;
extern const char * ATTR_ON_EXIT_REMOVE_CHECK;
extern const char * ATTR_ON_EXIT_SIGNAL;
extern const char * ATTR_POST_ON_EXIT_BY_SIGNAL;
extern const char * ATTR_POST_ON_EXIT_SIGNAL;
extern const char * ATTR_POST_ON_EXIT_CODE;
extern const char * ATTR_POST_EXIT_REASON;
extern const char * ATTR_PERIODIC_HOLD_CHECK;
extern const char * ATTR_PERIODIC_RELEASE_CHECK;
extern const char * ATTR_PERIODIC_REMOVE_CHECK;
extern const char * ATTR_TIMER_REMOVE_CHECK;
extern const char * ATTR_TIMER_REMOVE_CHECK_SENT;
extern const char * ATTR_GLOBUS_RESUBMIT_CHECK;
extern const char * ATTR_REMATCH_CHECK;

extern const char * ATTR_SEC_AUTHENTICATION_METHODS_LIST;
extern const char * ATTR_SEC_AUTHENTICATION_METHODS;
extern const char * ATTR_SEC_CRYPTO_METHODS;
extern const char * ATTR_SEC_AUTHENTICATION;
extern const char * ATTR_SEC_ENCRYPTION;
extern const char * ATTR_SEC_INTEGRITY;
extern const char * ATTR_SEC_ENACT;
extern const char * ATTR_SEC_RESPOND;
extern const char * ATTR_SEC_COMMAND;
extern const char * ATTR_SEC_AUTH_COMMAND;
extern const char * ATTR_SEC_SID;
extern const char * ATTR_SEC_SUBSYSTEM;
extern const char * ATTR_SEC_REMOTE_VERSION;
extern const char * ATTR_SEC_SERVER_ENDPOINT;
extern const char * ATTR_SEC_SERVER_COMMAND_SOCK;
extern const char * ATTR_SEC_SERVER_PID;
extern const char * ATTR_SEC_PARENT_UNIQUE_ID;
extern const char * ATTR_SEC_PACKET_COUNT;
extern const char * ATTR_SEC_NEGOTIATION;
extern const char * ATTR_SEC_VALID_COMMANDS;
extern const char * ATTR_SEC_SESSION_DURATION;
extern const char * ATTR_SEC_USER;
extern const char * ATTR_SEC_NEW_SESSION;
extern const char * ATTR_SEC_USE_SESSION;
extern const char * ATTR_SEC_COOKIE;

extern const char * ATTR_MULTIPLE_TASKS_PER_PVMD;

extern const char * ATTR_UPDATESTATS_TOTAL;
extern const char * ATTR_UPDATESTATS_SEQUENCED;
extern const char * ATTR_UPDATESTATS_LOST;
extern const char * ATTR_UPDATESTATS_HISTORY;

extern const char * ATTR_QUILL_ENABLED;
extern const char * ATTR_QUILL_NAME;
extern const char * ATTR_QUILL_IS_REMOTELY_QUERYABLE;
extern const char * ATTR_QUILL_DB_IP_ADDR;
extern const char * ATTR_QUILL_DB_NAME;
extern const char * ATTR_QUILL_DB_QUERY_PASSWORD;

extern const char * ATTR_QUILL_SQL_TOTAL;
extern const char * ATTR_QUILL_SQL_LAST_BATCH;

extern const char * ATTR_CHECKPOINT_PLATFORM;
extern const char * ATTR_LAST_CHECKPOINT_PLATFORM;
extern const char * ATTR_IS_VALID_CHECKPOINT_PLATFORM;

extern const char * ATTR_HAD_IS_ACTIVE;
extern const char * ATTR_HAD_LIST;
extern const char * ATTR_HAD_INDEX;
extern const char * ATTR_TERMINATION_PENDING;
extern const char * ATTR_TERMINATION_EXITREASON;

extern const char * ATTR_TREQ_DIRECTION;
extern const char * ATTR_TREQ_INVALID_REQUEST;
extern const char * ATTR_TREQ_INVALID_REASON;
extern const char * ATTR_TREQ_HAS_CONSTRAINT;
extern const char * ATTR_TREQ_JOBID_LIST;
extern const char * ATTR_TREQ_PEER_VERSION;
extern const char * ATTR_TREQ_FTP;
extern const char * ATTR_TREQ_TD_SINFUL;
extern const char * ATTR_TREQ_TD_ID;
extern const char * ATTR_TREQ_CONSTRAINT;
extern const char * ATTR_TREQ_JOBID_ALLOW_LIST;
extern const char * ATTR_TREQ_JOBID_DENY_LIST;
extern const char * ATTR_TREQ_CAPABILITY;
extern const char * ATTR_TREQ_WILL_BLOCK;
extern const char * ATTR_TREQ_NUM_TRANSFERS;
extern const char * ATTR_TREQ_UPDATE_STATUS;
extern const char * ATTR_TREQ_UPDATE_REASON;
extern const char *ATTR_TREQ_SIGNALED;
extern const char *ATTR_TREQ_SIGNAL;
extern const char *ATTR_TREQ_EXIT_CODE;


// This is a record of the job exit status from a standard universe job exit
// via waitpid. It is in the job ad to implement the terminate_pending
// feature. It has to be here because of rampant global variable usage in the
// standard universe shadow. It saved a tremendous amount of code to just
// put this value in the job ad.
extern const char * ATTR_WAITPID_STATUS;
extern const char * ATTR_TERMINATION_REASON;

// Valid settings for ATTR_JOB_MANAGED.
	// Managed by an external process (gridmanager)
extern const char * MANAGED_EXTERNAL;
	// Schedd should manage as normal
extern const char * MANAGED_SCHEDD;
	// Schedd should manage as normal.  External process doesn't want back.
extern const char * MANAGED_DONE;



// Enumerate the ones that can't be constant strings..
typedef enum
{
	ATTRE_CONDOR_LOAD_AVG = 0,
	ATTRE_CONDOR_ADMIN,
	ATTRE_PLATFORM,
	ATTRE_TOTAL_LOAD,
	ATTRE_VERSION,
	// ....
} CONDOR_ATTR;

// Prototypes
int AttrInit( void );
const char *AttrGetName( CONDOR_ATTR );


// ------------------------------------------------------
// Stuff private to the environment variable manager
// ------------------------------------------------------
#if defined _CONDOR_ATTR_MAIN

// Flags available
typedef enum
{
	ATTR_FLAG_NONE = 0,			// No special treatment
	ATTR_FLAG_DISTRO,			// Plug in the distribution name
	ATTR_FLAG_DISTRO_UC,		// Plug in the UPPER CASE distribution name
	ATTR_FLAG_DISTRO_CAP,		// Plug in the Capitolized distribution name
} CONDOR_ATTR_FLAGS;

// Data on each env variable
typedef struct
{
	CONDOR_ATTR			sanity;		// Used to sanity check
	const char			*string;	// My format string
	CONDOR_ATTR_FLAGS	flag;		// Flags
	const char			*cached;	// Cached answer
} CONDOR_ATTR_ELEM;

// The actual list of variables indexed by CONDOR_ATTR
static CONDOR_ATTR_ELEM CondorAttrList[] =
{
	{ ATTRE_CONDOR_LOAD_AVG,"%sLoadAvg",		ATTR_FLAG_DISTRO_CAP },
	{ ATTRE_CONDOR_ADMIN,	"%sAdmin",			ATTR_FLAG_DISTRO_CAP },
	{ ATTRE_PLATFORM,		"%sPlatform",		ATTR_FLAG_DISTRO_CAP },
	{ ATTRE_TOTAL_LOAD,		"Total%sLoadAvg",	ATTR_FLAG_DISTRO_CAP },
	{ ATTRE_VERSION,		"%sVersion",		ATTR_FLAG_DISTRO_CAP },
	// ....
};
#endif		// _CONDOR_ATTR_MAIN

#endif
