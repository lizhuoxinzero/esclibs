#include "log/log.h"
#include "timer/speed.h"
#include <unistd.h>
#include "db/esdb.h"
#include "db/mmapdb/mmapdb.h"

void run(void *args)
{
	esdb_t mmapdb;
	int ret = dbopen(&mmapdb, "db/testdb", &ESDB_MMAP_DB, NULL);
	if (ret != 0){
		LOG_ERROR("dbopen error, ret=%d", ret);
	}
	ret = dbset(&mmapdb, "test22", 5, "testdata hehe", 13);
	if (ret != 0){
		LOG_ERROR("dbset error, ret=%d", ret);
	}

	dbclose(&mmapdb);
}


int main(int argc, char **argv)
{
	if(argc < 1){
		LOG_INFO("please input something");
		return 0;
	}

	int64_t ret = speed_run(run, argv[1]);
	LOG_INFO("use time:%lldus", ret);

	return 0;
}

