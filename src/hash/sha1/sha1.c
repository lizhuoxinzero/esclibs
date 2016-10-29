#include <stdio.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "hash/sha1/sha1.h"

const int BUFFER_SIZE = 8192;

void print_sha1(uint8_t *sha1)
{
	int i;
	for(i=0; i<SHA1_STRLEN; i++){
		printf("%02x", sha1[i]);
	}
	printf("\n");
}

uint8_t* fmt_sha1(uint8_t *fmt, uint8_t *sha1)
{
	int i;
	for(i=0; i<SHA1_STRLEN; i++){
		sprintf(fmt+i*2,"%02x", sha1[i]);
	}
	fmt[SHA1_FMTLEN-1] = 0;
	return fmt;
}

int SHA1_file(const char* filepath, uint8_t *result)
{
	//打开文件
	int filefd = open(filepath, O_RDONLY);
	if (filefd == -1 ){
		return -1;
	}

	int ret = SHA1_file_fd(filefd, result);		
	if (ret == -1 ){
		close(filefd);
		return -1;
	}
	close(filefd);
	return 0;
}

int SHA1_file_fd(int filefd, uint8_t *result)
{
	int ret;
	
	//初始化sha1
	SHA_CTX sha1_ctx;
	ret = SHA1_Init(&sha1_ctx);
	if (ret != 1){
		return -1;
	}
	
	char buf[BUFFER_SIZE];
	//读取文件内容
	while(1){
		ret = read(filefd, buf, BUFFER_SIZE);
		if (ret == -1){
			return -1;
		}
		if (ret == 0 ){
			break;
		}
		ret = SHA1_Update(&sha1_ctx, buf, ret);
		if (ret != 1){
			return -1;
		}
	}

	//计算出sha1
	ret = SHA1_Final(result, &sha1_ctx);
	if (ret != 1) {
		return -1;
	}

	return 0;
}


