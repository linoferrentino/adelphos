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



#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include "dbg.h"
#include "hash.h"

/* random numbers only used to get a random key. */

#define DBG_PRIV_MOD "utils_hash"
#define DBG_PRIV_ENABLED
#include "dbgpriv.h"


#define INITIAL_KEYS  33

/* Definitions */
#define LOAD_FACTOR                 0.666666
#define FIRST(hash_set, key)        (s3b_hash_set_index((hash_set->hs.alen), (key)))
#define NEXT(hash_set, index)       ((index) + 1 < (hash_set)->hs.alen ? (index) + 1 : 0)
#define EMPTY_HASH(value)          ((value) == NULL)
#define VALUE(hash_set, index)      ((hash_set)->array[(index)])
#define KEY(value)                  (*(s3b_block_t *)(value))
#define UINT_MAX                    0x7FFFFFF


#define FIRST_STR(h,k) (hash_str(k) % h->hs.alen)
#define KEY_STR(V)   (*(const char**)(V))
#define KEY_CMP(k1,k2) (strcmp(k1, k2))

/*
  hash set implementation internal structure.
 */
struct hs_imp {
    u_int       maxkeys;            /* max capacity */
    u_int       numkeys;            /* number of keys in table */
    u_int       alen;               /* hash array length */
};

/* Hash table structure */
struct s3b_hash {
  struct hs_imp hs;
  void        **array;          /* hash array */
};

static int rehash_bigger_str(s_hash_t *hash);
static int rehash_bigger(struct s3b_hash *hashp);
static u_int s3b_hash_set_index(u_int max_len, s3b_block_t key);

/* this is the structure that holds the value */
struct str_hash_s
{
	const char *key;
	const char *value;
};


u_int
s3b_hash_size(struct s3b_hash *hash)
{
    return hash->hs.numkeys;
}



s3b_hash_it
s3b_hash_put_if_absent(struct s3b_hash *hash, void *value)
{
  if (hash->hs.numkeys > (hash->hs.maxkeys * LOAD_FACTOR)){
    //fprintf(stderr,"Rehashing the hash keys %d max %d\n", hash->hs.numkeys, hash->hs.maxkeys);
    if (rehash_bigger(hash) != 0){
      assert(0);
    }
  }
  
  const s3b_block_t key = KEY(value);
  u_int i;

  for (i = FIRST(hash, key); 1; i = NEXT(hash, i)) {
    void *const value2 = VALUE(hash, i);

    if (EMPTY_HASH(value2))
      break;
    if (KEY(value2) == key){
      return INVALID_ITERATOR;
    }
  }
  assert(hash->hs.numkeys < hash->hs.maxkeys);
  VALUE(hash, i) = value;
  hash->hs.numkeys++;
  return i;
}

void s3b_hash_modify_value(struct s3b_hash *hash, s3b_hash_it it, void *newvalue)
{
  void *const value2 = VALUE(hash, it);
  /*UNUSED(value2);*/
  assert(KEY(value2) == KEY(newvalue));
  VALUE(hash, it) = newvalue;
}


int rehash_bigger_str(struct s3b_hash *small_hash)
{
	s_hash_t *big_hash;
	int res = s_hash_create(&big_hash, small_hash->hs.maxkeys * 2);
	if (res){
		return res;
	}

	u_int i;

	for (i = 0; i < small_hash->hs.alen; i++) {
		void *const value = VALUE(small_hash, i);

		if (value != NULL){
			s_hash_put_new(big_hash, value);
		}
	}

	free(small_hash->array);
	memcpy(small_hash, big_hash, sizeof(*big_hash));
	free(big_hash);

	return 0;
}


int rehash_bigger(struct s3b_hash *small_hash)
{
  struct s3b_hash *big_hash;
  //fprintf(stderr,"Small hash has %d keys max I want now a double\n", small_hash->hs.maxkeys);
  int res = s3b_hash_create(&big_hash, small_hash->hs.maxkeys * 2);
  if (res){
    return res;
  }

  //fprintf(stderr,"The big hash has %d max keys and a len %d\n", big_hash->hs.maxkeys, big_hash->hs.alen);
  
  //then I add the values...
  u_int i;

  for (i = 0; i < small_hash->hs.alen; i++) {
    void *const value = VALUE(small_hash, i);

    if (value != NULL){
      s3b_hash_put_new(big_hash, value);
    }
  }

  free(small_hash->array);
  memcpy(small_hash, big_hash, sizeof(*big_hash));
  free(big_hash);

  return 0;
}

