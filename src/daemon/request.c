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

#include <unistd.h>

#include "request.h"
#include "jsmn_user.h"
#include "jfsm.h"
#include "json_api.h"

#define _LOG_MODULE "request"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;


static int req_handle(char *req, size_t len);

int req_init(void)
{

	/* OK, we have now to add the commands which the json 
	 * interface understands.*/
	alogi("calling the json api init");
	int res = json_api_init();

	return res; 
}

void req_close(void)
{
	alogi("finalizing the json api");
	json_api_free();
}

int handle_client(int cfd)
{

	/* let's get the message */
	int rd;
	uint32_t lreq;
	/* just to test */
#define MAX_BUFFER 512
	char msg[MAX_BUFFER];

	rd = read(cfd, &lreq, sizeof(lreq));
	if (rd == 0) {
		/* EOF */
		alogi("eof client, will get another.");
		goto close_client_and_do_another;
	}
	ok_or_goto_fail(rd == sizeof(lreq));
	alogi("I will read %d chars", lreq);
	ok_or_goto_fail(lreq <= MAX_BUFFER);


	rd = read(cfd, msg, lreq);
	ok_or_goto_fail(rd == lreq);

	alogi("this is the request %.*s", lreq, msg);

	rd = req_handle(msg, lreq);
	ok_or_goto_fail(rd == 0);

	/* let's try to send a json output */
	struct json_fsm *fsm;
	jfsm_str_init(&fsm);

	jfsm_object_push(fsm);
	jfsm_member(fsm, "hello");
	jfsm_value_string(fsm, "world");
	jfsm_object_pop(fsm);
	jfsm_finalize(fsm, 1);

	char *str = jfsm_json_str(fsm);
	size_t sz = jfsm_str_size(fsm);

	/* Also in this case I write the length first. */
	uint32_t lres = (uint32_t)sz;
	int wd;
	wd = write(cfd, &lres, sizeof(lres));
	ok_or_goto_fail(wd == sizeof(lres));

	alogi("Sending [%.*s] to client", lres, str);

	wd = write(cfd, str, sz);
	ok_or_goto_fail(wd == sz);


	jfsm_str_free(fsm, 1);

	/* all good */
	rd = 0;

fail:
close_client_and_do_another:
	close(cfd);

	return rd;

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
