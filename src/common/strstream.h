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

#include <stddef.h>


/* opaque structure that holds a string stream */
struct strstream_s;

/* creates a string stream, initially with length zero  */
void strstream_init(struct strstream_s **strs);

/* deletes the string stream */
void strstream_free(struct strstream_s *strs);

/* Emits the character c onto the stream */
int strstream_emitc(struct strstream_s *strs, char c);

/* emits some bytes */
int strstream_emitmem(struct strstream_s *strs, char *c, size_t count);

/* emits a null terminated string*/
int strstream_emitstr(struct strstream_s *strs, char *c);

/* gets the string which is for now built */
char* strstream_getstr(struct strstream_s *strs);

/* Resets the cursor in this string stream, but it does not 
 * free the memory. Useful to reuse this stream object */
void strstream_reset(struct strstream_s *strs);

/* 
 * gets the string and grabs the pointer, you
 * will have to deallocate it after.
 *
 * the object is deallocated. */
void strstream_getstr_grab_destroy(struct strstream_s *strs,
                char **str, size_t *len);

/* gets the length of the string which has been built.*/
size_t strstream_getsz(struct strstream_s *strs);


#endif
