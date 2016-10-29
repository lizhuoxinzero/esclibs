#include "log/log.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	open("fuck", O_RDWR);
	LOG_INFO("%s-%d", "test", 21);
	LOG_DEBUG("%s-%d", "test", 21);
	LOG_ERROR_MSG("%s-%d", "test", 21);
	LOG_FATAL("%s-%d", "test", 21);
	LOG_INFO("%s-%d", "test", 21);

	return 0;
}
