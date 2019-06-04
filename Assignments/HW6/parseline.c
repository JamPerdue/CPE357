#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_COM 512;






void printstage(int in, int out, int n, int tot, char stage[512]){
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
	printf("--------\n");
	printf("Stage %d: \"%s\"\n", n, stage);
	printf("--------\n");
	memset(inp, 0, 512);
	memset(outp, 0, 512);
	if(in == 0&& n == 0){
		printf("input: original stdin\n");
	}
	else if(in == 0){
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
			inp[i]= *puttemp;
			puttemp++;
			i++;
		}	
		printf("input: %s\n",inp);
	}
	if(out == 0&& n == tot-1){
		printf("output: original stdout\n");
	}
	else if(out == 0){
		printf("output: pipe to stage %d\n", n+1);
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
			outp[i]= *puttemp;
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
		strcpy(args[numarg-1], tok);
		tok = strtok(NULL, delim);	
		}
	}
	printf("argc: %d\n", numarg);
	printf("argv: ");
	for(i = 0; i < 10; i++){
		if(args[i][0] != '\0'){
			printf("\"%s\"", args[i]);
		}
		if(i != 9 && args[i+1][0] != '\0'){
			printf(",");	
		}
	}
	printf("\n");
	/* print argv(command)*/
	
}

void setstage(int n,int tot, char stage[512]){
	int in = 0;
	int out = 0;
	char *tok = NULL;
			
	if((tok = strpbrk(stage, "<"))!= NULL){
		in = 1;
	}
	if((tok = strpbrk(stage, ">"))!= NULL){
		out = 1;
	}

	printstage(in, out, n, tot, stage);
}	
void makestage(int n, char commline[512], char stage[512]){
	char str[512];
	char * tok = NULL;
	char delim[2] = "|";
	int in = 0;
	int out = 0;
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
}

int main(int argc, char * argv[]){
	int i = 0;
	char *command = NULL;
	char commline[512];
	char ch = 0;
	char stage[512];
	memset(commline, 0, 512);
	memset(stage, 0, 512);
	printf("line: ");
	i = 0;
	while((ch = fgetc(stdin))!= '\n'){
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
		setstage(i, tot, stage); 
	}
}
