##/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
##
## Condor Software Copyright Notice
## Copyright (C) 1990-2006, Condor Team, Computer Sciences Department,
## University of Wisconsin-Madison, WI.
##
## This source code is covered by the Condor Public License, which can
## be found in the accompanying LICENSE.TXT file, or online at
## www.condorproject.org.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
## AND THE UNIVERSITY OF WISCONSIN-MADISON "AS IS" AND ANY EXPRESS OR
## IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
## WARRANTIES OF MERCHANTABILITY, OF SATISFACTORY QUALITY, AND FITNESS
## FOR A PARTICULAR PURPOSE OR USE ARE DISCLAIMED. THE COPYRIGHT
## HOLDERS AND CONTRIBUTORS AND THE UNIVERSITY OF WISCONSIN-MADISON
## MAKE NO MAKE NO REPRESENTATION THAT THE SOFTWARE, MODIFICATIONS,
## ENHANCEMENTS OR DERIVATIVE WORKS THEREOF, WILL NOT INFRINGE ANY
## PATENT, COPYRIGHT, TRADEMARK, TRADE SECRET OR OTHER PROPRIETARY
## RIGHT.
##
##***************************Copyright-DO-NOT-REMOVE-THIS-LINE**/

# CondorTest.pm - a Perl module for automated testing of Condor
#
# 19??-???-?? originally written by Tom Stanis (?)
# 2000-Jun-02 total overhaul by pfc@cs.wisc.edu and wright@cs.wisc.edu

package CondorTest;

require 5.0;
use Carp;
use Condor;
use FileHandle;

BEGIN
{
    # disable command buffering so output is flushed immediately
    STDOUT->autoflush();
    STDERR->autoflush();

    $MAX_CHECKPOINTS = 2;
    $MAX_VACATES = 3;

    @skipped_output_lines = ( );
    @expected_output = ( );
    $checkpoints = 0;
    $vacates = 0;
    %test;
	%machine_ads;
}

sub Reset
{
    %machine_ads = ();
	Condor::Reset();
}

sub ForceVacate
{
    my %info = @_;

    return 0 if ( $checkpoints >= $MAX_CHECKPOINTS ||
		  $vacates >= $MAX_VACATES );

    # let the job run for a few seconds and then send vacate signal
    sleep 5;
    Condor::Vacate( "\"$info{'sinful'}\"" );
    $vacates++;
    return 1;
}

sub RegisterSubmit
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "submit: missing function reference argument";

    $test{$handle}{"RegisterSubmit"} = $function_ref;
}
sub RegisterExecute
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "execute: missing function reference argument";

    $test{$handle}{"RegisterExecute"} = $function_ref;
}
sub RegisterEvicted
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "evict: missing function reference argument";

    $test{$handle}{"RegisterEvicted"} = $function_ref;
}
sub RegisterEvictedWithCheckpoint
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterEvictedWithCheckpoint"} = $function_ref;
}
sub RegisterEvictedWithRequeue
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterEvictedWithRequeue"} = $function_ref;
}
sub RegisterEvictedWithoutCheckpoint
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterEvictedWithoutCheckpoint"} = $function_ref;
}
sub RegisterExited
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterExited"} = $function_ref;
}
sub RegisterExitedSuccess
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "exit success: missing function reference argument";

    $test{$handle}{"RegisterExitedSuccess"} = $function_ref;
}
sub RegisterExitedFailure
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterExitedFailure"} = $function_ref;
}
sub RegisterExitedAbnormal
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterExitedAbnormal"} = $function_ref;
}
sub RegisterAbort
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterAbort"} = $function_ref;
}
sub RegisterShadow
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterShadow"} = $function_ref;
}
sub RegisterWantError
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterWantError"} = $function_ref;
}
sub RegisterHold
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterHold"} = $function_ref;
}
sub RegisterRelease
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterRelease"} = $function_ref;
}
sub RegisterJobErr
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";

    $test{$handle}{"RegisterJobErr"} = $function_ref;
}

