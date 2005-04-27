#ifndef _QUILL_SCHEMA_DEF_H_
#define _QUILL_SCHEMA_DEF_H_

#define SCHEMA_CHECK_STR "SELECT relname FROM pg_class WHERE relname = 'clusterads_num' OR relname = 'clusterads_str' OR relname = 'history_horizontal' OR relname = 'history_vertical' OR relname = 'procads_num' OR relname = 'procads_str' OR relname = 'jobqueuepollinginfo' OR relname='procads' OR relname='clusterads';"

#define SCHEMA_SYS_TABLE_NUM 	9

/* 
	Defintion of Delete String 

	DELETE FROM ProcAds_Str;
	DELETE FROM ProcAds_Num; 
	DELETE FROM ClusterAds_Str;
	DELETE FROM ClusterAds_Num; 
	DELETE FROM JobQueuePollingInfo;
*/

#define SCHEMA_DELETE_STR "DELETE FROM ProcAds_Str; DELETE FROM ProcAds_Num; DELETE FROM ClusterAds_Str; DELETE FROM ClusterAds_Num; DELETE FROM JobQueuePollingInfo;"


/*
	Defintion of Drop Table String 

	DROP TABLE ProcAds_Str;
	DROP TABLE ProcAds_Num;
	DROP TABLE ClusterAds_Str;
	DROP TABLE ClusterAds_Num;
	DROP TABLE History_Horizontal;
	DROP TABLE History_Vertical;
	DROP TABLE JobQueuePollingInfo;
        DROP VIEW ProcAds;
        DROP VIEW ClusterAds;
*/

#define SCHEMA_DROP_TABLE_STR "DROP TABLE ProcAds_Str; DROP TABLE ProcAds_Num; DROP TABLE ClusterAds_Str; DROP TABLE ClusterAds_Num; DROP TABLE History_Horizontal; DROP TABLE History_Vertical; DROP TABLE JobQueuePollingInfo;DROP VIEW ProcAds; DROP VIEW ClusterAds;"

/*
	Defintion of Create ProcAds Table String 

	CREATE TABLE ProcAds_Str (
		cid		text,
		pid		text,
		attr	text,
		val		text,
		primary key (cid, pid, attr)
	);

#	CREATE INDEX ProcAds_Str_I_cid ON ProcAds_Str (cid);
#	CREATE INDEX ProcAds_Str_I_pid ON ProcAds_Str (pid);
#	CREATE INDEX ProcAds_Str_I_attr ON ProcAds_Str (attr);
#	CREATE INDEX ProcAds_Str_I_val ON ProcAds_Str (val);
#	CREATE INDEX ProcAds_Str_I_attr_val ON ProcAds_Str (attr, val);
all indices are removed.  Only the pkey index created by default exists

	CREATE TABLE ProcAds_Num (
		cid		text,
		pid		text,
		attr	text,
		val		double precision,
		primary key (cid, pid, attr)
	);

#	CREATE INDEX ProcAds_Num_I_cid ON ProcAds_Num (cid);
#	CREATE INDEX ProcAds_Num_I_pid ON ProcAds_Num (pid);
#	CREATE INDEX ProcAds_Num_I_attr ON ProcAds_Num (attr);
#	CREATE INDEX ProcAds_Num_I_val ON ProcAds_Num (val);
#	CREATE INDEX ProcAds_Num_I_attr_val ON ProcAds_Num (attr, val);
all indices are removed.  Only the pkey index created by default exists

    Defintion of ProcAds view
    CREATE VIEW ProcAds as 
	select cid, pid, attr, val from ProcAds_Str UNION ALL
	select cid, pid, attr, cast(val as text) from ProcAds_Num;
*/

#define SCHEMA_CREATE_PROCADS_TABLE_STR "CREATE TABLE ProcAds_Str (cid int, pid int, attr text, val text, primary key(cid, pid, attr)); CREATE TABLE ProcAds_Num (cid int, pid int, attr text, val double precision, primary key(cid,pid,attr)); CREATE VIEW ProcAds as select cid, pid, attr, val from ProcAds_Str UNION ALL select cid, pid, attr, cast(val as text) from ProcAds_Num;"

