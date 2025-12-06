/*
 *
 * miscellanea.
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "misc.h"


void dump_payload_dbg(const char *file, int line, const unsigned char *_p, int len)
{
	fprintf(stderr, "Dump @ %s:%d %d bytes (%p)\n", file, line, len, _p);
	char buf[128];
	int i, j;
	const unsigned char *p = (const unsigned char *)_p;

	for (i = 0; i < len;) {
		memset(buf, ' ', sizeof(buf));
		int res = sprintf(buf, "%5d: ", i);
		buf[res] = ' ';
		for (j=0; j < 16 && i < len; i++, j++){

			int dec  = p[i] / 16;
			int unit = p[i] % 16;
			buf[7 + j*3] = dec < 10 ? '0' + dec : 'a' + (dec-10);
			buf[8 + j*3] = unit < 10 ? '0' + unit : 'a' + (unit-10);
			buf[56 + j] = isprint(p[i]) ? p[i] : '.';

		}
		buf[72] = '\0';
		fprintf(stderr, "%s\n", buf);
	}
}

int read_all(int fd, uint8_t *buf, uint32_t req_len)
{
	uint32_t read_cur = 0;

	if (req_len == 0) {
		return 0;
	}

	while(1) {
		read_cur = read(fd, buf, req_len);
		if (read_cur <= 0) {
			/* Only in case of EOF at FIRST byte  I return
			 * AD_EOF */
			if (read_cur == 0 && read_cur == 0) {
				return AD_EOF;
			}
			return -1;
		}

		req_len -= read_cur;
		if (req_len == 0) {
			return 0;
		}

		buf += read_cur;
	}

	return 0;
}


/* here the buffer is preallocated. */
int write_all(int fd, const uint8_t *buf, uint32_t req_len)
{
	uint32_t write_cur = 0;
	if (req_len == 0) {
		return 0;
	}

	while(1) {
		write_cur = write(fd, buf, req_len);
		if (write_cur <= 0) {
			return -1;
		}

		req_len -= write_cur;
		if (req_len == 0) {
			return 0;
		}

		buf += write_cur;
	}

	return 0;
}


