/*
 *
 *
 * trust
 *
 * implementation of functions relative to the trust
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 *
 */

#include <stdio.h>
#include <math.h>
#include "trust.h"

#define _LOG_MODULE "trust"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;


int trust_cl_s_to_db(char *cl_s, double *res)
{
	/* let's try to convert the credit level to a number */
	/* a negative value is impossible */
	intmax_t cli;
	int resi;
	resi = sscanf(cl_s, "%jd", &cli);

	if (resi != 1) {
		alogw("error in conversion %s", cl_s);
		return -1;
	}	

	if (cli <= 0) {
		alogw("cannot have a trust for zero or negative");
		return -1;
	}

	*res = 10.0 * log10((double)cli);

	alogt("trust %s corresponds to %ju - %g in decibel", cl_s, 
			cli, *res);

	return 0;

}