sub RegisterTimed
{
    my $handle = shift || croak "missing handle argument";
    my $function_ref = shift || croak "missing function reference argument";
	my $alarm = shift || croak "missing wait time argument";

    $test{$handle}{"RegisterTimed"} = $function_ref;
    $test{$handle}{"RegisterTimedWait"} = $alarm;

	# relook at registration and re-register to allow a timer
	# to be set after we are running. 
	# Prior to this change timed callbacks were only regsitered
	# when we call "runTest" and similar calls at the start.

	CheckTimedRegistrations();
}

sub RemoveTimed
{
    my $handle = shift || croak "missing handle argument";

    $test{$handle}{"RegisterTimed"} = undef;
    $test{$handle}{"RegisterTimedWait"} = undef;
    Condor::debug( "Remove timer.......\n");
    Condor::RemoveTimed( );
}

sub DefaultOutputTest
{
    my %info = @_;

    croak "default_output_test called but no \@expected_output defined"
	unless defined @expected_output;

    Condor::debug( "\$info{'output'} = $info{'output'}\n" );

	my $output = "";
	my $error = "";
	my $initialdir = $info{'initialdir'};
	if($initialdir ne "") {
		Condor::debug( "Testing with initialdir = $initialdir\n" );
		$output = $initialdir . "/" . $info{'output'};
		$error = $initialdir . "/" . $info{'error'};
		Condor::debug( "$output is now being tested.....\n" );
	} else {
		$output = $info{'output'};
		$error = $info{'error'};
	}

    CompareText( $output, \@expected_output, @skipped_output_lines )
	|| die "$handle: FAILURE (STDOUT doesn't match expected output)\n";

    IsFileEmpty( $error ) || 
	die "$handle: FAILURE (STDERR contains data)\n";
}

sub RunTest
{
    $handle              = shift || croak "missing handle argument";
    my $submit_file      = shift || croak "missing submit file argument";
    my $wants_checkpoint = shift;

    my $status           = -1;

	# moved the reset to preserve callback registrations which includes
	# an error callback at submit time..... Had to change timing
	CondorTest::Reset();

    croak "too many arguments" if shift;

    # this is kludgey :: needed to happen sooner for an error message callback in runcommand
    $Condor::submit_info{'handle'} = $handle;

    # this is kludgey :: needed to happen sooner for an error message callback in runcommand
    #$Condor::submit_info{'handle'} = $handle;

    # if we want a checkpoint, register a function to force a vacate
    # and register a function to check to make sure it happens
	if( $wants_checkpoint )
	{
		Condor::RegisterExecute( \&ForceVacate );
		Condor::RegisterEvictedWithCheckpoint( sub { $checkpoints++ } );
	} else {
		if(defined $test{$handle}{"RegisterExecute"}) {
			Condor::RegisterExecute($test{$handle}{"RegisterExecute"});
		}
	}

	CheckRegistrations();

    # submit the job and get the cluster id
    $cluster = Condor::TestSubmit( $submit_file );
    
    # if condor_submit failed for some reason return an error
    return 0 if $cluster == 0;

    # monitor the cluster and return its exit status
    $retval = Condor::Monitor();

    die "$handle: FAILURE (job never checkpointed)\n"
	if $wants_checkpoint && $checkpoints < 1;

    return $retval;
}

