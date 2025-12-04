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
#include <stdlib.h>

#include "request.h"
#include "jsmn_user.h"
#include "jfsm.h"
#include "json_api.h"

#define _LOG_MODULE "request"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;




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


	byte_buf_def(out_s);

	/* I start with at least 4 bytes */
	byte_buf_def_n(in_s, sizeof(uint32_t));
	

redo_read:

	/* the storage for lreq is already acquired */
	rd = read_all(cfd, (uint8_t*)&in_s.bsz->cur, sizeof(uint32_t));
	if (rd == 0) {
		alogi("EOF client.");
		goto close_client_and_do_another;
	}
	ok_or_goto_fail(rd == 0);

	lreq = in_s.bsz->cur;
	alogi("I will read %d chars", lreq);

	byte_buf_wipe_ensure((&in_s), lreq);

	rd = read_all(cfd, &in_s.bsz->buf[0], in_s.bsz->cur);
	ok_or_goto_fail(rd == 0);

	alogi("this is the request %.*s", lreq, byte_buf_str(&in_s));
	

	rd = json_api_proc(&in_s, &out_s);

	/*rd = req_handle(msg, lreq);*/
	ok_or_goto_fail(rd == 0);

	alogi("Sending [%.*s] to client", out_s.bsz->cur,
		       	byte_buf_str(&out_s));

	int wd;
	wd = write_all(cfd, (uint8_t*)&out_s.bsz->cur,
			sizeof(out_s.bsz->cur));
	ok_or_goto_fail(wd == 0);


	/* write All! */
	wd = write_all(cfd, &out_s.bsz->buf[0], out_s.bsz->cur);
	ok_or_goto_fail(wd == 0);


	goto redo_read;


	/* all good */
	rd = 0;

fail:
close_client_and_do_another:
	/* I am done with the out string */
	byte_buf_free_loc(out_s);
	byte_buf_free_loc(in_s);

	close(cfd);

	return rd;

}

#define REQ_CMD_KEY "cmd"
#define REQ_CMD_KEY_SZ (sizeof(REQ_CMD_KEY)-1)

#if 0
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
#endif
