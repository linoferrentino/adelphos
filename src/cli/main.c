//#include "EmbeddableWebServer.h"


#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>


/*#include "acrypt.h"*/
#include "misc.h"
#include "adelphos.h"
#include "cli.h"

#define _LOG_MODULE "main"
#define MODULE_LEV ML_INFO
#include "alog.h"
LOG_MODULE_IMP;




int main(int argc, const char *argv[])
{
	int rc = 0;

	alogi("Adelphos v0.1. Copyright (C) 2025. Lino Ferrentino, <lino.ferrentino@gmail.com>");
	alogi("This program is given in the hope to be useful, but it comes with NO WARRANTY.");

	char *cmd;
	using_history();

	rc = ad_init();
	ok_or_goto_fail(rc == AD_OK);

	cli_init();
	
	while ((cmd = readline("> ")) != NULL) {
		alogi("processing command %s", cmd);
		add_history(cmd);
		if (strcmp(cmd, "quit") == 0) {
			free(cmd);
			break;
		}
		cli_cmd_exec(cmd);
		free(cmd);
	}

	cli_end();

	clear_history();

	rc = ad_close();
	
fail:
	return rc;
}


#if 0
int main_1(int argc, const char *argv[])
{

	int r = -1;
	alogi("Adelphos v0.1. Copyright (C) 2025. Lino Ferrentino, <lino.ferrentino@gmail.com>");
	alogi("This program is given in the hope to be useful, but it comes with NO WARRANTY.");
	//r = _read_conf_general(argc, argv);
	//ok_or_goto_fail(r == 0);

	uint8_t buf[10];
	acrypt_random_bytes(buf, 10);
	dump_payload(buf, 10);

	alogi("Success");

	

	return r;
}
#endif



int main_2(int argc, char **argv)
{
	alogi("Adelphos v0.1. Copyright (C) 2025. Lino Ferrentino, <lino.ferrentino@gmail.com>");
	alogi("This program is given in the hope to be useful, but it comes with NO WARRANTY.");
	
	/*
	utStart();
	adDatabaseStart();
	utStartPersistence("adelphos_db", true, true);
	adWorld my_world;
	my_world = adWorldAlloc();
	*/


	/* let's try to add an adelphos, here. */
	/*adAdelphos lino;
	lino = adAdelphosCreate(my_world, utSymCreate("lino"));*/

/*
	utTransactionComplete(true);
	*/

	//utCompactDatabase();

	//utManager();

	/*
	utStopPersistence();
	*/

	return 0;
}

#if 0
int main__old(int argc, char* argv[]) {
	return acceptConnectionsUntilStoppedFromEverywhereIPv4(NULL, 8080);
}

struct Response* createResponseForRequest(const struct Request* request, struct Connection* connection) {
	if (0 == strcmp(request->pathDecoded, "/welcome")) {
		return responseAllocHTML("<html><body><marquee><h1>Welcome to my home page</h1></marquee></body></html>");
	}
	if (0 == strcmp(request->pathDecoded, "/status/json")) {
		static const char* statuses[] = { ":-)", ":-(", ":-|" };
		int status = rand() % (sizeof(statuses) / sizeof(*statuses));
		/* There is also a family of responseAllocJSON functions */
		return responseAllocWithFormat(200, "OK", "application/json", "{ \"status\" : \"%s\" }", statuses[status]);
	}
	if (0 == strcmp(request->pathDecoded, "/100_random_numbers")) {
		struct Response* response = responseAllocHTML("<html><body><h1>100 Random Numbers</h1><ol>");
		for (int i = 1; i <= 100; i++) {
			heapStringAppendFormat(&response->body, "<li>%d</li>\n", rand());
		}
		heapStringAppendString(&response->body, "</ol></body></html>");
		return response;
	}
	/* Serve files from the current directory */
	if (request->pathDecoded == strstr(request->pathDecoded, "/files")) {
		return responseAllocServeFileFromRequestPath("/files", request->path, request->pathDecoded, ".");
	}
	return responseAlloc404NotFoundHTML("What?!");
}
#endif
