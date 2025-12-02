/*
 *
 * miscellanea header
 *
 *
 */

#ifndef _misc_h_
#define _misc_h_

#include <stdint.h>
#include <string.h>


void dump_payload_dbg(const char *file, int line, const unsigned char *_p, int len);


#define dump_payload(p,l) dump_payload_dbg(__FILE__, __LINE__, p, l)



/* I/O functions to complete a buffer */


/*
 * We do not plan to use more than 32bits wide buffer.
 *
 *
 * If the content of it is textual it is encoded in utf-8
 * 
 *
 * */


#pragma pack(push, 1)

struct buf_size_s {
};

struct byte_buf_s
{
	union{
		struct {

			/* the valid length of the buffer */
			uint32_t valid_len;
			union {
				uint8_t  *buf;
				uint32_t *i32;
			};

		} *bsz;
		uint8_t *arena;
	};
	/* pointer aliasing to have a nice interface */
	/*
	union {
		uint8_t  *buf;
		uint32_t *i32;
	};
	*/

	/* size in bytes of the buf, the arena will be 4 bytes longer */
	uint32_t sz;

};
#pragma pack(pop)

/* define a byte buffer */
#define byte_buf_def(x) struct byte_buf_s x; memset(&x,0, sizeof(x))

#define byte_buf_def_n(x, n) struct byte_buf_s x; \
	x.buf = malloc(n); x.sz = n

#define byte_buf_free(x) free(x->buf)

/* simply cast, we expect only utf8 string which are ASCII friendly. */
#define byte_buf_str(x) (char*)((x)->buf)

#define byte_buf_set(x, new_x, new_sz) (x->buf = (uint8_t*)new_x); \
			            x->sz = new_sz

/* 
 *
 * I do not expect to have more than 4gb to read.
 *
 * buf is allocated, at the start, pass NULL and 0 to avail_len
 *
 * After the call avail_len will be the new size of the buffer
 *
 * req_len is the required len
 *
 * the return values are 0 OK, -1 error, errno gives the actual error
 *
 * */
int read_all(int fd, struct byte_buf_s *buf, uint32_t req_len);


/* here the buffer is preallocated. */
int write_all(int fd, const struct byte_buf_s *buf, uint32_t req_len);


#endif
