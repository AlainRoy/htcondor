
#ifndef FILE_TYPES_H
#define FILE_TYPES_H

#include "condor_common.h"

/**
A printf-like function to display an unsupported or
unsafe opearation in three places:
stderr, the condor log, and the user's email.
*/

extern "C" void file_warning( char *format, ... );

/**
Pure virtual functions here cause a reference to a g++
symbol, __pure_virtual.  When the c++ library is not present,
i.e. linking with a fortran program, we need to provide 
a reference for this symbol.
<p>
Perhaps this function should be moved to a more
general purpose location...
*/

extern "C" void __pure_virtual();

/**
File is a virtual class which defines all the operations that
can be performed on an fd.  Methods of accessing an fd (local,
remote, ioserver, etc.) are built be extending File.
<p>
Open, close, read, write, checkpoint, suspend, and resume must be
implemented by subclasses of File.  Esoteric operations
(fcntl, fstat, etc.) have defaults which display a warning 
and return an error.
<p>
Notice that all the operations listed below operate on an fd.
Operations that work on a name instead of an fd are sent off
to the shadow or local system, as appropriate.
<p>
Caveats:
<dir>
<li> Each file stores a unique integer when a resume() is
performed, so that no file is accidentally resumed twice after
a checkpoint.  (See file_state.h for a instance where this
might happen.)  The open file table must provide a unique
integer every time a resume() is performed.
</dir>
*/

class File {
public:
	virtual void dump();

	virtual int open(const char *path, int flags, int mode)=0;
	virtual int close()=0;
	virtual int read(int offset, char *data, int length)=0;
	virtual int write(int offset, char *data, int length)=0;

	virtual void checkpoint()=0;
	virtual void suspend()=0;
	virtual void resume(int count)=0;

	virtual int fcntl( int cmd, int arg );
	virtual int fstat( struct stat *buf );
	virtual int ioctl( int cmd, int arg );
	virtual int flock( int op );
	virtual int fstatfs( struct statfs *buf );
	virtual int fchown( uid_t owner, gid_t group );
	virtual int fchmod( mode_t mode );
	virtual int ftruncate( size_t length ); 
	virtual int fsync();

	int	illegal( char *op );

	int	is_readable()		{ return readable; }
	int	is_writeable()		{ return writeable; }
	void	set_size(size_t s)	{ size = s; }
	int	get_size()		{ return size; }
	char	*get_kind()		{ return kind; }
	char	*get_name()		{ return name; }

	void	add_user()		{ use_count++; }
	void	remove_user()		{ use_count--; }
	int	get_use_count()		{ return use_count; }

	void	enable_buffer()		{ bufferable=1; }
	void	disable_buffer()	{ bufferable=0; }
	int	ok_to_buffer()		{ return bufferable; }

	/*
	Without performing an actual open, associate this
	object with an existing fd, and mark it readable or writable
	as indicated.
	*/

	int	force_open( int f, int r, int w ) {
		fd = f;
		readable = r;
		writeable = w;
		forced = 1;
	}

	/*
	Provide support for the MapFd and LocalAccess interfaces.
	These will go away soon.
	*/

	virtual int map_fd_hack();
	virtual int local_access_hack();

protected:

	int	fd;		// the real fd used by this file
	int	readable;	// can this file be read?
	int	writeable;	// can this file be written?
	char	*kind;		// text describing file type
	char	name[_POSIX_PATH_MAX]; // file name used at open
	int	size;		// number of bytes in the file
	int	use_count;	// how many people are using this object?
	int	forced;		// was this created with force_open?
	int	bufferable;	// should this file be buffered?
	int	resume_count;	// how many times has this object been resumed?
};

/**
This class sends all operations to a locally opened file.
*/

class LocalFile : public File {
public:
	LocalFile();

	virtual int open(const char *path, int flags, int mode);
	virtual int close();

	virtual int read(int offset, char *data, int length);
	virtual int write(int offset, char *data, int length);

	virtual void checkpoint();
	virtual void suspend();
	virtual void resume(int count);

	virtual int fcntl( int cmd, int arg );
	virtual int fstat( struct stat *buf );
	virtual int ioctl( int cmd, int arg );
	virtual int flock( int op );
	virtual int fstatfs( struct statfs *buf );
	virtual int fchown( uid_t owner, gid_t group );
	virtual int fchmod( mode_t mode );
	virtual int ftruncate( size_t length ); 
	virtual int fsync();

	virtual int local_access_hack();
};

/**
This class sends all I/O operations to a remotely opened file.
*/

class RemoteFile : public File {
public:
	RemoteFile();

	virtual int open(const char *path, int flags, int mode);
	virtual int close();

	virtual int read(int offset, char *data, int length);
	virtual int write(int offset, char *data, int length);

	virtual void checkpoint();
	virtual void suspend();
	virtual void resume(int count);

	virtual int fcntl( int cmd, int arg );
	virtual int fstat( struct stat *buf );
	virtual int ioctl( int cmd, int arg );
	virtual int flock( int op );
	virtual int fstatfs( struct statfs *buf );
	virtual int fchown( uid_t owner, gid_t group );
	virtual int fchmod( mode_t mode );
	virtual int ftruncate( size_t length ); 
	virtual int fsync();
};

/**
This class sends all I/O operations to an I/O server.
(Not implemented yet.)
*/

class IOServerFile : public File { };

#endif


