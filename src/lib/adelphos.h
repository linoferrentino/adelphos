/*
 *
 * adelphos.h
 *
 * main library header file.
 *
 * This is the C low level interface.
 *
 * End users should probably use the json_api or the cli_api
 * to access adelphos.
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 *
 *
 */

#ifndef _adelphos_h_
#define _adelphos_h_

#include <stdint.h>

/*
 *
 * adelphos library is NOT thread safe (at least in this code)
 *
 * The API here is low level and should be accessed only by a thread
 * at a time.
 *
 * The adelphos executable can act interactively or as a web server,
 * but not both. 
 *
 * We could design a multiprocess lock for the DB, but in that case
 * probably a real multi user DB like postgreSQL should be used.
 *
 *
 * Adelphos does not speak and understand JSON at this level.
 *
 * Communication is done using normal c structs.
 *
 * 
 *
 * */


#include "ad_object.h"
#include "ad_errors.h"
#include "ad_authenticate.h"



/*
 * results from the adelphos library are simply 8-bit integers.
 */
typedef int8_t ad_res;

/* tells adelphos that this object can be garbage collected. */
ad_res adob_gc(ad_hndl_t a_handle);

/*
ad_res 
*/

struct ad_args_provider_s;

/* an interface that gets arguments as list of objects or other elements */
struct ad_list_args_provider_s;

/* 
 *
 * One function will take a key and gets a value 
 *
 * */
typedef const char* (*kv_get_f_t)(void *param, const char *key);

/*
 *
 * The type of the function which gets the command.
 * */
typedef const char* (*get_cmd_f_t)(void *param);

/* Another function can get a key and a nested object, that
 * can be another kv provider or an array, I have to know
 * the format of the parsed object   */
typedef struct ad_args_provider_s (*kv_get_sub_kv)(void *param,
		const char *key);

struct ad_args_provider_s
{

	kv_get_f_t kv_get;
	get_cmd_f_t get_cmd_f;


};


/* 
 *
 * structure that holds all the possible parameters for an
 * adelphos command.
 *
 *
 * In this way I can pass parameters from different modalities,
 *
 * text files, binary data, json entities.
 *
 * */
struct adelphos_in_s
{
	struct ad_args_provider_s *a_prov;

	/* an opaque pointer which implementors can use to get
	 * the key value parameters. */
	void *param;
};



/* This to test the args provider */
ad_res add_user_marshall(struct adelphos_in_s *in);


/* this is the generic function to call adelphos from a marshalled data */
ad_res ad_exec_marshall(struct adelphos_in_s *in);


/* 
 * this is the currency type, currency, like in real life
 * is countable, not a float, rounded to the cent
 *
 * */
typedef uint64_t cur_t;





/* I can initialize the adelphos library with a transient db or a normal
 * db, but in that case if it exists it is opened, otherwise it is
 * created from scratch.  */

/* the common name of a transient db in sqlite3 */
#define AD_INIT_TRANSIENT  ":memory:"
/* in case of NULL string the default db is opened.*/
#define AD_INIT_PERSISTENT NULL

/* not all databases support a transient connection */
int ad_init(int is_transient, const char* conn_string);

void ad_close(void);


/* the generic handle for an adelphos object. */
/*
typedef void* ad_h;
*/

/*
 * the api is text based, the parameters are positional, but they have
 * different meaning for different commands.
 *
 * This is the low level interface.
 *
 * */

/* should be enough :) */
/* note that 30 is not the length of the struct, the params
 * might be arbitrarly long. */
/*
#define MAX_PARAMS 30
struct generic_pars {
	char *params[MAX_PARAMS];
};
*/


struct create_l0_group_pars {	 
	char *name;
	char *pass_adult;
	char *pass_minor;
	char *cl_adults;
	char *cl_minors;
};

#define AD_CREATE_L0_PARS_SIZE ((sizeof(struct create_l0_group_pars)) / \
		(sizeof(char*)))


/* It is not necessary to have the structure only with chars. */

struct add_user_in_s
{
	char *name;
	char *family_name;
	char *password;
	char *email;
};

/* It is empty. */
struct add_user_out_s
{
};


/* 
 * All the responses from adelphos have a response code.
 *
 * */
struct adelphos_out_s
{
	int8_t ret_code;

	union {
		struct add_user_out_s ausr;
	};
};

/* this is the start of the C interface. */

/*
 *
 * I could have a Python interface like this
 *
 *
 * import adelphos
 *
 * ad = adelphos.connection("user", "pass")
 *
 * ad.create_trust_line("to user bob", "equity", kv...)
 *
 * # the command are fixed, some parameters are fixed, but some
 * others are not, and are passed with a key value pair
 *
 *
 *
 */


