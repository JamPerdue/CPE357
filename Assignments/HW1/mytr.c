#include <stdio.h>
#include <string.h>
#include "mytr.h"
#include <stdlib.h>


void translate(char *orig, char *soln, char translate[]){
	char last = soln[strlen(soln)-1];
	int i = 0;
	int k = 0;
	int check = 0;
	char current;
	int len1;
	int len2;
	len1 = strlen(orig);
	len2 = strlen(soln);

	while(i<len1){
		if(i<len2){
			if(soln[k]=='\\'){
				k++;

				if(soln[k]==110){
					current = 10;
					translate[(int)orig[i]] = 10;
				}
				else if(soln[k] == 0){
					current = 92;
					translate[(int)orig[i]] = 92;
					
				}
				else if(soln[k] ==116){
					current = 9;
					translate[(int)orig[i]] = 9;
				}
				else{
					
					current = soln[k];
					translate[(int)orig[i]] = soln[k];
				}
			
			}
			else{
				current = soln[k];
				translate[(int)orig[i]] = soln[k];
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
		k++;	
		i++;
	}

}
void translateDel(char *orig, char translate[]){
	int i = 0;
	int len1;
	len1 = strlen(orig);

	while(i<len1){
				
		translate[(int)orig[i]] = 0;
		
	
		i++;
	}

}

int main(int argc, char *argv[]){
	char *key;
	char *sol;
	char translated[256];
	int curr;
	int cc = 0;
	int count = 0; 
	if(argc < 3){
		fprintf(stderr, "Too few arguments\nUsage:\nmytr [-d] set1 set2\n");
		return 1;
	}
	

	if(argc > 3){
		fprintf(stderr, "Too many arguments\nUsage:\nmytr [-d] set1 set2\n");
		return 1;
	}
	if(argv[1][0]== 45&& argv[1][1]!= 100){
		fprintf(stderr, "Bad switch\nUsage:\nmytr [-d] set1 set2\n");
		return 1;
		}
	while(count<256){
		translated[count] = count;
		count++;
	}
	if(strcmp(argv[1],"-d")==0){
		key = argv[2];
		translateDel(key,translated);	

	}
	else{
		key = argv[1];
		sol = argv[2];
	
		translate(key,sol,translated);
		/*for(cc=0;cc<256;cc++){
			printf("%c",translated[cc]);
		}*/
	}
		while(EOF!=(curr=getchar())){
			putchar(translated[curr]);
		}
		
	return 0;
	}
