/*
 * jfsm.c Implementation detail for the Json Finite State Machine (producer).
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 * 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "jfsm.h"
#include "strstream.h"

#define _LOG_MODULE "jfsm"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"

LOG_MODULE_IMP;


enum E_JFSM_STATE {
  JFSM_WANT_ELEMENT = 0,
  JFSM_WANT_MEMBER = 1,
};

#define MAX_JSON_DEPTH 20

#define JFSM_STACK_ROOT   0
#define JFSM_STACK_ARRAY  1
#define JFSM_STACK_OBJECT 2


static int emit_str(void *userptr, int c)
{
	strstream_s *strs = (strstream_s*)userptr;
	return strstream_emitc(strs, (char)c);

}




struct json_fsm {
  uint8_t state; /* the state of this machine */
  emit_json_char_fn emit;
  void   *userptr;
  int     flags;
  uint8_t stack[MAX_JSON_DEPTH];
  uint8_t sp;   /*stack pointer it points to first free element.*/
  uint8_t one_put; /* already one put element / member , next one will have a comma before*/
};


char *jfsm_json_str(struct json_fsm *jfsm)
{
	strstream_s *strs = (strstream_s*)(jfsm->userptr);
	return strstream_getstr(strs);
}

size_t jfsm_str_size(struct json_fsm *jfsm)
{
	strstream_s *strs = (strstream_s*)(jfsm->userptr);
	return strstream_getsz(strs);
}


int emit_stdout(void *userptr, int jsonc){
  if (jsonc < 0)
    return jsonc;
  return fprintf(stdout, "%c", (char)jsonc) > 0 ? 0 : 1;
}

int emit_stderr(void *userptr, int jsonc){
  if (jsonc < 0)
    return jsonc;
  return fprintf(stderr, "%c", (char)jsonc) > 0 ? 0 : 1;
}

static int _emit_str(emit_json_char_fn emit, void *userptr, const char *str)
{
  while(*str != '\0'){
    int res = (*emit)(userptr, *str++);
    if (res != 0)
      return res;
  }
  return 0;
}

static int _emit_str_esc(emit_json_char_fn emit, void *userptr, const char *str)
{
  while(*str != '\0'){

    char escaped = '\0';
    switch(*str){
      case '\b':
        escaped = 'b';
        break;
      case '/':
        escaped = '/';
        break;
      case '\\':
        escaped = '\\';
        break;
      case '\"':
        escaped = '\"';
        break;
      case '\f':
        escaped = 'f';
        break;
      case '\n':
        escaped = 'n';
        break;
      case '\t':
        escaped = 't';
        break;
      case '\r':
        escaped = 'r';
        break;
    }

    int res;
    if (escaped != '\0'){
      res = (*emit)(userptr, '\\');
      if (res)
        return res;
      res = (*emit)(userptr, escaped);
      if (res)
        return res;

      str++;
      continue;
    }

    res = (*emit)(userptr, *str++);
    if (res != 0)
      return res;
  }
  return 0;
}

#define EMIT_CHAR(x, c) do { int r = (*x->emit)(x->userptr, c); if (r) return JFSM_ERR_EMIT;} while(0)
#define EMIT_STR(x,s)   do { int r = _emit_str(x->emit, x->userptr, s); if (r) return JFSM_ERR_EMIT;} while(0)
#define EMIT_USR_STR(x,s)   do { int r = _emit_str_esc(x->emit, x->userptr, s); if (r) return JFSM_ERR_EMIT;} while(0)
#define EMIT_SEPARATOR(x) do { int r = _emit_separator(x); if (r) return JFSM_ERR_EMIT;} while(0)

/*#define WANT_ELEMENT(x)  do { if (x->state != JFSM_WANT_ELEMENT) return JFSM_ERR_SYN; } while(0)*/

#define WANT_ELEMENT(x)   ok_or_die(x->state == JFSM_WANT_ELEMENT) 
	/*return JFSM_ERR_SYN; } while(0)*/

#define END_ELEMENT(x) do { if (x->stack[x->sp] == JFSM_STACK_OBJECT) x->state = JFSM_WANT_MEMBER; } while(0)

static int _emit_separator(struct json_fsm *jfsm){

  if (jfsm->one_put){
    EMIT_STR(jfsm, " , ");
  } else {
    jfsm->one_put = 1;
  }

  return JFSM_EXIT_OK;
}

int jfsm_str_init (struct json_fsm **jfsm)
{
	strstream_s *strs;
	strstream_init(&strs);

	int r = jfsm_init(jfsm, emit_str, strs, 0);

	return r;
}

