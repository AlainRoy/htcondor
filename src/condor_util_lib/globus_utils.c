/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/


#include "condor_common.h"
#include "condor_config.h"
#include "condor_debug.h"
#include "util_lib_proto.h"

#include "globus_utils.h"

#if defined(HAVE_EXT_GLOBUS)
#     include "globus_gsi_credential.h"
#     include "globus_gsi_system_config.h"
#     include "globus_gsi_system_config_constants.h"
#     include "gssapi.h"
#     include "globus_gss_assist.h"
#     include "globus_gsi_proxy.h"
#endif

#define DEFAULT_MIN_TIME_LEFT 8*60*60;


static char * _globus_error_message = NULL;

#define GRAM_STATUS_STR_LEN		8

const char *
GlobusJobStatusName( int status )
{
	static char buf[GRAM_STATUS_STR_LEN];
#if defined(HAVE_EXT_GLOBUS)
	switch ( status ) {
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_PENDING:
		return "PENDING";
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_ACTIVE:
		return "ACTIVE";
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_FAILED:
		return "FAILED";
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_DONE:
		return "DONE";
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_SUSPENDED:
		return "SUSPENDED";
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_UNSUBMITTED:
		return "UNSUBMITTED";
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_STAGE_IN:
		return "STAGE_IN";
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_STAGE_OUT:
		return "STAGE_OUT";
	case GLOBUS_GRAM_PROTOCOL_JOB_STATE_UNKNOWN:
		return "UNKNOWN";
	default:
		snprintf( buf, GRAM_STATUS_STR_LEN, "%d", status );
		return buf;
	}
#else
	snprintf( buf, GRAM_STATUS_STR_LEN, "%d", status );
	return buf;
#endif
}

const char *
x509_error_string( void )
{
	return _globus_error_message;
}

static
void
set_error_string( const char *message )
{
	if ( _globus_error_message ) {
		free( _globus_error_message );
	}
	_globus_error_message = strdup( message );
}

/* Activate the globus gsi modules for use by functions in this file.
 * Returns zero if the modules were successfully activated. Returns -1 if
 * something went wrong.
 */
static
int
activate_globus_gsi( void )
{
#if !defined(HAVE_EXT_GLOBUS)
	set_error_string( "This version of Condor doesn't support X509 credentials!" );
	return -1;
#else
	static int globus_gsi_activated = 0;

	if ( globus_gsi_activated != 0 ) {
		return 0;
	}

/* This module is activated by GLOBUS_GSI_CREDENTIAL_MODULE
	if ( globus_module_activate(GLOBUS_GSI_SYSCONFIG_MODULE) ) {
		set_error_string( "couldn't activate globus gsi sysconfig module" );
		return -1;
	}
*/

	if ( globus_module_activate(GLOBUS_GSI_CREDENTIAL_MODULE) ) {
		set_error_string( "couldn't activate globus gsi credential module" );
		return -1;
	}


	if ( globus_module_activate(GLOBUS_GSI_GSSAPI_MODULE) ) {
		set_error_string( "couldn't activate globus gsi gssapi module" );
		return -1;
	}


	if ( globus_module_activate(GLOBUS_GSI_PROXY_MODULE) ) {
		set_error_string( "couldn't activate globus gsi proxy module" );
		return -1;
	}


	globus_gsi_activated = 1;
	return 0;
#endif
}

/* Return the path to the X509 proxy file as determined by GSI/SSL.
 * Returns NULL if the filename can't be determined. Otherwise, the
 * string returned must be freed with free().
 */
char *
get_x509_proxy_filename( void )
{
	char *proxy_file = NULL;
#if defined(HAVE_EXT_GLOBUS)
	globus_gsi_proxy_file_type_t     file_type    = GLOBUS_PROXY_FILE_INPUT;

	if ( activate_globus_gsi() != 0 ) {
		return NULL;
	}

	if ( GLOBUS_GSI_SYSCONFIG_GET_PROXY_FILENAME(&proxy_file, file_type) !=
		 GLOBUS_SUCCESS ) {
		set_error_string( "unable to locate proxy file" );
	}
#endif
	return proxy_file;
}

/* Return the subject name of a given proxy cert. 
  On error, return NULL.
  On success, return a pointer to a null-terminated string.
  IT IS THE CALLER'S RESPONSBILITY TO DE-ALLOCATE THE STIRNG
  WITH free().
 */
