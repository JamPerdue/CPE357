#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include "mush.h"
#define MAX_COM 512;




void execpipe(Stage stage){
	

}

void printstage(int in, int out, int n, int tot, char stage[512], Stage *real){
	char * tok = NULL;
	char temp[512];
	char *puttemp = NULL;
	int numarg = 0;
	char args[10][512];
	char *prev = NULL;
	char delim[5] = " ";
	char inp[512];
	char outp[512];
	int i = 0;
	for(i = 0; i < 10; i++){
	//	args[i] = NULL;
		memset(args[i], 0, 512);
	}
	strcpy(temp, stage);
//	printf("--------\n");
//	printf("Stage %d: \"%s\"\n", n, stage);
//	printf("--------\n");
	memset(inp, 0, 512);
	memset(outp, 0, 512);
	if(in == 0&& n == 0){
		printf("input: original stdin\n");
		memset(real->in, 0, 512);
	}
	else if(in == 0){
		memset(real->in, 0 , 512);
		sprintf(real->in, "%d", n-1);
		printf("input: pipe from stage %d\n", n-1);
	}
	else{
		puttemp = strpbrk(stage,"<");
		puttemp++;
		if(isspace(*puttemp)){
			puttemp++;
		}
		i = 0;
		while(isspace(*puttemp) == 0){
			real->in[i]= *puttemp;
			puttemp++;
			i++;
		}	
		printf("input: %s\n",inp);
	}
	if(out == 0&& n == tot-1){
		printf("output: original stdout\n");
		memset(real->out, 0, 512);

	}
	else if(out == 0){
		printf("output: pipe to stage %d\n", n+1);
		sprintf(real->out, "%d", n+1);
	}
	else{
		puttemp = NULL;
		puttemp = strpbrk(stage,">");
		puttemp++;
		if(isspace(*puttemp)){
			puttemp++;
		}
		i = 0;
		while(isspace(*puttemp) == 0){
			real->out[i]= *puttemp;
			puttemp++;
			i++;
		}	
		printf("output: %s\n",outp);

	}
	tok = strtok(temp, delim);
	//numarg++;
	//strcpy(args[0], temp);
	while(tok!= NULL){
		if(strcmp(tok, "<")==0 || strcmp(tok, ">") == 0){	
			tok = strtok(NULL, delim);
			tok = strtok(NULL, delim);
			continue;
		}
		else{
		numarg++;
		if(numarg > 10){
			fprintf(stderr, "%s: too many arguments\n", stage);
			exit(1);
		}
		strcpy(real->argv[numarg-1], tok);
		tok = strtok(NULL, delim);	
		}
	}
	printf("argc: %d\n", numarg);
	printf("argv: ");
	for(i = 0; i < 10; i++){
		if(real->argv[i][0] != '\0'){
			printf("\"%s\"", real->argv[i]);
		}
		if(i != 9 && real->argv[i+1][0] != '\0'){
			printf(",");	
		}
	}
	printf("\n");
	/* print argv(command)*/
	
}

void setstage(int n,int tot, char stage[512], Stage *real){
	int in = 0;
	int out = 0;
	char *tok = NULL;
			
	if((tok = strpbrk(stage, "<"))!= NULL){
		in = 1;
	}
	if((tok = strpbrk(stage, ">"))!= NULL){
		out = 1;
	}

	printstage(in, out, n, tot, stage, real);
}	
void makestage(int n, char commline[512], char stage[512]){
	char str[512];
	char * tok = NULL;
	char delim[2] = "|";
	int i = 0;
	char temp[512];
	memset(str, 0, 512);
	for(i = 0; i < 512; i++){
		str[i] = commline[i];
	}
	tok = strtok(str, delim);
	strcpy(temp, str);
	i = 0;
	while(tok != NULL&& i<=n){
		strcpy(temp, tok);
		tok = strtok(NULL, delim);
	//	printf("tok:%s\n", tok);
	//	printf("i:%d\n",i);	
		i++;
	}
	if(temp != NULL){
		strcpy(stage, temp);
	}
	
}
int pipecount(char commline[512]){
	char str[512];
	int pipe = 0;
	char *tok = NULL;
	char delim[2] = "|";
	int i = 0;
	memset(str, 0, 512);
	for(i = 0; i < 512; i++){
		str[i] = commline[i];
	}
	tok = strtok(str, delim);
	while(tok != NULL){
		pipe++;
		tok = strtok(NULL,delim);
	//	printf("%s\n",tok);
	}
	return pipe;
}

