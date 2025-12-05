/**
 *
 * jsmn_user.c utility functions for the jsmn library.
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 */

#include <string.h>
#include <stdlib.h>
#include "jsmn_user.h"

#define MODULE_LEV ML_TRIVIAL
#define _LOG_MODULE "jsmn_user"
#include "alog.h"
#include "units.h"
#include "misc.h"

LOG_MODULE_IMP;


int jsmn_val_alloc_mod(struct jsmn_val *jval, char *js, size_t len)
{


	alogi("Will parse");
	dump_payload((unsigned char*)js, len);


	jsmn_parser parser;
	int expected_tokens = len / 5 + 2;
	jval->tokens = NULL;

	do {
		jval->tokens = (jsmntok_t*)realloc(jval->tokens, 
				sizeof(jsmntok_t) * expected_tokens);

		jsmn_init(&parser);
		jval->num_tokens = jsmn_parse(&parser, js, len, 
				jval->tokens, expected_tokens);

		if (jval->num_tokens == -1){
			expected_tokens *= 2;
			continue;
		}
		if (jval->num_tokens < -1){
			alogi("Error parsing. Position %d character [%c]",
					parser.pos, js[parser.pos]);
			free(jval->tokens);
                        /* avoid double free. */
                        jval->tokens = NULL;
			return -1;
		}
		break;

	} while(1);

	jval->cursor_idx = 0;
	jval->cursor = jval->tokens;
	jval->js = js;

	return 0;
}

int jsmn_val_free(struct jsmn_val *jval)
{
	free(jval->tokens);
	return 0;
}


void jsmn_dump_v(const struct jsmn_val *jval)
{
	jsmn_dump(jval->tokens, jval->num_tokens, jval->js);
}

void _jsmn_dump_token(jsmntok_t *tok, int i, const char *js)
{

	switch(tok->type){
		case JSMN_OBJECT:
		alogi("[%d] object of size %d start %d end %d parent %d", 
		i, tok->size, tok->start, tok->end, tok->parent);
			break;
		case JSMN_ARRAY:
		alogi("[%d] array of size %d start %d end %d parent %d", 
		i, tok->size, tok->start, tok->end, tok->parent);
			break;
		case JSMN_PRIMITIVE:
		alogi("[%d] primitive of size %d start %d end %d par %d [%.*s]",
			i, tok->size, tok->start, tok->end, tok->parent, 
			tok->end - tok->start, &js[tok->start]);
			break;
		case JSMN_STRING:
		alogi("[%d] string of size %d start %d end %d par %d [%.*s]", 
			i, tok->size, tok->start, tok->end, 
			tok->parent, tok->end - tok->start, &js[tok->start]);
			break;
		default:
			alogi("Undefined type %d", tok->type);
			abort();
	}

}

void jsmn_dump_cur(const struct jsmn_val *jval)
{
	_jsmn_dump_token(jval->cursor, jval->cursor_idx, jval->js);
}

void jsmn_dump(jsmntok_t *tokens, int num_tok, const char *js)
{
	int i;
	for (i = 0 ; i < num_tok ; ++i)
	{
		jsmntok_t *tok = &tokens[i];
		_jsmn_dump_token(tok, i, js);
	}
}

jsmntok_t *jsmn_member(jsmntok_t *tok, int idx, int max_toks, const char *key,
		const char *js)
{
	return jsmn_member_l_ix(tok, idx, max_toks, key, strlen(key), js, NULL);
}

jsmntok_t *jsmn_member_ix(jsmntok_t *tok, int idx, int max_toks, const char
		*key, const char *js, int *tok_idx)
{
  return jsmn_member_l_ix(tok, idx, max_toks, key, strlen(key), js, tok_idx);
}

int jsmn_value_int(jsmntok_t *tok, const char *js)
{
  ad_assert(tok->type == JSMN_PRIMITIVE);
  return atoi(&js[tok->start]);
}

int jsmn_as_long(struct jsmn_val *jval, long *val)
{
	if (jval->cursor->type != JSMN_PRIMITIVE)
		return -1;
	*val = atol(&jval->js[jval->cursor->start]);
	return 0;
}

