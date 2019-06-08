#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){
	int i = 0;
	pid_t child1 = 0;
	pid_t child2 = 0;
	int one[2];
	char temp1[50];
	char temp2[50];
	int c = 0;
	char *first;
	char *second;
	int j = 0;
	int k = 0;
	char *tok = NULL;
	i = 1;
	first = "/bin/ls -l";
	second = "/bin/more";







	pipe(one);
	if(!(child1 = fork())){
		dup2(one[1], STDOUT_FILENO);
		//perror("dup");
	//	strcpy(temp1, first);
	//	tok =strtok(temp1, " ");
//		printf("one\n");
		printf("first: %s\n",first);
		execl("/bin/ls", "/bin/ls","-l", NULL);
		close(one[0]);
		close(one[1]);
		exit(-1);
	}	
//	perror("child1");
	if(!(child2 = fork())){
		
		dup2(one[0], STDIN_FILENO);
//		printf("two\n");
		execv("/bin/more", NULL);
		close(one[0]);
		close(one[1]);
		exit(-1);
	}
//	perror("child2");
	wait(NULL);
	wait(NULL);
	close(one[0]);
	close(one[1]);
	printf("done\n");
}
