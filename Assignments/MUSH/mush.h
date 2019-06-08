#ifndef MUSH_H
#define MUSH_H
#include <stdint.h>

typedef struct Stage{
	char out[512];
	char in[512];
	int n;
	int tot;
	char *argv[11];
	struct Stage * next;
} Stage;

void catcom(char *[], char[512]);

void do_cdcommand(char[512]);

void errorfind();













#endif
