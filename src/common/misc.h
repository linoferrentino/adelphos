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


/* a structure used to hold */
struct byte_buf_s
{
	/* Important! cur is the first field in this structure */
	union{
		struct {

			/* the valid length of the buffer */
			uint32_t cur;
			/* this is the place for all the bytes.*/
			uint8_t  buf[0];
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

	/* the capacity of the buf, the arena will be 4 bytes longer */
	uint32_t len;

};

#pragma pack(pop)

/* utility macro, do not call it outside */
#define _byte_buf_init(_x, _l) do { \
	(_x)->arena = malloc(_l + sizeof((_x)->bsz->cur)); \
	ok_or_die((_x)->arena != NULL); \
	(_x)->len = _l + sizeof((_x)->bsz->cur);\
	(_x)->bsz->cur = 0; \
} while (0)


/* define a byte buffer */
#define byte_buf_decl(_x) struct byte_buf_s _x

#define byte_buf_def(_x) \
	byte_buf_decl((_x)); \
	_byte_buf_init(&_x, 0)

/* this function does NOT move the memory block, no realloc. */
#define byte_buf_wipe_ensure(_x, _l) do { \
	if ((_x)->len >= _l) { \
		break;\
	} \
	free((_x)->arena); \
	_byte_buf_init(_x, _l); \
} while(0)

#define byte_buf_def_n(x, n) \
	byte_buf_decl(x); \
	_byte_buf_init(&x, n)
	

/* Only free a buffer on the stack */
#define byte_buf_free_loc(x) free(x.arena)

#define byte_buf_free(x) if (x != NULL) { \
	free(x->arena); free(x); }

/* simply cast, we expect only utf8 string which are ASCII friendly. */
#define byte_buf_str(x) (char*)((x)->bsz->buf)

#define byte_buf_len(x) ((x)->bsz->cur)

/*
#define byte_buf_set(x, new_x, new_sz) (x->buf = (uint8_t*)new_x); \
			            x->sz = new_sz
				    */

#define byte_buf_reset(_x) (_x)->bsz->cur = 0

/* this will preserve the buffer */
#define byte_buf_grow(_x, _l) do { \
	(_x)->len = ((_x)->bsz->cur + _l) + (_l / 2) + 30; \
	(_x)->len -= (_x)->len % 16; \
	(_x)->arena = realloc((_x)->arena, (_x)->len);\
	ok_or_die((_x)->arena != NULL); \
} while (0)

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
int read_all(int fd, uint8_t *buf, uint32_t req_len);


/* here the buffer is preallocated. */
int write_all(int fd, const uint8_t *buf, uint32_t req_len);


#endif
