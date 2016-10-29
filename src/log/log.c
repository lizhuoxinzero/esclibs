
#include <time.h>
#include "log.h"



void getLogTime(char *format)
{
	time_t t = time(NULL);
	struct tm *time = localtime(&t);
	strftime( format, 18, "%Y%m%d-%H:%M:%S", time);
	format[17] = 0;	
}
