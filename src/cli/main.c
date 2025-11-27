/*
 *
 *
 * adelphos CLI interface
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
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>


#include "misc.h"
/*
 * I do not include adelphos directly, it is initialized by
 * an api.
#include "adelphos.h"
#include "cli.h"
*/
#include "cli_api.h"

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
/*
	rc = ad_init(0, NULL);
	ok_or_goto_fail(rc == AD_OK);

	cli_init();
	*/
	
	while ((cmd = readline("> ")) != NULL) {
		if (strcmp(cmd, "quit") == 0) {
			free(cmd);
			break;
		}
		/*cli_cmd_exec(cmd);*/

		alogi("processing command %s", cmd);
		add_history(cmd);


		free(cmd);
	}

	/*cli_end();*/

	clear_history();

	/*rc = ad_close();*/
	
	/*
fail:
*/
	return rc;
}









