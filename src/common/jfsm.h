/*
 *
 * jfsm.h A finite state machine json producer.
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 
 *
 *  All functions return 0 on success.
 */


#ifndef JFSM_H
#define JFSM_H

#ifdef __cplusplus
extern "C" {
#endif


/** we have a FSM (Finite State Machine) which will output characters as soon
 * as they are ready
 */

#define JFSM_EXIT_OK 0
#define JFSM_ERR_SYN -1
#define JFSM_ERR_VAL -2
  /* the emitter has issued an error*/
#define JFSM_ERR_EMIT -3
#define JFSM_ERR_DEPTH -4


typedef int (*emit_json_char_fn)(void *userptr, int jsonc);

struct json_fsm;

/* For testing purposes, just emit to standard output*/
int emit_stdout(void *userptr, int jsonc);

int emit_stderr(void *userptr, int jsonc);

/* lots of whitespace, new lines... etc*/
#define JFSM_EMIT_PRETTY  0x1
/* no white space at all.*/
#define JFSM_EMIT_COMPACT 0x2

int jfsm_init          (struct json_fsm **jfsm, emit_json_char_fn emit,
    void *userptr, int flags);

/* init the builder with a string output. The emitter is fixed*/
int jfsm_str_init      (struct json_fsm **jfsm);

char *jfsm_json_str(struct json_fsm *jfsm);
size_t jfsm_str_size(struct json_fsm *jfsm);



int jfsm_free          (struct json_fsm *jfsm);

int jfsm_str_free(struct json_fsm *jfsm, int free_string);

int jfsm_object_push   (struct json_fsm *jfsm);
int jfsm_object_pop    (struct json_fsm *jfsm);
int jfsm_array_push    (struct json_fsm *jfsm);
int jfsm_array_pop     (struct json_fsm *jfsm);

int jfsm_member        (struct json_fsm *jfsm, const char *key);
int jfsm_value_int     (struct json_fsm *jfsm, int value);
int jfsm_value_long    (struct json_fsm *jfsm, long value);
int jfsm_value_double  (struct json_fsm *jfsm, double value);
int jfsm_value_string  (struct json_fsm *jfsm, const char *vals);
/* 0 = false, anything else true*/
int jfsm_value_bool    (struct json_fsm *jfsm, int valb);
int jfsm_value_null    (struct json_fsm *jfsm);
int jfsm_value_json_val(struct json_fsm *jfsm, const char *json_val);

int jfsm_member_array  (struct json_fsm *jfsm, const char *key);


int jfsm_finalize      (struct json_fsm *jfsm, int insert_null);

int jfsm_str_reset     (struct json_fsm *jfsm);

int jfsm_close_and_finalize(struct json_fsm *jfsm, int insert_null);

#ifdef __cplusplus
}
#endif


#endif
