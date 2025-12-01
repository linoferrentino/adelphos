/*
 *
 *
 *
 *
 *
 *
 */



/* The success return. */
#define AD_OK (0)

/* generic error (when nothing more specific can be given) */
#define AD_ERR (-1)


/* The last error which the library has encountered  */

typedef unsigned ad_err_t; 
extern ad_err_t ad_errno;

/* All the errors in adelphos are defined here. */
#define AD_WRONG_CREDENTIALS (1)
#define AD_EXPIRED_SESSION   (2)


/* these are OS Errors, probably we cannot do anything about them */
#define AD_NO_SPACE_LEFT     (10000)

/* this when there is a bug in the code */
#define AD_INTERNAL_SERVER_ERROR (99999)


/* 
 * gets a human like description of the error,
 *
 * please note that the string must not be freed or changed
 * as it might be statically allocated.
 *
 * */
const char* ad_strerror(ad_err_t error);
