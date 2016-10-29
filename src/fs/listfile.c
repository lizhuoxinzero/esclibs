#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "fs/listfile.h"
#include "container/queue/listqueue.h"

int listfile(const char * path, int prelen, int ifpasshid, listqueue_t *result)
{	
	
	DIR *dir = opendir(path);
	if (dir == NULL){
		printf("error:%s\n", strerror(errno));
		return -1;
	}		
	
	char buf[512];
	while(1){
		struct dirent* dirinfo = readdir(dir);
		if (dirinfo == NULL){
			break;
		}
		if (dirinfo->d_type == DT_DIR){ //目录情况
			//去除本级目录和上级目录
			if (ifpasshid!=0){
				if (dirinfo->d_name[0]=='.'){
					continue;
				}
			}else{
				if (dirinfo->d_name[0]=='.' && 
					(dirinfo->d_name[1]=='\0' || 
					dirinfo->d_name[1]=='.' && dirinfo->d_name[2]=='\0') ) {
					continue;
				}
			}
			int pathlen = strlen(path);
			int newdirlen = strlen(dirinfo->d_name);
			memcpy(buf, path, pathlen);
			buf[pathlen++]= '/';
			memcpy(&buf[pathlen], dirinfo->d_name, strlen(dirinfo->d_name)+1);
			int ret = listfile(buf, prelen, ifpasshid, result);
			if (ret == -1) {
				printf("sub listfile error\n");
				return -1;
			}
		}else{
			//过滤隐藏文件
			if (ifpasshid!=0 && dirinfo->d_name[0]=='.'){
				continue;
			}

			char name[512];
			if(strlen(path)==prelen){
				listqueue_push(result, dirinfo->d_name, strlen(dirinfo->d_name)+1);
				//printf("%s\n", dirinfo->d_name);
			} else {
				int prefixlen = strlen(&path[prelen+1]);
				memcpy(name, &path[prelen+1], prefixlen);
				name[prefixlen] = '/';
				strcpy(&name[prefixlen+1], dirinfo->d_name );
				listqueue_push(result, name, strlen(name)+1);
				//printf("%s%s\n",  name, dirinfo->d_name);
			}
		}
	}

	closedir(dir);
	return 0;
}

