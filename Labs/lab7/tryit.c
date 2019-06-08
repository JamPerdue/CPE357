#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]){
	pid_t child;
	int status = 0;
	int err_stat = 0;
	if(argc<2){
		fprintf(stderr, "usage: tryit command");
		exit(1);
	}
	child = fork();
	if(child ==0){
	err_stat = execl(argv[1], argv[1], (char *)NULL);
	}
	wait(&status);
	if(child > 0){
	if(err_stat == -1){
		fprintf(stderr, "Process %d exited with an error value.\n",child);
	}
	else{
		printf("Process %d succeeded.\n", child);
	}
	}
	exit(status);

}
