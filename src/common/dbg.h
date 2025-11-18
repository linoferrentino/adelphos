/*
 *
 * Adelphos, debug utilities
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 */
#ifndef _dbg_h
#define _dbg_h

#include <errno.h>


#ifndef NDEBUG

#define ok_or_goto_fail(X) if (!(X)) {fprintf(stderr, "fail @" __FILE__ ":%d (errno %d)\n", __LINE__, errno);goto fail;}

#define ad_assert(__x__)  do {if (!(__x__)){fprintf(stderr, "ops! -->\n\t\"" #__x__ "\" is false. \n\t" __FILE__ ":%d\n", __LINE__); \
   abort();}} while(0)

#define upanic_m(X,...) { \
        int now_errno = errno;\
        fprintf(stderr, "Anomaly(%s:%d) errno %d: " X "\n", \
                        __FILE__, __LINE__,  now_errno, ##__VA_ARGS__); abort();}

#else

#define ok_or_goto_fail(X) if (!(X)) {goto fail;}

#define ad_assert(__x__)  do {} while(0)

#define upanic_m(X,...) {syslog(LOG_CRIT, "Anomaly(%s:%d) errno %d: " X,\
                __FILE__, __LINE__, errno, ##__VA_ARGS__); abort();}


#endif

#endif


