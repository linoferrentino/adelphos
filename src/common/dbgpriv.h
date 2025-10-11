
/*
 *
 *
 * internal debug functions for adelphos
 *
 *  Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 */


#ifndef __dbg_priv_h
#define __dbg_priv_h

#include <stdio.h>


#define ERR(S) do { fprintf(stderr, "%d %s\n", __LINE__,  S );\
       	abort();} while(0)

#ifdef DBG_PRIV_ENABLED

#ifndef DBG_PRIV_MOD
#define DBG_PRIV_MOD "?PRIV_MOD?:"
#endif

#define DBG(S,...) do { fprintf(stderr, "%.10s:%.4d\t" S "\n", DBG_PRIV_MOD,  __LINE__,\
	##__VA_ARGS__ ); } while(0)

#else

#define DBG(S,...) do { } while(0)

#endif

#endif
