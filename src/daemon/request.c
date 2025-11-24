/*
 *
 *
 *
 * request.c
 *
 * definition for the request.
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 
 *
 *
 */


#include "request.h"
#include "jsmn_user.h"

#define _LOG_MODULE "request"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;



int req_init(void)
{

	/* OK, we have now to add the commands which the json 
	 * interface understands.*/

	return 0;
}

#define REQ_CMD_KEY "cmd"
#define REQ_CMD_KEY_SZ (sizeof(REQ_CMD_KEY)-1)

int req_handle(char *req, size_t len)
{
	struct jsmn_val jval;
	int res = jsmn_val_alloc_mod(&jval, req, len);

	if (res != 0) {
		res = 1;
		goto end;
	}

	jsmn_dump_v(&jval);

	jsmn_root(&jval);

	struct jsmn_memento meme;
        jsmn_mark(&jval, &meme);


	const char *vals = NULL;
	/* first of all we get the command name. */
	res = jsmn_child_as_str(&jval, &meme,
			REQ_CMD_KEY, REQ_CMD_KEY_SZ, &vals);

	ok_or_goto_fail(res == 0);
	
	if (vals == NULL) {
		alogw("Invalid json");
		res = 1;
		goto end;
	}

	alogi("Wanted [%s] command", vals);


	/* I do not need the value any more. */
	res = 0;
fail:
end:
	jsmn_val_free(&jval);
	return res;
}