void* s3b_hash_it_cur(struct s3b_hash *hash, s3b_hash_it a_it){
  return VALUE(hash, a_it);
}

void s3b_hash_it_begin(struct s3b_hash *hash, s3b_hash_it *a_it){
  if (hash->hs.numkeys == 0){
    *a_it = INVALID_ITERATOR;
    return;
  }
  u_int i;

  for (i = 0; i < hash->hs.alen; i++) {
    void *const value = VALUE(hash, i);
    if (value != NULL){
      *a_it = i;
      return;
    }
  }
  assert(0);
  *a_it = INVALID_ITERATOR;
}

void s3b_hash_foreach(struct s3b_hash *hash, 
		s3b_hash_visit_t *visitor, void *arg)
{
	int i = 0;
	for ( i = 0 ; i < hash->hs.alen ; ++i){
		void *const value = VALUE(hash, i);
		if (EMPTY_HASH(value)){
			continue;
		}
		(*visitor)(arg, value);
	}
}


void s3b_hash_dump(struct s3b_hash *hash)
{
  int keyC = 1;
  int i = 0;
  for ( i = 0 ; i < hash->hs.alen ; ++i){
    void *const value = VALUE(hash, i);
    if (EMPTY_HASH(value)){
      continue;
    }
    printf("key [%d/%d = %d]\n", keyC++, hash->hs.numkeys, KEY(value));
  }
}


void strhash_dump(strhash_t *hash)
{
	int keyC = 1;
	int i = 0;
	for ( i = 0 ; i < hash->hs.alen ; ++i){
		void *const value = VALUE(hash, i);
		if (EMPTY_HASH(value)){
			continue;
		}
		struct str_hash_s *shs = (struct str_hash_s*)value;
		fprintf(stderr, "key [%d/%d /%s/=/%s/]\n", 
				keyC++, hash->hs.numkeys, shs->key,
				shs->value);
	}

}


void*
s3b_hash_remove(struct s3b_hash *hash, s3b_block_t key)
{
    u_int i;
    u_int j;
    u_int k;

    void *res = NULL;

    /* Find entry */
    for (i = FIRST(hash, key); 1; i = NEXT(hash, i)) {
        void *const value = VALUE(hash, i);

        if (EMPTY_HASH(value))               /* no such entry */
            return res;
        if (KEY(value) == key){          /* entry found */
          res = value;
          break;
        }
    }

    /* Repair subsequent entries as necessary */
    for (j = NEXT(hash, i); 1; j = NEXT(hash, j)) {
        void *const value = VALUE(hash, j);

        if (value == NULL)
            break;
        k = FIRST(hash, KEY(value));
        if (j > i ? (k <= i || k > j) : (k <= i && k > j)) {
            VALUE(hash, i) = value;
            i = j;
        }
    }

    /* Remove entry */
    assert(VALUE(hash, i) != NULL);
    VALUE(hash, i) = NULL;
    hash->hs.numkeys--;
    return res;
}


/*
taken from 
http://www.cse.yorku.ca/~oz/hash.html
*/

static unsigned long hash_str(const char *str)
{
	/*DBG("computing hash %s", str);*/
	unsigned long hash = 5381;
	int c;

	while ((c = *str++) != '\0')
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	/*DBG("hash returns %lu", hash);*/
	return hash;
}


static u_int s3b_hash_set_index(u_int max_len, uint32_t key)
{
  return (key*(key + 3)) % max_len;
}

/*
 * Jenkins one-at-a-time hash
 */
/* static u_int s3b_hash_set_index(u_int max_len, s3b_block_t key) */
/* { */
/*   u_int value = 0; */
/*   int i; */
 
/*   for (i = 0; i < sizeof(key); i++) { */
/*     value += ((u_char *)&key)[i]; */
/*     value += (value << 10); */
/*     value ^= (value >> 6); */
/*   } */
/*   value += (value << 3); */
/*   value ^= (value >> 11); */
/*   value += (value << 15); */
/*   return value % max_len; */

