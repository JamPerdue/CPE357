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
#include <time.h>
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
	int k = 0;
	lstat(name, &file_stat);

	for(i = 0; i<512; i++){
		block[i] = '\0';
	}
/*	for(i = 0; i<strlen(name); i++){
		block[i] = name[i];
		namecounter++;
	}*/
	if(strlen(name)>99){
		i = strlen(name)-100;
		j = 0;
		k = 345;
		while(i> 0){
			block[k] = name[j];
			j++;
			i--;
			k++;
		}
		block[k]=0;
		i = 0;
		while(name[j]!=0){
			block[i] =name[j];
			i++;
			j++;
		}
		if(i<100){
		block[i] = 0;
		}
	}
	else{
		i = 0;
		while(name[i]!= 0){
			block[i] = name[i];
			i++;
		}
		block[i] = 0;
	}	
	if(i<100 &&(file_stat.st_mode & S_IFMT) == S_IFDIR){
		block[i] = '/';
		i++;
	}
	if(i < 100){
		block[i] = '\0';
	}
	/*printf("mode %o\n",file_stat.st_mode);	*/
	/*printf("%s\n",block);*/
	sprintf(mode, "%o", file_stat.st_mode);
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
		/*printf("size: %s\n",size);*/
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
	/*if(namecounter < strlen(name)-1){
		printf("left\n");
		for(i = 345; i<500; i++){
			block[i] = name[namecounter];
			namecounter++;
		}
	}*/
	/* prefix*/
	for(i = 0; i< 500; i++){
		checksum += block[i];
	}
	sprintf(check, "%o", checksum);
	/*printf("check %s\n",check);*/
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
	/*printf("writing %s\n", file);	*/
	memset(block_out, 0, 512);
	while((i = read(file_in, block_out, 512)) >0){
		write(file_out, block_out, 512);
		/*perror("Write error");*/
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
				continue;
			}
			strcat(path,temp);
			strcat(path, entry->d_name);
			/*printf("test1\n");
			printf("path: %s\n",path);*/

			dirtrav(path, file_out);
			writebody(entry->d_name, file_out);
		}
		else{
			/*printf("test2\n");
			printf("%s\n", entry->d_name);*/
			strcat(path, temp);
			strcat(path, entry->d_name);
	/*		printf("pppath:%s\n", path);*/

			writebody(path, file_out);
		}
	}
	/*writebody(name, file_out);*/

	closedir(dir);
}

