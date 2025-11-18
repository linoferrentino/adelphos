/* 
 *
 * units.c
 *
 * definitions for the functions that parse / unparse the sizes
 * in human readable strings.
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 */

#include <stdio.h>
#include <strings.h>

#include "units.h"

#define _LOG_MODULE "units"
#define MODULE_LEV ML_INFO
#include "alog.h"

/*
LOG_MODULE_IMP;
*/


/* Size suffixes */
struct size_suffix {
	const char  *suffix;
	int         bits;
};

static const struct size_suffix size_suffixes[] = {
	{
		.suffix=    "k",
		.bits=      10
	},
	{
		.suffix=    "m",
		.bits=      20
	},
	{
		.suffix=    "g",
		.bits=      30
	},
	{
		.suffix=    "t",
		.bits=      40
	},
	{
		.suffix=    "p",
		.bits=      50
	},
	{
		.suffix=    "e",
		.bits=      60
	},
	{
		.suffix=    "z",
		.bits=      70
	},
	{
		.suffix=    "y",
		.bits=      80
	},
};



int u_parse_size_string_approx(const char *s, uintmax_t *valp)
{
	char suffix[3] = { '\0' };
	int nconv;
	float vald;

	*valp = 1;
	nconv = sscanf(s, "%f%2s", &vald, suffix);
	alogt("Scan %s result %d val %f suffix %s",
			s, nconv, vald, suffix);

	if (nconv < 1)
		return -1;

	int found = 0;
	if (nconv >= 2) {

		int i;

		for (i = 0; i < sizeof(size_suffixes) / 
				sizeof(*size_suffixes); i++) {
			const struct size_suffix *const ss = 
				&size_suffixes[i];

			if (ss->bits >= sizeof(off_t) * 8)
				break;
			if (strcasecmp(suffix, ss->suffix) == 0) {
				*valp <<= ss->bits;
				found = 1;
				break;
			}
		}
		if (!found)
			return -1;

	} else {
		/* no units */
		found = 1;
	}

	if (found) {
		*valp = (vald) * (*valp);
		alogt("Will return %jd", *valp);
		return 0;
	}

	return -1;

}

int u_parse_size_string(const char *s, uintmax_t *valp)
{
	char suffix[3] = { '\0' };
	int nconv;

	nconv = sscanf(s, "%ju%2s", valp, suffix);

	if (nconv < 1)
		return -1;

	if (nconv >= 2) {

		int found = 0;
		int i;

		for (i = 0; i < sizeof(size_suffixes) / 
				sizeof(*size_suffixes); i++) {
			const struct size_suffix *const ss = 
				&size_suffixes[i];

			if (ss->bits >= sizeof(off_t) * 8)
				break;
			if (strcasecmp(suffix, ss->suffix) == 0) {
				*valp <<= ss->bits;
				found = 1;
				break;
			}
		}
		if (!found)
			return -1;
	}
	return 0;
}

char *u_unparse_approx(uintmax_t value)
{
	static char sizestr[10];
	u_unparse_size_string_approx(sizestr, 10, value);
	return sizestr;
}

void u_unparse_size_string_approx(char *buf, size_t bmax, uintmax_t value)
{

	uintmax_t unit;
	int i;

	if (value == 0) {
		snprintf(buf, bmax, "0");
		return;
	}
	for (i = sizeof(size_suffixes) 
			/ sizeof(*size_suffixes); i-- > 0; ) {

		const struct size_suffix *const ss = &size_suffixes[i];

		if (ss->bits >= sizeof(off_t) * 8)
			continue;

		unit = (uintmax_t)1 << ss->bits;

		if (value > unit) {
			snprintf(buf, bmax, "%.4g%s", 
					(double)value / (double)unit, 
					ss->suffix);
			return;
		}
	}
	snprintf(buf, bmax, "%ju", value);
}

void u_unparse_size_string(char *buf, size_t bmax, uintmax_t value)
{

	uintmax_t unit;
	int i;

	if (value == 0) {
		snprintf(buf, bmax, "0");
		return;
	}

	for (i = sizeof(size_suffixes) / 
			sizeof(*size_suffixes); i-- > 0; ) {

		const struct size_suffix *const ss = &size_suffixes[i];

		if (ss->bits >= sizeof(off_t) * 8)
			continue;
		unit = (uintmax_t)1 << ss->bits;
		if (value % unit == 0) {
			snprintf(buf, bmax, "%ju%s", 
					value / unit, ss->suffix);
			return;
		}
	}
	snprintf(buf, bmax, "%ju", value);

}
