#ifndef CONDOR_FILE_COMPRESS_H
#define CONDOR_FILE_COMPRESS_H

#include "condor_file.h"
#include "condor_compress.h"

/*
This object transparently converts any CondorFile into a gzipped file.
In general, only sequential access is supported.  Random-access reading
is allowed but is very slow because it is implemented by reading intervening
data.

Only the most basic flavor of compression, as described by the gzip documentation,
is supported.  This seems to interact just fine with normal invocations of
gzip, however, I would not be surprised were there more complicated formats
which this does not understand.
*/

#define COMPRESS_BUFFER_SIZE 32768

class CondorFileCompress : public CondorFile {
public:
	CondorFileCompress( CondorFile *original );
	virtual ~CondorFileCompress();

	virtual int cfile_open(const char *url, int flags, int mode);
	virtual int cfile_close();
	virtual int cfile_read(int offset, char *data, int length);
	virtual int cfile_write(int offset, char *data, int length);

	virtual int cfile_fcntl( int cmd, int arg );
	virtual int cfile_ioctl( int cmd, int arg );
	virtual int cfile_ftruncate( size_t length ); 
	virtual int cfile_fstat( struct stat* buf );
	virtual int cfile_fsync();
	virtual int cfile_flush();

	virtual int	is_readable();
	virtual int	is_writeable();
	virtual int	is_seekable();

	virtual int	get_size();
	virtual char	*get_url();

	virtual int get_unmapped_fd();
	virtual int is_file_local();

private:

	void	end_compression();

	int	write_data( char *data, int length, int flush );
	void	write_complete();
	int	write_header();
	int	write_int( int bytes, int value );

	int	read_consume( int length );
	int	read_data( char *data, int length );
	int	read_header();
	int	read_int( int bytes );
	void	read_string();

	// The URL of this file
	char url[_POSIX_PATH_MAX];

	// Compressed data buffer
	char buffer[COMPRESS_BUFFER_SIZE];

	// The last type of action performed
	enum {NONE,READ,WRITE} last_action;

	// The current compression state
	z_stream stream;

	// The file which is to be compressed
	CondorFile *original;

	// Current offset into the physical file
	int poffset;

	// Current offset into the virtual (compressed) file
	int voffset;

	// Accumulating checksum for writes
	int crc;
};

#endif /* CONDOR_FILE_COMPRESS_H */
