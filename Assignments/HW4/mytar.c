#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include "mytar.h"
#include <string.h>
#include <pwd.h>
#include <grp.h>

void build_dir(char * dirname){
	struct dirent *de;

	DIR *dr = opendir(dirname);
	if (dr == NULL)   
    { 
        printf("Could not open current directory" ); 
        return; 
    } 
  
     /*Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
         // for readdir() */
            while ((de = readdir(dr)) != NULL){
                         printf("%s\n", de->d_name); 
                           }
                               closedir(dr);     
}

uint8_t * pack_header(char *name){
	uint8_t * block = (uint8_t *)malloc(sizeof(uint8_t)*512);
	int i = 0;
	int j = 0;
	struct stat file_stat;
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char linkname[100];
	char uname[32];
	char gname[32];
	char dmaj[8];
	char dmin[8];
	char names[512];
	struct passwd* pass;
	struct group *grp;
	int namecounter = 0;
	int maj = 0;
	int min = 0;
	int checksum = 0;
	char check[8];
	lstat(name, &file_stat);

	for(i = 0; i<512; i++){
		block[i] = 0;
	}
	for(i = 0; i<strlen(name); i++){
		block[i] = name[i];
		namecounter++;
	}
	if(i < 100){
		block[i] = '\0';
	}
	/*printf("mode %o\n",file_stat.st_mode);	*/
	sprintf(mode, "%o", file_stat.st_mode);
	printf("modestr: %s\n", mode);
	for(i = 100, j = 0; i <108; i++, j++){
		block[i] = mode[j];		
		/*printf("mod: %d\n",mode[j]);*/
	}/* MODE*/
	sprintf(uid, "%o", file_stat.st_uid);
	for(i = 108, j = 0; i <116; i++, j++){
		block[i] = uid[j];		
	} /* UID*/

	sprintf(gid, "%o", file_stat.st_gid);
	for(i = 116, j = 0; i <124; i++, j++){
		block[i] = gid[j];		
	} /* GID*/


/*	printf("size: %d\n", file_stat.st_size);
	sprintf(size, "%o", file_stat.st_size);
	for(i = 124, j = 0; i <136; i++, j++){
		block[i] = size[j];		
	}  SIZE*/

	sprintf(mtime, "%o", file_stat.st_mtime);	
	for(i = 136, j = 0; i< 148; i++, j++){
		block[i] = mtime[j];
	} /* MTIME */
	/* calc checksum*/
	
	
	switch (file_stat.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  block[156] = '5';               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  block[156] = '2';                 break;
           case S_IFREG:  block[156] = '\0';            break;
           default:       printf("unknown?\n");                break;
           }	
	switch (file_stat.st_mode &S_IFMT){
		case S_IFLNK:
			
			break;
	}
	
	if(block[156] != '5'){		 
	
		sprintf(size, "%o", file_stat.st_size);
		for(i = 124, j = 0; i <136; i++, j++){
			block[i] = size[j];		
		}
	} 
	block[257] = 'u';
	block[258] = 's';
	block[259] = 't';
	block[260] = 'a';
	block[261] = 'r';
	block[262] = '\0';
	/*magic*/
	block[263] = '0';
	block[264] = '0';
 	/*version*/
	pass =getpwuid(file_stat.st_uid);
	strcpy(uname,pass->pw_name);
	for(i = 265, j = 0; i< 296; i++, j++){
		block[i] = uname[j];
	}
	block[296] = '\0';
	/* user name*/
	for(i = 0; i<32; i++){
		names[i] = '\0';
	}
	grp = getgrgid(file_stat.st_gid);
	strcpy(gname, grp->gr_name);
	for(i = 297, j = 0; i< 328; i++, j++){
		block[i] = gname[j];
	}
	block[328] = '\0';
	/* group name */

	/* group name 
	if (S_ISCHR(file_stat.st_mode) || S_ISBLK(file_stat.st_mode)) {
       	 maj = major(file_stat.st_rdev);
       	 min = minor(file_stat.st_rdev);
   	}

	sprintf(dmaj, "%o", maj);
	
	block[328] = '\0';*/

	/* group name */
	if (S_ISCHR(file_stat.st_mode) || S_ISBLK(file_stat.st_mode)) {
       	 maj = major(file_stat.st_rdev);
       	 min = minor(file_stat.st_rdev);
   	}

	sprintf(dmaj, "%o", maj);
	for(i = 329, j = 0; i <337; i++, j++){
		block[i] = dmaj[j];		
	} 

	sprintf(dmin, "%o", min);
	for(i = 337, j = 0; i <345; i++, j++){
		block[i] = dmin[j];		
	}
	/*devices*/
	if(namecounter < strlen(name)-1){
		for(i = 345, j = namecounter; j <strlen(name); i++, j++){
			block[i] = name[j];
		}
	}
	/* prefix*/
	for(i = 0; i< 512; i++){
		checksum += block[i];
	}
	sprintf(check, "%o", checksum);
	for(i = 148, j = 0; i<156; i++, j++){
		block[i] = check[j];
	}
	/*checksum*/
	return block;	
}

