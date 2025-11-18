/*
 * jsmn_user
 * 
 * utitily function for the mimimalistic json parser.
 *
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 *
 */

#ifndef _JSMN_USER
#define _JSMN_USER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "jsmn.h"

struct jsmn_val
{
	struct jsmntok *tokens;
	int num_tokens;
	char *js;
	struct jsmntok *cursor;
	int cursor_idx;
};

struct jsmn_memento
{
	struct jsmntok *cursor;
	int idx;
};


/** 
 * parse a (JSON) string: tokens are allocated automatically.
 * The string is not taken, but it is modified in place
 *
 * duplicate it if you do not want to do it.
 * */
int jsmn_val_alloc_mod(struct jsmn_val *jval, char *js, size_t len);

int jsmn_val_free(struct jsmn_val *jval);

/* debug / dump support. */

void jsmn_dump_cur(const struct jsmn_val *jval);
void jsmn_dump_v(const struct jsmn_val *jval);


/* */


/* Accessor functions: these funtion assume that the current position
 * of the parser is on a object type*/

const char *jsmn_val_member_str(struct jsmn_val *jval, const char *key, 
		int keylen, const char *defstring);

long jsmn_val_member_l(struct jsmn_val *jval, const char *key, 
		int keylen, long def_l);



/* end accessor functions. */

/* movement functions. */

void jsmn_root(struct jsmn_val *jval);

void jsmn_parent(struct jsmn_val *jval);

void jsmn_mark(struct jsmn_val *jval, struct jsmn_memento *memento);

void jsmn_jump(struct jsmn_val *jval, const struct jsmn_memento *memento);

/* From a string/key position it will advance the iterator to the value,
 * to go back use the jsmn_parent*/
int jsmn_key_v(struct jsmn_val *jval);

int jsmn_key(struct jsmn_val *jval, const char *key,
		int keylen);

/* goes to the key saving first the current position. ( the current position is
 * an object position). */
int jsmn_key_meme_to(struct jsmn_val *jval, const char *key, int keylen, struct
		jsmn_memento *meme);

/* goes to the key but first jumps to the position in meme,
 * which must be an object. */
int jsmn_key_meme_from(struct jsmn_val *jval, const char *key,
		int keylen, const struct jsmn_memento *meme);


/* end movement functions. */

/* children primitive support */

/* Goes to the child key and returns its value as int if found.
 *
 * it jumps to meme after it, which must be initialized.
 *
 * The absence of the key is not an error.
 *
 * these functions only return an error in case of malformed fields.
 * */
int jsmn_child_as_int(struct jsmn_val *jval, const struct jsmn_memento *meme,
                const char *key, int keylen, int *destint);

/* if there is this flag then there is an error if the key is not present */
#define JSMN_KEY_STRICT    0X01

/* if there is this flag then there is an error if the value cannot be
 * converted. */
#define JSMN_VALUE_STRICT  0x02

/* if this field is present then no error are issued, the default value
 * is used instead*/
#define JSMN_DEFAULT_VALUE 0X04

/* The lenient flag is used get a default value if
 * the value cannot be converted. */
#define JSMN_LENIENT_FLAG (JSMN_KEY_STRICT | \
		JSMN_VALUE_STRICT | JSMN_DEFAULT_VALUE)

int jsmn_int_strict(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *destint);

int jsmn_str_strict(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, const char **deststr);

int jsmn_bool_strict(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, const char **deststr);

/* gets a boolean value, with a default value if not present.
 * You will still get an error if the value cannot be converted.*/
int jsmn_bool_def(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *dstbool,
                int defbool);

int jsmn_int_def(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *dstint,
                int defint);

int jsmn_str_def(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, 
		const char **deststr, const char *defstr);

int jsmn_child_as_int_ex(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *destint, int flags,
                int defint);

int jsmn_child_as_str_ex(struct jsmn_val *jval, const struct jsmn_memento *meme,
                const char *key, int keylen, const char **deststr,
                int flags, const char* defstr);

int jsmn_child_as_bool_ex(struct jsmn_val *jval, const struct jsmn_memento *meme,
                const char *key, int keylen, int *dstbool,
                int flags, int defbool);