int jsmn_as_bool(struct jsmn_val *jval, int *val)
{
	alogt("valbool cursor type %d",
		jval->cursor->type);
        if (jval->cursor->type != JSMN_PRIMITIVE)
		return -1;
	alogt("val jstring [%s]", &jval->js[jval->cursor->start]);
	*val = jval->js[jval->cursor->start] == 't' ? 1 : 0;
	return 0;
}


int jsmn_as_int(struct jsmn_val *jval, int *val)
{
	if (jval->cursor->type != JSMN_PRIMITIVE)
		return -1;
	*val = atoi(&jval->js[jval->cursor->start]);
	return 0;
}

long jsmn_value_long(jsmntok_t *tok, const char *js)
{
  ad_assert(tok->type == JSMN_PRIMITIVE);
  return atol(&js[tok->start]);
}

double jsmn_value_double(jsmntok_t *tok, const char *js)
{
  ad_assert(tok->type == JSMN_PRIMITIVE);
  return atof(&js[tok->start]);
}

char* jsmn_dec_0(jsmntok_t *tok, char *js)
{

	/* decoded string is sometimes shorter; so we can put the flag in the
	 * last position (where the quote end character is put)*/

  ad_assert(tok->type == JSMN_STRING);
  alogx("dec0 tokendchar end %d, %c", tok->end, js[tok->end] );
  if (js[tok->end] != '"')
	  goto already_decoded;

  /*ad_assert(js[tok->end] == '"');*/
  char *cur = &js[tok->start];
  char *wcur = cur;

  while(*cur != '"'){

    if (*cur == '\\'){
      char decoded = 0;
      switch(*(cur+1)){
        case '\"':
          decoded = '\"';
          break;
        case '/' : 
          decoded = '/';
          break;
        case '\\' : 
          decoded = '\\';
          break;
        case 'b' :
          decoded = '\b';
          break;
        case 'f' : 
          decoded = '\f';
          break;
        case 'r' : 
          decoded = '\r';
          break;
        case 'n'  : 
          decoded = '\n';
          break;
        case 't' :
          decoded = '\t';
          break;
        default:
          alogw("unkown char in decoding %c", *(cur+1));
          ad_assert(0);
          break;
      }
      *wcur = decoded;
      cur++;
    }

    if (cur++ != wcur++){
      *wcur = *cur;
    }
  }
  *wcur = '\0';
  alogx("tok end %d, %d wcur %d", tok->end, js[tok->end], *wcur);
  if (&js[tok->end] != wcur)
	  js[tok->end] = '\0';
already_decoded:
  return &js[tok->start];
}

#ifndef NDEBUG
void jsmn_nullify(jsmntok_t *tok, char *js)
{
  ad_assert(tok->type == JSMN_STRING);
  ad_assert(js[tok->end] == '"');
  js[tok->end] = '\0';
}

void jsmn_unnullify(jsmntok_t *tok, char *js)
{
  ad_assert(tok->type == JSMN_STRING);
  ad_assert(js[tok->end] == '\0');
  js[tok->end] = '"';
}
#endif

int jsmn_value_bool(jsmntok_t *tok, const char *js)
{
  ad_assert(tok->type == JSMN_PRIMITIVE);
  return js[tok->start] == 't' ? 1 : 0;
}

char* jsmn_value_string_cpy(jsmntok_t *tok, const char *js, char *dest)
{
  size_t len = tok->end - tok->start;
  strncpy(dest, &js[tok->start], len);
  dest[len] = '\0';
  return dest;
}

jsmntok_t *jsmn_element_th(jsmntok_t *tok, int idx, int num_toks, int arr_idx)
{
  return jsmn_element_th_ix(tok, idx, num_toks, arr_idx, NULL);
}

