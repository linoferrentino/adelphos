/*
 *
 *
 * this is the authorization middleware
 *
 * its purpouse is to check the request and to
 * know if it is appropriate or not.
 *
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 * 
 *
 */


#ifndef _auth_ml_h
#define _auth_ml_h


/* I have a function that checks the permission of an object in adelphos
 * based on the role of the user.*/

/* the security context, this is used by the db */
typedef uint64_t ad_sec_ctx_t;


/* this is a token that gives to us the identifier for a user's
 * session in adelphos.*/
typedef uint64_t ad_session_id_t;


#endif
