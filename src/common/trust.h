/*
 *
 * trust
 *
 * declaration of functions relative to the trust
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 *
 */

#ifndef _trust_h_
#define _trust_h_

#include <stdint.h>

/* converts a credit limit from string in decibel */
int trust_cl_s_to_db(char *cl_s, double *res);

/* converts a decibel trust in a credit limit in float */
int64_t trust_db_to_cl_i(double db);

#endif
