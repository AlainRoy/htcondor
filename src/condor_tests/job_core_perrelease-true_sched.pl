#! /usr/bin/env perl
##**************************************************************
##
## Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
## University of Wisconsin-Madison, WI.
## 
## Licensed under the Apache License, Version 2.0 (the "License"); you
## may not use this file except in compliance with the License.  You may
## obtain a copy of the License at
## 
##    http://www.apache.org/licenses/LICENSE-2.0
## 
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##
##**************************************************************

use CondorTest;

$cmd = 'job_core_perrelease-true_sched.cmd';
$testname = 'Condor submit with for periodic release test - scheduler U';

$aborted = sub {
	my %info = @_;
	my $done;
	print "Abort event not expected!\n";
	print "Want to see only submit, release and successful completion events for periodic release test\n";
	exit(1);
};

$held = sub {
	my %info = @_;
	my $cluster = $info{"cluster"};

	print "Held event not expected.....\n";
	exit(1);
};

$executed = sub
{
	my %args = @_;
	my $cluster = $args{"cluster"};

	print "Periodic Hold should see and execute, followed by a release and then we reschedule the job\n";
};

$success = sub
{
	print "Success: ok\n";
};

$release = sub
{
	print "Release expected.........\n";
	my @adarray;
	my $status = 1;
	my $cmd = "condor_reschedule";
	$status = CondorTest::runCondorTool($cmd,\@adarray,2);
	if(!$status)
	{
		print "Test failure due to Condor Tool Failure<$cmd>\n";
		exit(1)
	}
};

CondorTest::RegisterExitedSuccess( $testname, $success);
CondorTest::RegisterExecute($testname, $executed);
CondorTest::RegisterRelease( $testname, $release );
CondorTest::RegisterHold( $testname, $held );

if( CondorTest::RunTest($testname, $cmd, 0) ) {
	print "$testname: SUCCESS\n";
	exit(0);
} else {
	die "$testname: CondorTest::RunTest() failed\n";
}

