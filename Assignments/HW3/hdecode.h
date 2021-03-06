#ifndef HDECODE_H
#define HDECODE_H

typedef struct Node{
	char c;
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

typedef struct Freq{
	int table[256];
} Freq;

int comp(Node, Node);

void insert(List *, Node *);
	
Node *pop(List *);

int lowerof(int,int);

void build_tree(List *, Tree *);

void create_code_helper(Node *, char [][256], char [256]);


void create_code(Tree *, char[][256]);

void read_header( Freq *, int);

void read_body(Tree *, int, int, int);

void freetree_help(Node *);

#endif
