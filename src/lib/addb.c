/*
 *
 *
 * adelphos database 
 *
 * this is the normal implementation of sqlite3.
 *
 * If you change db engine you will have to change here.
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 *
 *
 *
 */

/* this should the only place in adelphos where you include this file */
#include <sqlite3.h>

#include "addb.h"

struct addb_h 
{
	sqlite3 *db;

	/* maybe some prepared statements ? */
};


int addb_open_transient(struct addb_h **dbh)
{
	return 0;
}