jsmntok_t *jsmn_element_th_ix(jsmntok_t *tok, int idx, int num_toks, int arr_idx, int *tok_idx)
{
  if (tok->type != JSMN_ARRAY)
    return NULL; 

  if (arr_idx >= tok->size)
    return NULL;

  jsmntok_t *child = tok + 1;
  //logm("start searching element @%d ", idx);
  num_toks -= idx;
  int found = 0;
  int i;
  int child_idx = 0;

  for (i = 1 ; i < num_toks ; ++i){
    if (child->parent == idx){ 
      if (arr_idx == child_idx){
        found = 1;
        break;
      }
      if (++child_idx == tok->size)
        break;
    }
    child++;
  }
  if (tok_idx != NULL)
    *tok_idx = (i + idx);

  //logm("Return found %d idx %d child %p", found, (i + idx), child);
  return found ? child : NULL;
}

int jsmn_key_v(struct jsmn_val *jval)
{
	if (jval->cursor->type != JSMN_STRING) {
		alogi("cursor is not a string type %d",
				jval->cursor->type);
		return -1;
	}

	jval->cursor++;
	jval->cursor_idx++;


	return 0;
}

int jsmn_next(struct jsmn_val *jval)
{
	int pid = jval->cursor->parent;
	jsmntok_t *parent = &jval->tokens[pid];

	int idx = jval->cursor_idx;
	jsmntok_t *cur = jval->cursor;

	alogx("parent span is %d to %d",
			parent->start, parent->end);

	idx++;
	cur++;

	for(; idx < jval->num_tokens ; idx++, cur++) {

		if (cur->parent == pid) {
			alogx("found another child @%d, parent %d", 
					idx, pid);
			jval->cursor = cur;
			jval->cursor_idx = idx;
			goto end;
		}

		if (cur->start > parent->end) {
			alogt("not any more children %d > %d",
					cur->start, parent->end);
			jval->cursor = NULL;
			jval->cursor_idx = -1;
			return -1;
		}
	}

	alogt("no other children");
	return -1;

end:
	return 0;

}

int jsmn_children_size(struct jsmn_val *jval)
{
        if ((jval->cursor->type != JSMN_OBJECT) &&
		(jval->cursor->type != JSMN_ARRAY)) {
		alogi("cursor is not a composite object %d",
				jval->cursor->type);
		return -1;
	}

        return jval->cursor->size;

}

int jsmn_children_begin(struct jsmn_val *jval)
{
	if ((jval->cursor->type != JSMN_OBJECT) &&
		(jval->cursor->type != JSMN_ARRAY)) {
		alogi("cursor is not a composite object %d",
				jval->cursor->type);
		return -1;
	}
	
	alogx("found a composite object of size: %d", jval->cursor->size);

	if (jval->cursor->size == 0)
		return -1;

	jval->cursor++;
	jval->cursor_idx++;

	return 0;
}

jsmntok_t *jsmn_child_first(jsmntok_t *parent, int *child_first_pos)
{
  //ad_assert(parent->type == JSMN_OBJECT || parent->type == JSMN_ARRAY);
  if (parent->size == 0)
    return NULL;
  (*child_first_pos)++;
  return parent+1;
}


jsmntok_t *jsmn_child_next_ix(jsmntok_t *child, int child_idx, int *child_pos)
{
  jsmntok_t *parent = child - (*child_pos - child->parent);
  int parent_idx = child->parent;
  ad_assert(parent->type == JSMN_OBJECT || parent->type == JSMN_ARRAY);

  if (child_idx+1 == parent->size){
    return NULL;
  }

  child++;
  int i = *child_pos + 1;

  while(1){
    if (child->parent == parent_idx){ 
      break;  
    }
    child++;
    i++;
  }

  *child_pos = i;
  return child;
}

jsmntok_t *jsmn_member_th_ix(jsmntok_t *tok, int idx, int num_toks, int arrayIndex, int *tok_idx)
{

  if (tok->type != JSMN_OBJECT)
    return NULL;

  if (idx >= num_toks)
    return NULL;

  int children = 0;
  num_toks -= idx;
  jsmntok_t *child = tok + 1;
  int found = 0;
  int i;

  for (i = 1 ; i < num_toks ; ++i){
    if (child->parent == idx){ 
      if (child->type == JSMN_STRING){
        if (children++ == arrayIndex){
          found = 1;
          break;  
        } 
      }
    }
    child++;
  }

  if (tok_idx != NULL)
    *tok_idx = (i + idx);

  return found ? (child) : NULL;
}

