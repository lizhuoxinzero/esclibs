#ifndef ES_LOG_H
#define ES_LOG_H


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

extern void getLogTime(char *format);

#define LOG_INFO(format, args...) \
	do{\
		char formattime[18];\
		getLogTime(formattime);\
		printf("[INFO][%s:%d#%s][%s]"format"\n", __FILE__, __LINE__, __FUNCTION__, formattime, ##args);\
	}while(0)



#define LOG_DEBUG(format, args...) \
	do{\
		char formattime[18];\
		getLogTime(formattime);\
		fprintf(stderr,"[DEBUG][%s:%d#%s][%s]"format"\n", __FILE__, __LINE__, __FUNCTION__, formattime, ##args);\
	}while(0)


#define LOG_WORNING(format, args...) \
	do{\
		char formattime[18];\
		getLogTime(formattime);\
		fprintf(stderr,"[WORNING][%s:%d#%s][%s]"format"\n", __FILE__, __LINE__, __FUNCTION__, formattime, ##args);\
	}while(0)



#define LOG_ERROR(format, args...) \
	do{\
		char formattime[18];\
		getLogTime(formattime);\
		fprintf(stderr,"[ERROR][%s:%d#%s][%s]"format"\n", __FILE__, __LINE__, __FUNCTION__, formattime, ##args);\
	}while(0)

#define LOG_ERROR_MSG(format, args...) \
	do{\
		char formattime[18];\
		getLogTime(formattime);\
		fprintf(stderr,"[ERROR][%s:%d#%s][%s][errmsg:%s]"format"\n", __FILE__, __LINE__, __FUNCTION__, formattime, strerror(errno), ##args);\
	}while(0)


#define LOG_FATAL(format, args...) \
	do{\
		char formattime[18];\
		getLogTime(formattime);\
		fprintf(stderr,"[FATAL][%s:%d#%s][%s]"format"\n", __FILE__, __LINE__, __FUNCTION__, formattime, ##args);\
		_exit(1);\
	}while(0)

#define LOG_FATAL_MSG(format, args...) \
	do{\
		char formattime[18];\
		getLogTime(formattime);\
		fprintf(stderr,"[FATAL][%s:%d#%s][%s][errmsg:%s]"format"\n", __FILE__, __LINE__, __FUNCTION__, formattime, strerror(errno), ##args);\
		_exit(1);\
	}while(0)


#endif
