/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
 * CONDOR Copyright Notice
 *
 * See LICENSE.TXT for additional notices and disclaimers.
 *
 * Copyright (c)1990-1998 CONDOR Team, Computer Sciences Department, 
 * University of Wisconsin-Madison, Madison, WI.  All Rights Reserved.  
 * No use of the CONDOR Software Program Source Code is authorized 
 * without the express consent of the CONDOR Team.  For more information 
 * contact: CONDOR Team, Attention: Professor Miron Livny, 
 * 7367 Computer Sciences, 1210 W. Dayton St., Madison, WI 53706-1685, 
 * (608) 262-0856 or miron@cs.wisc.edu.
 *
 * U.S. Government Rights Restrictions: Use, duplication, or disclosure 
 * by the U.S. Government is subject to restrictions as set forth in 
 * subparagraph (c)(1)(ii) of The Rights in Technical Data and Computer 
 * Software clause at DFARS 252.227-7013 or subparagraphs (c)(1) and 
 * (2) of Commercial Computer Software-Restricted Rights at 48 CFR 
 * 52.227-19, as applicable, CONDOR Team, Attention: Professor Miron 
 * Livny, 7367 Computer Sciences, 1210 W. Dayton St., Madison, 
 * WI 53706-1685, (608) 262-0856 or miron@cs.wisc.edu.
****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/

/* Stuff shared by glibc and non-glibc Linux:  */

REMAP_THREE( write, __write, ssize_t, int, const __ptr_t, size_t )
REMAP_THREE( read, __read, ssize_t, int, __ptr_t, size_t )
REMAP_THREE( lseek, __lseek, __off_t, int, __off_t, int )
REMAP_TWO( access, __access, int , const char *, int )
REMAP_ONE( acct, __acct, int , const char *)
REMAP_ONE( chdir, __chdir, int, const char* )
REMAP_TWO( chmod, __chmod, int , const char *, mode_t )
REMAP_THREE( chown, __chown, int , const char *, uid_t , gid_t )
REMAP_ONE( chroot, __chroot, int , const char *)
REMAP_ONE( close, __close, int , int )
REMAP_TWO( creat, __creat, int, const char*, mode_t )
REMAP_ONE( dup, __dup, int, int )
REMAP_TWO( dup2, __dup2, int, int, int )
REMAP_THREE( execve, __execve, int , const char *, char * const *, char * const *)
REMAP_ONE( fchdir, __fchdir, int, int )
REMAP_TWO( fchmod, __fchmod, int , int , mode_t )
REMAP_THREE( fchown, __fchown, int , int , uid_t , gid_t )
REMAP_THREE_VARARGS( fcntl, __fcntl, int , int , int , int)
REMAP_TWO( flock, __flock, int , int , int )
REMAP_ZERO( fork, __fork, pid_t )
REMAP_TWO( fstatfs, __fstatfs, int , int , struct statfs *)
REMAP_ONE( fsync, __fsync, int , int )
REMAP_TWO( ftruncate, __ftruncate, int , int , size_t )
REMAP_ZERO( getegid, __getegid, gid_t )
REMAP_THREE( getdents, __getdents, int , int , struct dirent *, size_t )
REMAP_ZERO( geteuid, __geteuid, uid_t )
REMAP_ZERO( getgid, __getgid, gid_t )
REMAP_TWO( getgroups, __getgroups, int , int , gid_t*)
REMAP_TWO( getitimer, __getitimer, int , int , struct itimerval *)
REMAP_ONE( getpgid, __getpgid, pid_t , pid_t )
REMAP_ZERO( getpgrp, __getpgrp, pid_t )
REMAP_ZERO( getpid, __getpid, pid_t )
REMAP_ZERO( getppid, __getppid, pid_t )
REMAP_TWO( getrlimit, __getrlimit, int , int , struct rlimit *)
REMAP_TWO( gettimeofday, __gettimeofday, int , struct timeval *, struct timezone *)
REMAP_ZERO( getuid, __getuid, uid_t )
REMAP_TWO( kill, __kill, int, pid_t, int )
REMAP_TWO( link, __link, int , const char *, const char *)
REMAP_TWO( mkdir, __mkdir, int , const char *, mode_t )
REMAP_SIX( mmap, __mmap, void *, void *, size_t, int, int, int, off_t )
REMAP_FIVE( mount, __mount, int , const char *, const char *, const char *, unsigned long , const void *)
REMAP_THREE( mprotect, __mprotect, int , void *, size_t , int )
REMAP_THREE( msync, __msync, int , void *, size_t , int )
REMAP_THREE_VARARGS( open, __open, int, const char *, int, int )
REMAP_ONE( pipe, __pipe, int , int *)
REMAP_THREE( readlink, __readlink, int , const char *, char *, size_t )
REMAP_THREE( readv, __readv, int, int, const struct iovec *, ssize_t )
REMAP_THREE( reboot, __reboot, int , int , int , int )
REMAP_TWO( rename, __rename, int, const char*, const char* )
REMAP_ONE( rmdir, __rmdir, int , const char *)
REMAP_FIVE( select, __select, int , int , fd_set *, fd_set *, fd_set *, struct timeval *)
REMAP_TWO( setdomainname, __setdomainname, int , const char *, size_t )
REMAP_ONE( setgid, __setgid, int , gid_t )
REMAP_TWO( setgroups, __setgroups, int , int , gid_t *)
REMAP_TWO( sethostname, __sethostname, int , const char *, size_t )
REMAP_THREE( setitimer, __setitimer, int , int , const struct itimerval *, struct itimerval *)
REMAP_TWO( setpgid, __setpgid, int , pid_t , pid_t )
     /* REMAP_THREE( setpriority, __setpriority, int, int, int, int ) */
