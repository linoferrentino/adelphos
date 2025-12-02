/*
 *
 *
 * json_api
 *
 * this is the declaration of the json API for adelphos.
 *
 * A JSON object can be passed in adelphos via socket or a command line
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 */
 

#ifndef _json_api_h
#define _json_api_h

#include <stdint.h>
#include "misc.h"


/* Initialize the json API. This must be called once */
int json_api_init(void);



/*
 * Process the json request.
 *
 *
 * */
int json_api_proc(struct byte_buf_s *in, struct byte_buf_s *out);


/* called at the end of the application. */
void json_api_free(void);



#endif
