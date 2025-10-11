/*
 *
 * miscellanea.
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>


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