int jfsm_init(struct json_fsm **jfsm_pp, emit_json_char_fn emit, void *userptr, int flags)
{
  struct json_fsm *jfsm = (struct json_fsm*)calloc(1, sizeof(struct json_fsm));
  
  jfsm->state = JFSM_WANT_ELEMENT;
  jfsm->emit = emit;
  jfsm->flags = flags;
  jfsm->userptr = userptr;

  /* all zero is OK. the state in the stack is "root" */

  *jfsm_pp = jfsm;
  return 0;

}

int jfsm_str_free(struct json_fsm *jfsm, int free_string)
{
	if (free_string) {
		strstream_s *strs = (strstream_s*)
			jfsm->userptr;
		strstream_free(strs);

	} else {
		/* only free the pointer. */
		free(jfsm->userptr);
	}
	free(jfsm);
	return 0;
}

int jfsm_free(struct json_fsm *jfsm)
{
  free(jfsm);
  return 0;
}

int jfsm_object_push  (struct json_fsm *jfsm)
{

	ok_or_die_msg(jfsm->state == JFSM_WANT_ELEMENT, "state %d",
			jfsm->state);

		/*
  if (jfsm->state != JFSM_WANT_ELEMENT){
    alogt("Did not expect an object here.");
    ad_assert(0);
    return JFSM_ERR_SYN;
  }
  */

	ok_or_die_msg((jfsm->stack[jfsm->sp] != JFSM_STACK_ROOT) ||
			(jfsm->one_put == 0), "more of one element");

/*

  if (jfsm->stack[jfsm->sp] == JFSM_STACK_ROOT){
    if (jfsm->one_put){
      alogt("More than one element in root");
      ad_assert(0);
      return JFSM_ERR_SYN;
    }
  }
  */

	EMIT_SEPARATOR(jfsm);

	jfsm->state = JFSM_WANT_MEMBER;

	if (++jfsm->sp == MAX_JSON_DEPTH){
		return JFSM_ERR_DEPTH;
	}
	jfsm->stack[jfsm->sp] = JFSM_STACK_OBJECT;
	jfsm->one_put = 0;

	EMIT_STR(jfsm, "{ ");
	return 0;
}

int jfsm_object_pop   (struct json_fsm *jfsm)
{

	ok_or_die_msg(jfsm->state == JFSM_WANT_MEMBER, "state %d",
			jfsm->state);

	ok_or_die_msg(jfsm->stack[jfsm->sp] == JFSM_STACK_OBJECT,
			"stack %d", jfsm->stack[jfsm->sp]);

  	jfsm->sp--;

	if (jfsm->stack[jfsm->sp] != JFSM_STACK_OBJECT){
		jfsm->state = JFSM_WANT_ELEMENT;
	}
	jfsm->one_put = 1;

	EMIT_STR(jfsm, " } ");

	return 0;
}


int jfsm_array_push   (struct json_fsm *jfsm)
{
  if (jfsm->state != JFSM_WANT_ELEMENT){
    alogt("Did not expect an arrayhere.");
    ad_assert(0);
    return JFSM_ERR_SYN;
  }

  if (++jfsm->sp == MAX_JSON_DEPTH){
    return JFSM_ERR_DEPTH;
  }
  jfsm->stack[jfsm->sp] = JFSM_STACK_ARRAY;

  EMIT_SEPARATOR(jfsm);

  jfsm->one_put = 0;

  EMIT_STR(jfsm, "[ ");

  return 0;
}

int jfsm_array_pop    (struct json_fsm *jfsm)
{
  if (jfsm->state != JFSM_WANT_ELEMENT){
    return JFSM_ERR_SYN;
  }

  if (jfsm->stack[jfsm->sp] != JFSM_STACK_ARRAY){
    return JFSM_ERR_SYN;
  }
  jfsm->sp--;
  if (jfsm->stack[jfsm->sp] == JFSM_STACK_OBJECT){
    jfsm->state = JFSM_WANT_MEMBER;
  }
  jfsm->one_put = 1;
  
  EMIT_STR(jfsm, " ]");

  return 0;
}

int jfsm_member_array  (struct json_fsm *jfsm, const char *key)
{
	jfsm_member(jfsm, key);
	jfsm_array_push(jfsm);
	return 0;
}

int jfsm_member(struct json_fsm *jfsm, const char *key)
{
	ok_or_die_msg(jfsm->state == JFSM_WANT_MEMBER, "st %d",
			jfsm->state);
	/*
	if (jfsm->state != JFSM_WANT_MEMBER){
		alogw("Not possible a member here. key %s, state %d",
				key, jfsm->state);
		exit(1);
		return JFSM_ERR_SYN;
	}
	*/

	EMIT_SEPARATOR(jfsm);

	jfsm->state = JFSM_WANT_ELEMENT;
	jfsm->one_put = 0;

	EMIT_CHAR(jfsm, '"');
	EMIT_USR_STR(jfsm, key);
	EMIT_STR(jfsm, "\" : ");

	return JFSM_EXIT_OK;
}


