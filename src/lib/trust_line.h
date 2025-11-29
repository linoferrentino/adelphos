/*
 *
 * This is the trust line module.
 *
 * A trust line is a connection between two users in adelphos.
 *
 * It can be a part of a travel of money or of an item
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 
 *
 *
 *
 */

#ifndef _trust_line_h_
#define _trust_line_h_

#include "adelphos.h"

/* the usual CRUD operations on trust lines. */

/* every trust line has an handle and can be edited, removed, etc.  */


/*
 *
 * Creates a trust line.
 *
 * I create a trust line between two users and a security context
 *
 * (for example an administrator can create a trust line betwee
 * two independent users against their will)
 *
 * The trust line is created in the initial state.
 *
 * Returns a handle in case of success.
 *
 * */
ad_hndl_t ad_tl_create(ad_sec_ctx_t ctx, ad_hndl_t user_1,
		ad_hndl_t user_2);

/* the type of the callback that gets all the trust lines to
 * or from a given user.*/
typedef ad_res (ad_enum_tl_cb_t)(ad_hndl_t a_tl, void *param);

/* The list of the trust lines is given for a user, of course
 * a user can see his trust lines, the administrato can
 * see the trust lines for all the users.  */
ad_res ad_tl_enum_from(ad_sec_ctx_t ctx, 
		ad_hndl_t user_1, ad_enum_tl_cb_t a_cb,
		void *param);

/* This enums all the trust lines that goes to a certain user.*/
ad_res ad_tl_enum_to(ad_sec_ctx_t ctx, 
		ad_hndl_t user_1, ad_enum_tl_cb_t a_cb,
		void *param);



#endif
