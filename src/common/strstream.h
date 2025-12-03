/*
 *
 * strstream.h
 *
 * declaration for a stream backed up by a string.
 *
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 *
 */

#ifndef _STRSTREAM_H_
#define _STRSTREAM_H_

#include <stdint.h>
#include "misc.h"


/* opaque structure that holds a string stream */
typedef struct byte_buf_s strstream_s;

/* creates a string stream, initially with length zero  */
void strstream_init(strstream_s **strs);

/* deletes the string stream */
void strstream_free(strstream_s *strs);

/* Emits the character c onto the stream */
int strstream_emitc(strstream_s *strs, char c);

/* emits some bytes */
int strstream_emitmem(strstream_s *strs, char *c, uint32_t count);

/* emits a null terminated string*/
int strstream_emitstr(strstream_s *strs, char *c);

/* Gets the string which is for now built: it appends a NULL at the end */
char* strstream_getstr(strstream_s *strs);

/* Resets the cursor in this string stream, but it does not 
 * free the memory. Useful to reuse this stream object */
void strstream_reset(strstream_s *strs);

/* 
 * gets the string and grabs the pointer, you
 * will have to deallocate it after.
 *
 * the object is deallocated. */
void strstream_getstr_grab_destroy(strstream_s *strs,
                char **str, uint32_t *len);

/* gets the length of the string which has been built.*/
uint32_t strstream_getsz(strstream_s *strs);


/*
 * the strstream is a byte  buffer.
struct byte_buf_s *strstream_as_byte_buf();
*/


#endif