char *
x509_proxy_subject_name( const char *proxy_file )
{
#if !defined(HAVE_EXT_GLOBUS)
	set_error_string( "This version of Condor doesn't support X509 credentials!" );
	return NULL;
#else

	globus_gsi_cred_handle_t         handle       = NULL;
	globus_gsi_cred_handle_attrs_t   handle_attrs = NULL;
	char *subject_name = NULL;
	char *my_proxy_file = NULL;

	if ( activate_globus_gsi() != 0 ) {
		return NULL;
	}

	if (globus_gsi_cred_handle_attrs_init(&handle_attrs)) {
		set_error_string( "problem during internal initialization1" );
		goto cleanup;
	}

	if (globus_gsi_cred_handle_init(&handle, handle_attrs)) {
		set_error_string( "problem during internal initialization2" );
		goto cleanup;
	}

	/* Check for proxy file */
	if (proxy_file == NULL) {
		my_proxy_file = get_x509_proxy_filename();
		if (my_proxy_file == NULL) {
			goto cleanup;
		}
		proxy_file = my_proxy_file;
	}

	// We should have a proxy file, now, try to read it
	if (globus_gsi_cred_read_proxy(handle, proxy_file)) {
		set_error_string( "unable to read proxy file" );
	   goto cleanup;
	}

	if (globus_gsi_cred_get_subject_name(handle, &subject_name)) {
		set_error_string( "unable to extract subject name" );
		goto cleanup;
	}

 cleanup:
	if (my_proxy_file) {
		free(my_proxy_file);
	}

	if (handle_attrs) {
		globus_gsi_cred_handle_attrs_destroy(handle_attrs);
	}

	if (handle) {
		globus_gsi_cred_handle_destroy(handle);
	}

	return subject_name;

#endif /* !defined(GSS_AUTHENTICATION) */
}


/* Return the identity name of a given X509 cert. For proxy certs, this
  will return the identity that the proxy can act on behalf of, rather than
  the subject of the proxy cert itself. Normally, this will have the
  practical effect of stripping off any "/CN=proxy" strings from the subject
  name.
  On error, return NULL.
  On success, return a pointer to a null-terminated string.
  IT IS THE CALLER'S RESPONSBILITY TO DE-ALLOCATE THE STIRNG
  WITH free().
 */
char *
x509_proxy_identity_name( const char *proxy_file )
{
#if !defined(HAVE_EXT_GLOBUS)
	set_error_string( "This version of Condor doesn't support X509 credentials!" );
	return NULL;
#else

	globus_gsi_cred_handle_t         handle       = NULL;
	globus_gsi_cred_handle_attrs_t   handle_attrs = NULL;
	char *subject_name = NULL;
	char *my_proxy_file = NULL;

	if ( activate_globus_gsi() != 0 ) {
		return NULL;
	}

	if (globus_gsi_cred_handle_attrs_init(&handle_attrs)) {
		set_error_string( "problem during internal initialization1" );
		goto cleanup;
	}

	if (globus_gsi_cred_handle_init(&handle, handle_attrs)) {
		set_error_string( "problem during internal initialization2" );
		goto cleanup;
	}

	/* Check for proxy file */
	if (proxy_file == NULL) {
		my_proxy_file = get_x509_proxy_filename();
		if (my_proxy_file == NULL) {
			goto cleanup;
		}
		proxy_file = my_proxy_file;
	}

	// We should have a proxy file, now, try to read it
	if (globus_gsi_cred_read_proxy(handle, proxy_file)) {
		set_error_string( "unable to read proxy file" );
	   goto cleanup;
	}

	if (globus_gsi_cred_get_identity_name(handle, &subject_name)) {
		set_error_string( "unable to extract identity name" );
		goto cleanup;
	}

 cleanup:
	if (my_proxy_file) {
		free(my_proxy_file);
	}

	if (handle_attrs) {
		globus_gsi_cred_handle_attrs_destroy(handle_attrs);
	}

	if (handle) {
		globus_gsi_cred_handle_destroy(handle);
	}

	return subject_name;

#endif /* !defined(GSS_AUTHENTICATION) */
}

/* Return the time at which the proxy expires. On error, return -1.
 */
