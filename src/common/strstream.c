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


/*
struct strstream_s {
    char  *pc;
    uint32_t len;
    uint32_t cur;
};
*/

void strstream_reset(strstream_s *strs)
{
        /* Just reset the counter. No deallocation is made */
        strs->bsz->cur = 0;
}

static void _strstream_grow(strstream_s *strs, uint32_t amount) 
{
	strs->len = (strs->bsz->cur + amount) + (amount / 2) + 30;
	strs->len -= strs->len % 16;
	alogt("Realloc to %zd", strs->len);
	strs->arena = (uint8_t*)realloc(strs->arena, strs->len);
	ok_or_die(strs->arena != NULL);
}

int strstream_emitc(strstream_s *strs, char c)
{
        if (strs->bsz->cur == strs->len){
		/* We grow some characters at a time */
		_strstream_grow(strs, 8);
	}
	alogx("putting pc[%zd] = %c (%d)", strs->cur, c, c);
	strs->bsz->buf[strs->bsz->cur++] = c;
	return 0;
}

static void _strstream_ensure_put(strstream_s *strs, size_t count)
{
	uint32_t new_last_idx = strs->bsz->cur + count;
	if (new_last_idx <= strs->len) {
		return;
	}
	/* we must grow */
	_strstream_grow(strs, new_last_idx - strs->len);
	
}


int strstream_emitmem(strstream_s *strs, char *c, uint32_t count)
{
	_strstream_ensure_put(strs, count);
	memcpy(&strs->arena[strs->bsz->cur], c, count);
	strs->bsz->cur += count;
        return 0;
}

int strstream_emitstr(strstream_s *strs, char *c)
{
	size_t lens = strlen(c);
	/* you do not put the \0 */
	strstream_emitmem(strs, c, lens);
        return 0;
}



void strstream_init(strstream_s **strs)
{
        strstream_s *tmp = (strstream_s*)
                calloc(1, sizeof(strstream_s));
        *strs = tmp;
}

void strstream_free(strstream_s *strs)
{
        if (strs == NULL)
                return;
        free(strs->arena);
        free(strs);
}

void strstream_getstr_grab_destroy(strstream_s *strs,
                char **str, uint32_t *len)
{
        if (strs == NULL) {
                *str = NULL;
                *len = 0;
                return;
        }

	_strstream_ensure_put(strs, 1);

        strs->bsz->buf[strs->bsz->cur] = '\0';
        *str = (char*)strs->bsz->buf;
	/* do not advance the length */
        *len = strs->bsz->cur;
        free(strs);
}

char* strstream_getstr(strstream_s *strs)
{
	_strstream_ensure_put(strs, 1);
        strs->bsz->buf[strs->bsz->cur] = '\0';
        return (char*)strs->bsz->buf;
}


uint32_t strstream_getsz(strstream_s *strs)
{
        return strs->bsz->cur;
}