/* The user added is not yet confirmed. */
ad_res add_user(struct add_user_in_s *adu);


struct ad_login_in_s
{
	char *user;
	char *password;
};


struct ad_login_out_s
{
	/* the session identifier for this user.*/
	ad_session_id_t sess_id;
};


/* 
 * the login return a token which the user must present at
 * every time the application issues a command. 
 *
 * */
ad_res ad_login(struct ad_login_in_s *in, struct ad_login_out_s *out);



/* here I have already done my research. */
struct create_adelphos_pars {
	char *name;
	char *password;
	char *l0_id;
	char *is_minor;
	char *commission;
};

#define AD_CREATE_ADELPHOS_SIZE ((sizeof(struct create_adelphos_pars)) / \
		(sizeof(char*)))

#if 0
struct adelphos_param_s {

	/* 
	 * distcriminant, the API command 
	 *
	 * note that they are all string, aliased.
	 *
	 * */
	union {
		struct generic_pars gp;

		struct {
			/* the first string is the verb. */
			char *cmd_verb;

			struct create_l0_group_pars cl0g;
		};
		
	};

};
#endif


#if 0

/* API relative to an l0_group */
ad_res ad_l0_group_create(struct adelphos_param_s *ap);

/* API relative to an adelphos */

/* creates an adelphos based on a l0_group, adelphos names inside
 * a group should be unique*/
ad_res ad_adelphos_create(struct adelphos_param_s *ap);
#endif

/*
int ad_adelphos_delete(ad_h adelphos);
*/


/* 
 * create a l1 mapping, this is between two adelphos, but in reality
 * it is between two l0_groups 
 *
 * it is an error to connect two times the same groups.
 *
 * this instantiates the request, but the connection is not made yet.
 *
 * */
/*
int ad_adelphos_request_connect(ad_h l0_group_1, ad_h l0_group_2);
*/


/* 
 * this finalizes the connection, only at this moment we can have a trust channel
 *
 * */
/*
int ad_adelphos_finalize_connect(ad_h l0_group_2, ad_h l0_group_1);
*/


/*
 * a credit line is composed of various hops, much like a packet in TCP is routed
 * through various gateways.
 *
 */
#if 0
struct ad_credit_hop
{
	ad_h  creditor;

	/* this is the amount of credit you have obtained in each hop */
	cur_t amount;
};
#endif

/* 
 * a credit line is a linked lists of credit hops which the system
 * has found between two l0_groups
 *
 * 
 *
 * */
#if 0
struct ad_credit_line
{

	/* this is the front line debtor the l0_group_src in ad_adelphos_route_pay */
	ad_h debtor;

	/* how much you have to pay in REAL money */
	cur_t amount_real;
	
	/* variable array of hops to arrive to the destination, last pointer is NULL */
	struct ad_credit_hop **hops;
};
#endif


/*
 *
 * this is the propose pay function: it pays a certain amount from one l0_group to another.
 *
 * It might fail, otherwise it returns a proposed credit line.
 *
 * max_amount is the amount of credit that the payee is willing to pay.
 *
 * max_hops is the maximum of credit lines that the system is going to find.
 *
 * the caller is responsible of freeing the structure, using the ad_credit_line_free function.
 *
 * returns 
 *
 * AD_ROUTE_NOT_FOUND if there is not a credit line
 *
 * AD_TOO_MUCH_DEBT_FOR_YOU if the source has not enough credit, a credit line is not even searched for.
 *
 * AD_TOO_MUCH_CREDIT_DST if the destination holds too much credit
 *
 * AD_CONGESTED_ROUTE if there is a blocking channel, in this case it returns a partial routing.
 *
 */

#if 0
ad_res ad_adelphos_route_pay(ad_h l0_group_src, ad_h l0_group_dst, cur_t amount, cur_t max_amount,
		int max_hops, /* out */ struct ad_credit_line **adl);

/* the credit line must come from ad_adelphos_route_pay */
void ad_credit_line_free(struct ad_credit_line* a_line);

/*
 *
 * this actually does the payment.
 */
ad_res ad_adelphos_do_pay(ad_h l0_group_src, ad_h l0_group_dst, cur_t amount, 
		/* in */ struct ad_credit_line *adl);



/* 
 *
 * returns an adelphos with a certain name inside a group
 *
 *
 * */
ad_h ad_adelphos_find(ad_h l0_group, const char* name);
#endif






#endif
