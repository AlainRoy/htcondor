
#include "filename_tools.h"

void filename_url_parse( char *input, char *method, char *server, int *port, char *path )
{
	char *p,*q;

	method[0] = server[0] = path[0] = 0;
	*port = -1;

	/* Find a colon and record the method proceeding it */

	p = strchr(input,':');
	if(p) {
		*p=0;
		strncpy(method,input,_POSIX_PATH_MAX);
		*p=':';
		p++;
	} else {
		p = input;
	}

	/* Now p points to the first character following the colon */

	if( (p[0]=='/') && (p[1]=='/') ) {

		/* This is a server name, skip the slashes */
		p++; p++;

		/* Find the end of the server name */
		q = strchr(p,'/');

		if(q) {
			/* If there is an end, copy up to that. */
			*q=0;
			strncpy(server,p,_POSIX_PATH_MAX);
			*q='/';
			p = q++;
		} else {
		        /* If there is no end, copy all of it. */
			strncpy(server,p,_POSIX_PATH_MAX);
			p = 0;
		}

		/* Now, reconsider the server name for a port number. */
		q = strchr(server,':');

		if(q) {
			/* If there is one, delimit the server name and scan */
			*q=0;
			q++;
			*port = atoi(q);
		}
	}

	/* Now, p points to the beginning of the filename, or null */

	if( (!p) || (!*p) ) return;

	strncpy(path,p,_POSIX_PATH_MAX);
}

/* Copy in to out, removing whitespace. */

static void eat_space( char *in, char *out )
{
	while(1) {
		switch(*in) {
			case 0:
				*out=0;
				return;
				break;
			case ' ':
			case '\t':
			case '\n':
				in++;
				break;
			default:
				*out++ = *in++;
				break;
		}
	}
}

/*
Copy from in to out, stopping at null or delim.  If the amount
to copy exceeds length, eat the rest silently.  Return a
pointer to the delimiter, or return null at end of string.
*/

static char * copy_upto( char *in, char *out, char delim, int length )
{
	int copied=0;

	while(1) {
		if( *in==0 ) {
			*out=0;
			return 0;
		} else if(*in==delim) {
			*out=0;
			return in;
		} else {
			if(copied<length) {
				*out++ = *in++;
				copied++;
			} else {
				in++;
			}
		}
	}
}


int filename_remap_find( char *input, char *filename, char *output )
{
	char name[_POSIX_PATH_MAX];
	char url[_POSIX_PATH_MAX];
	char *buffer,*p;

	/* First make a copy of the input in canonical form */

	buffer = malloc(strlen(input));
	if(!buffer) return 0;
	eat_space(input,buffer);

	/* Now find things like name=url; name=url; ... */
	/* A trailing url with no ; shouldn't cause harm. */

	p = buffer;

	while(1) {
		p = copy_upto(p,name,'=',_POSIX_PATH_MAX);
		if(!p) break;
		p++;
		p = copy_upto(p,url,';',_POSIX_PATH_MAX);

		if(!strncmp(name,filename,_POSIX_PATH_MAX)) {
			strncpy(output,url,_POSIX_PATH_MAX);
			free(buffer);
			return 1;
		}

		if(!p) break;
		p++;
	}

	free(buffer);
	return 0;
}