/*
	Defintion of Create ClusterAds Table String 

CREATE TABLE ClusterAds_Str (
	cid		text,
	attr	text,
	val		text,
	primary key (cid, attr)
);

#CREATE INDEX ClusterAds_Str_I_cid ON ClusterAds_Str (cid);
#CREATE INDEX ClusterAds_Str_I_attr ON ClusterAds_Str (attr);
#CREATE INDEX ClusterAds_Str_I_val ON ClusterAds_Str (val);
#CREATE INDEX ClusterAds_Str_I_attr_val ON ClusterAds_Str (attr, val);
all indices are removed.  Only the pkey index created by default exists

CREATE TABLE ClusterAds_Num (
	cid		text,
	attr	text,
	val		double precision,
	primary key (cid, attr)
);

#CREATE INDEX ClusterAds_Num_I_cid ON ClusterAds_Num (cid);
#CREATE INDEX ClusterAds_Num_I_attr ON ClusterAds_Num (attr);
#CREATE INDEX ClusterAds_Num_I_val ON ClusterAds_Num (val);
#CREATE INDEX ClusterAds_Num_I_attr_val ON ClusterAds_Num (attr, val);
all indices are removed.  Only the pkey index created by default exists


    Defintion of ClusterAds view
    CREATE VIEW ClusterAds as 
	select cid, attr, val from ClusterAds_Str UNION ALL
	select cid, attr, cast(val as text) from ClusterAds_Num;
*/

#define SCHEMA_CREATE_CLUSTERADS_TABLE_STR "CREATE TABLE ClusterAds_Str (cid int, attr text, val text, primary key(cid, attr)); CREATE TABLE ClusterAds_Num (cid int, attr text, val double precision, primary key(cid, attr)); CREATE VIEW ClusterAds as select cid, attr, val from ClusterAds_Str UNION ALL select cid, attr, cast(val as text) from ClusterAds_Num;"

/*
	Defintion of Create JobQueuePollingInfo Table String 

CREATE TABLE JobQueuePollingInfo (
	last_file_mtime		BIGINT,
	last_file_size		BIGINT,
	last_next_cmd_offset	BIGINT,
	last_cmd_offset		BIGINT,
	last_cmd_type		SMALLINT,
	last_cmd_key		text,
	last_cmd_mytype 	text,
	last_cmd_targettype 	text,
	last_cmd_name		text,
	last_cmd_value		text
);

INSERT INTO JobQueuePollingInfo (last_file_mtime, last_file_size) VALUES (0,0);
*/

#define SCHEMA_CREATE_JOBQUEUEPOLLINGINFO_TABLE_STR "CREATE TABLE JobQueuePollingInfo (last_file_mtime BIGINT, last_file_size BIGINT, last_next_cmd_offset BIGINT, last_cmd_offset BIGINT, last_cmd_type SMALLINT, last_cmd_key text, last_cmd_mytype text, last_cmd_targettype text, last_cmd_name text, last_cmd_value text); INSERT INTO JobQueuePollingInfo (last_file_mtime, last_file_size) VALUES (0,0);"

/*
        Defintion of Create History Tables String
  
CREATE TABLE History_Vertical (
        cid     int,
	pid     int,
        attr    text,
        val     text,
        primary key (cid, pid, attr)
);
  
CREATE INDEX History_Vertical_I_attr_val ON History_Vertical (attr, val);

CREATE TABLE History_Horizontal(
        cid                  int, 
        pid                  int, 
        Owner                text, 
        QDate                int, 
        RemoteWallClockTime  int, 
        RemoteUserCpu        float, 
        RemoteSysCpu         float, 
        ImageSize            int, 
        JobStatus            int, 
        JobPrio              int, 
        Cmd                  text, 
        CompletionDate       int, 
        LastRemoteHost       text, 
        primary key(cid,pid)
); 

CREATE INDEX History_Horizontal_I_Owner on History_Horizontal(Owner);      
CREATE INDEX History_Horizontal_I_CompletionDate on History_Horizontal(CompletionDate);      

Note that some column names in the horizontal table are surrounded
by quotes in order to make them case-sensitive.  This is so that when
we convert the column names to their corresponding classad attribute name,
they look the same as the corresponding ad stored in the history file
*/

#define SCHEMA_CREATE_HISTORY_TABLE_STR "CREATE TABLE History_Vertical (cid int, pid int, attr text, val text, primary key (cid, pid, attr)); CREATE INDEX History_Vertical_I_attr_val ON History_Vertical (attr, val); CREATE TABLE History_Horizontal(cid int, pid int, \"Owner\" text, \"QDate\" int, \"RemoteWallClockTime\" int, \"RemoteUserCpu\" float, \"RemoteSysCpu\" float, \"ImageSize\" int, \"JobStatus\" int, \"JobPrio\" int, \"Cmd\" text, \"CompletionDate\" int, \"LastRemoteHost\" text, primary key(cid,pid)); CREATE INDEX History_Horizontal_I_Owner on History_Horizontal(\"Owner\");CREATE INDEX History_Horizontal_I_CompletionDate on History_Horizontal(\"CompletionDate\");"



#endif

