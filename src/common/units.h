/*
 *
 *
 * units.h
 *
 * declaration of utility and macro to parse and generate human friendly
 * names for numbers with suffixes
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 *
 */
#ifndef _UNITS_H_ 
#define _UNITS_H_

#include <stddef.h>
#include <stdint.h>

#define ONE_KB ( 1UL << 10 )
#define ONE_MB ( 1UL << 20 )
#define ONE_GB ( 1UL << 30 )
#define ONE_TB ( 1UL << 40 )
#define ONE_PT ( 1UL << 50 )


/* parses a human string like 5k in the value valp. */
int u_parse_size_string(const char *s, uintmax_t *valp);

int u_parse_size_string_approx(const char *s, uintmax_t *valp);

/* converts the machine value value in a human string, approximatively 
 *
 * bmax is the maximum bytes to use in but*/
void u_unparse_size_string_approx(char *buf, size_t bmax, uintmax_t value);

/* declares and creates a string which is automatically freed */
#define dtoha_d(_v, _c) char *_c;  \
	_c = alloca(10); \
	u_unparse_size_string_approx(_c, 10, _v)

/* creates a string which is automatically freed */
#define dtoha(_v, _c)  _c = alloca(10); \
	u_unparse_size_string_approx(_c, 10, _v)


/* it will use a static buffer so only suitable for one value at a time*/
char *u_unparse_approx(uintmax_t value);

/* converts the value in a human string, exactly. */
void u_unparse_size_string(char *buf, size_t bmax, uintmax_t value);

#endif