time_t
x509_proxy_expiration_time( const char *proxy_file )
{
#if !defined(HAVE_EXT_GLOBUS)
	set_error_string( "This version of Condor doesn't support X509 credentials!" );
	return -1;
#else

    globus_gsi_cred_handle_t         handle       = NULL;
    globus_gsi_cred_handle_attrs_t   handle_attrs = NULL;
	time_t expiration_time = -1;
	time_t time_left;
	char *my_proxy_file = NULL;

	if ( activate_globus_gsi() != 0 ) {
		return -1;
	}

    if (globus_gsi_cred_handle_attrs_init(&handle_attrs)) {
        set_error_string( "problem during internal initialization" );
        goto cleanup;
    }

    if (globus_gsi_cred_handle_init(&handle, handle_attrs)) {
        set_error_string( "problem during internal initialization" );
        goto cleanup;
    }

    /* Check for proxy file */
    if (proxy_file == NULL) {
        my_proxy_file = get_x509_proxy_filename();
        if (my_proxy_file == NULL) {
            goto cleanup;
        }
		proxy_file = my_proxy_file;
	}

    // We should have a proxy file, now, try to read it
    if (globus_gsi_cred_read_proxy(handle, proxy_file)) {
       set_error_string( "unable to read proxy file" );
       goto cleanup;
    }

	if (globus_gsi_cred_get_lifetime(handle, &time_left)) {
		set_error_string( "unable to extract expiration time" );
        goto cleanup;
    }

	expiration_time = time(NULL) + time_left;

 cleanup:
    if (my_proxy_file) {
        free(my_proxy_file);
    }

    if (handle_attrs) {
        globus_gsi_cred_handle_attrs_destroy(handle_attrs);
    }

    if (handle) {
        globus_gsi_cred_handle_destroy(handle);
    }

	return expiration_time;

#endif /* !defined(GSS_AUTHENTICATION) */
}

/* Return the number of seconds until the supplied proxy
 * file will expire.  
 * On error, return -1.    - Todd <tannenba@cs.wisc.edu>
 */
int
x509_proxy_seconds_until_expire( const char *proxy_file )
{
#if !defined(HAVE_EXT_GLOBUS)
	set_error_string( "This version of Condor doesn't support X509 credentials!" );
	return -1;
#else

	time_t time_now;
	time_t time_expire;
	time_t time_diff;

	time_now = time(NULL);
	time_expire = x509_proxy_expiration_time( proxy_file );

	if ( time_expire == -1 ) {
		return -1;
	}

	time_diff = time_expire - time_now;

	if ( time_diff < 0 ) {
		time_diff = 0;
	}

	return (int)time_diff;

#endif /* !defined(GSS_AUTHENTICATION) */
}

/* Attempt a gss_import_cred() to catch some certificate problems. This
 * won't catch all problems (it doesn't verify the entire certificate
 * chain), but it's a start. Returns 0 on success, and -1 on any errors.
 */
int
x509_proxy_try_import( const char *proxy_file )
{
#if !defined(HAVE_EXT_GLOBUS)

	set_error_string( "This version of Condor doesn't support X509 credentials!" );
	return -1;

#else
	unsigned int rc;
	unsigned int min_stat;
	gss_buffer_desc import_buf;
	gss_cred_id_t cred_handle;
	char buf_value[4096];
	char *my_proxy_file = NULL;

	if ( activate_globus_gsi() != 0 ) {
		return -1;
	}

	/* Check for proxy file */
	if (proxy_file == NULL) {
		my_proxy_file = get_x509_proxy_filename();
		if (my_proxy_file == NULL) {
			goto cleanup;
		}
		proxy_file = my_proxy_file;
	}

	snprintf( buf_value, sizeof(buf_value), "X509_USER_PROXY=%s", proxy_file);
	import_buf.value = buf_value;
	import_buf.length = strlen(buf_value) + 1;

	rc = gss_import_cred( &min_stat, &cred_handle, GSS_C_NO_OID, 1,
						  &import_buf, 0, NULL );

	if ( rc != GSS_S_COMPLETE ) {
		char *message;
        globus_gss_assist_display_status_str(&message,
											 "",
											 rc,
											 min_stat,
											 0);
		snprintf( buf_value, sizeof(buf_value), "%s", message );
		free(message);
//		snprintf( buf_value, sizeof(buf_value),
//				  "Failed to import credential maj=%d min=%d", rc,
//				  min_stat );
		set_error_string( buf_value );
		return -1;
	}

	gss_release_cred( &min_stat, &cred_handle );

 cleanup:
    if (my_proxy_file) {
        free(my_proxy_file);
    }

	return 0;
#endif /* !defined(HAVE_EXT_GLOBUS) */
}