int jsmn_child_as_bool(struct jsmn_val *jval, const struct jsmn_memento *meme,
                const char *key, int keylen, int *destbool);

int jsmn_child_as_str(struct jsmn_val *jval, const struct jsmn_memento *meme,
                const char *key, int keylen, const char **deststr);


/* extended support for size strings, like 15m or 5g with size
 * suffixes*/
int jsmn_child_as_sizestr(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, uintmax_t *valp);


/* end children primitive support */

/* primitive value support: they do not move the pointer. */

int jsmn_as_int(struct jsmn_val *jval, int *val);

/* same as jsmn_as_str_ex(jval, str, JSMN_VALUE_STRICT)*/
int jsmn_as_str(struct jsmn_val *jval, const char **str);

/* 
 * if flags is not JSMN_VALUE_STRICT then we do not convert it
 * otherwise we simply return the unconverted string.
 * */
int jsmn_as_str_ex(struct jsmn_val *jval, const char **str, int flags);

int jsmn_as_long(struct jsmn_val *jval, long *val);

int jsmn_as_bool(struct jsmn_val *jval, int *val);

/* end primitive */


/* iterator support */

int jsmn_children_begin(struct jsmn_val *jval);

/* how many children has this composite */
int jsmn_children_size(struct jsmn_val *jval);

int jsmn_next(struct jsmn_val *jval);

/* end iterator support. */

/* type */

#define jsmn_ispri(jval) ((jval)->cursor->type == JSMN_PRIMITIVE)

#define jsmn_isarr(jval) ((jval)->cursor->type == JSMN_ARRAY)

#define jsmn_isobj(jval) ((jval)->cursor->type == JSMN_OBJECT)

/* */


/* legacy interface: deprecated. */



jsmntok_t *jsmn_find_member(struct jsmn_val *jval, const char *key,
	       	int keylen);


void jsmn_dump(jsmntok_t *tokens, int num_tok, const char *js);

jsmntok_t *jsmn_member       (jsmntok_t *tok, int idx, int max_toks, const char
		*key, const char *js);

jsmntok_t *jsmn_member_ix    (jsmntok_t *tok, int idx, int max_toks, const char
		*key, const char *js, int *tok_idx);

jsmntok_t *jsmn_member_l_ix  (jsmntok_t *tok, int idx, int max_toks, const char
		*key, int keylen, const char *js, int *tok_idx);

/*
#define    jsmn_children_size(x) (x->size)
*/

jsmntok_t *jsmn_member_th_ix(jsmntok_t *tok, int idx, int num_toks, int
		arrayIndex, int *tok_idx);

jsmntok_t *jsmn_element_th(jsmntok_t *tok, int idx, int num_toks, int arrayIndex);

jsmntok_t *jsmn_element_th_ix(jsmntok_t *tok, int idx, int num_toks, int
		arrayIndex, int *tok_idx);



jsmntok_t *jsmn_child_first  (jsmntok_t *parent, int *parent_pos);

jsmntok_t *jsmn_child_next_ix(jsmntok_t *child, int child_idx, int *child_pos);

int jsmn_value_int(jsmntok_t *keyToken, const char *js);
double jsmn_value_double(jsmntok_t *tok, const char *js);
long jsmn_value_long(jsmntok_t *tok, const char *js);
int jsmn_value_bool(jsmntok_t *tok, const char *js);

char* jsmn_value_string_cpy(jsmntok_t *tok, const char *js, char *dest);

char* jsmn_dec_0(jsmntok_t *tok, char *js);

#if 0
#ifndef NDEBUG
void jsmn_nullify(jsmntok_t *tok, char *js);
void jsmn_unnullify(jsmntok_t *tok, char *js);
#else
#define jsmn_nullify(tok, js)   do {js[tok->end] = '\0'; } while (0)
#define jsmn_unnullify(tok, js) do {js[tok->end] = '"';  } while (0)
#endif
#endif

int jsmn_is_array(jsmntok_t *tok);
int jsmn_is_object(jsmntok_t *tok);
int jsmn_is_string(jsmntok_t *tok);

#ifdef __cplusplus
}
#endif


#endif
