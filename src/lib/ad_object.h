/*
 *
 *
 * object
 *
 * The main declaration file for objects in adelphos.
 *
 * In adelphos everything is an object composed or that
 * has relationship to other objects.
 *
 * In the backend, really back, all is transactional and
 * converted in rows and tables, but this is not
 * a requirement.
 *
 * adelphos could be (in theory) also implemented in a
 * object oriented database or a NO/SQL database, as long
 * as it has transaction support.
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 
 *
 *
 */


/*
 * Every object in adelphos has an handle, a non negative integer, which 
 * can be thought as the id in the database.
 *
 * Some objects are not visibile from a certain user.
 *
 * Some objects are visibile with limited permissions.
 *
 * The way in which an object is taken is decided in the database.
 *
 * for example there are methods like list_dir gets the objects in a
 * particular context, like a group
 *
 * But if I want a particular object I need to pass a security context
 * handle which is basically the authentication token of the user.
 *
 *
 * The handle is temporary, it can change, but the id is fixed for
 * the lifetime of the object.
 *
 *
 *
 * */

/* the security context, this is used by the db */
typedef uint64_t ad_sec_ctx_t;

/* This is the handle of an object in adelphos.
 *
 * */
typedef uintptr_t ad_hndl_t;

/* The invalid handle. */
#define AD_NULL ((uintptr_t)NULL)

/* every object has an id. This is the method to get it */

typedef uint64_t ad_id_t;

#define ADELPHOS_INVALID_ID ((ad_id_t)(unsigned)-1)

/* this simple function converts the handle to the id 
 *
 * returns ADELPHOS_INVALID_ID if this is not existent (or garbage)
 *
 * */
ad_id_t adob_get_id(ad_hndl_t a_handle);