/* } */

void
s3b_hash_destroy(struct s3b_hash *hash)
{
  assert(hash->hs.numkeys == 0); //must clear before destroy.
  s3b_hash_destroy_no_check(hash);
}

void s3b_hash_destroy_no_check(struct s3b_hash *hash)
{
  free(hash->array);
  free(hash);
}

void s_hash_put_new(s_hash_t *hash, void *value)
{
	if (hash->hs.numkeys > (hash->hs.maxkeys * LOAD_FACTOR)){
		DBG("rehashing %d", hash->hs.numkeys);
		
		if (rehash_bigger_str(hash) != 0){
			ERR("could not rehash bigger!");
			assert(0);
		}
	}

	const char* key = KEY_STR(value);
	/*DBG("this is the key %p, string %s", key, key);*/
	u_int i;

	for (i = FIRST_STR(hash, key); 1; i = NEXT(hash, i)) {
		void *const value2 = VALUE(hash, i);

		/*DBG("key %s considering Val %d", key, i);*/

		if (EMPTY_HASH(value2))
			break;
		assert(KEY_CMP(key, KEY_STR(value2)) != 0);
	}
	assert(hash->hs.numkeys < hash->hs.maxkeys);
	VALUE(hash, i) = value;
	hash->hs.numkeys++;
}

s3b_block_t s3b_hash_get_random_key(struct s3b_hash *hash, 
		s3b_block_t max)
{
	s3b_block_t res;
	s3b_block_t sentinel;
	void *val;

	if (max == hash->hs.numkeys) {
		DBG("hash is full, no space left %d", max);
		res = INVALID_ITERATOR;
		goto end;
	}

	res = random() % max;
	sentinel = res;

again:

	val = s3b_hash_get(hash, res);
	if (val == NULL) {
		/*DBG("key %d is free", res);*/
		goto end;
	}

	/*DBG("key %d/%d is taken %p", res, max, val);*/

	res++;
	if (res == max) {
		res = 0;
		goto again;
	}

	/* if you arrive at the sentinel something has gone wrong */
	if (res == sentinel) {
		DBG("No free keys max %d numkeys %d. This is an error",
				max, hash->hs.numkeys);
		abort();
	}


	goto again;


end:
	return res;
}

void
s3b_hash_put_new(struct s3b_hash *hash, void *value)
{
  if (hash->hs.numkeys > (hash->hs.maxkeys * LOAD_FACTOR)){
    //fprintf(stderr,"Rehashing the hash keys %d max %d\n", hash->hs.numkeys, hash->hs.maxkeys);
    if (rehash_bigger(hash) != 0){
      assert(0);
    }
  }
  
    const s3b_block_t key = KEY(value);
    u_int i;

    for (i = FIRST(hash, key); 1; i = NEXT(hash, i)) {
        void *const value2 = VALUE(hash, i);

        if (EMPTY_HASH(value2))
            break;
        assert(KEY(value2) != key);
    }
    assert(hash->hs.numkeys < hash->hs.maxkeys);
    assert(VALUE(hash, i) == NULL);
    VALUE(hash, i) = value;
    hash->hs.numkeys++;
}

extern void s3b_hash_put_new_rnd(struct s3b_hash *hash, void *value,
		s3b_block_t max)
{
	s3b_block_t val = s3b_hash_get_random_key(hash, max);
	if (val == INVALID_ITERATOR) {
		ERR("not possible to store");
	}
	KEY(value) = val;
	s3b_hash_put_new(hash, value);
}

void strhash_put_new(strhash_t *hash, const char *key, const char *value)
{
	/* create the value */
	struct str_hash_s *shs = malloc(sizeof(struct str_hash_s));
	shs->key = strdup(key);
	shs->value = strdup(value);

	s_hash_put_new(hash, shs);
}

const char* strhash_get(strhash_t *hash, const char *key)
{
	void *val = s_hash_get(hash, key);
	if (val == NULL)
		return NULL;

	struct str_hash_s *shs = (struct str_hash_s*)val;
	return shs->value;
}

static void _frstr_s(void *par)
{
	if (par == NULL)
		return;

	struct str_hash_s *shs = (struct str_hash_s*)par;
	free((void*)shs->key);
	free((void*)shs->value);
	free(shs);
}

