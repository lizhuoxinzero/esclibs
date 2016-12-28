

#include "db/esdb.h"



int dbopen(esdb_t *esdb, const char *dbname, esdb_interface_t *esdb_if, void *date)
{
	*esdb = *esdb_if;
	return esdb->dbopen(dbname, esdb, date);
}

void dbclose(esdb_t *esdb)
{
	esdb->dbclose(esdb->dbimple);
}

int dbget(esdb_t *esdb, const char *key, char *buf, int bufsize)
{
	return esdb->dbget(esdb->dbimple, key, buf, bufsize);
}

int dbset(esdb_t *esdb, const char *key, uint16_t key_len, const char *date, int datesize)
{
	return esdb->dbset(esdb->dbimple, key, key_len, date, datesize);
}
