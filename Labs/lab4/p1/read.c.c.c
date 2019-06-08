#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "read.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char * argv[]){
	printf("Child Max:%ld\n",sysconf(_SC_CHILD_MAX));
	printf("Open Max:%ld\n",sysconf(_SC_OPEN_MAX));
	printf("Page Size:%ld\n",sysconf(_SC_PAGE_SIZE));
	return 0;
}
