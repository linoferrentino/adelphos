


#include <stdio.h>
#include <sqlite3.h>
#include <err.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char **argv)
{
	printf("hello adelphos test\n");
	char *zErrMsg = 0;
	int rc;
	sqlite3 *db;

	if (argc != 2) {
		errx(-1, "I need sql to exec");
	}


	rc = sqlite3_open(":memory:", &db);
	if (rc) {
		errx(-1, "Can't open database: %s\n", sqlite3_errmsg(db));
	} 

	rc = sqlite3_exec(db, argv[1], callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %d  %s\n", rc, zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);

	return 0;
}
