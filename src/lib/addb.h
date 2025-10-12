/*
 *
 *
 * adelphos database.
 *
 * This is an astraction. For now we use sqlite3, but we might change
 * in the future, if needed.
 *
 * Copyright (c) 2025. Pasqualino Ferrentino.
 * lino.ferrentino@gmail.com
 *
 * Licence: GPL3 or above.
 *
 *
 */
#ifndef _addb_h_
#define _addb_h_

/* the opaque structure to a database */

struct addb_h;


/* 
 * this opens a transient database, it implicitely initializes the
 * schema, because every time it is new.
 *
 * not all sql engines support a transient db. This function
 * might fail.
 *
 * */
int addb_open_transient(struct addb_h **dbh);

/* tries to open the database, it might fail if the db is not
 * existing.  */
int addb_try_open(const char *conn_string, struct addb_h **dbh);

/* tries to open the database, if it is not existing it creates
 * it with the initial schema. */
int addb_open_or_create(const char *conn_string, struct addb_h **dbh);

/* this is the inverse of the previous, it fails if the database is still
 * existing, there is not a wipe database by design, if you want to delete
 * it, you delete it by hand. */
int addb_create_db(const char *conn_string, struct addb_h *dbh);


/* closes the db. */
int addb_close(struct addb_h *dbh);


#endif
