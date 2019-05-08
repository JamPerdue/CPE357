#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include "hencode.h"
#include <string.h>
#include <unistd.h>
#include <stdint.h>

int comp(Node first, Node second){
	if(first.freq < second.freq){
		return 0;
	}
	else if( first.freq > second.freq){
		return 1;
	}
	else if(first.c == -1){
		return 0;
	}
	else{
		if(first.c > second.c){
			return 1;
		}
		else{
			return 0;
		}
	}	
}
	
void insert(List *list , Node * node){
	Node * n = node;
	Node * prev = NULL;
	Node * nex = NULL;
	Node * head = NULL;
	Node * temp = NULL;
	int inserted = 0;
	head = list->head;

	if(head == NULL){
		list->head = n;
	}

	else if(comp(*n,*head)==0){
		n->next = list->head;
		list->head = n;
	}

	else{
		prev = list->head;
		temp = list->head;
		nex = temp->next;
		while(nex!= NULL){
			if(comp(*n,*temp)==0){
				prev->next = n;
				n->next = temp;
				inserted++;
				break;
			}
			else if(comp(*n,*nex)==0){
				temp->next = n;
				n->next = nex;
				inserted++;
				break;
			}
			prev = temp;
			temp = nex;
			if(temp!=NULL){

			nex = temp->next;
			}
		}
		if(inserted==0){
		
			
			
			temp->next = n;
			
		}
	}
	list->size= list->size+1;	
	/*printf("Inserting: %d\n",n->c);
	printf("Size %d\n", list->size);*/
}

Node *pop(List * list){
	Node * head = list->head;
	Node *temp = NULL;
	temp = head;
	if(head != NULL){
		list->head = head->next;
		list->size= list->size-1;
	}
	/*printf("%d\n",list->size);
	printf("success\n");*/
	return temp;
}

void build_tree(List * list, Tree * tree){
	Node * first;
	Node * second;
	Node * new;	
	while(list->size > 1){
		first = pop(list);
		second = pop(list);
		new = (Node *)malloc(sizeof(Node));
		new->freq = first->freq + second->freq;
		new->c = -1;
		printf("freq: %d\n",new->freq);
		new->right = second;
		new->left = first;	
		insert(list,new);
	}	
	tree->head = list->head;
}

int lowerof( int a,int b){
	if(a>b){
		return b;
	}
	else{
		return a;
	}
}

void create_code(Tree * tree, char code_list[][256]){
	
	
	create_code_helper(tree->head, code_list, "");
	
}

void create_code_helper(Node * node, char codes[][256], char code[256]){
	char temp[256];
	strcpy(temp,code);	
	/*printf("Codetemp: %s\n",temp);	*/
	if(node == NULL){
		return; 	
	}
	else if(node->left == NULL && node->right == NULL){
		if(node->c != 0){
			strcpy(codes[(int)node->c],code);

			/*codes[(int)node->c] = code;*/
		}
	}
	else{
		if(node->left!= NULL){
			
			temp[strlen(code)]= '0';
			temp[strlen(code)+1]= '\0';
			create_code_helper(node->left, codes, temp);
		}
		if(node->right != NULL){
		
			temp[strlen(code)]= '1';
			temp[strlen(code)+1]= '\0';
			create_code_helper(node->right, codes, temp);
		}
	}

}

void write_header(int *table, int file){
	int count = 0;
	int i = 0;
	uint32_t buffer[1];	
	uint32_t size1 = 0;
	uint8_t buff2[1];
	buffer[0] = count;
	for(i = 0; i < 256; i++){
		if(table[i]!= 0 ){
			count++;
		}
	}
	
	size1= (uint32_t)count;
	buffer[0]= size1;
	write(file, buffer, 4);
	
	for(i = 0; i < 256; i++){
		if(table[i]!= 0 ){
			buff2[0] = (uint8_t)i;
			write(file, buff2, 1);
			buffer[0] = (uint32_t)table[i];
			write(file, buffer, 4);
	
		}	
	}
 /*(strcmp(table[i],"")!= 0 )*/ } 

void write_encode(int file_in, int file_out, char codes[][256],int total_c){
	int buf_r[1] = {0};
	int i = 0;
	uint8_t *buf_w = (uint8_t *)calloc(sizeof(uint8_t),total_c);
	int code_size = 0;
	int tot_bytes = 0;
	int bit_counter = 1;
	int current = 0;
	int mask = 128;	
	uint8_t byte = 0;
	while(read(file_in, buf_r, sizeof(char)) > 0){
		/*printf("%d\n",buf_r[0]);*/	
		for(i = 0; i < strlen(codes[(int)buf_r[0]]); i++){
			if(codes[buf_r[0]][i]=='1'){
				byte = byte | mask;

				mask = mask >> 1;
				printf("1");
				code_size++;	
				bit_counter++;
				if(bit_counter>=8){
				/*	printf("byte %x\n",buf_w[current]);*/
					buf_w[current] = byte;
					current++;
					mask = 128;
					bit_counter = 1;
				}
			}
			else if(codes[buf_r[0]][i] == '0'){

				mask = mask >> 1;
				printf("0");

				code_size++;
				bit_counter++;
				if(bit_counter >= 8){
					buf_w[current] = byte;
					current++;
					mask = 128;
					bit_counter = 1;
				}
			}
		}
	}
	printf("\n");
	/*printf("totbytes : %d\n",code_size);*/
	tot_bytes = code_size/ 8;
	printf("Tot: %d\n",tot_bytes);
	if(bit_counter > 0 && bit_counter != 8){
		tot_bytes++;
		}
	
	write(file_out,buf_w,tot_bytes);
		free(buf_w);
}

int main(int argc, char * argv[]){
	Tree tree;
	List list;
	int * table = (int *)calloc(256,sizeof(int));
	
	int file_in = 0;
	int file_out = 0;
	
	int i = 0;
	Node* temp;
	int buf[1];
	char code_table[256][256];
	int total_chars = 0;
	list.head = NULL;
	list.size = 0;
	tree.head = NULL;
	if(argc > 1){
		file_in = open(argv[1],O_RDONLY);
		
	}

	else{
		fprintf(stderr,"Usuage: hencode infile [outfile]");
		return 1;
	}

	while(read(file_in, buf, sizeof(char)) > 0){	
		table[buf[0]]++;	
		total_chars++;
	}
	
	for(i = 0; i < 256; i++){
		if(table[i]>0){
			printf("Char: %d, Freq: %d\n", i, table[i]);
		}
	}
	/* build freq tabnle*/
	for(i = 0; i< 256; i++){
		if(table[i]>0){
			temp = (Node *)malloc(sizeof(Node));
			temp->c = i;
			temp->freq = table[i];
			printf("testfreq: %d\n",temp->freq);
			insert(&list, temp);
		}
	}	
	temp = list.head;
	while(temp!= NULL){
		printf("Char %c, Freq %d\n", temp->c, temp->freq);
		temp = temp->next;
	}
	build_tree(&list, &tree);
	for(i = 0; i< 256; i++){
		code_table[i][0] = '\0';
	}
	create_code(&tree,code_table);

	if(argc > 2){
		file_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644 );
	}
	else{
		file_out = STDOUT_FILENO;
	}

	for(i = 0; i < 256; i++){
		if(table[i]>0){
			printf("Code %d: %s\n",i, code_table[i]);
		}
	}	
	write_header(table, file_out);
	
	lseek(file_in,0, SEEK_SET);

	write_encode(file_in,file_out, code_table, total_chars);	
	
	close(file_in);

	printf("break\n");


	return 0;
}