int
check_x509_proxy( const char *proxy_file )
{
#if !defined(HAVE_EXT_GLOBUS)

	set_error_string( "This version of Condor doesn't support X509 credentials!" );
	return -1;

#else
	char *min_time_left_param = NULL;
	int min_time_left;
	int time_diff;

	if ( x509_proxy_try_import( proxy_file ) != 0 ) {
		/* Error! Don't set error message, it is already set */
		return -1;
	}

	time_diff = x509_proxy_seconds_until_expire( proxy_file );

	if ( time_diff < 0 ) {
		/* Error! Don't set error message, it is already set */
		return -1;
	}

	/* check validity */
	min_time_left_param = param( "CRED_MIN_TIME_LEFT" );

	if ( min_time_left_param != NULL ) {
		min_time_left = atoi( min_time_left_param );
		free(min_time_left_param);
	} else {
		min_time_left = DEFAULT_MIN_TIME_LEFT;
	}

	if ( time_diff == 0 ) {
		set_error_string( "proxy has expired" );
		return -1;
	}

	if ( time_diff < min_time_left ) {
		set_error_string( "proxy lifetime too short" );
		return -1;
	}

	return 0;

#endif /* !defined(GSS_AUTHENTICATION) */
}


#if defined(HAVE_EXT_GLOBUS)

int
buffer_to_bio( char *buffer, int buffer_len, BIO **bio )
{
	if ( buffer == NULL ) {
		return FALSE;
	}

	*bio = BIO_new( BIO_s_mem() );
	if ( *bio == NULL ) {
		return FALSE;
	}

	if ( BIO_write( *bio, buffer, buffer_len ) < buffer_len ) {
		BIO_free( *bio );
		return FALSE;
	}

	return TRUE;
}

int
bio_to_buffer( BIO *bio, char **buffer, int *buffer_len )
{
	if ( bio == NULL ) {
		return FALSE;
	}

	*buffer_len = BIO_pending( bio );

	*buffer = (char *)malloc( *buffer_len );
	if ( *buffer == NULL ) {
		return FALSE;
	}

	if ( BIO_read( bio, *buffer, *buffer_len ) < *buffer_len ) {
		free( *buffer );
		return FALSE;
	}

	return TRUE;
}

#endif /* defined(HAVE_EXT_GLOBUS) */

