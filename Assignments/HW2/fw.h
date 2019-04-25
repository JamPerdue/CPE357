#ifndef FW_H
#define FW_H
#define MAXLINE 101

int hash(char *);

typedef struct Node{
	char *key;

	int val;
	struct Node *next;
} Node;

typedef struct Map{
	Node *table;
} Map;
void read(FILE *, Map *);

Node * form(Node[], int);

void insert(char *, Map *);

int get_num_items(Node[]);

int get_table_size(Node[]);

int compare( const void *, const void *);

double get_load_factor(Node[]);
#endif
