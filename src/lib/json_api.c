/*
 *
 *
 * json_api
 *
 * implementation file for the json API
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 
 *
 *
 *
 *
 */

#include <stdlib.h>
#include <string.h>
#include "jsmn_user.h"
#include "jfsm.h"
#include "hash.h"

#include "json_api.h"
#define _LOG_MODULE "json_api"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;

struct jsmn_val;
struct json_fsm;

/* here I have the table of the commands */
static struct _json_api_priv_s
{

	s_hash_t *cmds;


} priv;


/* 
 *
 * This is the prototype for an API function. 
 *
 *
 * The caller is responsabile to free the memory
 *
 * json_out is initialized, and it is returned not finalized.
 *
 * */
typedef int (*json_api_f)(struct jsmn_val *json_in, 
		struct json_fsm *json_out);


static int _add_user_handler(struct jsmn_val *json_in,
		struct json_fsm *json_out)
{
	alogt("_add_user_handler called");

	jfsm_member(json_out, "hello_add");
	jfsm_value_string(json_out, "world! Città € euro!");
	/*jfsm_value_string(json_out, "world!");*/

	return 0;
}


/*
 *
 * The handler for a json command
 *
 * */
struct _json_api_cmd_hdl_s
{
	/* the command identifier as a string */
	const char *cmd;

	json_api_f f_hndl;

	/* various flags.*/
	unsigned needs_authorization:1;

	/* can be paged? */
	/* ... */
};

static void _free_handler(void *ptr)
{
	if (ptr == NULL)
		return;

	struct _json_api_cmd_hdl_s *hdlrp =
		(struct _json_api_cmd_hdl_s*)ptr;

	free((char*)hdlrp->cmd);
	free(hdlrp);
}

/* adds an handler to the hash table. */
static void _add_handler(const char *cmd, json_api_f hndl)
{
	/* Create the table for the commands */
	struct _json_api_cmd_hdl_s *phndl;

	phndl = (struct _json_api_cmd_hdl_s*)
		malloc(sizeof(struct _json_api_cmd_hdl_s));
	phndl->cmd = strdup(cmd);
	phndl->f_hndl = hndl;

	s_hash_put_new(priv.cmds, phndl);

}


#define REQ_CMD_KEY "cmd"
#define REQ_CMD_KEY_SZ (sizeof(REQ_CMD_KEY)-1)

/* The dispatcher is responsible to choose the right handler */
static int _json_api_dispatcher(struct jsmn_val *jval, 
				struct json_fsm *jfsm)
{
	int res;

	/* Only if trivial ZZZ */
	jsmn_dump_v(jval);

	jsmn_root(jval);

	struct jsmn_memento meme;
        jsmn_mark(jval, &meme);


	const char *vals = NULL;
	/* first of all we get the command name. */
	res = jsmn_child_as_str(jval, &meme,
			REQ_CMD_KEY, REQ_CMD_KEY_SZ, &vals);

	ok_or_goto_fail(res == 0);
	
	if (vals == NULL) {
		alogw("Invalid json");
		res = 1;
		goto end;
	}

	res = 1;
	alogi("Wanted [%s] command", vals);

	/* get the handler */
	struct _json_api_cmd_hdl_s *phndl =
		(struct _json_api_cmd_hdl_s*)
		s_hash_get(priv.cmds, vals);

	if (phndl == NULL) {
		alogw("unrecognized command %s", vals);
		goto end;
	}

	
	
	/* I have to get the payload and pass the object there */
	/* Create the json out and start the object creation. */
	res = (phndl->f_hndl)(jval, jfsm);

	

fail:
end:
	/* I do not free the string, as it is bound to be given to
	 * the outside.*/
	return res;

}


int json_api_proc(struct byte_buf_s *in, struct byte_buf_s *out)
{
	/* Here I have to call the function handler and return the
	 * result as a size terminated string. */

	/* First of all we have to parse the string. */

	struct jsmn_val jval;
	int res = jsmn_val_alloc_mod(&jval, byte_buf_str(in), in->sz);

	


	/* I create a handler payload where the handler might give
	 * its output */
	struct json_fsm *jfsm;
	jfsm_str_init(&jfsm);

	/* the json starts with an object */
	jfsm_object_push(jfsm);

	/* I did not get a json */
	if (res != 0) {
		alogt("?hwat");
		jfsm_member(jfsm, "error");
		jfsm_value_string(jfsm, "invalid json");
		goto end;
	}

	jfsm_member(jfsm, "payload");
	jfsm_object_push(jfsm);



	/* get the handler for this command .*/

	res = _json_api_dispatcher(&jval, jfsm);
	ok_or_goto_fail(res == 0);


#if 0
#define hello_from_json_api "hello from json api"

	/* Just a test */
	out->sz = sizeof(hello_from_json_api) - 1 ;
	out->str = realloc(out->str, sizeof(hello_from_json_api));
	strcpy(out->str, hello_from_json_api);

#else

	/* this ends the payload*/
	jfsm_object_pop(jfsm);


fail:
end:

	/* Now I pack the response */
	jfsm_member(jfsm, "res");
	jfsm_value_int(jfsm, res);

	/* this ends the root */
	jfsm_object_pop(jfsm);


#endif

	/* OK, now I can emit the json */
	res = jfsm_finalize(jfsm, 0);

	/* very bad: this is a coding error  */
	ok_or_die(res == 0);

	byte_buf_set(out, jfsm_json_str(jfsm), jfsm_str_size(jfsm));

	/* I do not need the value any more. */
	jsmn_val_free(&jval);
	/* but I do need the string!*/
	jfsm_str_free(jfsm, 0);

	return res;
}

int json_api_init(void)
{

	s_hash_create(&priv.cmds, 10);
	
	/* I add here all the commands that the json api understands. */
	_add_handler("add_user", _add_user_handler);
		
	alogi("json api initialized");
	return 0;
}

void json_api_free(void)
{


	s_hash_clear(priv.cmds, _free_handler);
	s_hash_destroy(priv.cmds);

	alogi("json api removed.");

}



