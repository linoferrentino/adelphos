/*

  Hash implementation taken from s3backer project .

  I have added the possibility to grow the hash,
  and also to have keys of other types (string)

 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 
 */
/*
 * s3backer - FUSE-based single file backing store via Amazon S3
 *
 * Copyright 2008-2023 Archie L. Cobbs <archie.cobbs@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations including
 * the two.
 *
 * You must obey the GNU General Public License in all respects for all
 * of the code used other than OpenSSL. If you modify file(s) with this
 * exception, you may extend this exception to your version of the
 * file(s), but you are not obligated to do so. If you do not wish to do
 * so, delete this exception statement from your version. If you delete
 * this exception statement from all source files in the program, then
 * also delete it here.
 */


/*
 * Our hash table implementation.
 *
 * We make the following simplifying assumptions:
 *
 * 1.  Keys are of type s3b_block_t
 * 2.  Values are structures in which the first field is the key
 * 3.  No attempts will be made to overload the table
 *
 *
 * Lino's modification:
 *
 * the hash table can grow if needed.
 *
 * Keyes can be also strings.
 */




#include <sys/types.h> /* for size_t */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int u_int;

/* Definitions */
typedef void s3b_hash_visit_t(void *arg, void *value);

typedef uint32_t s3b_block_t;

/* Declarations */
struct s3b_hash;

/* s_hash is an hash table where the key is a string.
 * The hash holds whatever object, as long as the first object
 * is the key (a string) (a NULL terminated sequence of bytes) */
typedef struct s3b_hash s_hash_t;

/* the string hash is a dictionary with string as values */
typedef struct s3b_hash strhash_t;

/* The iterator type*/
typedef uint32_t s3b_hash_it;
typedef uint32_t s_hash_it;
typedef uint32_t strhash_it;
#define INVALID_ITERATOR 0xFFFFFFFF


/* s_hash functions */
#define s_hash_create s3b_hash_create
#define s_hash_destroy s3b_hash_destroy
#define s_hash_size s3b_hash_size

#define strhash_create s3b_hash_create
#define strhash_destroy s3b_hash_destroy
#define strhash_size s3b_hash_size

void s_hash_put_new(s_hash_t *hash, void *value);

/* puts a new pair key = value into the hash. It
 * is an error if the key is already present. */
void strhash_put_new(strhash_t *hash, const char *key, const char *value);

/* dumps the dictionary on stderr */
void strhash_dump(strhash_t *hash);

void* s_hash_get(s_hash_t *hash, const char *key);

/* gets the string pointed by the key. In theory... you
 * could modify the string returned as long as the length
 * remains below the threshold. */
const char* strhash_get(strhash_t *hash, const char *key);

/* Will clear the objects using the free function provided.*/
typedef void (free_func)(void*);
void s3b_hash_clear_ex(s_hash_t *hash, free_func *frfnc);
#define s_hash_clear s3b_hash_clear_ex

void strhash_clear(strhash_t *hash);

#define s_hash_it_begin s3b_hash_it_begin
#define s_hash_it_cur   s3b_hash_it_cur
#define s_hash_it_next  s3b_hash_it_next
#define s_hash_visit_t  s3b_hash_visit_t
#define s_hash_foreach  s3b_hash_foreach

#define strhash_foreach s3b_hash_foreach
#define strhash_visit_t s3b_hash_visit_t

/* serialization support for my hash */
/* the serialization function to store the object in a buffer */

/* 
 * type for a serialization function. 
 * */
typedef int (_save_func_t)(void *obj, void *ctx, char *buf, size_t sz);


/* hash.c */
extern int s3b_hash_create(struct s3b_hash **hashp, u_int maxkeys);
extern void s3b_hash_destroy(struct s3b_hash *hash);
extern void s3b_hash_destroy_no_check(struct s3b_hash *hash);
extern u_int s3b_hash_size(struct s3b_hash *hash);


/* returns the value and the iterator to the current value, so that
 * it can be modified, if needed. 
 *
 * If the value is not found INVALID_ITERATOR is returned and
 * valuep is unchanged. */
extern s3b_hash_it s3b_hash_get_it(struct s3b_hash *hash, s3b_block_t key,
		void **valuep);
extern void *s3b_hash_get(struct s3b_hash *hash, s3b_block_t key);
extern void *s3b_hash_get_v(struct s3b_hash *hash, s3b_block_t key);
extern void *s3b_hash_put(struct s3b_hash *hash, void *value);
extern s3b_hash_it s3b_hash_put_if_absent(struct s3b_hash *hash, void *value);
extern void s3b_hash_modify_value(struct s3b_hash *hash, s3b_hash_it it, 
		void *newvalue);
extern void s3b_hash_put_new(struct s3b_hash *hash, void *value);

/* 
 * puts value inside the hash giving it a new, unused key; it will
 * modify value. It will exit if the hash cannot have a new value.
 *
 * it is a convenient way to avoid two function calls and a modification
 * of the key.
 *
 * */
extern void s3b_hash_put_new_rnd(struct s3b_hash *hash, void *value,
		s3b_block_t max);

extern void *s3b_hash_remove(struct s3b_hash *hash, s3b_block_t key);
extern void s3b_hash_foreach(struct s3b_hash *hash, s3b_hash_visit_t *visitor, 
		void *arg);
/*
  This works only if the value is a POD, otherwise memory leaks will
  occur (and to clean the hash you have to do the visitor pattern)
 */
extern void s3b_hash_clear(struct s3b_hash *hash);
/*
   Returns a random value inside the hash; NULL if the hash is empty
 */
extern void* s3b_hash_get_random(struct s3b_hash *hash);

/* Returns a key which is not used in this hash table choosing from
 * [0..max). It returns INVALID_ITERATOR if this hash is already full*/
extern s3b_block_t s3b_hash_get_random_key(struct s3b_hash *hash, 
		s3b_block_t max);

/*Modifies the iterator pointing to the first valid element. if the iterator is INVALID no elements are present*/
extern void s3b_hash_it_begin(struct s3b_hash *hash, s3b_hash_it *a_it);
/*Returns the element pointed by the current iterator. It will crash or return garbage if the iterator is not valid.*/
extern void* s3b_hash_it_cur(struct s3b_hash *hash, s3b_hash_it a_it);
/*Modifies the iterator making it point to the next valid element. It will return 0 if no more elements are present.*/
extern int s3b_hash_it_next(struct s3b_hash *hash, s3b_hash_it *a_it);

extern void s3b_hash_dump(struct s3b_hash *hash);

#ifdef __cplusplus
}
#endif