int
x509_send_delegation( const char *source_file,
					  int (*recv_data_func)(void *, void **, size_t *), 
					  void *recv_data_ptr,
					  int (*send_data_func)(void *, void *, size_t),
					  void *send_data_ptr,
					  unsigned char *policy, int policy_nid )
{
#if !defined(HAVE_EXT_GLOBUS)

	_globus_error_message = "This version of Condor doesn't support X509 credentials!" ;
	return -1;

#else
	int rc = 0;
	int error_line = 0;
	globus_result_t result = GLOBUS_SUCCESS;
	globus_gsi_cred_handle_t source_cred =  NULL;
	globus_gsi_proxy_handle_t new_proxy = NULL;
	char *buffer = NULL;
	int buffer_len = 0;
	BIO *bio = NULL;
	X509 *cert = NULL;
	STACK_OF(X509) *cert_chain = NULL;
	int idx = 0;
	globus_gsi_cert_utils_cert_type_t cert_type;

	if ( activate_globus_gsi() != 0 ) {
		return -1;
	}

	result = globus_gsi_cred_handle_init( &source_cred, NULL );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	result = globus_gsi_proxy_handle_init( &new_proxy, NULL );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	result = globus_gsi_cred_read_proxy( source_cred, source_file );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	if ( recv_data_func( recv_data_ptr, &buffer, &buffer_len ) != 0 ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	if ( buffer_to_bio( buffer, buffer_len, &bio ) == FALSE ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	free( buffer );
	buffer = NULL;

	result = globus_gsi_proxy_inquire_req( new_proxy, bio );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	BIO_free( bio );
	bio = NULL;

		// modify certificate properties
		// set the appropriate proxy type
	result = globus_gsi_cred_get_cert_type( source_cred, &cert_type );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}
	switch ( cert_type ) {
	case GLOBUS_GSI_CERT_UTILS_TYPE_CA:
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	case GLOBUS_GSI_CERT_UTILS_TYPE_EEC:
	case GLOBUS_GSI_CERT_UTILS_TYPE_GSI_3_INDEPENDENT_PROXY:
	case GLOBUS_GSI_CERT_UTILS_TYPE_GSI_3_RESTRICTED_PROXY:
		cert_type = GLOBUS_GSI_CERT_UTILS_TYPE_GSI_3_IMPERSONATION_PROXY;
		break;
	case GLOBUS_GSI_CERT_UTILS_TYPE_RFC_INDEPENDENT_PROXY:
	case GLOBUS_GSI_CERT_UTILS_TYPE_RFC_RESTRICTED_PROXY:
		cert_type = GLOBUS_GSI_CERT_UTILS_TYPE_RFC_IMPERSONATION_PROXY;
		break;
	case GLOBUS_GSI_CERT_UTILS_TYPE_GSI_3_IMPERSONATION_PROXY:
	case GLOBUS_GSI_CERT_UTILS_TYPE_GSI_3_LIMITED_PROXY:
	case GLOBUS_GSI_CERT_UTILS_TYPE_GSI_2_PROXY:
	case GLOBUS_GSI_CERT_UTILS_TYPE_GSI_2_LIMITED_PROXY:
	case GLOBUS_GSI_CERT_UTILS_TYPE_RFC_IMPERSONATION_PROXY:
	case GLOBUS_GSI_CERT_UTILS_TYPE_RFC_LIMITED_PROXY:
	default:
			// Use the same certificate type
		break;
	}
	if( policy != NULL ) {
		cert_type = GLOBUS_GSI_CERT_UTILS_TYPE_RFC_RESTRICTED_PROXY;
	}

	result = globus_gsi_proxy_handle_set_type( new_proxy, cert_type );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	if( policy != NULL ) {
		result = globus_gsi_proxy_handle_set_policy( new_proxy,
													 policy,
													 strlen(policy),
													 policy_nid );
		if( result != GLOBUS_SUCCESS ) {
			rc = -1;
			error_line = __LINE__;
			goto cleanup;
		}
	}

	/* TODO Do we have to destroy and re-create bio, or can we reuse it? */
	bio = BIO_new( BIO_s_mem() );
	if ( bio == NULL ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	result = globus_gsi_proxy_sign_req( new_proxy, source_cred, bio );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

		// Now we need to stuff the certificate chain into in the bio.
		// This consists of the signed certificate and its whole chain.
	result = globus_gsi_cred_get_cert( source_cred, &cert );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}
	i2d_X509_bio( bio, cert );
	X509_free( cert );
	cert = NULL;

	result = globus_gsi_cred_get_cert_chain( source_cred, &cert_chain );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	for( idx = 0; idx < sk_X509_num( cert_chain ); idx++ ) {
		X509 *next_cert;
		next_cert = sk_X509_value( cert_chain, idx );
		i2d_X509_bio( bio, next_cert );
	}
	sk_X509_pop_free( cert_chain, X509_free );
	cert_chain = NULL;

	if ( bio_to_buffer( bio, &buffer, &buffer_len ) == FALSE ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	if ( send_data_func( send_data_ptr, buffer, buffer_len ) != 0 ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

 cleanup:
	/* TODO Extract Globus error message if result isn't GLOBUS_SUCCESS */
	if ( error_line ) {
		char buff[1024];
		snprintf( buff, sizeof(buff), "x509_send_delegation failed at line %d",
				  error_line );
		set_error_string( buff );
	}

	if ( bio ) {
		BIO_free( bio );
	}
	if ( buffer ) {
		free( buffer );
	}
	if ( new_proxy ) {
		globus_gsi_proxy_handle_destroy( new_proxy );
	}
	if ( source_cred ) {
		globus_gsi_cred_handle_destroy( source_cred );
	}
	if ( cert ) {
		X509_free( cert );
	}
	if ( cert_chain ) {
		sk_X509_pop_free( cert_chain, X509_free );
	}

	return rc;
#endif
}


int
x509_receive_delegation( const char *destination_file,
						 int (*recv_data_func)(void *, void **, size_t *), 
						 void *recv_data_ptr,
						 int (*send_data_func)(void *, void *, size_t),
						 void *send_data_ptr )
{
#if !defined(HAVE_EXT_GLOBUS)

	_globus_error_message = "This version of Condor doesn't support X509 credentials!" ;
	return -1;

#else
	int rc = 0;
	int error_line = 0;
	globus_result_t result = GLOBUS_SUCCESS;
	globus_gsi_cred_handle_t proxy_handle =  NULL;
	globus_gsi_proxy_handle_t request_handle = NULL;
	char *buffer = NULL;
	int buffer_len = 0;
	BIO *bio = NULL;

	if ( activate_globus_gsi() != 0 ) {
		return -1;
	}

	result = globus_gsi_proxy_handle_init( &request_handle, NULL );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	bio = BIO_new( BIO_s_mem() );
	if ( bio == NULL ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	result = globus_gsi_proxy_create_req( request_handle, bio );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}


	if ( bio_to_buffer( bio, &buffer, &buffer_len ) == FALSE ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	BIO_free( bio );
	bio = NULL;

	if ( send_data_func( send_data_ptr, buffer, buffer_len ) != 0 ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	free( buffer );
	buffer = NULL;

	if ( recv_data_func( recv_data_ptr, &buffer, &buffer_len ) != 0 ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	if ( buffer_to_bio( buffer, buffer_len, &bio ) == FALSE ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	result = globus_gsi_proxy_assemble_cred( request_handle, &proxy_handle,
											 bio );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

	/* globus_gsi_cred_write_proxy() declares its second argument non-const,
	 * but never modifies it. The cast gets rid of compiler warnings.
	 */
	result = globus_gsi_cred_write_proxy( proxy_handle, (char *)destination_file );
	if ( result != GLOBUS_SUCCESS ) {
		rc = -1;
		error_line = __LINE__;
		goto cleanup;
	}

 cleanup:
	/* TODO Extract Globus error message if result isn't GLOBUS_SUCCESS */
	if ( error_line ) {
		char buff[1024];
		snprintf( buff, sizeof(buff), "x509_receive_delegation failed "
				  "at line %d", error_line );
		set_error_string( buff );
	}

	if ( bio ) {
		BIO_free( bio );
	}
	if ( buffer ) {
		free( buffer );
	}
	if ( request_handle ) {
		globus_gsi_proxy_handle_destroy( request_handle );
	}
	if ( proxy_handle ) {
		globus_gsi_cred_handle_destroy( proxy_handle );
	}

	return rc;
#endif
}


void parse_resource_manager_string( const char *string, char **host,
									char **port, char **service,
									char **subject )
{
	char *p;
	char *q;
	size_t len = strlen( string );

	char *my_host = (char *)calloc( len+1, sizeof(char) );
	char *my_port = (char *)calloc( len+1, sizeof(char) );
	char *my_service = (char *)calloc( len+1, sizeof(char) );
	char *my_subject = (char *)calloc( len+1, sizeof(char) );

	p = my_host;
	q = my_host;

	while ( *string != '\0' ) {
		if ( *string == ':' ) {
			if ( q == my_host ) {
				p = my_port;
				q = my_port;
				string++;
			} else if ( q == my_port || q == my_service ) {
				p = my_subject;
				q = my_subject;
				string++;
			} else {
				*(p++) = *(string++);
			}
		} else if ( *string == '/' ) {
			if ( q == my_host || q == my_port ) {
				p = my_service;
				q = my_service;
				string++;
			} else {
				*(p++) = *(string++);
			}
		} else {
			*(p++) = *(string++);
		}
	}

	if ( host != NULL ) {
		*host = my_host;
	} else {
		free( my_host );
	}

	if ( port != NULL ) {
		*port = my_port;
	} else {
		free( my_port );
	}

	if ( service != NULL ) {
		*service = my_service;
	} else {
		free( my_service );
	}

	if ( subject != NULL ) {
		*subject = my_subject;
	} else {
		free( my_subject );
	}
}

/* Returns true (non-0) if path looks like an URL that Globus
   (specifically, globus-url-copy) can handle

   Expected use: is the input/stdout file actually a Globus URL
   that we can just hand off to Globus instead of a local file
   that we need to rewrite as a Globus URL.

   Probably doesn't make sense to use if universe != globus
*/
int
is_globus_friendly_url(const char * path)
{
	if(path == 0)
		return 0;
	// Should this be more aggressive and allow anything with ://?
	return 
		strstr(path, "http://") == path ||
		strstr(path, "https://") == path ||
		strstr(path, "ftp://") == path ||
		strstr(path, "gsiftp://") == path ||
		0;
}

#if 0 /* We're not currently using these functions */

/*
 * Function: simple_query_ldap()
 *
 * Connect to the ldap server, and return all the value of the
 * fields "attribute" contained in the entries maching a search string.
 *
 * Parameters:
 *     attribute -     field for which we want the list of value returned.
 *     maximum -       maximum number of strings returned in the list.
 *     search_string - Search string to use to select the entry for which
 *                     we will return the values of the field attribute.
 * 
 * Returns:
 *     a list of strings containing the result.
 *     
 */ 
#if defined(GLOBUS_SUPPORT) && defined(HAS_LDAP)
static
int
retrieve_attr_values(
    LDAP *            ldap_server,
    LDAPMessage *     reply,
    char *            attribute,
    int               maximum,
    char *            search_string,
    globus_list_t **  value_list)
{
	LDAPMessage *entry;
	int cnt = 0;
    
	for ( entry = ldap_first_entry(ldap_server, reply);
		  (entry) && (maximum);
		  entry = ldap_next_entry(ldap_server, entry) ) {
		char *attr_value;
		char *a;
		BerElement *ber;
		int numValues;
		char **values;
		int i;

		for (a = ldap_first_attribute(ldap_server,entry,&ber);
			 a; a = ldap_next_attribute(ldap_server,entry,ber) ) {
			if (strcmp(a, attribute) == 0) {
				/* got our match, so copy and return it*/
				cnt++;
				values = ldap_get_values(ldap_server,entry,a);
				numValues = ldap_count_values(values);
				for (i=0; i<numValues; i++) {
					attr_value = strdup(values[i]);
					globus_list_insert(value_list,attr_value);
					if (--maximum==0)
						break;
				}
				ldap_value_free(values);
		
				/* we never have 2 time the same attibute for the same entry,
				   steveF said this is not always the case XXX */
				break;
			}
		}
	}
	return cnt;

} /* retrieve_attr_values */
#endif /* defined(GLOBUS_SUPPORT) */

#if defined(GLOBUS_SUPPORT) && defined(HAS_LDAP)
/*
static
void
set_ld_timeout(LDAP  *ld, int timeout_val, char *env_string)
{
	if(timeout_val > 0) {
		ld->ld_timeout=timeout_val;
        } else { 
		char *tmp=getenv(env_string);
		int tmp_int=0;
		if(tmp) tmp_int=atoi(tmp);
		if(tmp_int>0) ld->ld_timeout=tmp_int;
	}
}
*/
#endif /* defined(GLOBUS_SUPPORT) */

#if defined(GLOBUS_SUPPORT) && defined(HAS_LDAP)
static
int
simple_query_ldap(
    char *            attribute,
    int               maximum,
    char *            search_string,
    globus_list_t **  value_list,
	char *            server,
	int               port,
	char *            base_dn)
{
	LDAP *ldap_server;
	LDAPMessage *reply;
	LDAPMessage *entry;
	char *attrs[3];
	int rc;
	int match, msgidp;
    struct timeval timeout;

	*value_list = GLOBUS_NULL;
	match=0;
    
	/* connect to the ldap server */
	if((ldap_server = ldap_open(server, port)) == GLOBUS_NULL) {
		ldap_perror(ldap_server, "rsl_assist:ldap_open");
		free(server);
		free(base_dn);
		return -1;
	}
	free(server);

    // OpenLDAP 2 changed API
	//set_ld_timeout(ldap_server, 0, "GRID_INFO_TIMEOUT");

	/* bind anonymously (we can only read public records now */
	if(ldap_simple_bind_s(ldap_server, "", "") != LDAP_SUCCESS) {
		ldap_perror(ldap_server, "rsl_assist:ldap_simple_bind_s");
		ldap_unbind(ldap_server);
		free(base_dn);
		return -1;
	}
    
	/* I should verify the attribute is a valid string...     */
	/* the function allows only one attribute to be returned  */
	attrs[0] = attribute;
	attrs[1] = GLOBUS_NULL;
    
	/* do a search of the entire ldap tree,
	 * and return the desired attribute
	 */
    // OpenLDAP 2 changed the API. ldap_search is deprecated

    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;
    if ( rc = ldap_search_ext( ldap_server, base_dn, LDAP_SCOPE_SUBTREE,
                                  search_string, attrs, 0, NULL, 
                                  NULL, &timeout, -1, &msgidp ) == -1 ) {
		ldap_perror( ldap_server, "rsl_assist:ldap_search" );
		return( msgidp );
	}

//	if ( ldap_search( ldap_server, base_dn, LDAP_SCOPE_SUBTREE,
//					  search_string, attrs, 0 ) == -1 ) {
//		ldap_perror( ldap_server, "rsl_assist:ldap_search" );
//		return( ldap_server->ld_errno );
//	}

	while ( (rc = ldap_result( ldap_server, LDAP_RES_ANY, 0, NULL, &reply ))
			== LDAP_RES_SEARCH_ENTRY || (rc==-1)) {
		if(rc==-1) {       // This is timeout
			continue;
		}
		match += retrieve_attr_values(ldap_server,reply, attrs[0],
									  maximum, search_string, value_list);
	}

	if( rc == -1 ) {
		ldap_perror(ldap_server, "rsl_assist:ldap_search");
	}
	/* to avoid a leak in ldap code */
	ldap_msgfree(reply);

	/* disconnect from the server */
	ldap_unbind(ldap_server);
	free(base_dn);

	if(match)
		return GLOBUS_SUCCESS;
	else
		return rc;

} /* simple_query_ldap() */
#endif /* defined(GLOBUS_SUPPORT) */


int
check_globus_rm_contacts(char* resource)
{
#if !defined(GLOBUS_SUPPORT) || !defined(HAS_LDAP)
	fprintf( stderr, "This is not a Globus-enabled version of Condor!\n" );
	exit( 1 );
	return 0;
#else

	int rc;
	int len;
	char *host;
	char *ptr;
	char *ptr2;
	char *service;
	char *search_string;
	char *format = "(&(objectclass=GlobusService)(hn=%s*)(service=%s))";
	globus_list_t *rm_contact_list = GLOBUS_NULL;

	// Pick out the hostname of the resource
	// It's at the beginning of the resource string
	len = strcspn( resource, ":/" );
	host = (char *)malloc( (len + 1) * sizeof(char) );
	strncpy( host, resource, len );
	host[len] = '\0';

	// Pick out the servicename of the resource, if it's there
	// It'll be after the first '/' character in the string unless two
	// colons precede it (confusing, eh?)
	ptr = strchr( resource, '/' );
	ptr2 = strchr( resource, ':' );
	if ( ptr2 != NULL )
		ptr2 = strchr( ptr2 + 1, ':' );
	if ( ptr != NULL && ( ptr2 == NULL || ptr2 > ptr ) ) {
		ptr++;
		len = strcspn( ptr, ":" );
		service = (char *)malloc( (len + 1) * sizeof(char) );
		strncpy( service, ptr, len );
		service[len] = '\0';
	} else {
		service = strdup( "*" );
	}

	search_string = (char *)malloc( strlen(format) + 
									strlen(host)   +
									strlen(service) + 1 );
	if ( !search_string ) {
		free( service );
		free( host );
		return 1;
	}

	sprintf( search_string, format, host, service );

	rc = simple_query_ldap( "contact", 1, search_string, &rm_contact_list,
							host, 2135, "o=Grid" );

	free( search_string );
	free( service );
	free( host );

	if ( rc == GLOBUS_SUCCESS && rm_contact_list ) {
		globus_list_remove( &rm_contact_list, rm_contact_list );
		return 0;
	} else {
		return 1;
	}

#endif /* !defined(GLOBUS_SUPPORT) */

} /* check_globus_rm_contact() */


#endif /* 0 */

