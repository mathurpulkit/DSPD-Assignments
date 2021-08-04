/*
Name: Pulkit Mathur
Enrollment No: BT19CSE087
Compilation command: gcc main.c utility.c member.c books.c
*/

#include <stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include "utility.h" // contains self-defined date/time diff functions and data types
#include "member.h"
#include "books.h"
#define MAX_FAC_DAYS 30
#define MAX_STUD_DAYS 14
#define B_TREE_DEG 5

int book_count = 0;

void new_member(struct mem_tree* stud_list, struct mem_tree* fac_list){
    int i;
    struct member new_mem;
    printf("Enter details:\n");
    printf("Enter 0 for faculty, any other number for student: ");
    scanf("%d", &i);
    printf("Enter name: ");
    fgets(new_mem.name, NAME_SIZE, stdin); // reads full name
    // we used fgets as scanf stops reading at space
    printf("Enter Department's 3 letter name: ");
    fgets(new_mem.dept, DEPT_SIZE, stdin);
    printf("Enter member ID: ");
    scanf("%d", new_mem.member_id);
    new_mem.no_books = 0; //initialisation necessary for this element
    if(i==0){ // Member is faculty
        new_mem.designation = Faculty;
        insert_mem(fac_list, new_mem);
        printf ("New Faculty member added in the database!!\n");
    }
    else{ // Member is student
        new_mem.designation = Student;
        insert_mem(stud_list, new_mem);
        printf ("New Student member added in the database!!\n");
    }
}

void max_issue(struct mem_tree* stud_list, struct  mem_tree* fac_list){
    int mem_id_fac, mem_id_stud;
    mem_id_stud = traverse_book_is(stud_list->root, 1);
    mem_id_fac = traverse_book_is(fac_list->root, 1);
    struct member *fac = search_mem(fac_list->root, mem_id_fac);
    struct member *stud = search_mem(stud_list->root, mem_id_stud);
    printf("The student who borrowed maximum books is ");
    fputs(stud->name, stdout); //prints string with spaces
    printf(" and they issued %d books.\n", stud->no_books);
    printf("The faculty who borrowed maximum books is ");
    fputs(fac->name, stdout); //prints string with spaces
    printf("and they issued %d books.\n", fac->no_books);
}

void ins_book(struct book_tree* book_list){
    struct book new_book;
    printf("Enter book details:");
    printf("Enter book name: ");
    fgets(new_book.name, NAME_SIZE, stdin);
    printf("Enter subject name: ");
    fgets(new_book.subject, NAME_SIZE, stdin);
    printf("Enter author(s) names: ");
    fgets(new_book.authors, NAME_SIZE, stdin);
    new_book.acc_no = book_count+1;
    printf("Accession number assigned is: %d\n", book_count+1);
    printf("Enter Year Of Publication: ");
    scanf("%d", &new_book.year_pub);
    new_book.no_issued = 0;
    printf("Enter number of copies acquired: ");
    scanf("%d", &new_book.no_unissued);
    insert_book(book_list, new_book);
    book_count++;
    printf("Book Inserted in the database!!\n\n");
}

float calc_fine(struct mem_tree* stud_list, struct mem_tree* fac_list,
                int mem_id, int print_flag){

    if(mem_id==0){
        printf("Enter Member ID: ");
        scanf("%d", &mem_id);
    }
    struct member* tmp = search_mem(stud_list, mem_id);
    if (tmp==NULL){
        tmp = search_mem(stud_list, mem_id);
    }
    if(tmp==NULL){
        if (print_flag){
            printf("\nMember ID %d not found!!\n\n", mem_id);
        }
        return 0.0;
    }
    float fine;
    fine = calcu_fine(tmp);
    return fine;
}

void max_fine(struct mem_tree* stud_list, struct mem_tree* fac_list){
    int mem_id_stud, mem_id_fac;
    float fine_stud = 0.0, fine_fac = 0.0;
    mem_id_stud = traverse_fine(stud_list, 1);
    mem_id_fac = traverse_fine(fac_list, 1);
    if(mem_id_stud){
        fine_stud = calc_fine(stud_list, fac_list, mem_id_stud, 0);
    }
    if(mem_id_fac){
        fine_fac = calc_fine(stud_list, fac_list, mem_id_stud, 0);
    }
    if(fine_fac>=fine_stud && fine_fac > 0.01 ){
        printf("Max fine was received by Member No %d of %f rupees\n\n", mem_id_fac, fine_fac);
    }
    else if(fine_fac < fine_stud && fine_stud > 0.01 ){
        printf("Max fine was received by Member No %d of %f rupees\n\n", mem_id_stud, fine_stud);
    }
    else{ // no one got fines
        printf("No one has any fines!!\n\n");
    }
}

void print_menu(){
    printf("=========================================\n\n");
    printf("    Welcome to VNIT Library Program\n\n");
    printf("Select the operation you want to perform:\n");
    printf("1. Exit program\n");
    printf("2. Add New Student/Faculty Member\n");
    printf("3. See who has issued maximum books\n");
    printf("4. Insert a new book in  the database\n");
    printf("5. Calculate fine of a Library Member\n");
    printf("6. Get info of member with Maximum fine for late return\n");
    printf("Enter your choice: ");
}

int main() {
    int flag = 1;
    struct mem_tree fac_list, stud_list;
    init_mem_tree(&fac_list, B_TREE_DEG);
    init_mem_tree(&stud_list, B_TREE_DEG);
    struct book_node book_lib;
    init_book_tree(&book_lib, B_TREE_DEG);
    while (flag!=0){
        print_menu();
        scanf("%d", &flag);
        printf("\n");
        switch(flag){
            case 1:
                break;
            case 2:
                new_member(&stud_list, &fac_list);
                break;
            case 3:
                max_issue(&stud_list, &fac_list);
                break;
            case 4:
                ins_book(&book_lib);
                break;
            case 5:
                calc_fine(&stud_list, &fac_list, 0, 1);
                break;
            case 6:
                max_fine(&stud_list, &fac_list);
                break;
            default:
                printf("\nInvalid Choice!\n\n");
        }
    }
    printf("\nExiting program!!\n\n");

    return 0;
}
