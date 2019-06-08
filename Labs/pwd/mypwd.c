#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>

int main(int argc, char *argv[]){
	struct dirent *dir = NULL;
	DIR * direct = NULL;
	char path[2048];
	getcwd(path,2048);	
	printf("%s\n", path);
}
