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

/* These remaps are needed by file_state.c */
#if defined(FILE_TABLE)

REMAP_ONE( close, _close, int , int )
REMAP_TWO( creat, _creat, int, const char *, mode_t )
REMAP_ONE( dup, _dup, int, int )
REMAP_ONE( fchdir, _fchdir, int, int )
REMAP_THREE( fcntl, _fcntl, int , int , int , int)
REMAP_THREE( fcntl, __fcntl, int , int , int , int)
REMAP_ONE( fdsync, ___fdsync, int , int )
REMAP_ONE( fdsync, __fdsync, int , int )
REMAP_THREE( ioctl, _ioctl, int , int , int , int)
REMAP_THREE( llseek, _llseek, offset_t , int , offset_t , int )
REMAP_THREE( lseek, _lseek, off_t , int , off_t , int )
REMAP_THREE( open, _open, int , const char *, int , int)
REMAP_THREE( open, __open, int , const char *, int , int)
REMAP_THREE( read, _read, ssize_t , int , void *, size_t )
REMAP_THREE( write, _write, ssize_t , int , const void *, size_t )

#endif

/* These remaps are needed by signals_support.c */
#if defined(SAVE_SIGSTATE)

REMAP_THREE( sigaction, __sigaction, int, int, const struct sigaction *, struct sigaction * )
REMAP_THREE( sigaction, _sigaction, int, int, const struct sigaction *, struct sigaction * )
REMAP_TWO( signal, _signal, void *, int, void * )
REMAP_THREE( sigprocmask, _sigprocmask, int, int, const sigset_t *, sigset_t * )
REMAP_ONE( sigsuspend, _sigsuspend, int, const sigset_t * )

#endif

/* These remaps fill out the rest of the remote syscalls */
#if defined(REMOTE_SYSCALLS)

REMAP_TWO( access, _access, int , const char *, int )
REMAP_ONE( acct, _acct, int , const char *)
REMAP_TWO( adjtime, _adjtime, int , struct timeval *, struct timeval *)
REMAP_ONE( chdir, _chdir, int, const char * )
REMAP_TWO( chmod, _chmod, int , const char *, unsigned long )
REMAP_THREE( chown, _chown, int , const char *, uid_t , gid_t )
REMAP_ONE( chroot, _chroot, int , const char *)
REMAP_THREE( execve, _execve, int , const char *, char * const *,  char * const * )
REMAP_TWO( fchmod, _fchmod, int , int , unsigned long )
REMAP_THREE( fchown, _fchown, int , int , uid_t , gid_t )
REMAP_ZERO( fork, _fork, pid_t )
REMAP_ZERO( fork1, _fork1, pid_t )
REMAP_TWO( fstat, _fstat, int, int, struct stat * )
REMAP_FOUR( fstatfs, _fstatfs, int , int , struct statfs *, int, int )
REMAP_THREE( fxstat, _fxstat, int , const int , int , struct stat *)
REMAP_THREE( getdents, _getdents, int , int , struct dirent *, size_t )
REMAP_ZERO( getgid, _getgid, gid_t )
REMAP_TWO( getgroups, _getgroups, int , int , gid_t *)
REMAP_TWO( getitimer, _getitimer, int , int , struct itimerval *)
REMAP_ZERO( getpid, _getpid, pid_t )
REMAP_TWO( getrlimit, _getrlimit, int , int , struct rlimit *)
REMAP_TWO( gettimeofday, _gettimeofday, int , struct timeval *, void *)
REMAP_ZERO( getuid, _getuid, uid_t )
REMAP_TWO( kill, _kill, int, pid_t, int )
REMAP_THREE( lchown, _lchown, int , const char *, uid_t , gid_t )
REMAP_TWO( link, _link, int , const char *, const char *)
REMAP_TWO( lstat, _lstat, int, const char *, struct stat * )
REMAP_THREE( lxstat, _lxstat, int , const int , const char *, struct stat *)
REMAP_TWO( mkdir, _mkdir, int , const char *, unsigned long )
REMAP_THREE( mknod, _mknod, int , const char *, int , dev_t )
REMAP_SIX( mmap, _mmap, void *, void *, size_t, int, int, int, off_t )
REMAP_SIX( mount, _mount, int , const char *, const char *, int , int , char *, int )
REMAP_THREE( mprotect, _mprotect, int , void *, size_t , int )
     /* REMAP_TWO( munmap, _munmap, int , void *, size_t ) */
REMAP_ONE( pipe, _pipe, int , int *)
REMAP_ONE( plock, _plock, int , int )
REMAP_THREE( poll, _poll, int, struct pollfd *, unsigned long, int )
REMAP_FOUR_VOID( profil, _profil, void , unsigned short *, unsigned int , int , unsigned int )
REMAP_FOUR( ptrace, _ptrace, int , int , long , int , int )
REMAP_THREE( readlink, _readlink, int , const char *, char *, size_t )
REMAP_THREE( readv, _readv, ssize_t, int, struct iovec *, int )
REMAP_TWO( rename, _rename, int, const char *, const char * )
REMAP_ONE( rmdir, _rmdir, int , const char *)
REMAP_ONE( setgid, _setgid, int , gid_t )
/* REMAP_TWO( setgroups, _setgroups, int , int , const gid_t []) */
REMAP_TWO( setgroups, _setgroups, int , int , const gid_t *)
REMAP_THREE( setitimer, _setitimer, int , int , struct itimerval *, struct itimerval *)
REMAP_TWO( setregid, _setregid, int , int , int )
REMAP_TWO( setreuid, _setreuid, int , int , int )
REMAP_TWO( setrlimit, _setrlimit, int , int , const struct rlimit *)
REMAP_ONE( setuid, _setuid, int , uid_t )
REMAP_TWO( stat, _stat, int, const char *, struct stat * )
REMAP_FOUR( statfs, _statfs, int , char *, struct statfs *, int, int )
REMAP_TWO( symlink, _symlink, int , const char *, const char *)
REMAP_ZERO_VOID( sync, _sync, void )
REMAP_ONE( umask, _umask, unsigned long , unsigned long )
/* REMAP_ONE( uname, _uname, int , struct utsname *) */
REMAP_ONE( unlink, _unlink, int , const char *)
/* REMAP_TWO( utimes, _utimes, int , const char *, struct timeval []) */
REMAP_TWO( utimes, _utimes, int , const char *, struct timeval *)
REMAP_ZERO( vfork, _vfork, pid_t )
REMAP_ONE( wait, _wait, pid_t , int *)
REMAP_THREE( writev, _writev, int, int, const struct iovec *, int )
REMAP_THREE( xstat, _xstat, int , const int , const char *, struct stat *)

#endif /* REMOTE_SYSCALLS */
