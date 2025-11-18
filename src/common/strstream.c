/*
 *
 *
 * strstream.c
 *
 *
 * implementation file of the string stream.
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 * 
 *
 */

#include <stdlib.h>
#include <string.h>

#include "strstream.h"

#define _LOG_MODULE "strstream"
#define MODULE_LEV ML_INFO
#include "alog.h"

/*LOG_MODULE_IMP;*/


struct strstream_s {
    char  *pc;
    size_t len;
    size_t cur;
};

void strstream_reset(struct strstream_s *strs)
{
        /* Just reset the counter. No deallocation is made */
        strs->cur = 0;
}

int strstream_emitc(struct strstream_s *strs, char c)
{
        if (strs->cur == strs->len){
		strs->len = strs->len * 1.5 + 20;
		strs->len -= strs->len % 16;
		alogx("Allocating %zd bytes", strs->len);
		strs->pc = (char*)realloc(strs->pc, strs->len);
	}
	alogx("putting pc[%zd] = %c (%d)", strs->cur, c, c);
	strs->pc[strs->cur++] = c;
	return 0;
}

static void _strstream_ensure_put(struct strstream_s *strs, size_t count)
{
	if (strs->cur + count < strs->len) {
		return;
	}
	/* we must grow */
	strs->len = (strs->cur + count) + (count / 2) + 30;
	strs->len -= strs->len % 16;
	alogt("Realloc to %zd", strs->len);
	strs->pc = (char*)realloc(strs->pc, strs->len);
}


int strstream_emitmem(struct strstream_s *strs, char *c, size_t count)
{
	_strstream_ensure_put(strs, count);
	memcpy(&strs->pc[strs->cur], c, count);
	strs->cur += count;
        return 0;
}

int strstream_emitstr(struct strstream_s *strs, char *c)
{
	size_t lens = strlen(c);
	/* you do not put the \0 */
	strstream_emitmem(strs, c, lens);
        return 0;
}



void strstream_init(struct strstream_s **strs)
{
        struct strstream_s *tmp = (struct strstream_s*)
                calloc(1, sizeof(struct strstream_s));
        *strs = tmp;
}

void strstream_free(struct strstream_s *strs)
{
        if (strs == NULL)
                return;
        free(strs->pc);
        free(strs);
}

void strstream_getstr_grab_destroy(struct strstream_s *strs,
                char **str, size_t *len)
{
        if (strs == NULL) {
                *str = NULL;
                *len = 0;
                return;
        }
        strs->pc[strs->cur] = '\0';
        *str = strs->pc;
        *len = strs->cur;
        free(strs);
}

char* strstream_getstr(struct strstream_s *strs)
{
        strs->pc[strs->cur] = '\0';
        return strs->pc;
}


size_t strstream_getsz(struct strstream_s *strs)
{
        return strs->cur;
}