int jfsm_value_int    (struct json_fsm *jfsm, int value)
{
  char ints[20];
  snprintf(ints, sizeof(ints), " %d ", value);
  WANT_ELEMENT(jfsm);
  EMIT_SEPARATOR(jfsm);
  EMIT_STR(jfsm, ints);
  END_ELEMENT(jfsm);
  return JFSM_EXIT_OK;
}

int jfsm_value_long   (struct json_fsm *jfsm, long value)
{
  char longs[20];
  snprintf(longs, sizeof(longs), " %ld ", value);
  WANT_ELEMENT(jfsm);
  EMIT_SEPARATOR(jfsm);
  EMIT_STR(jfsm, longs);
  END_ELEMENT(jfsm);
  return JFSM_EXIT_OK;
}

int jfsm_value_double (struct json_fsm *jfsm, double value)
{
  if (!isnormal(value)){
    value = 0;
  }
  char doubles[20];
  snprintf(doubles, sizeof(doubles), " %g ", value);
  WANT_ELEMENT(jfsm);
  EMIT_SEPARATOR(jfsm);
  EMIT_STR(jfsm, doubles);
  END_ELEMENT(jfsm);
  return JFSM_EXIT_OK;
}

int jfsm_value_json_val(struct json_fsm *jfsm, const char *json_val)
{
  WANT_ELEMENT(jfsm);
  EMIT_SEPARATOR(jfsm);
  EMIT_STR(jfsm, json_val);
  END_ELEMENT(jfsm);
  return JFSM_EXIT_OK;
}

int jfsm_value_string (struct json_fsm *jfsm, const char *vals)
{
  WANT_ELEMENT(jfsm);
  EMIT_SEPARATOR(jfsm);
  EMIT_CHAR(jfsm, '"');
  EMIT_USR_STR(jfsm, vals);
  EMIT_STR(jfsm, "\" ");
  END_ELEMENT(jfsm);
  return JFSM_EXIT_OK;
}

int jfsm_value_bool   (struct json_fsm *jfsm, int valb)
{
  WANT_ELEMENT(jfsm);
  EMIT_SEPARATOR(jfsm);
  EMIT_STR(jfsm, valb == 0 ? " false ": " true ");
  END_ELEMENT(jfsm);
  return JFSM_EXIT_OK;
}

int jfsm_value_null   (struct json_fsm *jfsm)
{
  WANT_ELEMENT(jfsm);
  EMIT_SEPARATOR(jfsm);
  EMIT_STR(jfsm, " null ");
  END_ELEMENT(jfsm);
  return JFSM_EXIT_OK;
}

int jfsm_close_and_finalize(struct json_fsm *jfsm, int insert_null)
{
	int res;

	while(jfsm->sp != 0){
		if (jfsm->stack[jfsm->sp] == JFSM_STACK_ARRAY) {
			res = jfsm_array_pop(jfsm);
			if (res != 0)
				return res;
		} else if (jfsm->stack[jfsm->sp] == JFSM_STACK_OBJECT) {
			res = jfsm_object_pop(jfsm);
			if (res != 0)
				return res;
		} else {
			return JFSM_ERR_SYN;
		}
	}

	return jfsm_finalize(jfsm, insert_null);
}

int jfsm_str_reset     (struct json_fsm *jfsm)
{
	upanic_m("Unsupported for now");
	return 0;
}

int jfsm_finalize     (struct json_fsm *jfsm, int insert_null)
{
  int res = JFSM_EXIT_OK;

  if (jfsm->state != JFSM_WANT_ELEMENT){
    alogt("Must return with want element");
    res = JFSM_ERR_SYN;
    goto end;
  }

  if (jfsm->one_put == 0){
    alogt("I must have at least one element");
    res = JFSM_ERR_SYN;
    goto end;
  }

  if (jfsm->sp != 0){
    alogt("composite object not closed");
    res = JFSM_ERR_SYN;
    goto end;
  }

  ad_assert(jfsm->stack[jfsm->sp] == JFSM_STACK_ROOT);

	if(insert_null) {
		EMIT_CHAR(jfsm, '\0');
	}

end:

  //ready to go for another object.
  jfsm->one_put = 0;
  jfsm->sp = 0;
  jfsm->state = JFSM_WANT_ELEMENT;
  jfsm->stack[jfsm->sp] = JFSM_STACK_ROOT;

  return res;
}




