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

#include "condor_common.h"

/*
** free_fs_blocks: reports the number of kbytes free for the filesystem where
** given filename resides.
*/

#include "sysapi.h"
#include "sysapi_externs.h"

/* static function declarations */
static int reserve_for_afs_cache();
static int reserve_for_fs();

/* the code starts here */

#if defined(WIN32)

#include <limits.h>

int
sysapi_disk_space_raw(const char *filename)
{
	ULARGE_INTEGER FreeBytesAvailableToCaller, TotalNumberOfBytes, TotalNumberOfFreeBytes;
	unsigned int t_hi, t_lo, temp;
	const unsigned int lowest_ten_mask = 0x00000cff;
	
	t_hi = t_lo = temp = 0;
	sysapi_internal_reconfig();

	
	if (GetDiskFreeSpaceEx(filename, &FreeBytesAvailableToCaller, &TotalNumberOfBytes, 
		&TotalNumberOfFreeBytes) == 0) {
		return -1;
	} else {
		
		// we have to shift everything down 10 bits since we report the number
		// of kbytes free. There's a HighPart and a LowPart, so it gets a little ugly.

		// first shift the low part (divide by 1024 to get it into kbytes)
		t_lo = FreeBytesAvailableToCaller.LowPart;
		t_lo = t_lo >> 10;
		// now grab the lowest 10 bits in the high part
		t_hi = FreeBytesAvailableToCaller.HighPart;
		temp = lowest_ten_mask & t_hi;
		// shift the high part 10 bits to get it into kbytes
		t_hi = t_hi >> 10;
		
		// shift the lowest 10 bits from the high part up to the 
		// highest 10 bits, and stick them into the low part
		temp = temp << 22;
		t_lo |= temp;

		if (t_hi > 0) {
			return INT_MAX;
		} else {
			return (int)t_lo;
		}
	}
}

#else

#include "debug.h"

/* Can't include condor_config.h since it depends on classads which is
   only C++.  -Derek 6/25/98 */
extern char* param();

static char	*Afs_Cache_Name;

#define FS_PROGRAM "/usr/afsws/bin/fs"
#define FS_COMMAND "getcacheparms"
#define FS_OUTPUT_FORMAT "\nAFS using %d of the cache's available %d"

#if defined(__STDC__)
FILE * my_popen( char * cmd, char * mode );
int my_pclose( FILE *fp );
#else
FILE * my_popen();
int my_pclose();
#endif

#endif

/*
  How much disk space we need to reserve for the AFS cache.  Answer is
  in kilobytes.
*/
static int
reserve_for_afs_cache()
{
#ifdef WIN32
	return 0;
#else
	int		answer;
	FILE	*fp;
	char	cmd[512];
	int		cache_size, cache_in_use;
	int		do_it;
	char	*str;

	/* See if we're configured to deal with an AFS cache */
	do_it = _sysapi_reserve_afs_cache;

		/* If we're not configured to deal with AFS cache, just return 0 */
	if( !do_it ) {
		return 0;
	}

		/* Run an AFS utility program to learn how big the cache is and
		   how much is in use. */
	dprintf( D_FULLDEBUG, "Checking AFS cache parameters\n" );
	sprintf( cmd, "%s %s", FS_PROGRAM, FS_COMMAND );
	fp = my_popen( cmd, "r" );
	fscanf( fp, FS_OUTPUT_FORMAT, &cache_in_use, &cache_size );
	my_pclose( fp );
	dprintf( D_FULLDEBUG, "cache_in_use = %d, cache_size = %d\n",
		cache_in_use,
		cache_size
	);
	answer = cache_size - cache_in_use;

		/* The cache may be temporarily over size, in this case AFS will
		   clean up itself, so we don't have to allow for that. */
	if( answer < 0 ) {
		answer = 0;
	}

	dprintf( D_FULLDEBUG, "Reserving %d kbytes for AFS cache\n", answer );
	return answer;
#endif
}

/*
  How much disk space we need to reserve for the regular file system.
  Answer is in kilobytes.
*/
static int
reserve_for_fs()
{
	/* XXX make cleaner */
	int	answer = -1;

	if( answer < 0 ) {
		dprintf( D_FULLDEBUG, "Looking up RESERVED_DISK parameter\n" );

		answer = _sysapi_reserve_disk;
		if( answer < 0 ) {
			answer = 0;
		}
	}

	answer = _sysapi_reserve_disk;
	dprintf( D_FULLDEBUG, "Reserving %d kbytes for file system\n", answer );
	return answer;
}


#if defined(__STDC__)
int sysapi_disk_space_raw( const char *filename);
#else
int sysapi_disk_space_raw();
#endif

#if defined(ULTRIX42) || defined(ULTRIX43)
int
sysapi_disk_space_raw(filename)
const char *filename;
{
	struct fs_data statfsbuf;
	struct fs_data_req *fs;
	int free_kbytes;

	sysapi_internal_reconfig();

	if(statfs(filename, &statfsbuf) < 0) {
		dprintf(D_ALWAYS, "sysapi_disk_space_raw: statfs(%s,0x%x) failed\n",
													filename, &statfsbuf );
		return(0);
	}

	fs = &statfsbuf.fd_req;

	/* bfreen is in kbytes */
	free_kbytes = fs->bfreen;

	dprintf(D_FULLDEBUG, "number of kbytes available for (%s): %d\n", 
			filename, free_kbytes);

	return(free_kbytes);
}
#endif /* MIPS && ULTRIX */

