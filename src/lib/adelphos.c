/*
 *
 * implementation file for the adelphos library
 *
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 *
 */

#include <stdint.h>
#include <sqlite3.h>
#include <alloca.h>
#include <sys/stat.h>

#include "adelphos.h"
#include "trust.h"
#include "acrypt.h"

/* just a simple file */
#define ADELPHOS_DB "ad_db.sqlite3"
/* this is the memory db, used to */
#define TRANSIENT_DB ":memory:"

#define _LOG_MODULE "ad"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;


/*
 * our private data.
 */
static struct {

	sqlite3 *db;
	uint8_t init;

} priv = {
	.init = 0
};

static void _create_schema()
{
	alogi("create the schema");
}


int ad_init(int is_transient, const char *conn_string)
{
	int res = AD_OK;
	int rc;
	if (__sync_val_compare_and_swap(&priv.init, 0, 1) != 0) {
		goto end;
	}

	int create_schema = 0;

	if (is_transient) {
		create_schema = 1;
	} else {
		/* is there the file */
		struct stat statbuf;
		res = lstat(ADELPHOS_DB, &statbuf);

		if (res == -1 && errno == ENOENT){
			alogi("I will create the schema");
			create_schema = 1;
		} else {
			alogi("database existent, I won't create it");
		}

	}

	/* let's search if this is a temporary db, */

	res = AD_ERR;
	
	rc = sqlite3_open(ADELPHOS_DB, &priv.db);
	ok_or_goto_fail(rc == 0);

	if (create_schema) {
		_create_schema();
	}

	res = AD_OK;
	alogi("Adelphos library initialized");

fail:
end:
	return res;
}

int ad_close(void)
{
	int res = AD_OK;
	if (__sync_val_compare_and_swap(&priv.init, 1, 0) != 1) {
		goto end;
	}

	sqlite3_close(priv.db);
end:
	return res;
}

ad_res add_user(struct add_user_in_s *adu)
{
	return 0;
}

ad_res ad_l0_group_create(struct adelphos_param_s *ap)
{
	/* I have to insert the data here .*/

	/*const char *sql = "insert into ...";*/

	int res;
	char *pass_adult_hashed = alloca(200);
	char *pass_minor_hashed = alloca(200);
       
	acrypt_encode_password(ap->cl0g.pass_adult, pass_adult_hashed, 200);
	acrypt_encode_password(ap->cl0g.pass_minor, pass_minor_hashed, 200);

	alogt("The password adult is %s, converted is %s",
			ap->cl0g.pass_adult, pass_adult_hashed);

	alogt("The password minor is %s, converted is %s",
			ap->cl0g.pass_minor, pass_minor_hashed);

	double trust_adult_db, trust_minor_db;

	res = trust_cl_s_to_db(ap->cl0g.cl_adults, &trust_adult_db);
	ok_or_goto_fail(res == 0);

	res = trust_cl_s_to_db(ap->cl0g.cl_minors, &trust_minor_db);
	ok_or_goto_fail(res == 0);

	/*
	const char *insert_l0_sql = "insert into l0_group "
		"(city_id, name, join_pass_adult, joint_pass_minor, "
		"credit_limit_adults, credit_limit_minors) values ( "
		"( ?1, ?2, ?3, ?4, ?5, ?6);";

	sqlite3_stmt *insert;
	sqlite3_prepare_v2();
	*/







fail:
	return res;
}
