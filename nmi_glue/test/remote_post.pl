#!/usr/bin/env perl

######################################################################
# $Id: remote_post.pl,v 1.2.2.4 2006-09-14 16:08:06 wright Exp $
# post script for Condor testsuite runs
######################################################################

use File::Copy;

my $BaseDir = $ENV{BASE_DIR} || die "BASE_DIR not in environment!\n";
my $SrcDir = $ENV{SRC_DIR} || die "SRC_DIR not in environment!\n";
my $testdir = "condor_tests";
my $exit_status = 0;

# This is debugging output for the sake of the NWO infrastructure.
# However, it might be useful to us some day so we can see what's
# going on in case of failures...
if( defined $ENV{_NMI_STEP_FAILED} ) { 
    my $nmi_task_failure = "$ENV{_NMI_STEP_FAILED}";
    print "The value of _NMI_STEP_FAILED is: '$nmi_task_failure'\n";
} else {
    print "The _NMI_STEP_FAILED variable is not set\n";
}


######################################################################
# kill test suite personal condor daemons
######################################################################

$pid_file = "$BaseDir/condor_master_pid";

if( -f "$pid_file" ) {
    # Get master PID from file
    open (PIDFILE, "$pid_file") || die "Can't open $pid_file: $!\n";
    while( <PIDFILE> ) {
        chomp;
        $master_pid = $_;
    }
    close PIDFILE;

    # probably try to stop more gracefully, then wait 30 seconds and
    # kill if necessary
    print "KILLING Personal condor_master (pid: $master_pid)\n";
    if( ! kill(15, $master_pid) ) {
        print "ERROR: Can't kill condor_master (pid: $master_pid): $!\n";
        $exit_status = 1;
    }
} else {
    # if there's no pid_file, there must be no personal condor running
    # which we'd have to kill.  this would be caused by an empty
    # tasklist.  so, make sure the tasklist is empty.  if so, we can
    # move on with success.  if there are tasks but no pid_file,
    # that's a weird fatal error and we should propagate that.
    if( ! -f "tasklist.nmi" || -z "tasklist.nmi" ) {
        # our tasklist is empty, good.
        print "tasklist.nmi is empty and there's no condor_master_pid " .
            "file.\nNothing to cleanup, returning SUCCESS.\n";
    } else {
        print "ERROR: tasklist.nmi contains data but " .
            "condor_master_pid does not exist!\n";
        $exit_status = 1;
    }
}
    

######################################################################
# save and tar up test results
######################################################################

if( ! -f "tasklist.nmi" || -z "tasklist.nmi" ) {
    # our tasklist is empty, so don't do any real work
    print "No tasks in tasklist.nmi, nothing to do\n";
    exit $exit_status;
}

chdir("$BaseDir") || die "Can't chdir($BaseDir): $!\n";

my $etc_dir = "$BaseDir/results/etc";
my $log_dir = "$BaseDir/results/log";

if( ! -d "$BaseDir/results" ) {
    # If there's no results, and we can't even make the directory, we
    # might as well die, since there's nothing worth saving...
    mkdir( "$BaseDir/results", 0777 ) || die "Can't mkdir($BaseDir/results): $!\n";
}


#----------------------------------------
# debugging info from the personal condor
#----------------------------------------

if( ! -d $etc_dir ) {
    if( ! mkdir( "$etc_dir", 0777 ) ) {
        print "ERROR: Can't mkdir($etc_dir): $!\n";
        $exit_status = 1;
    }
}
if( -d $etc_dir ) {
    print "Copying config files to $etc_dir\n";
    system( "cp $BaseDir/condor/etc/condor_config $etc_dir" );
    if( $? >> 8 ) {
        print "Can't copy $BaseDir/condor/etc/condor_config to $etc_dir\n";
        $exit_status = 1;
    }
    system( "cp $BaseDir/local/condor_config.local $etc_dir" );
    if( $? >> 8 ) {
        print "Can't copy $BaseDir/local/condor_config.local to $etc_dir\n";
        $exit_status = 1;
    }
}

if( ! -d $log_dir ) {
    if( ! mkdir( "$log_dir", 0777 ) ) {
        print "ERROR: Can't mkdir($log_dir): $!\n";
        $exit_status = 1;
    }
}
if( -d $log_dir ) {
    print "Copying log files to $log_dir\n";
    system( "cp $BaseDir/local/log/* $log_dir" );
    if( $? >> 8 ) {
        print "Can't copy $BaseDir/local/log/* to $log_dir\n";
        $exit_status = 1;
    }
}
 

#----------------------------------------
# save output from tests
#----------------------------------------

system( "cp tasklist.nmi $BaseDir/results/" );
if( $? ) {
    print "Can't copy tasklist.nmi to $BaseDir/results\n";
    $exit_status = 1;
}

open (TASKFILE, "tasklist.nmi") || die "Can't tasklist.nmi: $!\n";
while( <TASKFILE> ) {
    chomp;
    my ($testname, $compiler) = split(/\./);
    if( $compiler eq "." ) {
        $resultdir = "$BaseDir/results/base";
    } else {
        $resultdir = "$BaseDir/results/$compiler";
    }
    if( ! -d "$resultdir" ) {
        mkdir( "$resultdir", 0777 ) || die "Can't mkdir($resultdir): $!\n";
    }
    chdir( "$SrcDir/$testdir/$compiler" ) ||
      die "Can't chdir($SrcDir/$testdir/$compiler): $!\n";

    # first copy the files that MUST be there.
    copy_file( "$testname.run.out", $resultdir, true );

    # these files are all optional.  if they exist, we'll save 'em, if
    # they do not, we don't worry about it.
    copy_file( "$testname.out*", $resultdir, false );
    copy_file( "$testname.err*", $resultdir, false );
    copy_file( "$testname.log", $resultdir, false );
    copy_file( "$testname.cmd.out", $resultdir, false );

    # if it exists, tarup the 'saveme' subdirectory for this test, which
    # may contain test debug info etc.
    if( -d "$testname.saveme" ) {
        system( "tar zcf $testname.saveme.tar.gz $testname.saveme/" );
        if( $? >> 8 ) {
            print "Tar failed to save test dir $testname.saveme\n";
        } else {
            print "Created $testname.saveme.tar.gz.\n";
            copy_file( "$testname.saveme.tar.gz", $resultdir, true );
        }
    }
}


#----------------------------------------
# final tar and exit
#----------------------------------------

$results = "results.tar.gz";
print "Tarring up all results\n";
chdir("$BaseDir") || die "Can't chdir($BaseDir): $!\n";
system( "tar zcf $BaseDir/$results results" );
if( $? >> 8 ) {
    print "Can't tar zcf $BaseDir/$results results\n";
    $exit_status = 1;
}

exit $exit_status;


######################################################################
# helper methods
######################################################################

sub copy_file {
    my( $src, $dest, $required ) = @_;
    my $had_error = false;
    copy($src, $dest);
    if( $? >> 8 ) {
        if( $required ) {
            print "ERROR: Can't copy $src to $dest: $!\n";
        } else {
            print "Optional file $src not copied into $dest: $!\n";
        }
        $had_error = true;
    } else {
        print "Copied $src to $dest\n";
    }
    return $had_error;
}

