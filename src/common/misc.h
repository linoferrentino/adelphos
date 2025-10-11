/*
 *
 * miscellanea header
 *
 *
 */

#ifndef _misc_h_
#define _misc_h_


void dump_payload_dbg(const char *file, int line, const unsigned char *_p, int len);

#define dump_payload(p,l) dump_payload_dbg(__FILE__, __LINE__, p, l)

#endif
