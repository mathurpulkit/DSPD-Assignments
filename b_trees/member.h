#ifndef MEMBER_H
#define MEMBER_H
#include "utility.h"
#include <stdlib.h>
#define MAX_B_ISSUE_F 4
#define MAX_B_ISSUE_S 2
#define NAME_SIZE 50
#define DEPT_SIZE 4

typedef struct member{
    char name[NAME_SIZE];
    char dept[DEPT_SIZE]; //three letter short form for departments
    enum designation_type designation;
    int member_id;
    int no_books; //no of books issued
    int iss_book_id[MAX_B_ISSUE_F]; //Accession number of books issued
    struct date ret_date[MAX_B_ISSUE_F]; //return date of books issued
};

typedef struct mem_node{
    struct member *keys;  //array of keys
    int t;      //min degree
    struct mem_node **C; // array of child pointers
    int n;     // current no of keys
    int leaf; // is key leaf(boolean value)
};

typedef struct mem_tree{ // B-Tree
    struct mem_node *root; // ptr to root node
    int t;  // min degree
};

int mem_cmp(struct member a, struct member b);
// member node functions
struct mem_node* init_mem_node(int, int);   // Constructor

void insertNonFull_mem(struct mem_node *mem , struct member k);
// A utility function to split the child y of this node. i is index of y in
// child array C[].  The Child y must be full when this function is called
void splitChild_mem(struct mem_node* mem, int i, struct mem_node *y);
// A function to traverse all nodes in a subtree rooted with this node
float traverse_fine(struct mem_node*, int);
int traverse_book_is(struct mem_node* mem, int reset);
// A function to search_mem a key in the subtree rooted with this node.
struct member* search_mem(struct mem_node *mem, int id);
float calcu_fine(struct member* mem);
// member node functions end
// member tree functions
void init_mem_tree(struct mem_tree*, int);
void insert_mem(struct mem_tree*, struct member);


#endif
