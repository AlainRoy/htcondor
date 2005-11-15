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
Arguments				= 10
Notification			= NEVER
Log                     = job_core_max-running_local.log
Output                  = job_core_max-running_local.out
Error                   = job_core_max-running_local.error
ShouldTransferFiles     = yes
WhenToTransferOutput    = ON_EXIT_OR_EVICT
Requirements            = Target.LocalUniverseJobsRunning < 1
Queue

Arguments				= 10
Queue
Arguments				= 10
Queue
Arguments				= 10
Queue
Arguments				= 10
Queue
Arguments				= 10
Queue