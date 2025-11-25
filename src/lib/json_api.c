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


int add_user_handler(struct jsmn_val *json_in, struct json_fsm *json_out)
{
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


int json_api_init(void)
{

	s_hash_create(&priv.cmds, 10);
		
	alogi("json api initialized");
	return 0;
}

void json_api_free(void)
{


	s_hash_clear(priv.cmds, _free_handler);
	s_hash_destroy(priv.cmds);

	alogi("json api removed.");

}



