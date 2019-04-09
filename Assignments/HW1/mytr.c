#include <stdio.h>
#include <string.h>
#include "mytr.h"
#include <stdlib.h>


void translate(char *orig, char *soln, char translate[]){
	char last = soln[strlen(soln)-1];
	int i = 0;
	int check = 0;
	char current;
	int len1;
	int len2;
	len1 = strlen(orig);
	len2 = strlen(soln);

	while(i<len1){
		if(i<len2){
			current = soln[i];	
			translate[i] = soln[i];
			
		}
		else{
			current = last;
			translate[i] = last;
		}
		
		check = 0;
		while(check<i){
			if(orig[i] == orig[check]){
				printf("duplicate\n");
				translate[check] = current;
			}
			check++;
		}	
		i++;
	}

}

int main(int argc, char *argv[]){
	char *key;
	char *sol;
	char translated[101];
	char *final;

	if(1<0){
		return 1;
	}

	key = argv[1];
	sol = argv[2];

	translate(key,sol,translated);
	final = translated;
	printf("%s\n",final);	
	
	return 0;
	}