#if defined(VAX) && defined(ULTRIX)
/* statfs() did not work on our VAX_ULTRIX machine.  use getmnt() instead. */
int
sysapi_disk_space_raw(filename)
char *filename;
{
	struct fs_data mntdata;
	int start;
	struct fs_data_req *fs_req;
	int free_kbytes;

	sysapi_internal_reconfig();

	if( getmnt(&start, &mntdata, sizeof(mntdata), NOSTAT_ONE, filename) < 0) {
		dprintf(D_ALWAYS, "sysapi_disk_space_raw(): getmnt failed");
		return(0);
	}
	
	fs_req = &mntdata.fd_req;

	/* bfreen is in kbytes. */
	free_kbytes = fs_req->bfreen;

	dprintf(D_FULLDEBUG, "number of kbytes available for filename: %d\n", 
			free_kbytes);

	return(free_kbytes);
}
#endif /* VAX && ULTRIX */

#if (defined(I386) && defined(DYNIX)) || defined(LINUX) || (defined(VAX) && defined(BSD43)) || (defined(MC68020) && defined(SUNOS41)) || (defined(IBM032) && defined(BSD43)) || (defined(MC68020) && defined(BSD43)) || (defined(SPARC) && defined(SUNOS41)) || (defined(R6000) && defined(AIX31)) || defined(AIX32) || defined(IRIX331) || (defined(SPARC) && defined(CMOS)) || defined(HPUX) || defined(OSF1) || defined(Solaris) || defined(IRIX53) || defined(IRIX65) || defined(IRIX62)

#include <limits.h>

#if defined(AIX31) || defined(AIX32) || defined(IRIX53) || defined(IRIX65) || defined(IRIX62)
#include <sys/statfs.h>
#elif defined(Solaris)
#include <sys/statvfs.h>
#elif defined(OSF1)
#include <sys/mount.h>
#endif

#if defined(IRIX331) || defined(IRIX53) || defined(IRIX65) || defined(IRIX62)
/* There is no f_bavail on IRIX */
#define f_bavail f_bfree
#endif

int
sysapi_disk_space_raw(filename)
const char *filename;
{
#if defined(Solaris)
	struct statvfs statfsbuf;
#else
	struct statfs statfsbuf;
#endif
	double free_kbytes;
	float kbytes_per_block;

	sysapi_internal_reconfig();

#if defined(IRIX331) || defined(IRIX53) || defined(IRIX65) || defined(IRIX62)
	if(statfs(filename, &statfsbuf, sizeof statfsbuf, 0) < 0) {
#elif defined(OSF1)
	if(statfs(filename, &statfsbuf, sizeof statfsbuf) < 0) {
#elif defined(Solaris)
	if(statvfs(filename, &statfsbuf) < 0) {
#else
	if(statfs(filename, &statfsbuf) < 0) {
#endif
		dprintf(D_ALWAYS, "sysapi_disk_space_raw: statfs(%s,0x%x) failed\n",
													filename, &statfsbuf );
		dprintf(D_ALWAYS, "errno = %d\n", errno );
		return(0);
	}

	/* Convert to kbyte blocks: available blks * blksize / 1k bytes. */
	/* overflow problem fixed by using doubles to hold result - 
		Keller 05/17/99 */

#if defined(Solaris)
		/* On Solaris, we need to use f_frsize, the "fundamental
		   filesystem block size", not f_bsize, the "preferred file
		   system block size".  3/25/98  Derek Wright */
	kbytes_per_block = ( (unsigned long)statfsbuf.f_frsize / (float)1024 );
#elif defined(OSF1)
	kbytes_per_block = ( (unsigned long)statfsbuf.f_fsize / (float)512 );
#else
	kbytes_per_block = ( (unsigned long)statfsbuf.f_bsize / (float)1024 );
#endif

	free_kbytes = (double)statfsbuf.f_bavail * (double)kbytes_per_block; 
	if(free_kbytes > INT_MAX) {
		return(INT_MAX);
	}

	dprintf(D_FULLDEBUG, "number of kbytes available for (%s): %f\n", 
			filename, free_kbytes);

	return (int)free_kbytes;
}
#endif /* I386 && DYNIX */

/*
  Return number of kbytes condor may play with in the named file
  system.  System administrators may reserve space which condor should
  leave alone.  This could be either a static amount of space, or an
  amount of space based on the current size of the AFS cache (or
  both).

  Note: This code assumes that if we have an AFS cache, that cache is
  on the filesystem we are asking about.  This is not true in general,
  but it is the case at the developer's home site.  A more correct
  implementation would check whether the filesystem being asked about
  is the one containing the cache.  This is messy to do if the
  filesystem name we are given isn't a full pathname, e.g.
  sysapi_disk_space("."), which is why it isn't implemented here. -- mike

*/
int
sysapi_disk_space(const char *filename)
{
	int answer;

	sysapi_internal_reconfig();

	answer =  sysapi_disk_space_raw(filename)
			- reserve_for_afs_cache()
			- reserve_for_fs();
	return answer < 0 ? 0 : answer;

}


