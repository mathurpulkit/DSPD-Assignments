#include "member.h"
#include "utility.h"
#include "stdlib.h"
#include <stdlib.h>
#include <stdio.h>

int mem_cmp(struct member a, struct member b){ // comparing members by id
    return (a.member_id > b.member_id);
}

// mem tree functions

struct mem_node *init_mem_node(int t1, int leaf1)
{
    struct mem_node* ans = (struct mem_node*)malloc(sizeof(struct mem_node));
    ans->t = t1;
    ans->leaf = leaf1;
    //allocate memory  keys and child pointers
    ans->keys = (struct member*)malloc(sizeof(struct member)*(2*ans->t-1));
    ans->C =  (struct mem_node**)malloc(sizeof(struct mem_node*)*(2*ans->t));
    ans->n = 0;
    return ans;
}

int traverse_book_is(struct mem_node* mem, int reset){
    static int book_is;
    int ans;
    if(reset){
        book_is = 0;
        ans = 0;
    }
    //traverse through n key and first n children
    int i;
    for (i = 0; i < mem->n; i++){
        //if this is not leaf then before reading key[i],
        //traverse the subtree rooted with child C[i].
        if (mem->leaf == 0)
            traverse_book_is(mem->C[i], 0);
        if(mem->keys[i].no_books > book_is){
            book_is = mem->keys[i].no_books;
            ans = mem->keys[i].member_id;
        }
    }
    //read subtree rooted with last child, if it's there
    if (mem->leaf == 0) {
        traverse_book_is(mem->C[i], 0);
    }
    return ans;
}

// Function to traverse all nodes in a subtree to calc fines
float traverse_fine(struct mem_node* mem, int reset){
    // code similar to the last function
    static float fine;
    int ans;
    if(reset){
        fine = 0.01;
        ans = 0;
    }
    int i;
    for (i = 0; i < mem->n; i++){
        if (mem->leaf == 0)
            traverse_fine(mem->C[i], 0);
        if(calcu_fine(&(mem->keys[i])) > fine){
            fine = calcu_fine(&mem->keys[i]);
            ans = mem->keys[i].member_id;
        }
    }
    if (mem->leaf == 0){
        traverse_fine(mem->C[i], 0);
    }
    return ans;
}

// Function to search key k in subtree rooted with this node
struct member* search_mem(struct mem_node *mem, int id) {
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < mem->n && id > mem->keys[i].member_id) {
    i++;
    }
    // If the found key is equal to k, return this node
    if (mem->keys[i].member_id == id){
        return &(mem->keys[i]);
    }
    // If key is not found here and this is a leaf node
    if (mem->leaf) {
        return NULL;
    }
    // Go to the appropriate child
    return search_mem(mem->C[i], id);
}

void insertNonFull_mem(struct mem_node *mem, struct member k){
    // Initialize index as index of rightmost element
    int i = mem->n - 1;
    // If this is a leaf node
    if (mem->leaf)
    {
        //loop does two things
        //1. finds location of new key to insert
        //2. moves all greater keys to one place ahead
        while (i >= 0 && mem_cmp(mem->keys[i], k))
        {
            mem->keys[i+1] = mem->keys[i];
            i--;
        }
        // Insert the new key at found location
        mem->keys[i+1] = k;
        mem->n = mem->n + 1;
    }
    else { // If this node is not leaf
        // Find the child which is going to have the new key
        while (i >= 0 && mem_cmp(mem->keys[i], k)) {
            i--;
        }
        if (mem->C[i+1]->n == 2*mem->t-1){
            splitChild_mem(mem ,i+1, mem->C[i+1]);
            if (!mem_cmp(mem->keys[i+1], k))
                i++;
        }
        insertNonFull_mem(mem->C[i+1], k);
    }
}

//utility function to split the child y of this node
void splitChild_mem(struct mem_node *mem , int i, struct mem_node *y){
    // Create a new node which is going to store (t-1) key of y
    struct mem_node *z = init_mem_node(y->t, y->leaf);
    z->n = mem->t - 1;
    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < mem->t-1; j++) {
        z->keys[j] = y->keys[j + mem->t];
    }
    // Copy the last t children of y to z
    if (y->leaf == 0){
        for (int j = 0; j < mem->t; j++)
            z->C[j] = y->C[j+mem->t];
    }
    // Reduce the number of keys in y
    y->n = mem->t - 1;
    // Since this node is going to have a new child,
    // create space of new child
    for (int j = mem->n; j >= i+1; j--) {
        mem->C[j + 1] = mem->C[j];
    }
    // Link the new child to this node
    mem->C[i+1] = z;
    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = mem->n-1; j >= i; j--) {
        mem->keys[j + 1] = mem->keys[j];
    }
    mem->keys[i] = y->keys[mem->t-1];
    mem->n = mem->n + 1;
}

float calcu_fine(struct member* mem){
    struct date today = get_cur_date(); //gets today's date
    float fine = 0.0;
    for(int j=0;j<mem->no_books;j++){
        if(date_diff(mem->ret_date[j],today)>0){
            fine += 0.5*date_diff(mem->ret_date[j],today);
        }
    }
    return fine;
}

// functions of member tree

void init_mem_tree(struct mem_tree* mem, int t){ //initialises member tree
    mem->root = NULL;
    mem->t = t;
}

void insert_mem(struct mem_tree* mem, struct member k){
    // If tree is empty
    if (mem->root == NULL)
    {
        //allocate memory for root
        mem->root = init_mem_node(mem->t, 1);
        mem->root->keys[0] = k;  // Insert key
        mem->root->n = 1;  // Update number of keys in root
    }
    else //if tree not empty
    {
        //if root is full, then tree grows in height
        if (mem->root->n == 2*mem->t-1)
        {
            struct mem_node *s = init_mem_node(mem->t, 1);
            //allocate memory for new root, make old root as child of new root
            s->C[0] = mem->root;
            splitChild_mem(s, 0, mem->root);
            //split the old root and move 1 key to the new root
            //new root has two children now, decide which of the
            //two children is going to have new key
            int i = 0;
            if (!mem_cmp(s->keys[0], k)) {
                i++;
            }
            insertNonFull_mem(s->C[i], k);
            // Change root
            mem->root = s;
        }
        else {  // If root is not full, call insertNonFull for root
            insertNonFull_mem(mem->root, k);
        }
    }
}


