#include <stdio.h>
#include "uniq.h"
#include <string.h>
#include <stdlib.h> 

char *read_long_line(FILE *file){
	char * line = (char *)malloc(sizeof(char)*20);
	int count = 0;
	int size = 20;
	int move = 0;
	char c = 'a';
	while((c = fgetc(file))!=EOF && c != '\n'){
		count ++;
		if(count>size){
			line = (char *)realloc(line,sizeof(char)*count+2);
			size = count;
		}
		line[move] = c;
		move++;
	}
	if(c!=EOF){
		count ++;
		if(count>size){
			line = (char *)realloc(line,sizeof(char)*count+2);
			size = count;
			}
		line[move] = c;
		move++;
	}
	line[move] = '\0';
	return line;
}



int main(int argc, char *argv[]){
	char * old = NULL;
	char * current = NULL;
	
	FILE * file = stdin;
	int c = 0;
	old = read_long_line(file);
	for(c = 0; c<strlen(old);c++){
		putc(old[c],stdout);
	}
	
	while(strlen(current = read_long_line(file))!= 0){
		/*current = read_long_line(file);*/
		if(strcmp(current,old)!= 0){
			for(c = 0; c<strlen(current);c++){
				putc(current[c],stdout);
			}
		}
		strcpy(old,current);
	 	free(current);
		current = NULL;	
	}

	free(current);				
	free(old);
	return 0;
}		
