/* 
** Copyright 1994 by Miron Livny, and Mike Litzkow
** 
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted,
** provided that the above copyright notice appear in all copies and that
** both that copyright notice and this permission notice appear in
** supporting documentation, and that the names of the University of
** Wisconsin and the copyright holders not be used in advertising or
** publicity pertaining to distribution of the software without specific,
** written prior permission.  The University of Wisconsin and the 
** copyright holders make no representations about the suitability of this
** software for any purpose.  It is provided "as is" without express
** or implied warranty.
** 
** THE UNIVERSITY OF WISCONSIN AND THE COPYRIGHT HOLDERS DISCLAIM ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE UNIVERSITY OF
** WISCONSIN OR THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT
** OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
** OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
** OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
** OR PERFORMANCE OF THIS SOFTWARE.
** 
** Author:  Mike Litzkow
**
*/ 

#ifndef _IMAGE_H
#define _IMAGE_H

#include <sys/types.h>
#include <limits.h>
#include "machdep.h"

#define NAME_LEN 64
typedef unsigned long RAW_ADDR;
typedef int BOOL;

#if defined(TRUE)
#	undef TRUE
#	undef FALSE
#endif

#include "condor_constants.h"

#if defined(SUNOS41) || defined(ULTRIX42) || defined(ULTRIX43) || defined(IRIX4) || defined(IRIX5)
typedef int ssize_t; // should be included in <sys/types.h>, but some don't
#endif

const int MAGIC = 0xfeafea;
const int SEG_INCR = 25;
const int  MAX_SEGS = 200;

class Header {
public:
	void Init();
	void IncrSegs() { n_segs += 1; }
	int	N_Segs() { return n_segs; }
	void Display();
private:
	int		magic;
	int		n_segs;
	char	pad[ 1024 - 2 * sizeof(int) - NAME_LEN ];
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
extern "C" void restart();
extern "C" void init_image_with_file_name( char *ckpt_name );
extern "C" void init_image_with_file_descriptor( int ckpt_fd );
extern "C" void _condor_prestart( int syscall_mode );
}

#define DUMP( leader, name, fmt ) \
	printf( "%s%s = " #fmt "\n", leader, #name, name )


#include "setjmp.h"
extern "C" {
	int SETJMP( jmp_buf env );
	void LONGJMP( jmp_buf env, int retval );
}

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
