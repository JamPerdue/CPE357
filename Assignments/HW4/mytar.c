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
	struct passwd* pass;
	struct group *grp;
	int namecounter = 0;
	int maj = 0;
	int min = 0;
	int checksum = 0;
	char check[8];
	lstat(name, &file_stat);

	for(i = 0; i<512; i++){
		block[i] = '\0';
	}
	printf("%s\n",name);
	for(i = 0; i<strlen(name); i++){
		block[i] = name[i];
		namecounter++;
	}
	if(i<100 &&(file_stat.st_mode & S_IFMT) == S_IFDIR){
		block[i] = '/';
		i++;
	}
	if(i < 100){
		block[i] = '\0';
	}
	/*printf("mode %o\n",file_stat.st_mode);	*/
	printf("%s\n",block);
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
	for(i = 148; i< 156; i++){
		block[i] = ' ';
	}	
	
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
		printf("size: %s\n",size);
		for(i = 124, j = 0; i <136; i++, j++){
			block[i] = size[j];		
		}
	}
	else{
		for(i = 124; i <136; i++){
			block[i] = '0';
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
	for(i = 265, j = 0; j<strlen(uname); i++, j++){
		block[i] = uname[j];
	}
	block[296] = '\0';
	/* user name*/
	grp = getgrgid(file_stat.st_gid);
	strcpy(gname, grp->gr_name);
	for(i = 297, j = 0;j <strlen(gname); i++, j++){
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
	else{
		min = 0;
		maj = 0;
	}
		sprintf(dmaj, "%o", maj);
		sprintf(dmin, "%o", min);


	for(i = 329, j = 0; i <337; i++, j++){
		block[i] = dmaj[j];		
	} 

	for(i = 337, j = 0; i <345; i++, j++){
		block[i] = dmin[j];		
	}
	/*devices*/
	if(namecounter < strlen(name)-1){
		printf("left\n");
		for(i = 345, j = namecounter; j < strlen(name); i++, j++){
			block[i] = name[j];
		}
	}
	/* prefix*/
	for(i = 0; i< 500; i++){
		checksum += block[i];
	}
	sprintf(check, "%o", checksum);
	printf("check %s\n",check);
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

void create(int file_out, char *path, int v){
	dirtrav(path, file_out);
}

void writebody(char * file, int file_out){
	int file_in;
	uint8_t block_out[512];
	uint8_t *block;
	int i = 0;
	if(access(file, F_OK) == -1){
		return;
	}
	file_in = open(file, O_RDONLY);
	block = pack_header(file);
	write(file_out, block, 512);
	printf("writing %s\n", file);	
	memset(block_out, 0, 512);
	while((i = read(file_in, block_out, 512)) >0){
		write(file_out, block_out, 512);
		perror("Write error");
		memset(block_out, 0 ,512);
	}
	memset(block_out, 0 ,512);
	free(block);
	close(file_in);
}
void dirtrav(char *name, int file_out){
	DIR *dir;
	struct dirent *entry;
	struct stat filestats;
	char *temp = "/";
	int len = 0;
	lstat(name, &filestats);
	if((filestats.st_mode & S_IFMT) != S_IFDIR){
		writebody(name, file_out);
		return;
	}
	if(!(dir = opendir(name))){
		return;
	}
	writebody(name, file_out);

	while((entry = readdir(dir))){
		char path[1024];
		strcpy(path, name);
		if(entry->d_type == DT_DIR){
			if(strcmp(entry->d_name, ".") == 0 || 
				strcmp(entry->d_name, "..") == 0){
				printf("cont\n");
				continue;
			}
			strcat(path,temp);
			strcat(path, entry->d_name);
			printf("test1\n");
			printf("path: %s\n",path);

			dirtrav(path, file_out);
			writebody(entry->d_name, file_out);
		}
		else{
			printf("test2\n");
			printf("%s\n", entry->d_name);
			strcat(path, temp);
			strcat(path, entry->d_name);
			printf("pppath:%s\n", path);

			writebody(path, file_out);
		}
	}
	/*writebody(name, file_out);*/

	closedir(dir);
}
/*void listdir(char * name, int level, int file_out){
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
            printf("test1\n");
            listdir(path, level, file_out);
		writebody(entry->d_name, file_out);
        }
        else{
            printf("test2\n");
		writebody(entry->d_name, file_out);
        }
    } while(entry = readdir(dir));

	writebody(name, file_out);
    closedir(dir);

}*/
/*void table(char *tarfile, int v){
	int file_in = open(tarfile, O_RDONLY);
	int i = 0;
	uint8_t buff[512];
	int bytesread = 0;
	while((bytesread = read(file_in, buff, 512))> 0){
		

	}
}*/
/*void parse_cm(char * cm, char *tarfile, char *path, int file_out){
	int i = 0;
	int v = 0;
	if(cm[0] == 'c'){
		create(file_out,path);
	}
	else if(cm[0] == 't'){
		if(cm[1] == 'v'){
			v = 1;
		}
		table(tarfile, v);
	}
	else if(cm[0] =='x'){
		extract(tarfile, path);
	}
	else{
		perror("Need c t or x");
	}
}
*/
void proper_parse(int argc, char **argv){
	int v = 0;
	int file_out = 0;
	int i =0;
	uint8_t buf[512];
	if(argv[1][0] == 'c'){
		if(argv[1][1] == 'v'){
			v = 1;
		}
		if(argc <= 2){
			perror("need tarfile");
			return;
		}
		if(argc <= 3){
			perror("Need at least 1 file");
			return;
		}
		file_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		printf("FIle to be written: %d\n", file_out);
		for(i = 3; i< argc; i++){
			create(file_out, argv[i], v);
		}
		memset(buf, 0 ,512);

		write(file_out, buf, 512);
		write(file_out, buf, 512);
		

		close(file_out);
	}
	
}	
int main(int argc, char *argv[]){
	char * path = "";
	char * tarfile = "";
	int i = 0;
	int tarfile_read = 0;
	uint8_t blockbuf[512];
	int file_out;
	if(argc <= 1){
		perror("No arguments\n");
		return 1;
	}	
	if(argv[1][strlen(argv[1])-1] != 'f'){
		perror("STOP");
		return 1;
	}	
	/*file_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	for(i = 3; i<argc; i++){
		parse_cm(argv[1], argv[2] ,argv[i], file_out);
	}
	close(file_out);*/
	proper_parse(argc, argv);
	return 0;
}
