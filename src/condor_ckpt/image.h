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

 

#ifndef _IMAGE_H
#define _IMAGE_H

#include "condor_common.h"
#include "machdep.h"


#define NAME_LEN 64
typedef unsigned long RAW_ADDR;
typedef int BOOL;

const int MAGIC = 0xfeafea;
const int SEG_INCR = 25;
const int  MAX_SEGS = 200;

class Header {
public:
	void Init();
	void IncrSegs() { n_segs += 1; }
	int	N_Segs() { return n_segs; }
	void Display();
 	RAW_ADDR	low_shlib_start;      // Start address of the lowest shlib
 	RAW_ADDR        addr_of_Env;          // Location of ENV in ckpt'd process
 	RAW_ADDR        addr_of_FileTab;      // ...and location of FileTab
 	RAW_ADDR        addr_of_syscall_sock; // ...and location of syscall_sock
	RAW_ADDR        addr_of_str;
private:
	int		magic;
	int		n_segs;
	// FIXME: Was
	// char	pad[ 1024 - 2 * sizeof(int) - NAME_LEN ];
	// What is the purpose of this?  To make the head 1k?  How do we
	// know that we can calculate this size correctly for a C++ class?
	// -zandy 6/18/1998
	char	pad[(1024 - 2 * sizeof(int) - 5 * sizeof (RAW_ADDR)
		     - NAME_LEN)];
};

class SegMap {
public:
	void Init( const char *name, RAW_ADDR core_loc, long len, int prot );
	ssize_t Read( int fd, ssize_t pos );
	ssize_t Write( int fd, ssize_t pos );
	ssize_t SetPos( ssize_t my_pos );
	BOOL Contains( void *addr );
	char *GetName() { return name; }
	void Display();
private:
	char		name[14];
	off_t		file_loc;
	RAW_ADDR	core_loc;
	long		len;
	int			prot;		// segment protection mode
};

typedef enum { STANDALONE, REMOTE } ExecutionMode;

class Image {
public:
	void Save();
	int	Write();
	int Write( int fd );
	int Write( const char *name );
	int Read();
	int Read( int fd );
	int Read( const char *name );
	void Close();
	void Restore();
	char *FindSeg( void *addr );
	void Display();
	void RestoreSeg( const char *seg_name );
	void RestoreAllSegsExceptStack();
	void SetFd( int fd );
	void SetFileName( char *ckpt_name );
	void SetMode( int syscall_mode );
	ExecutionMode	GetMode() { return mode; }
	size_t			GetLen()  { return len; }
	int				GetFd()   { return fd; }
	RAW_ADDR GetAddrOfEnv () { return head.addr_of_Env; }
	RAW_ADDR GetAddrOfFileTab () { return head.addr_of_FileTab; }
	RAW_ADDR GetAddrOfsyscall_sock () { return head.addr_of_syscall_sock; }
	RAW_ADDR GetAddrOfStr () { return head.addr_of_str; }
 	int unloadGangrenousSegments ();
protected:
	RAW_ADDR	GetStackLimit();
	void AddSegment( const char *name, RAW_ADDR start, RAW_ADDR end,
			int prot );
	void SwitchStack( char *base, size_t len );
	char	*file_name;
	Header	head;
	SegMap	map[ MAX_SEGS ];
	ExecutionMode	mode;	// executing in standalone/remote mode
	BOOL	valid;		// initialized and ready to write ckpt file or restore
	int		fd;		// descriptor pointing to ckpt file
	ssize_t	pos;	// position in ckpt file of seg currently reading/writing
	size_t	len;	// size of our ckpt file
};
void RestoreStack();

#if defined(HPUX10)
extern "C" void Checkpoint( int, siginfo_t *, void * );
#else
extern "C" void Checkpoint( int, int, void * );
#endif

extern "C" {
	void ckpt();
	void restart();
	void init_image_with_file_name( char *ckpt_name );
	void init_image_with_file_descriptor( int ckpt_fd );
	void _condor_prestart( int syscall_mode );
	void Suicide();
}

#define DUMP( leader, name, fmt ) \
	printf( "%s%s = " #fmt "\n", leader, #name, name )


#include "condor_fix_setjmp.h"


long data_start_addr();
long data_end_addr();
long stack_start_addr();
long stack_end_addr();
BOOL StackGrowsDown();
int JmpBufSP_Index();
void ExecuteOnTmpStk( void (*func)() );
void patch_registers( void  *);
#if defined(Solaris) || defined(IRIX53)
     int find_map_for_addr(caddr_t addr);
     int num_segments( );
     int segment_bounds( int seg_num, RAW_ADDR &start, RAW_ADDR &end,
	int &prot );
     void display_prmap();
	 extern "C" int open_ckpt_file( const char *name,
								   int flags, size_t n_bytes );
#endif
#if defined(LINUX)
extern "C" {
     int find_map_for_addr(long addr);
     int num_segments( );
     int segment_bounds( int seg_num, RAW_ADDR &start, RAW_ADDR &end,
	int &prot );
     void display_prmap();
     unsigned long find_correct_vm_addr(unsigned long, unsigned long, int);
};
#endif

#	define JMP_BUF_SP(env) (((long *)(env))[JmpBufSP_Index()])
#endif
