/*
 *
 *
 * implementation file for the adelphos CLI interface.
 *
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 *
 *
 */

#include <stdint.h>
/* we do not heed threads. */
/*#include <pthread.h>*/
#include <string.h>

#include "cli.h"

#include "adelphos.h"
/*#include "json_api.h"*/
#include "misc.h"


#define _LOG_MODULE "cli"
#define MODULE_LEV ML_DEBUG
#include "alog.h"
LOG_MODULE_IMP;


struct cli_output_s;
struct jsmn_val;



/*  this is the low level interface */
/*typedef int8_t (*cli_cmd_fn_t)(struct adelphos_param_s *ap);*/


/* the function will take the payload, just as the entry point to
 * the library.*/
#if 0
typedef int8_t (*cli_cmd_fn_t)(struct jsmn_val *payload);

  
/* this is the struct to execute a command */
struct cmd_exec {

	/* this is the string identifier for the command */
	const char *cmd;

	/* the function to execute the command */
	cli_cmd_fn_t exe_fn;

	/* this is the number of params that the function wants,
	 * they are positional, and there are no default.*/
	/*uint8_t n_pars;*/

};
#endif

#if 0
static int8_t _cli_create_adelphos_cmd(struct adelphos_param_s *ap);
static int8_t _cli_create_l0g_cmd(struct adelphos_param_s *ap);

static struct cmd_exec priv_cmds[] = {
	{ "cad", _cli_create_adelphos_cmd, AD_CREATE_ADELPHOS_SIZE },
	{ "cl0", _cli_create_l0g_cmd, AD_CREATE_L0_PARS_SIZE },
	/* end of the table */
	{ NULL, NULL }

};
#endif

static struct {

	uint8_t init;

	pthread_mutex_t mux;

	/*struct cmd_exec *cmd_defs;*/

	/* 
	 *
	 * I have here the session identifier,
	 *
	 * until then every request fails, as I do not
	 * have the token.
	 *
	 * */


} priv = {
	.init = 0,
	/*cmd_defs = &priv_cmds[0]*/
};


static const char* _cli_kv_arg(void *param, const char *key);
static const char* _cli_get_cmd(void *param);

static struct ad_args_provider_s _my_prov = {
	.kv_get = _cli_kv_arg,
	.get_cmd_f = _cli_get_cmd
};

static const char* _cli_kv_arg(void *param, const char *key)
{
	alogt("Requested key %s from pointer %p", key, param);

	return "lino";
}


/* gets the command */
const char* _cli_get_cmd(void *param)
{
	alogt("Requested the command");
	return "add_user";
}


/* this struct holds the marshalled data which is used to parse
 * the linear text given by the cli.*/
struct marshalled_data_s
{
	/* I have the dictionary, string -> id */

	/* this is the command */
	const char *cmd;

	/* a session identifier */

	/* then a list of verbs  */

	/* a list of flags flagged true */

	/* a list of flags flagged false */

};


/* this function will modify the string, so be careful */
static int _create_marshalled_data(char *cmd, size_t sz)
{

	char *str1;
	char *saveptr1;
	char *token;
	int j;

	for (j = 1, str1 = cmd; ; j++, str1 = NULL) {
		token = strtok_r(str1, " \t\n\r", &saveptr1);
		if (token == NULL)
			break;

		alogt("token %d -> %s", j, token);

		/*
		for (str2 = token; ; str2 = NULL) {
			subtoken = strtok_r(str2, argv[3], &saveptr2);
			if (subtoken == NULL)
				break;
			printf("\t --> %s\n", subtoken);
		}
		*/
	}
	



	return 0;
}



