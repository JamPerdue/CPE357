#include <stdio.h>
#include <string.h>
#include "mytr.h"
#include <stdlib.h>


void translate(char *orig, char *soln, char translate[]){
	char last = soln[strlen(soln)-1];
	int i = 0;
	int len1 = strlen(orig);
	int len2 = strlen(soln);

	while(i<len1){
		if(i<len2){
			printf("%c",soln[i]);
			translate[i]=soln[i];
		}
		else{
			printf("%c",last);
			translate[i] = last;
		}
		i++;
	}

}

int main(int argc, char *argv[]){
	char *key;
	char *sol;
	char translated[101];
	if(1<0){
		return 1;
	}
	key = argv[1];
	sol = argv[2];
	/*translated = (char *)malloc(sizeof(char)*strlen(key));*/
	translate(key,sol,translated);
	
	
	return 0;
	}
