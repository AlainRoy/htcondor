#ifndef CONDOR_SYSCALL_SYSDEP_H
#define CONDOR_SYSCALL_SYSDEP_H

#if defined(LINUX)
#   if defined(GLIBC) 
#		if defined(GLIBC20)
#			define MMAP_T char*
#			define GLIBC_CONST 
#		else
#			define MMAP_T void*
			/* The man page on a glibc21 machine is wrong, there is no const */
#			define GLIBC_CONST 
#		endif
#   else
#		define MMAP_T void*
#		define GLIBC_CONST const
#   endif
#endif

/* this is used for ftruncate */
#if defined(LINUX)
#	if defined(GLIBC21)
#		define LENGTH_TYPE off_t
#	else
#		define LENGTH_TYPE size_t
#	endif
#endif

#if defined(OSF1)
#   define SYSCALL_PTR	void
#   define INV_SYSCALL_PTR	char
#   define NEED_CONST	const
#elif defined(HPUX)
#   define SYSCALL_PTR	char
#   define INV_SYSCALL_PTR	const void
#	define NEED_CONST
#else
#   define SYSCALL_PTR	char
#   define INV_SYSCALL_PTR	void
#   if defined(Solaris26)
#	define NEED_CONST	const
#   else
#	define NEED_CONST
#   endif
#endif

#if defined(LINUX)
#	define SYNC_RETURNS_VOID 0
#else /* Solaris, IRIX, Alpha, DUX4.0 all confirmed... */
#	define SYNC_RETURNS_VOID 1
#endif

#if defined(HPUX10) || defined(Solaris26)
#	define HAS_64BIT_STRUCTS	1
#	define HAS_64BIT_SYSCALLS	1
#endif

#endif /* CONDOR_SYSCALL_SYSDEP_H */