sub RunDagTest
{
    $handle              = shift || croak "missing handle argument";
    my $submit_file      = shift || croak "missing submit file argument";
    my $wants_checkpoint = shift;
	my $dagman_args = 	shift || croak "missing dagman args";

    my $status           = -1;

    croak "too many arguments" if shift;

	# moved the reset to preserve callback registrations which includes
	# an error callback at submit time..... Had to change timing
	CondorTest::Reset();

    # this is kludgey :: needed to happen sooner for an error message callback in runcommand
    $Condor::submit_info{'handle'} = $handle;

    # if we want a checkpoint, register a function to force a vacate
    # and register a function to check to make sure it happens
	if( $wants_checkpoint )
	{
		Condor::RegisterExecute( \&ForceVacate );
		Condor::RegisterEvictedWithCheckpoint( sub { $checkpoints++ } );
	} else {
		if(defined $test{$handle}{"RegisterExecute"}) {
			Condor::RegisterExecute($test{$handle}{"RegisterExecute"});
		}
	}

	CheckRegistrations();

    # submit the job and get the cluster id
    $cluster = Condor::TestSubmitDagman( $submit_file, $dagman_args );
    
    # if condor_submit failed for some reason return an error
    return 0 if $cluster == 0;

    # monitor the cluster and return its exit status
    $retval = Condor::Monitor();

    die "$handle: FAILURE (job never checkpointed)\n"
	if $wants_checkpoint && $checkpoints < 1;

    return $retval;
}

sub CheckTimedRegistrations
{
	# this one event should be possible from ANY state
	# that the monitor reports to us. In this case which
	# initiated the change I wished to start a timer from
	# when the actual runtime was reached and not from
	# when we started the test and submited it. This is the time 
	# at all other regsitrations have to be registered by....

    if( defined $test{$handle}{"RegisterTimed"} )
    {
		Condor::debug( "Found a timer to regsiter.......\n");
		Condor::RegisterTimed( $test{$handle}{"RegisterTimed"} , $test{$handle}{"RegisterTimedWait"});
    }
}

sub CheckRegistrations
{
    # any handle-associated functions with the cluster
    # or else die with an unexpected event
    if( defined $test{$handle}{"RegisterExitedSuccess"} )
    {
        Condor::RegisterExitSuccess( $test{$handle}{"RegisterExitedSuccess"} );
    }
    else
    {
	Condor::RegisterExitSuccess( sub {
	    die "$handle: FAILURE (got unexpected successful termination)\n";
	} );
    }

    if( defined $test{$handle}{"RegisterExitedFailure"} )
    {
	Condor::RegisterExitFailure( $test{$handle}{"RegisterExitedFailure"} );
    }
    else
    {
	Condor::RegisterExitFailure( sub {
	    my %info = @_;
	    die "$handle: FAILURE (returned $info{'retval'})\n";
	} );
    }

    if( defined $test{$handle}{"RegisterExitedAbnormal"} )
    {
	Condor::RegisterExitAbnormal( $test{$handle}{"RegisterExitedAbnormal"} );
    }
    else
    {
	Condor::RegisterExitAbnormal( sub {
	    my %info = @_;
	    die "$handle: FAILURE (got signal $info{'signal'})\n";
	} );
    }

    if( defined $test{$handle}{"RegisterShadow"} )
    {
	Condor::RegisterShadow( $test{$handle}{"RegisterShadow"} );
    }

    if( defined $test{$handle}{"RegisterWantError"} )
    {
	Condor::RegisterWantError( $test{$handle}{"RegisterWantError"} );
    }

    if( defined $test{$handle}{"RegisterAbort"} )
    {
	Condor::RegisterAbort( $test{$handle}{"RegisterAbort"} );
    }
    else
    {
	Condor::RegisterAbort( sub {
	    my %info = @_;
	    die "$handle: FAILURE (job aborted by user)\n";
	} );
    }

    if( defined $test{$handle}{"RegisterHold"} )
    {
	Condor::RegisterHold( $test{$handle}{"RegisterHold"} );
    }
    else
    {
	Condor::RegisterHold( sub {
	    my %info = @_;
	    die "$handle: FAILURE (job held by user)\n";
	} );
    }

    if( defined $test{$handle}{"RegisterSubmit"} )
    {
	Condor::RegisterSubmit( $test{$handle}{"RegisterSubmit"} );
    }

    if( defined $test{$handle}{"RegisterRelease"} )
    {
	Condor::RegisterRelease( $test{$handle}{"RegisterRelease"} );
    }
    #else
    #{
	#Condor::RegisterRelease( sub {
	    #my %info = @_;
	    #die "$handle: FAILURE (job released by user)\n";
	#} );
    #}

    if( defined $test{$handle}{"RegisterJobErr"} )
    {
	Condor::RegisterJobErr( $test{$handle}{"RegisterJobErr"} );
    }
    else
    {
	Condor::RegisterJobErr( sub {
	    my %info = @_;
	    die "$handle: FAILURE (job error -- see $info{'log'})\n";
	} );
    }

    # if we wanted to know about requeues.....
    if( defined $test{$handle}{"RegisterEvictedWithRequeue"} )
    {
        Condor::RegisterEvictedWithRequeue( $test{$handle}{"RegisterEvictedWithRequeue"} );
    } 

    # if evicted, call condor_resched so job runs again quickly
    if( !defined $test{$handle}{"RegisterEvicted"} )
    {
        Condor::RegisterEvicted( sub { sleep 5; Condor::Reschedule } );
    } else {
	Condor::RegisterEvicted( $test{$handle}{"RegisterEvicted"} );
    }

    if( defined $test{$handle}{"RegisterTimed"} )
    {
		Condor::RegisterTimed( $test{$handle}{"RegisterTimed"} , $test{$handle}{"RegisterTimedWait"});
    }
}