int cli_cmd_exec(char *cmd, size_t sz)
{
	alogi("executing command [%.*s]", (int)sz, cmd);
	dump_payload((uint8_t*)cmd, sz);


	ad_res res = _create_marshalled_data(cmd, sz);
	ok_or_goto_fail(res == 0);


	/* I have to transform this in a json value */

	/* OK, suppose now that the command is a add_user command.*/

	/* how will I get it? */

	/* the key is to transform a linear view of the text
	 * in a hierarchical one */

	struct adelphos_in_s in;
	in.a_prov = &_my_prov;
	in.param = NULL;

	res = ad_exec_marshall(&in);


fail:
	return res == AD_OK ? 0 : -1;
}



#if 0
/* executes a command, we will tokenize the command. */
int cli_cmd_exec(char *cmd)
{
	int res = 0;
	/*pthread_mutex_lock(&priv.mux);*/
/*
	struct vecptr *cmdline;
	vptr_init(&cmdline);
	*/
	/*
	struct adelphos_param_s ap;
	*/

	char **tk_it = &ap.gp.params[0];

	alogx("Executing line %s it is %p", cmd, tk_it);
	/* strtok is not thread safe, but we have a mutex here.*/

	char *token = strtok(cmd, " ");
	while(token != NULL) {
		/*vptr_push_tail(cmdline, token);*/
		*tk_it = token;
		
		alogx("Adding token %s on %p", *tk_it, tk_it);
		tk_it++;
		alogx("now iterator is %p", tk_it);

		token = strtok(NULL, " ");
	}

	if (tk_it == &ap.gp.params[0]) {
		alogw("empty string?");
		/* in any case it is a success */
		goto end;

	}

	char *cmd_verb = ap.cmd_verb;

	/* first token is a command, let's search in the table. */
	alogt("Executing command %s", cmd_verb);

	
	struct cmd_exec *cmd_entry = &priv.cmd_defs[0];
	while (cmd_entry->cmd != NULL) {
		if (strcmp(cmd_entry->cmd, cmd_verb) != 0) {
			cmd_entry++;
			continue;
		}

		break;
	}

	if (cmd_entry->cmd == NULL ) {
		alogw("invalid command %s", cmd_verb);
		goto end;
	}

	/* I found the command, do I have enough params? */
	int npars = tk_it - &ap.gp.params[0];
	/* I have to remove the command */
	npars--;
	alogt("I have got %d parms", npars);

	if (npars < cmd_entry->n_pars) {
		alogw("not enough parameters wanted %d got %d",
				cmd_entry->n_pars, npars);
		goto end;
	}


	/* executing the command ! */
	alogt("Will exec cmd %s", cmd_verb);
	res = (*cmd_entry->exe_fn)(&ap);


end:
	/*pthread_mutex_unlock(&priv.mux);*/
	return res;
}
#endif

void cli_init(void)
{
	/*pthread_mutex_init(&priv.mux, NULL);*/
	if (priv.init != 0) {
		return;
	}

	/*json_api_init();*/
	ad_init(1, NULL);

	priv.init = 1;
}

void cli_end(void)
{
	/*pthread_mutex_destroy(&priv.mux);*/
	if (priv.init == 0) {
		return;
	}

	/*json_api_free();*/
	ad_close();

	priv.init = 0;
}

/*
 * in reality first I have to create a l0_group
 *
 * let's distinguish between the administrative function
 * and the GUI function.
 *
 */
#if 0
static int8_t _cli_create_l0g_cmd(struct adelphos_param_s *ap)
{
	/* the parameters are 
	 *
	 * index 0 is the command.
	 *
	 * 1 city_id, 2 name, 3 pass_adult, 4 pass_minor
	 * 5 cl for adults, 6 cl for minors*/
#if 0

	if (vptr_size(cmdline) < 7) {
		alogw("I need six parameters");
		res = 
	}
#endif

	/* I simply call the function. */
	ad_res res = ad_l0_group_create(ap);

	return res;
}


static int8_t _cli_create_adelphos_cmd(struct adelphos_param_s *ap)
{
	alogt("I will create an adelphos, here.");




	return 0;
}
#endif
