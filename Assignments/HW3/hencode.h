#ifndef HENCODE_H
#define HENCODE_H
#include <stdint.h>
typedef struct Node{
	uint8_t c;
	int freq;
	struct Node *right;
	struct Node *left;
	struct Node *next;
} Node;

typedef struct Tree{
	Node *head;
} Tree;

typedef struct List{
	Node *head;
	int size;
} List;

int comp(Node, Node);

void insert(List *, Node *);
	
Node *pop(List *);

int lowerof(int,int);

void build_tree(List *, Tree *);

void create_code_helper(Node *, char [][256], char [256]);

void write_header(int *,int);

void create_code(Tree *, char[][256]);

char * create_header(int *);

void write_encode(int, int, char [][256],int);

void freetree_help(Node *);
#endif
