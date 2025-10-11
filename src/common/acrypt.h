/*
 *
 * Adelphos, cryptographic utilities.
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 */

#ifndef _acrypt_h_
#define _acrypt_h_

#include <inttypes.h>
#include <stddef.h>

#define UCRYPT_KEY_SZ   32
typedef uint8_t  ucrypt_key_t  [UCRYPT_KEY_SZ];

/* generates a public/private key pair. */
void acrypt_key_generation(ucrypt_key_t pk, ucrypt_key_t sk);

/* just a convenience function (this is where we need Windows headers) */
void acrypt_random_bytes(uint8_t *buf, size_t size);

/*
 * encodes a password for storing in adelphos db.
 *
 *
 */
char *acrypt_encode_password(const char *password, char* output, int sz);



#endif