REMAP_TWO( setregid, __setregid, int , uid_t , uid_t )
REMAP_TWO( setreuid, __setreuid, int , uid_t , uid_t )
REMAP_TWO( setrlimit, __setrlimit, int , int , const struct rlimit *)
REMAP_ZERO( setsid, __setsid, pid_t )
REMAP_TWO( settimeofday, __settimeofday, int , const struct timeval *, const struct timezone *)
REMAP_ONE( setuid, __setuid, int , uid_t )
REMAP_THREE( sigprocmask, __sigprocmask, int, int, const sigset_t *, sigset_t * )
REMAP_ONE( sigsetmask, __sigsetmask, int, int )
REMAP_TWO( statfs, __statfs, int , const char *, struct statfs *)
REMAP_ONE( swapoff, __swapoff, int , const char *)
REMAP_TWO( swapon, __swapon, int , const char *, int )
REMAP_TWO( symlink, __symlink, int , const char *, const char *)
REMAP_TWO( truncate, __truncate, int , const char *, size_t )
REMAP_ONE( umask, __umask, mode_t , mode_t )
REMAP_ONE( umount, __umount, int , const char *)
REMAP_ONE( uname, __uname, int , struct utsname *)
REMAP_ONE( unlink, __unlink, int , const char *)
REMAP_FOUR( wait4, __wait4, pid_t , pid_t , void *, int , struct rusage *)
REMAP_THREE( waitpid, __waitpid, pid_t , pid_t , int *, int )
REMAP_THREE( writev, __writev, int, int, const struct iovec *, ssize_t )

/* Differences */

#if defined(GLIBC)

REMAP_TWO( clone, __clone, pid_t , void *, unsigned long )
REMAP_TWO( fstat, __fstat, int , int , struct stat *)
REMAP_TWO( getrusage, __getrusage, int, enum __rusage_who, struct rusage *)
REMAP_THREE_VARARGS( ioctl, __ioctl, int , int , unsigned long , int)
REMAP_TWO( lstat, __lstat, int , const char *, struct stat *)
REMAP_THREE( mknod, __mknod, int , const char *, mode_t , dev_t )
REMAP_THREE( sigaction, __sigaction, int, int, const struct sigaction *, struct sigaction * )
REMAP_ONE( sigsuspend, __sigsuspend, int, const sigset_t * )
REMAP_TWO( stat, __stat, int , const char *, struct stat *)

#else 

REMAP_ONE( fdatasync, __fdatasync, int , int )
REMAP_TWO( getrusage, __getrusage, int, int , struct rusage *)
REMAP_ZERO_VOID( idle, __idle, void )
REMAP_THREE_VARARGS( ioctl, __ioctl, int , int , int , int)
REMAP_THREE( ioperm, __ioperm, int , unsigned long , unsigned long , int )
REMAP_ONE( iopl, __iopl, int , int )
REMAP_TWO( prev_fstat, __prev_fstat, int , int , struct stat *)
REMAP_TWO( prev_lstat, __prev_lstat, int , const char *, struct stat *)
REMAP_THREE( prev_mknod, __prev_mknod, int , const char *, mode_t , dev_t )
REMAP_TWO( prev_stat, __prev_stat, int , const char *, struct stat *)
REMAP_THREE( setpriority, __setpriority, int, int, int, int )
REMAP_THREE( sigaction, __sigaction, int, int, struct sigaction *, struct sigaction * )
REMAP_ZERO( sync, __sync, int )

#endif

