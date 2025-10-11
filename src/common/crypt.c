/*
 *
 * Adelphos, crypto utilities.
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 * 
 */



#include <sys/random.h>
#include <errno.h>
#include <stdlib.h>
#include <crypt.h>
#include "acrypt.h"

#define _LOG_MODULE "acrypt"
#define MODULE_LEV ML_TRIVIAL
#include "alog.h"
LOG_MODULE_IMP;


void acrypt_random_bytes(uint8_t *buf, size_t size) 
{
	ssize_t res;
	res = getrandom(buf, size, 0);
	if (res < 0) {
		alogw("Cannot get %zd random bytes, %d",
				size, errno);
		exit(1);
	}

	if (res != size) {
		alogw("Could not get %zd random bytes %d",
				size, errno);
		exit(1);
	}
}


char *acrypt_encode_password(const char *password, char* output, int sz)
{

	/* a default method */
	char *salt = crypt_gensalt(NULL, 0, NULL, 0);

	/* now we pass it to crypt */

	char *encoded_pass = crypt(salt, password);
	alogt("The salt is %s encoded is %s", salt, encoded_pass);

	snprintf(output, sz, "%s$%s", salt, encoded_pass);

	return output;

}

