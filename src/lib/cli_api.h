/*
 *
 *
 * cli_api
 *
 * This is the declaration of the cli api which is
 * the interface using the command line.
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 
 *
 */

#ifndef _cli_api_h
#define _cli_api_h

/* 
 *
 * You initialize the cli api and then call the function
 *
 * The cli is not intended to be run by multiple threads. 
 *
 * (if needed we can later add a thread safety mechanism)
 *
 *
 * */
int cli_api_init(void);


/*
 * This will execute a command which is given to the command line.
 *
 * The command will return a string.
 */
int cli_exec(const char *cmd);

/* 
 * the login is particular type of command because it creates
 * a security context which is then passed to all the commands
 * which are issued
 *
 * We can have here a mfa challenge, like asking the user
 * to give an OTP code.
 *
 * */
int cli_login(const char *user, const char *password);



void cli_api_free(void);




#endif
