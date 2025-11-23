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
	return 0;
}

#define REQ_CMD_KEY "cmd"
#define REQ_CMD_KEY_SZ sizeof(REQ_CMD_KEY)

int req_handle(char *req, size_t len)
{
	struct jsmn_val jval;
	int res = jsmn_val_alloc_mod(&jval, req, len);

	if (res != 0) {
		res = 1;
		goto end;
	}

	jsmn_dump_v(&jval);

	/* first of all we get the command name. */
	const char *cmd = jsmn_val_member_str(&jval,
			REQ_CMD_KEY, REQ_CMD_KEY_SZ, NULL);
	
	if (cmd == NULL) {
		alogw("Invalid json");
		res = 1;
		goto end;
	}

	alogi("Wanted %s command", cmd);


	/* I do not need the value any more. */
end:
	jsmn_val_free(&jval);
	return 1;
}