sub CompareText
{
    my $file = shift || croak "missing file argument";
    my $aref = shift || croak "missing array reference argument";
    my @skiplines = @_;
    my $linenum = 0;

	Condor::debug("DEBUG opening file $file to compare to array of expected results\n");
    open( FILE, "<$file" ) || die "error opening $file: $!\n";
    
    while( <FILE> )
    {
	fullchomp($_);
	$line = $_;
	$linenum++;

	Condor::debug("DEBUG: linenum $linenum\n");
	Condor::debug("DEBUG: \$line: $line\n");
	Condor::debug("DEBUG: \$\$aref[0] = $$aref[0]\n");

	Condor::debug("DEBUG: skiplines = \"@skiplines\"\n");
	Condor::debug("DEBUG: grep returns ", grep( /^$linenum$/, @skiplines ), "\n");

	next if grep /^$linenum$/, @skiplines;

	$expectline = shift @$aref;
	if( ! defined $expectline )
	{
	    die "$file contains more text than expected\n";
	}
	fullchomp($expectline);

	Condor::debug("DEBUG: \$expectline: $expectline\n");

	# if they match, go on
	next if $expectline eq $line;

	# otherwise barf
	warn "$file line $linenum doesn't match expected output:\n";
	warn "actual: $line\n";
	warn "expect: $expectline\n";
	return 0;
    }

    # barf if we're still expecting text but the file has ended
    ($expectline = shift @$aref ) && 
        die "$file incomplete, expecting:\n$expectline\n";

    # barf if there are skiplines we haven't hit yet
    foreach $num ( @skiplines )
    {
	if( $num > $linenum )
	{
	    warn "skipline $num > # of lines in $file ($linenum)\n";
	    return 0;
	}
	croak "invalid skipline argument ($num)" if $num < 1;
    }
    
	Condor::debug("DEBUG: CompareText successful\n");
    return 1;
}

sub IsFileEmpty
{
    my $file = shift || croak "missing file argument";
    return -z $file;
}

