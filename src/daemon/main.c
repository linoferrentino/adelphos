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


#define MY_SOCK_PATH "/tmp/adelphos"
#define LISTEN_BACKLOG 50

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)


#define _LOG_MODULE "daemon-main"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;

#include "adelphos_config.h"

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
	char msg[50];
	rd = read(cfd, msg, 50);
	ok_or_goto_fail(rd >= 0);
	alogi("Read %d chars %.*s", rd, rd, msg);

	if (rd == 0) {
		/* EOF */
		alogi("eof client, will get another.");
		goto close_client_and_do_another;
	}

	/* OK, now we sent back the answer.*/

	/* double end of line to finish answer */
	time_t now = time(NULL);
	char* answer = "hello %.*s time is %d\n";
	char buf_out[100];
	rd = snprintf(buf_out, 100, answer, rd, msg, now);
	ok_or_goto_fail(rd > 0);

	alogi("Sending [%s] to client, len %d", buf_out, rd);

	int wd;
	wd = write(cfd, buf_out, rd);
	ok_or_goto_fail(wd == rd);

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