long jsmn_val_member_l(struct jsmn_val *jval, const char *key, 
		int keylen, long def_l)
{
	long res = def_l;


	jsmntok_t *tok = jsmn_find_member(jval, key, keylen);
	if (tok != NULL){
		return jsmn_value_long(tok, jval->js);
	}

	return res;
}

const char *jsmn_val_member_str(struct jsmn_val *jval, const char *key, 
		int keylen, const char *defstring)
{
	int new_idx;
	jsmntok_t *tok = jsmn_member_l_ix(jval->cursor, jval->cursor_idx,
			jval->num_tokens, key, keylen, jval->js, &new_idx);

	if (tok != NULL) {
		return jsmn_dec_0(tok, jval->js);
	} else {
		return defstring;
	}

}

int jsmn_as_str(struct jsmn_val *jval, const char **str)
{
	return jsmn_as_str_ex(jval, str, JSMN_VALUE_STRICT);
}

int jsmn_str_def(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, 
		const char **deststr, const char *defstr)
{
	return jsmn_child_as_str_ex(jval, meme, key, keylen, deststr,
                        JSMN_KEY_STRICT | JSMN_DEFAULT_VALUE, defstr);

}

int jsmn_as_str_ex(struct jsmn_val *jval, const char **str, int flags)
{
	if ((jval->cursor->type != JSMN_STRING) && 
			(flags & JSMN_VALUE_STRICT)) {
		return -1;
	}

	if (jval->cursor->type == JSMN_STRING) {
		*str = jsmn_dec_0(jval->cursor, jval->js);
		return 0;
	}

	/* not a string? well, we simply put a null at the end and
	 * return what it is inside */
	jval->js[jval->cursor->end] = '\0';
	*str = &jval->js[jval->cursor->start];

	return 0;
}


int jsmn_int_strict(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *destint)
{
        return jsmn_child_as_int_ex(jval, meme, key, keylen, destint,
                        JSMN_KEY_STRICT | JSMN_VALUE_STRICT, 0);
}

int jsmn_int_def(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *destint,
                int defint)
{

        return jsmn_child_as_int_ex(jval, meme, key, keylen, destint,
                        JSMN_KEY_STRICT | JSMN_VALUE_STRICT 
                        | JSMN_DEFAULT_VALUE, defint);
}


int jsmn_child_as_int_ex(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *destint, int flags,
                int defint)
{

        int res;

        res = jsmn_key(jval, key, keylen);

	if (0 == res) {
		res = jsmn_as_int(jval, destint);
                if (res != 0 ) {
                        if (flags & JSMN_VALUE_STRICT) {
                                alogt("value %.*s is invalid",
                                   jval->cursor->end - jval->cursor->start,
                                   &jval->js[jval->cursor->start]);
                                goto fail;
                        }
                }
		alogt("%s = %d", key, *destint);
	} else {
                /* key not present */
                if (flags & JSMN_KEY_STRICT) {
                        alogt("No key %.*s", keylen, key);
                        res = -1;
                        goto fail;
                }
        }
        res = 0;

fail:
        jsmn_jump(jval, meme);
        if (res) {
                if (flags & JSMN_DEFAULT_VALUE) {
                        alogt("Using default value %d", defint);
                        *destint = defint;
                        res = 0;
                }
        }
        return res;
}



int jsmn_child_as_int(struct jsmn_val *jv, const struct jsmn_memento *meme,
                const char *key, int keylen, int *destint)
{
        int res = 0;


        res = jsmn_key(jv, key, keylen);

	if (0 == res) {
		res = jsmn_as_int(jv, destint);
		ok_or_goto_fail(res == 0);
		alogt("%s = %d", key, *destint);
	}
        res = 0;

fail:
        jsmn_jump(jv, meme);
        return res;
}

