#ifndef ES_ESDB_H
#define ES_ESDB_H

#include <stdint.h>
typedef struct ESDB esdb_t;
typedef esdb_t esdb_interface_t;

typedef struct ESDB{
	void *dbimple;
	char dbname[512];
	int (*dbopen)(const char *dbname, esdb_t* esdb, void *date);
	void (*dbclose)(void *dbimple);
	int (*dbget)(void *dbimple, const char *key, char *buf, int bufsize);
	int (*dbset)(void *dbimple, const char *key, uint16_t key_len, const char *data, int datesize);
	int (*dbdel)();
	int (*dbflush)();
}esdb_t;


extern int dbopen(esdb_t *esdb, const char *dbname, esdb_interface_t *esdb_if, void *date);
extern void dbclose(esdb_t *esdb);
extern int dbget(esdb_t *esdb, const char *key, char *buf, int bufsize);
extern int dbset(esdb_t *esdb, const char *key, uint16_t key_len, const char *date, int datesize);

 

#endif
