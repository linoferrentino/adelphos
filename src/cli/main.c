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
#include "cli.h"
/*#include "cli_api.h"*/

#define _LOG_MODULE "main"
#define MODULE_LEV ML_TRIVIAL
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
	cli_init();

	/* this is the start of the total command */
	char *total_command = NULL;


	size_t tot_size = 0;
	/* chained size, where to write next */
	size_t chsz = 0;

	/* I convert it to a byte buffer */
	

	alogi("Enter command: .quit exit");
	do {
		cmd = readline(tot_size == 0 ? "$ " : "> ");

		if (cmd == NULL) {
			if (tot_size == 0)
				break;
			/* there is a command to execute */
			goto exec_command;

		}
		alogt("got @%p line [%s]", cmd, cmd);


		if (strcmp(cmd, ".quit") == 0) {
			free(cmd);
			cmd = NULL; /* do not repeat cycle */
			if (tot_size == 0) {
				break;
			}
			goto exec_command;
			
		}
		
		size_t cur_size = strlen(cmd);

		if (cur_size == 0) {
			if (tot_size == 0)
				break;
			free(cmd);
			/* I do not want to repeat */
			cmd = NULL;
			goto exec_command;
				
		}


		/* maybe I have to add this to the batch of other commands
		 * */
		int continuation = 0;

		if (cmd[cur_size-1] == '\\') {
			/* this is a continuation */
			alogt("this is a continuation");

			/* I convert the slash in a space */
			cmd[cur_size-1] = '@';
			

			/* if this is the first line there is no point
			 * in copying */
			if (tot_size == 0) {
				alogt("this is the first");
				tot_size = cur_size;
				total_command = cmd;
				chsz = cur_size - 1;
				continue;
			}
			continuation = 1;

		}


		/* maybe I have to add the line */
		if (continuation || tot_size != 0 ) {

			
			/* -1 because there is the chaining char */
			if (continuation) {
				tot_size += cur_size - 1;
			} else {
				/* this is the last, I will add the '\0'*/
				tot_size += cur_size;
			}

			total_command = realloc(total_command,
					tot_size);

			ok_or_die(total_command != NULL);

			alogt("allocate %zd chained %zd on %c",
					tot_size, chsz,
					total_command[chsz]);

			ok_or_die(total_command[chsz] == '@');


			memcpy(total_command + chsz, cmd, cur_size);
			free(cmd);

			/*dump_payload((uint8_t*)total_command, tot_size);*/

			/*alogt("len cmd  [%.*s]",
					(int) tot_size, total_command);*/


			if (continuation) {
				/* -1 to cancel the '\\' */
				chsz += cur_size-1;
				continue;
			}

			total_command[tot_size-1] = '\0';

			/* this is not a continuation, I have to exec it */
			goto exec_command;

		}
		

		total_command = cmd;
		tot_size = cur_size;


exec_command:
		cli_cmd_exec(total_command, tot_size);

		add_history(total_command);
		alogt("Will free %p", total_command);
		free(total_command);
		tot_size = 0;


	} while (cmd != NULL);



	
#if 0
	while ((cmd = readline("> ")) != NULL) {
		if (strcmp(cmd, ".quit") == 0) {
			free(cmd);
			break;
		}
		/*cli_cmd_exec(cmd);*/


		/* if the line finishes with '\\' then it is a
		 * continuation line and we redo.*/
		size_t cur_size = strlen(cmd);



		alogi("processing command %s", cmd);
		add_history(cmd);


		free(cmd);
	}
#endif

	/*cli_end();*/

	clear_history();

	/*rc = ad_close();*/
	
	/*
fail:
*/
	cli_end();
	return rc;
}









