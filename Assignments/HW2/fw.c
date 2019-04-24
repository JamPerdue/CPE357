#include <stdio.h>
#include "fw.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>




int table_size;
int total_words;
int num_items;
int hash(char *word){
	int num = 0;
	int i = 0;
	if(strlen(word)<=8){
		for(i = 0;i<strlen(word);i++){
			num = abs(num + word[i]*pow(31.,((strlen(word)-1.)-i)));

		}
	}
	else{
		for(i = 0; i<8; i++){
			num = abs(num + word[i]*pow(31.,(7.-i)));
		}
	}
	return num%table_size;
}

void insert(char * key, Node **table){
	int i = 0;
	int ha = hash(key);
	int index = 0;
	int sec;
	int j= 0;
	int oldsize = table_size;
	Node n;
	Node *newtable = NULL;
	char *word = NULL;
	word = (char *)malloc(sizeof(char)*strlen(key));
	strcpy(word,key);
	n.key = word;
	n.val = 1;
	
	for(i=0;i<table_size;i++){
		index = (ha + i*i)%table_size;

		if((*table)[index].key==NULL){
	
			(*table)[index] = n;
		
			num_items++;
			break;
		}
		else if(strcmp((*table)[index].key,key)==0){

			(*table)[index].val++;
			break;
		}

	}	
	
	if(get_load_factor(*table) > .5){
		printf("rehashing\n");
		table_size = table_size*2+1;
		newtable = (Node *)malloc(sizeof(Node)*table_size);
		for(i=0; i<oldsize; i++){
			if((*table)[i].key!= NULL){
				index = hash((*table)[i].key);
				sec = 0;
				for(j = 0; j<table_size;j++){
					sec =(index+j*j)%table_size;
					if(newtable[sec].key == NULL){
						newtable[sec]=(*table)[i];
						break;
					}
				}
			}
		}
		for(i = 0; i<oldsize; i++){
			if((*table)[i].key!=NULL){
				free((*table)[i].key);
			}
		}
		free(*table);
		table = &newtable;
	}		
		
} 

int get_num_items(Node table[]){
	int cnt = num_items;
	/*for(i = 0; i<get_table_size(table);i++){
		if(table[i].key!= NULL){
			cnt++;
		}
	}*/
	return cnt;
}

int get_table_size(Node table[]){
	return table_size;
}

double get_load_factor(Node table[]){
	return (double)get_num_items(table)/table_size;
}

void read(FILE * file, Node *table[]){
	int c = 0;
	int count = 0;
	int len = 20;
	char *word= (char *)malloc(sizeof(char)*len);
	while((c=fgetc(file))!= EOF){
		
		if(isalpha(c)){
			count = 0;
			word[0]=c;
			count++;
			c = fgetc(file);
			while(isalpha(c)){
				if(count + 1 >= len){
					len = count+2;
					word =(char *)realloc(word,sizeof(char)*len);
				}
				word[count] = c;
				count++;
				c = fgetc(file);
			}
			word[count]= '\0';

			insert(word,table);
		}	
			



	}
	free(word);	
}

int compare(const void *fir,const void *seco){
	int l = ((Node *)fir)->val;
	int r = ((Node *)seco)->val;
	char * c = ((Node *)fir)->key;
	char * k = ((Node *)seco)->key;	
	if(r!=l){
		return r-l;
	}
	else{
		return strcmp(k,c);
	}
	

}

Node * form(Node table[], int num){
	int i = 0;
	Node *list;
	int j = 0;
	list = (Node *)malloc(sizeof(Node)*num_items);
	for(i = 0; i< table_size; i++){
		if(table[i].key!=NULL){
			list[j]=table[i];
			j++;
		}
	}
	/*for(i = 0; i < num_items; i++){
		printf("[%d]: %s,%d\n",i,list[i].key,list[i].val);
	}*/
	qsort(list,num_items,sizeof(Node),compare);
	/*printf("sorted");
	for(i = 0; i < num_items; i++){
		printf("[%d]: %s,%d\n",i,list[i].key,list[i].val);
	}*/
	return list;
}
	
int main(int argc, char * argv[]){
	FILE *file = NULL;
	int filecount;
	int i = 0;
	Node ** table_ptr = NULL;
	Node * table = NULL;
	int num_list = 10;
	Node *sorted;
	table_size = 191;
	total_words = 0;
	num_items = 0;
	table = (Node *)malloc(sizeof(Node)*table_size);
	table_ptr = &table;
	if(argc < 2){
		read(stdin,table_ptr);
		
	
	}
	if(argc>= 2 &&  argv[1][0] == 45 && argv[1][1] =='n'){
		if(argc == 3){
			fprintf(stderr,"usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
			return 0;
		}
		else if(atoi(argv[2])>-1){
			num_list = atoi(argv[2]);
		}
		else{
			fprintf(stderr,"usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
			return 0;
		}
	}
	
	if(num_list !=10){
		filecount = 3;
	}
	for(filecount = filecount; filecount < argc; filecount++){
		file = fopen(argv[filecount],"r");
		if(file != NULL){
			read(file,table_ptr);
			fclose(file);
		}
		else{
			printf("%s: No such file or directory\n",argv[filecount]);
		}
		
	}
	/*insert("hi",table);
	insert("hi",table);*/
	sorted = form(table,10);
	printf("The top %d words (out of %d) are:\n",num_list, num_items);
	for(i = 0; i < num_list;i++){
		printf("    %d %s\n", sorted[i].val, sorted[i].key);
	}

	for(i = 0; i <table_size; i++){
		if(table[i].key!= NULL){
			free(table[i].key);
			
		}
	}	
	free(table);
	return 1;
}	
