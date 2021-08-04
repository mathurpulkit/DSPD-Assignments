#include "books.h"
#include <string.h>
#include <stdlib.h>

int book_cmp(struct book a, struct book b){ // if book a > book b
    if(strcmp(a.name, b.name)>0){
        return 1;
    }
    else if (strcmp(a.name, b.name)<0){
        return 0;
    }
    else{
        if(strcmp(a.authors, b.authors)>0){
            return 1;
        }
        else if (strcmp(a.authors, b.authors)<0){
            return 0;
        }
        else{
            if(a.year_pub>b.year_pub){
                return 1;
            }
            else{
                return 0;
            }
        }
    }
}

// book tree functions

// Constructor for book_node class
struct book_node *init_book_node(int t1, int leaf1){
    struct book_node* ans = (struct book_node*)malloc(sizeof(struct book_node));
    ans->t = t1;
    ans->leaf = leaf1;
    ans->keys = (struct book*)malloc(sizeof(struct book)*(2*ans->t-1));
    ans->C =  (struct book_node*)malloc(sizeof(struct book_node*)*(2*ans->t));
    ans->n = 0;
    return ans;
}

void insertNonFull_book(struct book_node *bookm, struct book k){
    // Initialize index as index of rightmost element
    int i = bookm->n - 1;
    // If this is a leaf node
    if (bookm->leaf)
    {
        //following loop does two things
        //1. finds the location of new key to be inserted
        //2. moves all greater keys to one place ahead
        while (i >= 0 && book_cmp(bookm->keys[i], k))
        {
            bookm->keys[i+1] = bookm->keys[i];
            i--;
        }
        // Insert the new key at found location
        bookm->keys[i+1] = k;
        bookm->n = bookm->n + 1;
    }
    else{  // If this node is not leaf
        while (i >= 0 && book_cmp(bookm->keys[i], k))
            i--;
        // See if the found child is full
        if (bookm->C[i+1]->n == 2*bookm->t-1)
        {
            //if child full, then split it
            splitChild_book(bookm ,i+1, bookm->C[i+1]);
            //after split, the middle key of C[i] goes up and
            //C[i] is splitted into two.  See which of the two
            //is going to have the new key
            if (!book_cmp(bookm->keys[i+1], k)) {
                i++;
            }
        }
        insertNonFull_book(bookm->C[i+1], k);
    }
}

// A utility function to split the child of this node
void splitChild_book(struct book_node *bookm , int i, struct book_node *y) {
    struct book_node *z = init_book_node(y->t, y->leaf);
    z->n = bookm->t - 1;
    for (int j = 0; j < bookm->t - 1; j++)
        z->keys[j] = y->keys[j + bookm->t];
    if (y->leaf == 0) {
        for (int j = 0; j < bookm->t; j++)
            z->C[j] = y->C[j + bookm->t];
    }
    y->n = bookm->t - 1;
    //create space of new child
    for (int j = bookm->n; j >= i + 1; j--) {
    bookm->C[j + 1] = bookm->C[j];
    }
    bookm->C[i+1] = z;
    for (int j = bookm->n-1; j >= i; j--)
        bookm->keys[j+1] = bookm->keys[j];
    // Copy the middle key of y to this node
    bookm->keys[i] = y->keys[bookm->t-1];
    // Increment count of keys in this node
    bookm->n = bookm->n + 1;
}

// functions of book tree

void init_book_tree(struct book_tree* bookm, int t){
    bookm->root = NULL;
    bookm->t = t;
}

void insert_book(struct book_tree* bookm, struct book k){
    // If tree is empty
    if (bookm->root == NULL){
        // Allocate memory for root
        bookm->root = init_book_node(bookm->t, 1);
        bookm->root->keys[0] = k;  // Insert key
        bookm->root->n = 1;  // Update number of keys in root
    }
    else{ //if tree is not empty
        //if root full, then tree grows in height
        if (bookm->root->n == 2*bookm->t-1){
            //allocate memory for new root
            struct book_node *s = init_book_node(bookm->t, 1);
            //make old root as child of new root
            s->C[0] = bookm->root;
            //split the old root and move 1 key to the new root
            splitChild_book(s, 0, bookm->root);
            //new root has two children now.  Decide which of the
            //two children is going to have new key
            int i = 0;
            if (!book_cmp(s->keys[0], k))
                i++;
            insertNonFull_book(s->C[i], k);
            //change root
            bookm->root = s;
        }
        else  //if root is not full, call insertNonFull for root
            insertNonFull_book(bookm->root, k);
    }
}


