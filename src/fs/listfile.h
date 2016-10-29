#ifndef ES_LITTLETOOL_H
#define ES_LITTLETOOL_H

#include "container/queue/listqueue.h"


/**
 * @param path 指定目录
 * @param prelen 目录长度
 * @param ifpasshid 是否过滤隐藏文件，0为否，其它为过滤
 */
extern int listfile(const char * path, int prelen, int ifpasshid, listqueue_t *result);



#endif
