#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "read.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
	FILE * count = fopen(argv[1],"r");
	char * buf = (char *)malloc(sizeof(char)*atoi(argv[2]));
	
	fread(buf,atoi(argv[2]),sizeof(char),count);



	printf("%s\n", buf);
	free(buf);
	fclose(count);
	return 0;
}
