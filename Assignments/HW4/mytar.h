#ifndef MYTAR_H
#define MYTAR_H
#include <stdint.h>
typedef struct Header{
	uint8_t block[512];
} Header;
void build_dir(char *); /* char * directory name*/

uint8_t * pack_header(char *);/* char *file*/

void unpack_head(uint8_t[512]);

int verify_head(Header *);

void listdir(char *, int, int);

void writebody(char *, int);

void create(int, char *, int);

void table(int, char *, int);

void dirtrav(char *, int);

void extract(int, char *);

void newfile(uint8_t[512],char[256], int, int);
#endif