int jsmn_str_strict(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, const char **deststr)
{
        return jsmn_child_as_str_ex(jval, meme, key, keylen, deststr,
                        JSMN_KEY_STRICT | JSMN_VALUE_STRICT, NULL);
}

int jsmn_child_as_str_ex(struct jsmn_val *jv, const struct jsmn_memento *meme,
                const char *key, int keylen, const char **deststr,
                int flags, const char* defstr)
{

        int res = 0;

        res = jsmn_key(jv, key, keylen);


        if (0 == res) {
                res = jsmn_as_str_ex(jv, deststr, flags);
                if (res != 0 ) {
                        if (flags & JSMN_VALUE_STRICT) {
                                alogt("value %.*s is invalid",
                                     jv->cursor->end - jv->cursor->start,
                                     &jv->js[jv->cursor->start]);
                                goto fail;
                        }
                }
                alogx("%s = %s ", key, *deststr);
        } else {
                /* key not present */
                if (flags & JSMN_KEY_STRICT) {
                        alogt("No key %.*s", keylen, key);
                        res = -1;
                        goto fail;
                }
        }

fail:
        jsmn_jump(jv, meme);
        if (res && (flags & JSMN_DEFAULT_VALUE)) {
                *deststr = defstr;
                res = 0;
        }
        return res;


}

int jsmn_child_as_str(struct jsmn_val *jv, const struct jsmn_memento *meme,
                const char *key, int keylen, const char **deststr)
{
        return jsmn_child_as_str_ex(jv, meme, key, keylen, deststr, 0, NULL);
}


int jsmn_bool_def(struct jsmn_val *jval, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *dstbool,
                int defbool)
{
        return jsmn_child_as_bool_ex(jval, meme,
                        key, keylen, dstbool,
                        JSMN_DEFAULT_VALUE, defbool);
}



int jsmn_child_as_bool_ex(struct jsmn_val *jv, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, int *destbool,
                int flags, int defbool)
{
        int res = 0;

        res = jsmn_key(jv, key, keylen);

	if (0 == res) {

                res = jsmn_as_bool(jv, destbool);

                if (res && (flags & JSMN_VALUE_STRICT)) {
                        alogt("value %.*s is invalid",
                                        jv->cursor->end - jv->cursor->start,
                                        &jv->js[jv->cursor->start]);
                        goto fail;
                }

		alogt("%s = %d", key, *destbool);

        } else {

                /* key not present */
                if (flags & JSMN_KEY_STRICT) {
                        alogt("No key %.*s", keylen, key);
                        res = -1;
                        goto fail;
                }
        }

fail:
        jsmn_jump(jv, meme);

        if (res && (flags & JSMN_DEFAULT_VALUE)) {
                *destbool = defbool;
                res = 0;
        }

        return res;


}




int jsmn_child_as_bool(struct jsmn_val *jv, const struct jsmn_memento *meme,
                const char *key, int keylen, int *destbool)
{

        return jsmn_child_as_bool_ex(jv, meme, key, keylen, destbool, 0, 0);

}



int jsmn_child_as_sizestr(struct jsmn_val *jv, 
                const struct jsmn_memento *meme,
                const char *key, int keylen, uintmax_t *valp)
{
        int res = 0;
        const char *vals;
        

        res = jsmn_key(jv, key, keylen);


        if (0 == res) {
		/* the value could be a primitive. */
                res = jsmn_as_str_ex(jv, &vals, 0);
                ok_or_goto_fail(res == 0);
                res = u_parse_size_string(vals, valp);
                ok_or_goto_fail(res == 0);
                alogt("%s = %s = %jd", key, vals, *valp);
        }
        res = 0;


fail:
        jsmn_jump(jv, meme);
        return res;
}



void jsmn_parent(struct jsmn_val *jval)
{
	alogx("cursor parent is %d", jval->cursor->parent);
	jval->cursor = &jval->tokens[jval->cursor->parent];
	jval->cursor_idx = jval->cursor - &jval->tokens[0];
}

void jsmn_root(struct jsmn_val *jval)
{
	jval->cursor = jval->tokens;
	jval->cursor_idx = 0;

}


