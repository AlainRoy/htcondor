#! /usr/bin/env perl

use strict;
use Cwd;

my $pid;
my $envvar;

my $opsys = $ARGV[0];
my $sleeptime = $ARGV[1];

system("date");

print "OPSYS = $opsys\n";
print "SLEEP = $sleeptime\n";

my $path;

$path = getcwd();
my $pout = "$path/parent";
my $cout = "$path/child";
my $count;
my $innercount = 0;
my $innerpid = 0;

# parent shouldn't wait for any children
$SIG{'CHLD'} = "IGNORE";

open(POUT, "> job_core_child_cleanup.data") or die "Could not open file 'job_core_child_cleanup.data': $?";
print POUT "Parent's pid is $$\n\n";

system("rm -rf job_core_child_cleanup.data.kids");

$count = 0;
while ($count < 10) 
{
	$pid = fork();
	$innercount = 0;
	if ($pid == 0)
	{
		# child code....

		open(GCPOUT, ">> job_core_child_cleanup.data.kids") or die "Could not open file 'job_core_child_cleanup.data.kids': $?";
		# child waits until a signal shows up
		$SIG{'INT'} = \&handler;
		$SIG{'HUP'} = \&handler;
		$SIG{'TERM'} = \&handler;

		while ($innercount < 1) 
		{

			$innerpid = fork();
			if ($innerpid == 0)
			{
				# child code....
		
				# child waits until a signal shows up
				$SIG{'INT'} = \&handler;
				$SIG{'HUP'} = \&handler;
				$SIG{'TERM'} = \&handler;
	
				while (1) { sleep 1; }
	
				exit 1;
			}

			# parent code...
	
			if (!defined($innerpid)) {
				print GCPOUT "Grand kid fork failed!!!!!!!!!!!!!!!!!!!\n";
				die "some problem forking. Oh well.\n";
			}

			print GCPOUT "Relationship: $$ child created $innerpid\n";
			close(GCPOUT);
	
			$innercount++;
		}

		while (1) { sleep 1; }

		exit 1;
	}

	# parent code...

	if (!defined($pid)) {
		die "some problem forking. Oh well.\n";
	}

	print POUT "Relationship: $$ created $pid\n";

	$count++;
}

#print POUT "Parent's environment is:\n";
#foreach $envvar (sort keys (%ENV)) {
	#print POUT "$envvar = $ENV{$envvar}\n";
#}

close(POUT);
sleep $sleeptime;
system("date");

exit 0;

sub handler
{
	my $sig = shift(@_);

	open(COUT, "> ${cout}_$$") or die "Could not open file '${cout}_$$': $?";
	print COUT "Child's pid is $$\n";
	print COUT "Got signal $sig\n\n";
	print COUT "Child's environment is:\n";
	foreach $envvar (sort keys (%ENV)) {
		print COUT "$envvar = $ENV{$envvar}\n";
	}
	close(COUT);

	exit 0;
}