void do_cdcommand(char stage[512]){
	DIR * dir = NULL;
	struct dirent *entry;
	int i = 0;
	char * tok = NULL;
	char temp[512];
	char delim[2]= " ";
	strcpy(temp,stage);
	tok = strtok(temp, delim);
	if(strcmp(temp, "cd") != 0){
	while(tok != NULL && strcmp(tok,"cd") != 0){
		tok = strtok(NULL, delim);
	}
	tok = strtok(NULL, delim);
	}
	if(strlen(tok)< 8){
		memset(temp, 0, 512);
		getcwd(temp, 512);
		strcat(temp, tok);
		chdir(temp);
	}
	else{
		chdir(tok);
	} 
	/*
	dir = opendir(".");
	while((entry = readdir(dir)) != NULL){
		if(strcmp("..",entry->d_name)!= 0 && strcmp(".", entry->d_name) != 0){
		
		printf("%s ", entry->d_name);
		}
	}
	printf("\n");
	*/
	printf("%s\n", getcwd(temp, 512));
}

void checkstage(int n, int tot, char stage[512]){
	int i = 0;
	int blank = 0;
	char str[512];
	char *tok = NULL;
	char *temp = NULL;
	char *in = "<";
	char *out = ">";
	if((temp = strpbrk(stage, in))!= NULL){
		if(n != 0){
			fprintf(stderr, "%s: ambiguous input\n", stage);
			exit(1);
		}
		if((tok = strpbrk(temp+1, in))!= NULL){
			fprintf(stderr, "%s: bad input redirection\n", stage);
			exit(1);
		}
	}
	if((temp = strpbrk(stage, out))!= NULL){
		if(n != tot-1){
			fprintf(stderr, "%s: ambiguous output\n", stage);
			exit(1);
		}
		if((tok = strpbrk(temp+1, out))!= NULL){
			fprintf(stderr, "%s: bad output redirection\n", stage);
			exit(1);
		}
	}
	for(i = 0; i<strlen(stage); i++){
		if(stage[i] != ' '){
			blank = 1;
		}
	}
	if(blank == 0){
		fprintf(stderr, "invalid null command\n");
		exit(1);
	}
	if((temp = strpbrk(stage, "cd"))!= NULL){
		if(*(temp+1) == 'd'){
			do_cdcommand(stage);
		}
	}
}
/*
void do_cdcommand(char stage[512]){
	DIR * dir = NULL;
	struct dirent *entry;
	int i = 0;
	dir = opendir(".");
	while((entry = readdir(dir)) != NULL){
		if(strcmp("..",entry->d_name)!= 0 && strcmp(".", entry->d_name) != 0){
		
		printf("%s ", entry->d_name);
		}
	}
	printf("\n");
}
*/
int main(int argc, char * argv[]){
	int i = 0;
	char *command = NULL;
	char commline[512];
	char ch = 0;
	char stage[512];
	pid_t child[10];
	Stage stages[10];
//	while(read(0, NULL, 1) != 0){
	while(1){
//	lseek(0, SEEK_SET, 0);
	memset(commline, 0, 512);
	memset(stage, 0, 512);
	printf("8-P ");
	i = 0;
//	fflush(stdout);
	while((ch = fgetc(stdin))!= '\n'){
		if(i>= 512){
			fprintf(stderr, "command line too long\n");
			exit(1);
		}
		commline[i] = ch;
		i++;
	}
	
//	fscanf(stdin, "%s", commline);	
	if(pipecount(commline) > 9){
		fprintf(stderr, "pipeline too deep\n");
		exit(1);
	}
	int tot = pipecount(commline);
	for(i = 0; i<tot; i++){
		memset(stage, 0, 512);
		makestage(i, commline, stage);
		checkstage(i, tot, stage);
		setstage(i, tot, stage, &stages[i]); 
		
	}
	for(i = 0; i<tot; i++){
		if(!(child[i] =fork())){
			execpipe(stages[i]);
			printf("%d\n",child[i]);
			exit(-1);
		}
	}
	for(i = 1; i<tot+1; i++){
		wait(NULL);
	}
	}
	return 0;
}