sub verbose_system 
{
	my $args = shift @_;


	my $catch = "vsystem$$";
	$args = $args . " 2>" . $catch;
	my $rc = 0xffff & system $args;

	printf "system(%s) returned %#04x: ", $args, $rc;

	if ($rc == 0) 
	{
		print "ran with normal exit\n";
		return $rc;
	}
	elsif ($rc == 0xff00) 
	{
		print "command failed: $!\n";
	}
	elsif (($rc & 0xff) == 0) 
	{
		$rc >>= 8;
		print "ran with non-zero exit status $rc\n";
	}
	else 
	{
		print "ran with ";
		if ($rc &   0x80) 
		{
			$rc &= ~0x80;
			print "coredump from ";
		}
		print "signal $rc\n"
	}

	if( !open( MACH, "<$catch" )) { 
		warn "Can't look at command  output <$catch>:$!\n";
	} else {
    	while(<MACH>) {
        	print "ERROR: $_";
    	}
    	close(MACH);
	}

	return $rc;
}

sub MergeOutputFiles
{
	my $Testhash = shift || croak "Missing Test hash to Merge Output\n";
	my $basename = $Testhash->{corename};

	foreach my $m ( 0 .. $#{$Testhash->{extensions}} )
	{
		my $newlog = $basename . $Testhash->{extensions}[$m];
		#print "Creating core log $newlog\n";
		open(LOG,">$newlog") || return "1";
		print LOG "***************************** Merge Sublogs ***************************\n";
		foreach my $n ( 0 .. $#{$Testhash->{tests}} )
		{
			# get file if it exists
			#print "Add logs for test $Testhash->{tests}[$n]\n";
			my $sublog = $Testhash->{tests}[$n] . $Testhash->{extensions}[$m];
			if( -e "$sublog" )
			{
				print LOG "\n\n***************************** $sublog ***************************\n\n";
				open(INLOG,"<$sublog") || return "1";
				while(<INLOG>)
				{
					print LOG "$_";
				}
				close(INLOG);
			}
			else
			{
				#print "Can not find $sublog\n";
			}
			#print "$n = $Testhash->{tests}[$n]\n";
			#print "$m = $Testhash->{extensions}[$m]\n";
		}
		close(LOG);
	}
}

sub ParseMachineAds
{
    my $machine = shift || croak "missing machine argument";
    my $line = 0;

	if( ! open(PULL, "condor_status -l $machine 2>&1 |") )
    {
		print "error getting Ads for \"$machine\": $!\n";
		return 0;
    }
    
    #Condor::debug( "reading machine ads from $machine...\n" );
    while( <PULL> )
    {
	fullchomp($_);
#	Condor::debug("Raw AD is $_\n");
	$line++;

	# skip comments & blank lines
	next if /^#/ || /^\s*$/;

	# if this line is a variable assignment...
	if( /^(\w+)\s*\=\s*(.*)$/ )
	{
	    $variable = lc $1;
	    $value = $2;

	    # if line ends with a continuation ('\')...
	    while( $value =~ /\\\s*$/ )
	    {
		# remove the continuation
		$value =~ s/\\\s*$//;

		# read the next line and append it
		<PULL> || last;
		$value .= $_;
	    }

	    # compress whitespace and remove trailing newline for readability
	    $value =~ s/\s+/ /g;
	    fullchomp($value);

	
		# Do proper environment substitution
	    if( $value =~ /(.*)\$ENV\((.*)\)(.*)/ )
	    {
			my $envlookup = $ENV{$2};
	    	#Condor::debug( "Found $envlookup in environment \n");
			$value = $1.$envlookup.$3;
	    }

	    #Condor::debug( "$variable = $value\n" );
	    
	    # save the variable/value pair
	    $machine_ads{$variable} = $value;
	}
	else
	{
#	    Condor::debug( "line $line of $submit_file not a variable assignment... " .
#		   "skipping\n" );
	}
    }
    return 1;
}

sub FetchMachineAds
{
	return %machine_ads;
}

sub FetchMachineAdValue
{
	my $key = shift @_;
	if(exists $machine_ads{$key})
	{
		return $machine_ads{$key};
	}
	else
	{
		return undef;
	}
}

#
# Is condor_q able to respond at this time? We'd like to get
# a valid response for whoever is asking.
#

sub getJobStatus
{
	my @status;
	my $qstatcluster = shift;
	my $cmd = "condor_q $qstatcluster -format %d JobStatus";
	my $qstat = CondorTest::runCondorTool($cmd,\@status,3);
	if(!$qstat)
	{
		print "Test failure due to Condor Tool Failure<$cmd>\n";
	    return(1)
	}

	foreach $line (@status)
	{
		#print "Line: $line\n";
		if( $line =~ /^(\d).*/)
		{
			return($1);
		}
		else
		{
			return(-1);
		}
	}
}

#
# Run a condor tool and look for exit value. Apply multiplier
# upon failure
#

sub runCondorTool
{
	my $trymultiplier = 1;
	my $start_time = time; #get start time
	my $delta_time = 0;
	my $status = 1;
	my $done = 0;
	my $cmd = shift;
	my $arrayref = shift;
	my $multiplier = shift;
	my $force = "";
	$force = shift;
	my $count = 0;
	my $catch = "runCTool$$";

	# clean array before filling
    #${arrayref} = -1;
	system("date");

	while( !$done)
	{
		my @tmparray;
		Condor::debug( "Try command <$cmd>\n");
		open(PULL, "$cmd 2>$catch |");
		while(<PULL>)
		{
			fullchomp($_);
			Condor::debug( "Process: $_\n");
			push @tmparray, $_; # push @{$arrayref}, $_;
			$count += 1;
		}
		close(PULL);
		$status = $? >> 8;
		Condor::debug("Status is $status after command\n");
		if( $status != 0 )
		{
			my $current_time = time;
			$delta_time = $current_time - $start_time;
			print "runCondorTool: its been $delta_time since call\n";
			if( $trymultiplier )
			{
				$trymultiplier = 0;
				$ENV{_CONDOR_TOOL_TIMEOUT_MULTIPLIER} = $multiplier;
			}
			else
			{
				print "runCondorTool: $cmd timestamp $start_time failed!\n";
				if( !open( MACH, "<$catch" )) { 
					warn "Can't look at command output <$catch>:$!\n";
				} else {
    				while(<MACH>) {
        				print "ERROR: $_";
    				}
    				close(MACH);
				}
				return(0);
			}
		}
		else
		{
			$line = "";
			foreach $value (@tmparray)
			{
				push @{$arrayref}, $value;
			}
			$done = 1;
			# There are times like the security tests when we want
			# to see the stderr even when the command works.
			if( $force ne "" ) {
				if( !open( MACH, "<$catch" )) { 
					warn "Can't look at command output <$catch>:$!\n";
				} else {
    				while(<MACH>) {
						fullchomp($_);
						$line = $_;
						push @{$arrayref}, $line;
    				}
    				close(MACH);
				}
			}
			my $current_time = time;
			$delta_time = $current_time - $start_time;
			Condor::debug("runCondorTool: its been $delta_time since call\n");
		}
	}
	Condor::debug( "runCondorTool: $cmd worked!\n");

	return(1);
}

#
# Cygwin's perl chomp does not remove cntrl-m but this one will
# and linux and windows can share the same code. The real chomp
# totals the number or changes but I currently return the modified
# array. bt 10/06
#

sub fullchomp
{
	push (@_,$_) if( scalar(@_) == 0);
	foreach my $arg (@_) {
		$arg =~ s/\012+$//;
		$arg =~ s/\015+$//;
	}
	return(0);
}

sub changeDaemonState
{
	my $timeout = 0;
	my $daemon = shift;
	my $state = shift;
	$timeout = shift;
	my $counter = 0;
	my $cmd = "";
	my $timeout_increment = 5;
	my $foundTotal = "no";
	my $status;
	my @cmdarray1, @cmdarray2;

	print "Checking for $daemon being $state\n";
	if($state eq "off") {
		$cmd = "condor_off -$daemon";
	} elsif($state eq "on") {
		$cmd = "condor_on -$daemon";
	} else {
		die "Bad state given in changeScheddState<$state>\n";
	}

	$status = runCondorTool($cmd,\@cmdarray1,2);
	if(!$status)
	{
		print "Test failure due to Condor Tool Failure<$cmd>\n";
		exit(1);
	}

	$cmd = "condor_status -$daemon";
	while($counter < $timeout ) {
		$foundTotal = "no";
		@cmdarray2 = {};
		print "about to run $cmd\n";
		$status = CondorTest::runCondorTool($cmd,\@cmdarray2,2);
		if(!$status)
		{
			print "Test failure due to Condor Tool Failure<$cmd>\n";
			exit(1)
		}

		foreach my $line (@cmdarray2)
		{
			print "<<$line>>\n";
			if($daemon eq "schedd") {
				if( $line =~ /.*Total.*/ ) {
					# hmmmm  scheduler responding
					print "Schedd running\n";
					$foundTotal = "yes";
				}
			} elsif($daemon eq "startd") {
				if( $line =~ /.*Backfill.*/ ) {
					# hmmmm  Startd responding
					print "Startd running\n";
					$foundTotal = "yes";
				}
			}
		}

		if( $state eq "on" ) {
			if($foundTotal eq "yes") {
				# is running again
				return(1);
			} else {
				sleep($timeout_increment);
				$counter = $counter + $timeout_increment;
			}
		} elsif( $state eq "off" ) {
			if($foundTotal eq "no") {
				#is stopped
				return(1);
			} else {
				sleep($timeout_increment);
				$counter = $counter + $timeout_increment;
			}
		}

	}
	print "Timeout watching for $daemon state change to <<$state>>\n";
	return(0);
}

#######################
#
# find_pattern_in_array
#
#	Find the array index which contains a particular pattern.
#
# 	First used to strip off variant line in output from
#	condor_q -direct when passed quilld, schedd and rdbms
#	prior to comparing the arrays collected from the output
#	of each command....

sub find_pattern_in_array
{
    $pattern = shift;
    $harray = shift;
    $place = 0;

    Condor::debug( "Looking for <<$pattern>> size <<$#{$harray}>>\n");
    foreach $member (@{$harray}) {
        #Condor::debug( "consider $member\n");
        if($member =~ /.*$pattern.*/) {
            Condor::debug( "Found <<$member>> line $place\n");
            return($place);
        } else {
            $place = $place + 1;
        }
    }
    print "Got to end without finding it....<<$pattern>>\n";
    return(-1);
}

#######################
#
# compare_arrays
#
#	We hash each line from an array and verify that each array has the same contents
# 	by having a value for each key equalling the number of arrays. First used to
#	compare output from condor_q -direct quilld, schedd and rdbms

sub compare_arrays
{
    $startrow = shift;
    $endrow = shift;
    $numargs = shift;
    %lookup = ();
    $counter = 0;
    Condor::debug( "Check $numargs starting row $startrow end row $endrow\n");
    while($counter < $numargs) {
        $href = shift;
        my $thisrow = 0;
        for $item (@{$href}) {
            if( $thisrow >= $startrow) {
                if($counter == 0) {
                    #initialize each position
                    $lookup{$item} = 1;
                } else {
                    $lookup{$item} = $lookup{$item} + 1;
                    Condor::debug( "Set at:<$lookup{$item}:$item>\n");
                }
                #Condor::debug( "Store: $item\n");
            } else {
                Condor::debug( "Skip: $item\n");
            }
            $thisrow = $thisrow + 1;
        }
        $counter = $counter + 1;
    }
    #loaded up..... now look!
    foreach $key (keys %lookup) {
        Condor::debug( " $key equals $lookup{$key}\n");
		if($lookup{$key} != $numargs) {
			print "Arrays are not the same! key <$key> is $lookup{$key} and not $numargs\n";
			return(1);
		}
    }
	return(0);
}

1;
