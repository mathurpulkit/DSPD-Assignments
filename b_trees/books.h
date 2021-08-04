#ifndef BOOKS_H
#define BOOKS_H
#include "utility.h"
#include <stdlib.h>
#define NAME_SIZE 50

typedef struct book{
    char name[NAME_SIZE];
    char subject[NAME_SIZE];
    char authors[NAME_SIZE];
    int acc_no;
    int year_pub;
    int no_issued; //number of copies of books issued
    int no_unissued; //number of copies of books available
}; // book element

typedef struct book_node{
    struct book *keys;  //array of keys
    int t;      //min degree
    struct book_node **C; // array of child pointers
    int n;     // current no of keys
    int leaf; // is key leaf(boolean value)
};

typedef struct book_tree{ // B-Tree
    struct book_node *root; // ptr to root node
    int t;  // min degree
};


int book_cmp(struct book a, struct book b);
// book node functions
struct book_node* init_book_node(int, int);   // Constructor

void insertNonFull_book(struct book_node *bookm , struct book k);
// A utility function to split the child y of this node. i is index of y in
// child array C[].  The Child y must be full when this function is called
void splitChild_book(struct book_node* bookm, int i, struct book_node *y);
// A function to traverse all nodes in a subtree rooted with this node
// void traverse(struct book_node* bookm);

// A function to search_book a key in the subtree rooted with this node.
// struct book_node *search_book(struct book_node *bookm, int k);
// book node functions end
// book tree functions
void init_book_tree(struct book_tree*, int);
void insert_book(struct book_tree*, struct book);


#endif
