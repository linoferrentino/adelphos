/*
 *
 * main file for the adelphos daemon
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>

#include "jfsm.h"


#define MY_SOCK_PATH "/tmp/adelphos"
#define LISTEN_BACKLOG 50

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)


#define _LOG_MODULE "daemon-main"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;

#include "adelphos_config.h"

#define dump_payload(p, len) dump_payload_dbg( \
		__FILE__, __LINE__, p, len)

void dump_payload_dbg(const char *file, int line, const char *_p, int len)
{
  fprintf(stderr, "Dump @ %s:%d %d bytes (%p)\n", file, line, len, _p);
  char buf[128];
  int i, j;
  const unsigned char *p = (const unsigned char *)_p;

  for (i = 0; i < len;) {
    memset(buf, ' ', sizeof(buf));
    int res = sprintf(buf, "%5d: ", i);
    buf[res] = ' ';
    for (j=0; j < 16 && i < len; i++, j++){

      int dec  = p[i] / 16;
      int unit = p[i] % 16;
      buf[7 + j*3] = dec < 10 ? '0' + dec : 'a' + (dec-10);
      buf[8 + j*3] = unit < 10 ? '0' + unit : 'a' + (unit-10);
      buf[56 + j] = isprint(p[i]) ? p[i] : '.';

    }
    buf[72] = '\0';
    fprintf(stderr, "%s\n", buf);
  }
}


int main(int argc, const char *argv[])
{

	int                 sfd, cfd;
	socklen_t           peer_addr_size;
	struct sockaddr_un  my_addr, peer_addr;


	alogi("adelphos daemon version " ADELPHOS_TAG );

	/* maybe a socket was closed */
	if (unlink(MY_SOCK_PATH) == -1)
		handle_error("unlink");

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	ok_or_goto_fail(sfd != 0);

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, MY_SOCK_PATH,
			sizeof(my_addr.sun_path) - 1);

	if (bind(sfd, (struct sockaddr *) &my_addr,
				sizeof(my_addr)) == -1)
		handle_error("bind");

	if (listen(sfd, LISTEN_BACKLOG) == -1)
		handle_error("listen");

	/* Now we can accept incoming connections one
	   at a time using accept(2). */

	peer_addr_size = sizeof(peer_addr);
cycle_accept:
	alogi("Accept a client, here");


	cfd = accept(sfd, (struct sockaddr *) &peer_addr,
			&peer_addr_size);
	if (cfd == -1)
		handle_error("accept");

	/* Code to deal with incoming connection(s)... */


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
	

	/* OK, now we sent back the answer.*/

	/* double end of line to finish answer */
	/*
	time_t now = time(NULL);
	char* answer = "hello %.*s time is %d\n";
	char buf_out[100];
	rd = snprintf(buf_out, 100, answer, rd, msg, now);
	ok_or_goto_fail(rd > 0);

	alogi("Sending [%s] to client, len %d", buf_out, rd);
	*/
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
/*	dump_payload(str, sz);*/

	wd = write(cfd, str, sz);
	ok_or_goto_fail(wd == sz);

	/*wd = write(cfd, buf_out, rd);
	ok_or_goto_fail(wd == rd);*/


	jfsm_free(fsm);

close_client_and_do_another:
	close(cfd);
	goto cycle_accept;


	if (close(sfd) == -1)
		handle_error("close");

	if (unlink(MY_SOCK_PATH) == -1)
		handle_error("unlink");




fail:
	return 0;
}


