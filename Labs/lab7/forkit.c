#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>




int main(int argc, char *argv[]){
	pid_t parent = getpid();
	pid_t child;
	int status = 0;
	printf("Hello, World!\n");
	child = fork();
	if( child == 0){
	exit(7);
	}
	printf("This is the child, pid %d\n",child);

	printf("This is the parent, pid %d\n", parent);
	wait(&status);
	printf("This is the parent, pid %d, signing off\n",parent);
	exit(0);
}
	
