#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	int i = 0;
	pid_t child1 = 0;
	pid_t child2 = 0;
	int one[2];
	int two[2];
	int c = 0;
	pipe(one);
	pipe(two);
	while(1){
		c = 0;
		scanf(" %d", &c);	
		if(!(child1 = fork()) ){
			//dup2(one[1],1);	
			c = c*c;
		
	//	printf("test\n");
	//	printf("%d\n",c);
	//	dup2(one[1],1);	
		write(one[1], &c, sizeof(int));
		close(one[0]);
		close(one[1]);
		close(two[0]);
		close(two[1]);
		exit(-1);
		}
		if(!(child2 = fork())){
			read(one[0], &c, sizeof(int));
			//dup2(one[0], STDIN_FILENO);
			//dup2(two[1], STDOUT_FILENO);
			c = c + 1;
			//printf("test2\n");
			//printf("c2 : %d\n",c);
			write(two[1], &c, sizeof(int));		
		close(one[0]);
		close(one[1]);
		close(two[0]);
		close(two[1]);
		exit(-1);
		}
		wait(NULL);	
		wait(NULL);
		
		read(two[0], &c, sizeof(int));
		printf("child1: %d\n", child1);
		printf("child2: %d\n", child2);
		printf("%d\n", c);
//		close(one[0]);
//		close(one[1]);
//		close(two[0]);
//		close(two[1]);


	}
		close(one[0]);
		close(one[1]);
		close(two[0]);
		close(two[1]);



}
