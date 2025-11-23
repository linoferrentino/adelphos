/*
 *
 *
 * request.h
 *
 * declarations for the request functions.
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 
 *
 */


#ifndef _request_h_
#define _request_h_

#include <stdlib.h>

int req_handle(char *req, size_t len);

/* 
 *
 * The function to initialize the requests. It builds the
 * dictionary of requests.
 *
 * */
int req_init(void);

#endif
