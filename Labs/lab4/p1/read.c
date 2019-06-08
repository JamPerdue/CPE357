#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "read.h"
#include <unistd.h>
#include <time.h>


int main(int argc, char * argv[]){
	int count = 0;
	void * buf;	
	clock_t t = clock();
	double time;
	count = open(argv[1], O_RDONLY);
	
	read(count,buf,atoi(argv[2]));
	t = clock() - t;
	time = ((double)t)/CLOCKS_PER_SEC;
	printf("Program ran in %f",time);







	return 0;
}
