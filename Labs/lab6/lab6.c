#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
/*#include <sys/sysmacros.h>
#include "lab6.h"
*/
void print_info(char * curr_file){
	struct stat stats;
	stat(curr_file, &stats);
	printf("File: '%s'\n",curr_file);
	printf("Type: ");
	switch (stats.st_mode & S_IFMT) {
           case S_IFBLK:  printf("Block device\n");            break;
           case S_IFCHR:  printf("Character device\n");        break;
           case S_IFDIR:  printf("Directory\n");               break;
           case S_IFIFO:  printf("FIFO\n");               break;
           case S_IFLNK:  printf("Symbolic Link\n");                 break;
           case S_IFREG:  printf("Regular File\n");            break;
           case S_IFSOCK: printf("Socket\n");                  break;
           default:       printf("Unknown?\n");                break;	
	}
	printf("Size: %d\n",stats.st_size);
	printf("Inode: %d\n", stats.st_ino);
	printf("Links: %d\n", stats.st_nlink);
	printf("Access: ");
	switch (stats.st_mode & S_IFMT) {
           case S_IFBLK:  printf("b");            break;
           case S_IFCHR:  printf("c");        break;
           case S_IFDIR:  printf("d");               break;
           case S_IFIFO:  printf("p");               break;
           case S_IFLNK:  printf("l");                 break;
           case S_IFREG:  printf("-");            break;
           case S_IFSOCK: printf("Socket\n");                  break;
           default:       printf("Unknown?\n");                break;	
	}
	printf( (stats.st_mode & S_IRUSR) ? "r" : "-");
   	printf( (stats.st_mode & S_IWUSR) ? "w" : "-");
   	printf( (stats.st_mode & S_IXUSR) ? "x" : "-");
  	printf( (stats.st_mode & S_IRGRP) ? "r" : "-");
    	printf( (stats.st_mode & S_IWGRP) ? "w" : "-");
   	printf( (stats.st_mode & S_IXGRP) ? "x" : "-");
    	printf( (stats.st_mode & S_IROTH) ? "r" : "-");
    	printf( (stats.st_mode & S_IWOTH) ? "w" : "-");
    	printf( (stats.st_mode & S_IXOTH) ? "x" : "-");	
	printf("\n");
}

/*char * typeit(mode_t mode){
	char c = '\0';
	char * type = "";
	
	switch(mode & S_IFMT)
	{
	case S_IFBLK:
		type = "b";
		break;
	case S_IFCHR:
		type = "c";
		break;
	case S_IFDIR:
		type = "d";
		break;
	case S_IFIFO:
		type = "p";
		break;
	case S_IFLNK:
		type = "l";
		break;
	case S_IFREG:
		type = "-";
		break;
	}
	return type;
}	
*/
int main(int argc, char *argv[]){
	int file_count = argc - 1;
	int file_ptr = 1;
	int curr_file = 0;
	int test = S_IFMT;
	char * file = NULL;
	if(file_count == 0){
		fprintf(stderr, "put filenames");
		return 1;
	}
	
	while(file_ptr < argc){
		file = argv[file_ptr];
		print_info(file);
		file_ptr++;
	}
	return 1;
}
