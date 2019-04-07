#include <stdio.h>





int main(int argc, char * argv[]){
	int i = 0;
	while(i<argc){
		if(argv[i][0]==45){
			printf("%s\n",argv[i]);
		}
		i++;
	}
	return 0;





}
