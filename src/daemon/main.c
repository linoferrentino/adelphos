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
#include <poll.h>
#include <signal.h>


#include "jfsm.h"
#include "request.h"


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

static int interrupt_flag = 0;

static void _daemon_sigint(int signal)
{
	alogi("Got control-c, exiting");
	interrupt_flag = 1;
}


int main(int argc, const char *argv[])
{

	int                 sfd, cfd;
	socklen_t           peer_addr_size;
	struct sockaddr_un  my_addr, peer_addr;
	int res;
	srandom(time(NULL));


	alogi("adelphos daemon version " ADELPHOS_TAG );


        struct sigaction act;
        memset(&act, 0, sizeof(act));
        act.sa_handler = _daemon_sigint;
        sigaction(SIGINT, &act, NULL);


	/* Initialization of the requests. */
	req_init();

	/* maybe a socket was closed. Here a failure is not
	 * a fatal error*/
	unlink(MY_SOCK_PATH);

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd < 0) {
		handle_error("socket");
	}

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, MY_SOCK_PATH,
			sizeof(my_addr.sun_path) - 1);

	if (bind(sfd, (struct sockaddr *) &my_addr,
				sizeof(my_addr)) == -1)
		handle_error("bind");

	if (listen(sfd, LISTEN_BACKLOG) == -1)
		handle_error("listen");


	peer_addr_size = sizeof(peer_addr);

	/* OK, let's do a cycle. */
	struct pollfd pollfd[1];

	pollfd[0].fd = sfd;
	pollfd[0].events = POLLIN;


cycle_accept:
	alogi("Accept a client, here");
redo_cycle_accept:
	if (interrupt_flag != 0) {
		alogi("requested end");
		goto end;
	}

	res = poll(pollfd, 1, 1000);
	if (res < 0) {
		goto end;
	}

	if (res == 0) {
		goto redo_cycle_accept;
	}


	cfd = accept(sfd, (struct sockaddr *) &peer_addr,
			&peer_addr_size);
	if (cfd == -1)
		handle_error("accept");


	/* I get the request, this for now is here, later
	 * we might have a separate thread. */
	res = handle_client(cfd);
	ok_or_goto_fail(res == 0);

	
	goto cycle_accept;

end:
fail:

	if (close(sfd) == -1)
		handle_error("close");

	if (unlink(MY_SOCK_PATH) == -1)
		handle_error("unlink");

	req_close();


	return res;
}