void strhash_clear(strhash_t *hash)
{
	s_hash_clear(hash, _frstr_s);
}

/*
void strhash_dump(strhash_t *hash)
{
	strhash_it it;

	it = strhash_begin(hash, &it);
}
*/


void* s_hash_get(s_hash_t *hash, const char *key)
{
	u_int i;

	for (i = FIRST_STR(hash, key); 1; i = NEXT(hash, i)) {
		void *const value = VALUE(hash, i);

		if (EMPTY_HASH(value))
			return NULL;
		if (KEY_CMP(KEY_STR(value),key) == 0)
			return value;
	}

}


s3b_hash_it s3b_hash_get_it(struct s3b_hash *hash, s3b_block_t key,
		void **valuep)
{
	u_int i;

	for (i = FIRST(hash, key); 1; i = NEXT(hash, i)) {
		void *const value = VALUE(hash, i);

		if (EMPTY_HASH(value))
			return INVALID_ITERATOR;

		if (KEY(value) == key) {
			*valuep = value;
			return i;
		}
	}

}


void *
s3b_hash_get(struct s3b_hash *hash, s3b_block_t key)
{
  u_int i;

  for (i = FIRST(hash, key); 1; i = NEXT(hash, i)) {
    void *const value = VALUE(hash, i);

    if (EMPTY_HASH(value))
      return NULL;
    if (KEY(value) == key)
      return value;
  }
}

void *s3b_hash_get_v(struct s3b_hash *hash, s3b_block_t key)
{
  u_int i;


  i = FIRST(hash, key);
  printf("Searching %d I start with %d\n", key, i);

  for (; 1; i = NEXT(hash, i)) {
    void *const value = VALUE(hash, i);

    if (EMPTY_HASH(value)){
      printf("NULL, found %d\n", i);
      return NULL;
    }
    if (KEY(value) == key){
      printf("FOUND! @%d\n", i);
      return value;
    }

    printf("%d != %d I continue...\n", KEY(value), key);
  }
}

int
s3b_hash_create(struct s3b_hash **hashp, u_int maxkeys)
{
    struct s3b_hash *hash;
    u_int alen;

    if (maxkeys >= (u_int)(UINT_MAX * LOAD_FACTOR) - 1)
        return EINVAL;
    alen = (u_int)(maxkeys / LOAD_FACTOR) + 1;
    //    S3LOG("Allocating %lu bytes for hash; %lu bytes for each array elemet (len %d)", sizeof(*hash), sizeof(*hash->array), alen);
    if ((hash = calloc(1, sizeof(*hash) /*+ alen * sizeof(*hash->array)*/)) == NULL)
        return ENOMEM;
    if ((hash->array = calloc(1, alen * sizeof(*hash->array))) == NULL)
      return ENOMEM;
    hash->hs.maxkeys = maxkeys;
    hash->hs.alen = alen;
    *hashp = hash;
    return 0;
}

void s3b_hash_clear_ex(struct s3b_hash *hash, free_func *frfnc)
{
	int i;
	for (i = 0; i < hash->hs.alen; i++) {
		void *const value = VALUE(hash, i);
		(*frfnc)(value);
	}
	u_int sizeBytes = hash->hs.alen * sizeof(*hash->array);
	memset(hash->array, 0, sizeBytes);
	hash->hs.numkeys = 0;

}

void s3b_hash_clear(struct s3b_hash *hash)
{
  int i;
  for (i = 0; i < hash->hs.alen; i++) {
    void *const value = VALUE(hash, i);
    free(value);
  }
  u_int sizeBytes = hash->hs.alen * sizeof(*hash->array);
  memset(hash->array, 0, sizeBytes);
  hash->hs.numkeys = 0;
}

extern int s3b_hash_it_next(struct s3b_hash *hash, s3b_hash_it *a_it){
  assert(*a_it != INVALID_ITERATOR);
  u_int i = *a_it + 1;
  for (; i < hash->hs.alen; i++){
    void *const value = VALUE(hash, i);
    if (value != NULL){
      *a_it = i;
      return 1;
    }
  }
  *a_it = INVALID_ITERATOR;
  return 0;
}
