##
## Max Local Universe Job Submission
## We set the requirements so that only local job is allowed to
## to run at a time. We pass the perl script the # of seconds it
## should sleep after it prints out the time. This way we can check
## to make sure that each time is that many seconds after each other
## If two jobs have times that are less than this time then we know
## that the jobs weren't blocked properly
##
Universe				= local
Executable				= ./x_time.pl
Notification			= NEVER
Log                     = job_core_max-running_local.log
Error                   = job_core_max-running_local.error
ShouldTransferFiles		= yes
WhenToTransferOutput	= ON_EXIT_OR_EVICT
Requirements            = Target.TotalLocalJobsRunning < 1
Arguments				= 10

##
## Queue a bunch of jobs 
## They have to output to the same file because the Condor
## testing library can't differeniate between multiple jobs
##
Output                  = job_core_max-running_local.out
Queue 6