int octDec(int n){
	int num = n;
	int dec = 0;
	int base = 1;
	int temp = num;
	int lastdigit = 0;
	while(temp){
		lastdigit = temp %10;
		temp = temp/10;
		dec += lastdigit * base;
		base = base * 8;
	}
	return dec;
}
void table(int file_read, char *name, int v){
	int i = 0;
	uint8_t buff[512];
	char sizestring[12];
	char *ptr;
	int amount = 0;
	int j = 0;
	struct stat filestat;
	struct tm  ts;
   	char buf[17];
	int blockcount = 0;
	int rem = 0;
	int dircheck = 0;
	char namebuff[256];
	lseek(file_read, 0, SEEK_SET);
	if(name != NULL){
		if(v == 0){	
		while(read(file_read, buff, 512)>0){
		memset(namebuff,0,256);
	if(buff[345] != '\0'){
			i = 345;
			j = 0;
			while(buff[i] != '\0' && i<500){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			i = 0;
			namebuff[j] = '/';
			j++;
			while(i<100){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			namebuff[j] = '\0';
		}
		else{
			for(i = 0; i< 100; i++){
				namebuff[i] = buff[i];
			}
		}

		for(i = 0, j = 124; i<12; i++, j++){
			sizestring[i] = buff[j];
		}
		amount = octDec(atoi(sizestring));
		/*while(buff[i]!= 0 && i<100){
			if(buff[i] == name[0]){
				j = 1;
				while(j<strlen(name)&&buff[i]==name[j]){
					i++;
					j++;
				}
				if(buff[i] =='/'){
					dircheck = 1;
					break;
				}
			}
			i++;
		}*/
				
		if(namebuff[0]!='\0'&&strcmp(namebuff,name)==0&& dircheck == 0){
			printf("%s\n", namebuff);
		}
		 blockcount = amount/512;
		 rem = amount%512;
		if(rem!=0){
			blockcount++;
		}
		memset(buff, 0 ,512);
		lseek(file_read, blockcount*512, SEEK_CUR);
		}
		}
		else{
			while(read(file_read, buff, 512)>0){
			memset(namebuff, 0 ,256);
		if(buff[345] != '\0'){
			i = 345;
			j = 0;
			while(buff[i] != '\0' && i<500){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			i = 0;
			namebuff[j] = '/';
			j++;
			while(i<100){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			namebuff[j] = '\0';
		}
		else{
			for(i = 0; i< 100; i++){
				namebuff[i] = buff[i];
			}
		}

			for(i = 0, j = 124; i<12; i++, j++){
				sizestring[i] = buff[j];
			}
			amount = octDec(atoi(sizestring));

			if(namebuff[0]!= 0&&strcmp(namebuff,name)==0){
				i = 0;
				name[i] = 0;
				char mod[8];
				i = 100;
				j = 0;
				while(buff[i] != 0){
					mod[j] = buff[i];
					i++;
					j++;
				}
				mod[j] = 0;		
				int mode = octDec(atoi(mod));	
				switch (buff[156]){
					case '0': printf("-"); break;
					case '\0': printf("-"); break;
					case '2': printf("l"); break;
					case '5': printf("d"); break;
				}
 			printf( (mode & S_IRUSR) ? "r" : "-");
 			printf( (mode & S_IWUSR) ? "w" : "-");
 			printf( (mode & S_IXUSR) ? "x" : "-");
 			printf( (mode & S_IRGRP) ? "r" : "-");
 			printf( (mode & S_IWGRP) ? "w" : "-");
 			printf( (mode & S_IXGRP) ? "x" : "-");
 			printf( (mode & S_IROTH) ? "r" : "-");
 			printf( (mode & S_IWOTH) ? "w" : "-");
 			printf( (mode & S_IXOTH) ? "x" : "-");
				printf(" ");
				char user[8];
				char group[8];
				for(i = 0, j = 265; i<8; i++, j++){
					user[i] = buff[j];
				}
				user[7] = '\0';
				for(i = 0, j = 297; i<8; i++, j++){
					group[i] = buff[j];
				}
				group[7] = '\0';
				printf("%s/%s", user,group);
			
				char size[8];
				sprintf(size, "%d", amount);

				for(i = 0; i< 15-strlen(size); i++){
					printf(" ");
				}
				printf("%s",size);	
				printf(" ");
				char tt[12];
				i = 136;
				j = 0;
				while(buff[i] != 0){
					tt[j] = buff[i];
					i++;
					j++;
				}
				tt[j] =0; 
				time_t tit = (time_t) strtol(tt, &ptr, 8);

				ts = *localtime(&tit);
				
			memset(buf, 0 ,17);
			strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
				printf("%s ",buf);
				printf("%s\n", namebuff);
			}
			memset(buff, 0 ,512);
			 blockcount = amount/512;
			 rem = amount%512;
			if(rem!=0){
				blockcount++;
			}

			lseek(file_read, blockcount*512, SEEK_CUR);
		}
		}


	}
	else{
		if(v == 0){
		while(read(file_read, buff, 512)>0){
			if(buff[257] == 'u'){
		memset(namebuff, 0 ,256);
	/*	for(i = 0,j = 100; i< 100; i++, j++){
			namebuff[j] = buff[i];
		}
		if(namebuff[99]!= '\0'){
			i = 345;
			j = 100;
			while(buff[i] != '\0' && i< 500){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			
			namebuff[j] = '\0';
			
		}
		*/	
		if(buff[345] != '\0'){
			i = 345;
			j = 0;
			while(buff[i] != '\0' && i<500){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			i = 0;
			namebuff[j] = '/';
			j++;
			while(i<100){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			namebuff[j] = '\0';
		}
		else{
			for(i = 0; i< 100; i++){
				namebuff[i] = buff[i];
			}
		}
		for(i = 0, j = 124; i<12; i++, j++){
			sizestring[i] = buff[j];
		}
		amount = octDec(atoi(sizestring));
		if(buff[0]!= '\0'){
			printf("%s\n", namebuff);
		}
		 blockcount = amount/512;
		 rem = amount%512;
		if(rem!=0){
			blockcount++;
		}
		memset(buff, 0 ,512);
		lseek(file_read, blockcount*512, SEEK_CUR);
		}
		}
		}	
		else{
			
			while(read(file_read, buff, 512)>0){
				if(buff[257] == 'u'){
			memset(namebuff, 0 ,255);
		if(buff[345] != '\0'){
			i = 345;
			j = 0;
			while(buff[i] != '\0' && i<500){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			i = 0;
			namebuff[j] = '/';
			j++;
			while(i<100){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			namebuff[j] = '\0';
		}
		else{
			for(i = 0; i< 100; i++){
				namebuff[i] = buff[i];
			}
		}

			for(i = 0, j = 124; i<12; i++, j++){
				sizestring[i] = buff[j];
			}
			amount = octDec(atoi(sizestring));

			if(buff[0]!= 0){
				char name[100];
				i = 0;
				while(buff[i]!= 0){
					name[i] = buff[i];
					i++;
				}
				name[i] = 0;
				char mod[8];
				i = 100;
				j = 0;
				while(buff[i] != 0){
					mod[j] = buff[i];
					i++;
					j++;
				}
				mod[j] = 0;		
				int mode = octDec(atoi(mod));	
				switch (buff[156]){
					case '0': printf("-"); break;
					case '\0': printf("-"); break;
					case '2': printf("l"); break;
					case '5': printf("d"); break;
				}
 			printf( (mode & S_IRUSR) ? "r" : "-");
 			printf( (mode & S_IWUSR) ? "w" : "-");
 			printf( (mode & S_IXUSR) ? "x" : "-");
 			printf( (mode & S_IRGRP) ? "r" : "-");
 			printf( (mode & S_IWGRP) ? "w" : "-");
 			printf( (mode & S_IXGRP) ? "x" : "-");
 			printf( (mode & S_IROTH) ? "r" : "-");
 			printf( (mode & S_IWOTH) ? "w" : "-");
 			printf( (mode & S_IXOTH) ? "x" : "-");
				printf(" ");
				char user[8];
				char group[8];
				for(i = 0, j = 265; i<8; i++, j++){
					user[i] = buff[j];
				}
				user[7] = '\0';
				for(i = 0, j = 297; i<8; i++, j++){
					group[i] = buff[j];
				}
				group[7] = '\0';
				printf("%s/%s", user,group);
			
				char size[8];
				sprintf(size, "%d", amount);

				for(i = 0; i< 15-strlen(size); i++){
					printf(" ");
				}
				printf("%s",size);	
				printf(" ");
				char tt[12];
				i = 136;
				j = 0;
				while(buff[i] != 0){
					tt[j] = buff[i];
					i++;
					j++;
				}
				tt[j] =0; 
				time_t tit = (time_t) strtol(tt, &ptr, 8);

				ts = *localtime(&tit);
				
			memset(buf, 0 ,17);
			strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
				printf("%s ",buf);
				printf("%s\n", namebuff);
			}
			memset(buff, 0 ,512);
			 blockcount = amount/512;
			 rem = amount%512;
			if(rem!=0){
				blockcount++;
			}

			lseek(file_read, blockcount*512, SEEK_CUR);
		}
		}
		}
	}
}
void extract(int file_read, char * name){
	uint8_t buff[512];
	int i = 0;
	char namebuff[256];
	int j = 0;
	char sizestring[12];
	int amount = 0;
	int blockcount = 0;
	int rem = 0;
	memset(buff, 0 ,512);
	if(name != NULL){
		while(read(file_read, buff, 512)>0){
		/*printf("First header: %s\n", buff);*/
		memset(namebuff, 0 ,255);
		for(i = 0; i< 100; i++){
			namebuff[i] = buff[i];
		}
		if(namebuff[99]!= '\0'){
			i = 345;
			j = 100;
			while(buff[i] != '\0' && i< 500){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			
			namebuff[j] = '\0';
			
		}
	/*	printf("Name:%s\n",namebuff);
		printf("expect:%s\n",name);*/
		for(i = 0, j = 124; i<12; i++, j++){
			sizestring[i] = buff[j];
		}
		amount = octDec(atoi(sizestring));
		if(namebuff[0]!= 0&&strcmp(name,namebuff)== 0){
			printf("named\n");
			newfile(buff, namebuff, file_read, amount);
		}
		else{
			lseek(file_read, amount, SEEK_CUR);
		}
		 blockcount = amount/512;
		 rem = amount%512;
		if(rem!=0){
			blockcount++;
		}
		memset(buff, 0 ,512);
		lseek(file_read, blockcount*512-amount, SEEK_CUR);
		}


	}
	else{
	while(read(file_read, buff, 512)>0){
		/*printf("First header: %s\n", buff);*/
		memset(namebuff, 0 ,255);
		for(i = 0; i< 100; i++){
			namebuff[i] = buff[i];
		}
		if(namebuff[99]!= '\0'){
			i = 345;
			j = 100;
			while(buff[i] != '\0' && i< 500){
				namebuff[j] = buff[i];
				i++;
				j++;
			}
			
			namebuff[j] = '\0';
			
		}
		for(i = 0, j = 124; i<12; i++, j++){
			sizestring[i] = buff[j];
		}
		amount = octDec(atoi(sizestring));
		if(namebuff[0]!= 0){
			newfile(buff, namebuff, file_read, amount);
		}
		 blockcount = amount/512;
		 rem = amount%512;
		if(rem!=0){
			blockcount++;
		}
		memset(buff, 0 ,512);
		lseek(file_read, blockcount*512-amount, SEEK_CUR);
		}
	}
}

void newfile(uint8_t buff[512],char namebuff[256], int file_read, int amount){
	int i = 0;
	int newfile = 0;
	uint8_t bu[1];
	int curr = 0;
	char mod[8];
	int j = 0;
	char *ptr = NULL;
	mode_t mode = 0;
	i = 100;
	j = 0;
	while(buff[i] != 0){
		mod[j] = buff[i];
		i++;
		j++;
	}
	mod[j] = 0;		
	mode = (mode_t)strtol(mod, &ptr, 8);
	newfile = creat(namebuff, mode);
	while(curr< amount){
		read(file_read, bu, 1);
		write(newfile, bu, 1);
		curr++;
	}
/*	if(curr%512 != 0){
		lseek(file_read, curr%512, SEEK_CUR);
	}*/
}
void proper_parse(int argc, char **argv){
	int v = 0;
	int file_out = 0;
	int i =0;
	int file_read = 0;
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
	else if(argv[1][0] == 't'){
		if(argv[1][1] == 'v'){
			v = 1;
		}
		if(argc <= 2){
			perror("need tarfile");
			return;
		}
		if((file_read = open(argv[2], O_RDONLY)) >0){
			if(argc > 3){
				for(i = argc-1; i> 2; i--){
					lseek(file_read, 0, SEEK_SET);
					table(file_read, argv[i], v);
				}
			}
			else{
				table(file_read, NULL, v);
			}
			close(file_read);	
		}
		else{
			perror("bad tar");
			return;
		}
	}
	else if(argv[1][0] == 'x'){
		if(argc <= 2){
			perror("need tarfile");
			return;
		}
		if((file_read = open(argv[2], O_RDONLY)) >0){
			if(argc > 3){
				for(i = argc;i > 2; i--){
					lseek(file_read, 0, SEEK_SET);
					extract(file_read, argv[i]);
				}
			}
			else{
				extract(file_read, NULL);
			}
			close(file_read);	
		}
		else{
			perror("bad tar");
			return;
		}
	}

}	
int main(int argc, char *argv[]){
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
