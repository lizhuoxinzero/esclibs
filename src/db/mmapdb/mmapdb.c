#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "db/mmapdb/mmapdb.h"


typedef struct{
	uint32_t dbsize;
	uint32_t max_size;
}mmap_db_config_t;

typedef struct{
	char dbname[512];
	char dbindex[512];
	uint32_t cur_used_size;	//当前.db文件大小
	uint32_t cur_used_index_size; //当前.idx文件大小
	int fd_index;
	int fd_data;
	void *data_map;
	void *index_map;
}mmap_db_t;


typedef struct{
	uint32_t slotnum;
	uint32_t db_offset;
	uint32_t max_size; //.db文件最大大小
	uint32_t new_node_addr; //新节点信息地址
	uint32_t hashslot[8*1024];
}mmap_index_t;

uint32_t index_head_len = sizeof(mmap_index_t);

typedef struct{
	uint16_t data_len;//data内容长度
	uint16_t key_len;
	uint32_t next;
	uint64_t index;//data文件偏移
}hash_node_t;

void mmap_db_close(void *dbimple);

int mmap_db_open(const char *dbname, esdb_t *esdb, void *date)
{
	esdb->dbimple = malloc(sizeof(mmap_db_t));
	if (esdb->dbimple == NULL){
		return -1;
	}
	mmap_db_t *db = esdb->dbimple;

	sprintf(db->dbname, "%s%s", dbname, ".db" );
	sprintf(db->dbindex, "%s%s", dbname, ".idx" );

	db->fd_data = open(db->dbname, O_RDWR|O_CREAT, 0600);
	if (db->fd_data == -1) {
		return -2;
	}
	
	db->fd_index = open(db->dbindex, O_RDWR|O_CREAT, 0600);
	if (db->fd_index == -1) {
		return -2;
	}
	

	db->data_map = mmap(NULL, 1*1024*1024*1024, PROT_READ |PROT_WRITE, MAP_SHARED, db->fd_data, 0);
	db->index_map = mmap(NULL, 1*1024*1024*1024, PROT_READ |PROT_WRITE, MAP_SHARED, db->fd_index, 0);

	mmap_index_t *mmap_index = (mmap_index_t *)db->index_map;

	//初始化index表
	//判断文件是否大于mmap_index_t; 否则加大文件并初始化；
	struct stat sb;
	if (fstat(db->fd_index, &sb) == -1){
		mmap_db_close(db);
		return -3;
	}
	db->cur_used_index_size = sb.st_size;
	if( sb.st_size < index_head_len){
		ftruncate(db->fd_index, index_head_len+8*1024);
		db->cur_used_index_size = index_head_len + 8*1024;
		mmap_index->new_node_addr = index_head_len;
		mmap_index->slotnum = 8*1024;
	}

	if (fstat(db->fd_data, &sb) == -1){
		mmap_db_close(db);
		return -3;
	}
	db->cur_used_size = sb.st_size;
	if( sb.st_size == 0){
		ftruncate(db->fd_data, 8*1024*1024);
		db->cur_used_size = 8*1024*1024;
	}
	

	return 0;
}

void mmap_db_close(void *dbimple)
{
	mmap_db_t *db =  (mmap_db_t*)dbimple;
	munmap(db->data_map, 1*1024*1024*1024);
	munmap(db->index_map, 1*1024*1024*1024);
	close(db->fd_data);
	close(db->fd_index);
	free(db);
}

uint32_t get_hash(const char *key, uint16_t key_len, uint32_t slotnum)
{
	uint32_t hash = 0;
	do{
		hash = hash * 131 + (*key++);
	}while(--key_len);

	return hash % slotnum;
}

int check_size(uint32_t len, uint32_t cur_size, uint32_t addr)
{
	if (cur_size - addr < len)
		return -1;
	return 0;
}

int mmap_db_set(void *dbimple, const char *key, uint16_t key_len, const char *data, int datesize)
{
	mmap_db_t *db =  (mmap_db_t*)dbimple;
	mmap_index_t *mmap_index = (mmap_index_t *)db->index_map;
	uint32_t hash = get_hash(key, key_len, mmap_index->slotnum);
	uint32_t addr = mmap_index->hashslot[hash];
	if (addr == 0) {//该槽没有数据
		mmap_index->hashslot[hash] = mmap_index->new_node_addr;
		mmap_index->new_node_addr += sizeof(hash_node_t)+key_len;
		if (-1 == check_size(sizeof(hash_node_t)+key_len, db->cur_used_index_size, mmap_index->hashslot[hash])) {
			db->cur_used_index_size *=2;
			ftruncate(db->fd_index, db->cur_used_index_size);
		}
		hash_node_t *node = (hash_node_t*)((uint8_t*)db->index_map+mmap_index->hashslot[hash]);
		node->data_len = datesize;
		node->key_len = key_len;
		node->next =0;
		node->index = mmap_index->db_offset;
		mmap_index->db_offset += datesize;
		if (mmap_index->db_offset >= db->cur_used_size){//判断是否足够空间
			db->cur_used_size *=2;
			ftruncate(db->fd_data, db->cur_used_size);
		}
		memcpy((uint8_t *)db->data_map+node->index, data, datesize);
	}
	return 0;
}


mmap_db_t mmap_db={
	.dbname		= "",
	.fd_index	= 0,
	.fd_data	= 0,
};

esdb_interface_t ESDB_MMAP_DB = {
	.dbimple		= NULL,
	.dbopen			= mmap_db_open,
	.dbclose		= mmap_db_close,
	.dbget			= NULL,
	.dbset			= mmap_db_set,
};
