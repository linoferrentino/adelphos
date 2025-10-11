/*
 * declaration file for the adelphos CLI interface.
 *
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 * 
 *
 */

#ifndef _cli_h_
#define _cli_h_



/* 
 *
 * the cli interface takes a string and performs command
 * on the adelphos db.
 *
 * the result for now it is a text, later we could have a json response.
 *
 *
 *  */

/* you should call the cli_init interface to set the mutex.*/
void cli_init(void);

/* call this at the end to release the mutex. */
void cli_end(void);

#define CMD_EXEC_UNDEFINED_CMD 1
#define CMD_NOT_ENOUGH_PARS    2

/* 
 * this function is thread safe. 
 *
 * return value AD_OK in case of success, several other errors 
 * defined in adelphos.h if anything goes wrong.
 *
 * adelphos' error code are negative, here we have positive ones.
 *
 * cmd could be modified, but it is not freed
 *
 * */
int cli_cmd_exec(char *cmd);


#endif
