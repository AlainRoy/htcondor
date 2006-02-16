##
## Skeleton for the CronTab Tests
## The perl script will insert what universe the job will be
## This file SHOULD NOT contain "Queue" at the end
##
Executable		= ./x_time.pl
Notification	= NEVER
Arguments		= 20

##
## The job will run every 2 minutes
##
CronHour		= *
CronMinute		= */3
CronDayOfMonth	= *
CronMonth		= *
CronDayOfWeek	= *

## 
## Preparation Time
##
DeferralPrep	= 30

##
## We only want to run it 3 times
##
OnExitRemove	= JobRunCount >= 3
