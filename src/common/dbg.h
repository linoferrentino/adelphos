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


#define ok_or_goto_fail(X) if (!(X)) {fprintf(stderr, "fail @" __FILE__ ":%d (errno %d)\n", __LINE__, errno);goto fail;}

#endif


