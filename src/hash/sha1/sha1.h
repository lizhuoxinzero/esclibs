#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>

#define SHA1_STRLEN 20
#define SHA1_FMTLEN 41
extern void print_sha1(uint8_t *sha1);


extern int SHA1_file(const char* filepath, uint8_t *result);

extern int SHA1_file_fd(int filefd, uint8_t *result);

#endif