void unpack_header(uint8_t head[512]){
	uint8_t mover = 0;
	int i = 0;
	mover = head[0];
	while(mover != 0){
		printf("%c",mover);	
		i++;
		mover = head[i];
	}
	printf("\n");
	printf("mode: i\n");
	
}

/*void writearchive(char * name, char * archivename){

	

}*/

void create(char *archive, char *path){
	int file_out = open(archive, O_WRONLY | O_CREAT, 0644);
	int i = 0;
	uint8_t buff[512];
	struct stat file_stat;
	lstat(path, &file_stat);
	
	for(i = 0; i< 512; i++){
		buff[i] = 0;
	}
	
	listdir(path, 0, file_out);
	write(file_out, buff, 512);
	write(file_out, buff, 512);

	close(file_out);

}

void writebody(char * file, int file_out){
	int file_in = open(file, O_RDONLY);
	uint8_t block_in[1];
	uint8_t block_out[512];

	int i = 0;
	int j = 0;
	write(file_out, pack_header(file), 512);
	printf("head\n");
		
	
	while(read(file_in, block_in, 1)>0);
	{
		/*printf("looping\n");*/
		block_out[i] = block_in[0];
		i++;
		if(i >= 512){
			write(file_out, block_out, 512);
			i = 0;
		}
	}
	if(i != 0){
		write(file_out, block_out, i);
	}
	
	close(file_in);
}
void listdir(char * name, int level, int file_out){
    DIR *dir;
    struct dirent *entry;
    char path[1024];
    int len = 0;
    if (!(dir = opendir(name))){
	writebody(name,file_out);
        return;
	}
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
                continue;
		}
            printf("%*s%s\n", level*2, "", entry->d_name);
            listdir(path, level, file_out);
		writebody(entry->d_name, file_out);
        }
        else{
            printf("%*s- %s\n", level*2, "", entry->d_name);
		writebody(entry->d_name, file_out);
        }
    } while(entry = readdir(dir));
	/*printf("%s\n",name);*/
	writebody(name, file_out);
    closedir(dir);

}

void parse_cm(char * cm, char *tarfile, char *path){
	int i = 0;
	int v = 0;
	if(cm[0] == 'c'){
		create(tarfile,path);
	}/*
	else if(cm[0] == 't'){
		if(cm[1] == 'v'){
			v = 1;
		}
		table(tarfile, v);
	}
	else if(cm[0] =='x'){
		extract(tarfile, path);
	}*/
	else{
		perror("Need c t or x");
	}
}
int main(int argc, char *argv[]){
	char * path = "";
	char * tarfile = "";
	int i = 0;
	int tarfile_read = 0;
	uint8_t blockbuf[512];
	
	/*if(argc < 4 || argv[1][strlen(argv[1])] != 'f'){
		perror("STOP");
		return 1;
	}*/
	for(i = 3; i<argc; i++){
		parse_cm(argv[1], argv[2] ,argv[i]);
	}
	return 0;
}