int jsmn_key_meme_to(struct jsmn_val *jval, const char *key, 
		int keylen, struct jsmn_memento *meme)
{
	int res = jsmn_key(jval, key, keylen);
	if (res == 0)
		jsmn_mark(jval, meme);
	return res;
}

/* goes to the key but first jumps to the position in meme,
 * which must be an object. */
int jsmn_key_meme_from(struct jsmn_val *jval, const char *key,
		int keylen, const struct jsmn_memento *meme)
{
	jsmn_jump(jval, meme);
	return jsmn_key(jval, key, keylen);
}



void jsmn_mark(struct jsmn_val *jval, struct jsmn_memento *memento)
{
	memento->cursor = jval->cursor;
	memento->idx = jval->cursor_idx;
}

void jsmn_jump(struct jsmn_val *jval, const struct jsmn_memento *memento)
{
	jval->cursor = memento->cursor;
	jval->cursor_idx = memento->idx;
}


int jsmn_key(struct jsmn_val *jval, const char *key,
		int keylen)
{
	jsmntok_t *tok = jsmn_find_member(jval, key, keylen);

	if (tok != NULL) {
		return 0;
	}

	return -1;
}


jsmntok_t *jsmn_find_member(struct jsmn_val *jval, const char *key,
	       	int keylen)

{
	int absent = 1;
	if (jval->cursor->type != JSMN_OBJECT)
		goto end;

	if (jval->cursor_idx >= jval->num_tokens)
		goto end;

	int children = 0;

	int max_toks = jval->num_tokens;
	max_toks -= jval->cursor_idx;

	jsmntok_t *child = jval->cursor + 1;
	int i;

	for (i = 1 ; i < max_toks ; ++i, ++child){

		alogx("[%d] child->parent %d val cursor idx %d",
				i, child->parent, jval->cursor_idx);
		if (child->parent != jval->cursor_idx){ 
			continue;
		}

		alogx("child type %d", child->type);
		if (child->type != JSMN_STRING){
			continue;
		}

		alogx("child len %d keylen %d", child->end-child->start,
				keylen);
		if (keylen != (child->end - child->start)){
			continue;
		}

		alogx("comparing with %.*s", child->end - child->start,
				&jval->js[child->start]);
		if (strncmp(key, &jval->js[child->start], keylen) == 0){
			absent = 0;
			break;
		}

		alogx("children %d cur size %d", children,
				jval->cursor->size);
		if (++children == jval->cursor->size)
			break;
	}


	if (absent) {
		goto end;
	}

	jval->cursor     = child + 1;
	jval->cursor_idx = i + jval->cursor_idx + 1;

	alogx("find child %s with index %d ", key,
			jval->cursor_idx );
end:
	return absent ? NULL : (child+1);
}


jsmntok_t *jsmn_member_l_ix(jsmntok_t *token, int idx, int max_toks, 
		const char *key, int keylen, const char *js, int *tok_idx)
{
  if (token->type != JSMN_OBJECT)
    return NULL;

  if (idx >= max_toks)
    return NULL;

  int children = 0;
  max_toks -= idx;
  jsmntok_t *child = token+1;
  int found = 0;
  int i;

  for (i = 1 ; i < max_toks ; ++i){
    if (child->parent == idx){ 
      if (child->type == JSMN_STRING){
        if (keylen == (child->end - child->start)){
          if (strncmp(key, &js[child->start], keylen) == 0){
            found = 1;
            break;
          }
        }
      }
      if (++children == token->size)
        break;
    }
    child++;
  }

  if (tok_idx != NULL)
    *tok_idx = (i+1+idx);

  return found ? (child+1) : NULL;
}

int jsmn_is_array(jsmntok_t *tok)
{
  return tok->type == JSMN_ARRAY;
}

int jsmn_is_object(jsmntok_t *tok)
{
  return tok->type == JSMN_OBJECT;
}

int jsmn_is_string(jsmntok_t *tok)
{
  return tok->type == JSMN_STRING;
}



