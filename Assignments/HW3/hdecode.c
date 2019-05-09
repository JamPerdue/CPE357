#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include "hdecode.h"
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
	Node * prev;
	Node * nex = NULL;
	Node * head = NULL;
	Node * temp = NULL;
	int inserted = 0;
	head = list->head;

	if(head == NULL){
		list->head = n;
	}

	else if(comp(*n,*head)==0){
		n->next = head;
		list->head = n;
	}

	else{
		prev = head;
		temp = head;
		nex = temp->next;
		while(nex!= NULL){
			if(comp(*n,*temp)==0){
				prev->next = n;
				n->next = temp;
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
			if(temp != NULL){
				temp->next = n;	
			}	
			else{
			prev->next = n;
			}
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
		new->c = lowerof(first->c,second->c);
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

void read_header(Freq *freqs, int file){
	uint32_t buff1[1];
	uint8_t buff2[1];
	int num_chars = 0;
	int i = 0;
	read(file, buff1, 4);
	num_chars = buff1[0];
	for(i = 1; i < num_chars+1; i++){
		read(file, buff2, 1);
		read(file, buff1, 4);
		/*printf("Char: %c freq %d\n",buff2[0],buff1[0]);*/
		freqs->table[buff2[0]] = buff1[0];
	}



}
int main(int argc, char *argv[]){
	int file_in = 0;
	Freq freq_table;
	Tree tree;
	List list;
	Node * temp;
	int i = 0;
	list.head = NULL;
	tree.head = NULL;

	for(i = 0; i < 256; i++){
		freq_table.table[i] = 0;
	}
	if(argc > 1){
                 file_in = open(argv[1],O_RDONLY);
	}

        else{
                fprintf(stderr,"Usuage: hdecode infile [outfile]");
                return 1;
        }
	read_header(&freq_table, file_in);
	
	for(i = 0; i < 256; i++){
		if(freq_table.table[i] != 0){
			temp = (Node *)malloc(sizeof(Node));
			temp->c = i;
			temp->freq = freq_table.table[i];
			temp->next = NULL;
	/*		printf("Inserting %c freq %d\n",temp->c, temp->freq);*/
			insert(&list, temp);
		}
	}
	build_tree(&list,&tree);
	

}
