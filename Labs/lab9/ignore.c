#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void intHandler(int sig){
	printf("type quit\n");
}



int main(){
	char message[256];
	char ch = 0;
	int i = 0;
	memset(message, 0, 256);
	signal(SIGINT, intHandler);
	while(strcmp(message, "quit") != 0){
		memset(message, 0, 256);
		i = 0;
		while((ch = fgetc(stdin))!= '\n'){
			message[i]= ch;
			i++;
		}
		printf("message: %s\n", message);
	}
	return 0;
}
