#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include "mush.h"
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_COM 512;




void execpipe(Stage stage, int in, int out){
	

}

void printstage(int in, int out, int n, int tot, char stage[512], Stage *real){
	char * tok = NULL;
	char temp[512];
	char *puttemp = NULL;
	int numarg = 0;
	char delim[5] = " ";
	char inp[512];
	char outp[512];
	int i = 0;
	/*for(i = 0; i < 10; i++){
	//	args[i] = NULL;
		memset(args[i], 0, 512);
	}*/
	strcpy(temp, stage);
//	printf("--------\n");
//	printf("Stage %d: \"%s\"\n", n, stage);
//	printf("--------\n");
	for(i = 0; i < 11; i++){
		real->argv[i] = (char *)malloc(sizeof(char)*512);
		memset(real->argv[i], 0 , 512);
	}
	real->argv[10] = NULL;
	memset(inp, 0, 512);
	memset(outp, 0, 512);
	if(in == 0&& n == 0){
	//	printf("input: original stdin\n");
		memset(real->in, 0, 512);
	}
	else if(in == 0){
		memset(real->in, 0 , 512);
		sprintf(real->in, "%d", n-1);
	//	printf("input: pipe from stage %d\n", n-1);
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
	//	printf("input: %s\n",real->in);
	}
	if(out == 0&& n == tot-1){
	//	printf("output: original stdout\n");
		memset(real->out, 0, 512);

	}
	else if(out == 0){
	//	printf("output: pipe to stage %d\n", n+1);
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
	//	printf("output: %s\n",real->out);

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
//	printf("argc: %d\n", numarg);
//	printf("argv: ");
	/*for(i = 0; i < 10; i++){
		if(real->argv[i][0] != '\0'){
			printf("\"%s\"", real->argv[i]);
		}
		if(i != 9 && real->argv[i+1][0] != '\0'){
			printf(",");	
		}
	}*/
//	printf("\n");
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

void do_cdcommand(char * arg2){
	char temp[512];
	if(strcmp(arg2, "..") == 0){
		chdir("..");
	}
	else{
		memset(temp, 0, 512);
		getcwd(temp, 512);
		strcat(temp, arg2);
		chdir(temp);
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
	//printf("%s\n", getcwd(temp, 512));
}

void checkstage(int n, int tot, char stage[512]){
	int i = 0;
	int blank = 0;
	char * tok = NULL;
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
}
void closepipes(int pipes[10][2], int tot){
	int i = 0;
	for(i = 0; i < tot; i++){
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}
void pipeline(Stage stages[10], pid_t child[10], int pipes[10][2], int tot, int filein, int fileout){
	int i = 0;
	if(tot == 1){
		//printf("here\n");
		if(strcmp(stages[0].argv[0], "cd")==0){
			do_cdcommand(stages[0].argv[1]);
		}
		else{
		dup2(filein, STDIN_FILENO);
		dup2(fileout, STDOUT_FILENO);
		execv(stages[0].argv[0], (char**)stages[0].argv);
		perror("execerror");
		}
	}
	else{
	for(i = 0; i<tot; i++){
		if(strcmp(stages[i].argv[0], "cd")==0){
			do_cdcommand(stages[0].argv[1]);
		}
		else{
		if(!(child[i] =fork())){
			if(i== 0 && i != tot-1){
				dup2(filein, STDIN_FILENO);
			}
			else{
				dup2(pipes[i-1][0], STDIN_FILENO);
			}
			if(i != tot-1){
			dup2(pipes[i][1], STDOUT_FILENO);
			}
			else if(fileout != STDOUT_FILENO){
				dup2(fileout, STDOUT_FILENO);
			}
			//execpipe(stages[i]);
			//printf("command: %s\n", stages[i].argv[0]);
			execv(stages[i].argv[0],(char**)stages[i].argv);
			perror("execerror");
			closepipes(pipes, tot);
			if(filein != STDIN_FILENO){
				close(filein);
			}
			if(fileout != STDOUT_FILENO){
				close(fileout);
			}
			exit(-1);
		}
		}
	}
	for(i = 1; i<tot+1; i++){
		wait(NULL);
	}
	}

}

int main(int argc, char * argv[]){
	int i = 0;
	int j = 0;
	char commline[512];
	char ch = 0;
	char stage[512];
	pid_t child[10];
	int pipes[10][2];
	int filein = 0;
	int fileout = 0;
	Stage stages[10];
	FILE *file = stdin;
	if(argc ==2){
		file = fopen(argv[1], "r");
	}
	while(strcmp(commline, "QUIT")!= 0){
	memset(commline, 0, 512);
	memset(stage, 0, 512);
	printf("8-P ");
	i = 0;

	while((ch = fgetc(file))!= '\n'){
	//	printf("I:%d\n", i);
		
		if(i>= 512){
	//		printf("I:%d\n", i);
			fflush(stdout);
			fprintf(stderr, "command line too long\n");
			exit(1);
		}
		commline[i] = ch;
		i++;
	}
	if(strcmp(commline, "QUIT")== 0){
		continue;
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
//		stages[i].argv[10] = NULL;
	}
	filein = open(stages[0].in, O_RDONLY);
	if(filein == -1){
		filein = STDIN_FILENO;
	}
	fileout = open(stages[tot-1].out, O_WRONLY | O_CREAT | O_TRUNC); 
	if(fileout == -1){
		fileout = STDOUT_FILENO;
	}
	for(i = 0; i <tot; i++){
		pipe(pipes[i]);
	}
	//printf("total stages: %d\n",tot);
	pipeline(stages, child, pipes, tot, filein, fileout);
	for(i = 0; i < tot; i++){
		for(j = 0; j < 10; j++){
			free(stages[i].argv[j]);
		}
	}
//	dup2(STDIN_FILENO, STDIN_FILENO);
	closepipes(pipes, tot);
	if(filein != STDIN_FILENO){
	close(filein);
	}
	if(fileout != STDOUT_FILENO){
	close(fileout);
	}
	i = 0;
	fflush(stdout);
	}
	fclose(file);
	return 0;
}
