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
#include <fcntl.h>
#include <unistd.h>

#include "adelphos.h"
#include "trust.h"
#include "acrypt.h"
#include "misc.h"

/* just a simple file */
#define ADELPHOS_DB "ad_db.sqlite3"
/* this is the memory db, used to */
#define TRANSIENT_DB ":memory:"

/* this should be absolute, based on the installation directory . */
#define SCHEMA_FILE "scripts/sql/sqlite3/init/ad_schema.sql"

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

static int _create_schema()
{
	alogi("create the schema");

	struct stat statbuf;
	int res = lstat(SCHEMA_FILE, &statbuf);
	ok_or_goto_fail(res == 0);

	/* Ok, now I can read the file */
	alogi("Schema file %jd", statbuf.st_size);


	res = -1;
	uint8_t *buffer = malloc(statbuf.st_size + 1);
	ok_or_goto_fail(buffer != NULL);

	int fd = open(SCHEMA_FILE, O_RDONLY);
	ok_or_goto_fail(fd >= 0);

	res = read_all(fd, buffer, statbuf.st_size);
	ok_or_goto_fail(res == 0);

	/* add the null at the end */
	buffer[statbuf.st_size] = '\0';

	/* Now I can execute the command */
	char *errmsg;
	res = sqlite3_exec(priv.db, (char*)buffer, NULL, NULL, &errmsg);

	free(buffer);

	if (res != SQLITE_OK) {
		alogw("Error %s creating schema", errmsg);
		sqlite3_free(errmsg);
		/* No point in continuing. */
		exit(1);
	}
	alogi("schema created");

	res = close(fd);

fail:
	return res;
}


int ad_init(int is_transient, const char *conn_string)
{
	int res = AD_OK;
	if (__sync_val_compare_and_swap(&priv.init, 0, 1) != 0) {
		goto end;
	}

	int create_schema = 0;

	const char * filename_db;

	if (is_transient) {
		create_schema = 1;
		filename_db = TRANSIENT_DB;
	} else {
		/* is there the file */
		struct stat statbuf;
		filename_db = ADELPHOS_DB;
		res = lstat(ADELPHOS_DB, &statbuf);
		ok_or_goto_fail( 
				(res == 0) || (errno == ENOENT)
				);

		if (res < 0){
			alogi("I will create the schema");
			create_schema = 1;
		} else {
			alogi("database existent, I won't create it");
		}
		

	}

	/* let's search if this is a temporary db, */
	res  = sqlite3_open(filename_db, &priv.db);
	ok_or_goto_fail(res == SQLITE_OK);

	if (create_schema) {
		res = _create_schema();
	} 

	alogi("Adelphos library initialized");

fail:
end:
	return res == 0 ? 0 : AD_ERR;
}

void ad_close(void)
{
	if (__sync_val_compare_and_swap(&priv.init, 1, 0) != 1) {
		goto end;
	}

	sqlite3_close(priv.db);
end:
	;
}

ad_res add_user(struct add_user_in_s *adu)
{
	return 0;
}

ad_res add_user_marshall(struct adelphos_in_s *in)
{

	ad_res res = AD_ERR;
	/* I take the user name */
	const char *user_name = (in->a_prov->kv_get)(in->param, "user");
	ok_or_goto_fail(user_name != NULL);
	alogt("I will add the user %s", user_name);


	res = AD_OK;
fail:
	return res;
}



#if 0
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
#endif
