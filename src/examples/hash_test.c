#include "hash/sha1/sha1.h"
#include "log/log.h"
#include "timer/speed.h"
#include <unistd.h>

uint8_t tsha1[SHA1_STRLEN];
uint8_t tsha1fmt[SHA1_FMTLEN];

void run(void *args)
{
	int ret = SHA1_file((char*)args, tsha1);
	if (ret == -1){
		LOG_ERROR_MSG("sha1file error");
	}
	LOG_DEBUG("start sleep 3s");
	sleep(3);
	LOG_DEBUG("timeout");
}


int main(int argc, char **argv)
{
	if(argc < 2){
		LOG_INFO("please input path");
		return 0;
	}

	int64_t ret = speed_run(run, argv[1]);
	print_sha1(tsha1);
	fmt_sha1(tsha1fmt, tsha1);
	LOG_INFO("%s", tsha1fmt);
	LOG_INFO("use time:%lldus", ret);

	return 0;
}

