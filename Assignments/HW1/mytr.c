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
			if(soln[i]==92){
				i++;
				if(soln[i]==110){
					current = 10;
					translate[(int)orig[i-1]] = 10;
				}
				if(soln[i] == 92){
					current = 92;
					translate[(int)orig[i-1]] = 92;
				}
				if(soln[i] ==116){
					current = 9;
					translate[(int)orig[i-1]] = 9;
				}
			
			}
			else{
				current = soln[i];
				translate[(int)orig[i]] = soln[i];
			}	
		}
		else{
			current = last;
			translate[(int)orig[i]] = last;
		}
		
		check = 0;
		while(check<i){
			if(orig[i] == orig[check]){
				
				translate[(int)orig[check]] = current;
			}
			check++;
		}	
		i++;
	}

}

int main(int argc, char *argv[]){
	char *key;
	char *sol;
	char translated[256];
	int curr;
	int lenkey;
	
	if(1<0){
		return 1;
	}

	key = argv[1];
	sol = argv[2];
	translate(key,sol,translated);
	while(EOF!=(curr=getchar())){
		if(translated[curr]!=0){
		
			printf("%c",translated[curr]);
		}
		else{
			printf("%c",(char)curr);
		}
	}
	return 0;
	}
