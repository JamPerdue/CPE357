#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
pid_t child = -1;
void ticker(int signum){
	if(child > 0){
		kill(child, SIGKILL);
	}	
}
int main(int argc, char * argv[]){
	int sleep = 0;
	struct itimerval it;
	struct timeval tv;
	struct timeval tim;
	struct sigaction action;
	char *args[5] = {NULL,NULL,NULL,NULL,NULL};
	int status = 0;
	int i = 0;
	action.sa_handler = ticker;
	sigaction(SIGALRM, &action, NULL);
	sleep = atoi(argv[1]);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	tim.tv_sec = sleep;
	tim.tv_usec = 0;
	it.it_interval = tv;
	it.it_value = tim;
	setitimer(ITIMER_REAL, &it, NULL);
	
	for(i = 2; i < argc; i++){
		args[i-2] = (char *)malloc(sizeof(char)* 256);
		strcpy(args[i-2], argv[i]);
		//printf("command:%s\n", args[i-2]);
	}	
	if(!(child = fork())){
		execvp(args[0], args);
		exit(1);
	//	return 1;
	}
/*	else{
	printf("status: %d\n", status);
	return status;
	}*/

	wait(&status);
	int exitstatus =0;
	exitstatus = WIFSTOPPED(status);
	if(WIFSTOPPED(status)){
		return WEXITSTATUS(status);
	}
	else{
		return 1;
	}
}
