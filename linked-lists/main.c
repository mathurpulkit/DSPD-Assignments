/*
Name: Pulkit Mathur
Enrollment No: BT19CSE087
Compilation command: gcc main.c utility.c
*/

#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include"utility.h" // self defined library for useful functions
#define MAX_B_ISSUE_F 4
#define MAX_B_ISSUE_S 2
#define NAME_SIZE 50
#define DEPT_SIZE 4
#define MAX_FAC_DAYS 30
#define MAX_STUD_DAYS 14
typedef enum designation_type {Student, Faculty};

typedef struct book{
	char name[NAME_SIZE];
	char subject[NAME_SIZE]; 
	char authors[NAME_SIZE];
	int acc_no;
	int year_pub;
	int hall_no;
	int rack_no;
	int row_no;
	int no_issued; //number of copies of books issued
	int no_unissued; //number of copies of books available
	struct book* next;
}; // linked list book element

int book_count = 0;

typedef struct member{
	char name[NAME_SIZE];
	char dept[DEPT_SIZE]; //three letter short form for departments
	enum designation_type designation;
	int member_id;
	int no_books; //no of books issued
	int iss_book_id[MAX_B_ISSUE_F]; //Accession number of books issued
	struct date ret_date[MAX_B_ISSUE_F]; //return date of books issued
	struct member* next;
};

void new_member(struct member** stud_list, struct member** fac_list){
	int i;
	struct member* new_mem = (struct member*)malloc(sizeof(struct member));
	struct member* temp = NULL;
	printf("Enter details:\n");
	printf("Enter 0 for faculty, any other number for student: ");
	scanf("%d", &i);
	printf("Enter name: ");
	fgets(&new_mem->name, NAME_SIZE, stdin); // reads full name
	// we used fgets as scanf stops reading at space
	printf("Enter Department's 3 letter name: ");
	fgets(&new_mem->dept, DEPT_SIZE, stdin);
	printf("Enter member ID: ");
	scanf("%d", &new_mem->member_id);
	new_mem->no_books = 0; //initialisation necessary for this element
	new_mem->next = NULL; // inserted at last of linked list
	if(i==0){ // Member is faculty
		new_mem->designation = Faculty;
		temp = *fac_list;
		if (temp==NULL){ // empty list, so initialise it
			*fac_list = new_mem;
		}
		else{
			while(temp->next!=NULL){
				temp = temp->next;
			}
			temp->next = new_mem;
		}
		printf ("New Faculty member added in the database!!\n");
	}
	else{ // Member is student
		new_mem->designation = Student;
		temp = *stud_list;
		if (temp==NULL){ // empty list, so initialise it
			*stud_list = new_mem;
		}
		else{
			while(temp->next!=NULL){
				temp = temp->next;
			}
			temp->next = new_mem;
		}
		printf ("New Student member added in the database!!\n");
	}
	
}

void max_issue(struct member* stud_list, struct member* fac_list){
	int max_val = 0;
	struct member* max_ptr = NULL;
	int i;
	while (stud_list!=NULL){
		if (stud_list->no_books >= max_val){
			max_ptr = stud_list;
			max_val = stud_list->no_books;
		}
		stud_list = stud_list->next;
	}
	if(max_ptr!=NULL){
		printf("The student who borrowed maximum books is ");
		fputs(max_ptr->name, stdout); //prints string with spaces
		printf(" and they issued %d books.\n", max_val);
	}
	//printing for faculty done
	max_val = 0;
	max_ptr = NULL;
	while (fac_list!=NULL){
		if (fac_list->no_books > max_val){
			max_ptr = fac_list;
			max_val = fac_list->no_books;
		}
		fac_list = fac_list->next;
	}
	if(max_ptr!=NULL){
		printf("The faculty who borrowed maximum books is ");
		fputs(max_ptr->name, stdout); //prints string with spaces
		printf("and they issued %d books.\n", max_val);
	}
}

float calc_fine(struct member* stud_list, struct member* fac_list,int mem_id, int print_flag){
	// print flag is used whether to print details or not
	// we usually want to print them, but when we call it from max_fine, we don't
	if (mem_id == 0){ // if mem_id is passed zero, it asks the user for id
		printf("Enter Member ID whose fine you want to calculate: ");
		scanf("%d", &mem_id);
	}
	int i, j, flag = 0;
	float fine = 0.0;
	struct date today = get_cur_date(); //gets today's date
	while(stud_list!=NULL&&flag==0){ // traverses the list to find the member
		if(mem_id==stud_list->member_id){
			for(j=0;j<stud_list->no_books;j++){
				if(date_diff(stud_list->ret_date[i],today)>0){
					fine += 0.5*date_diff(stud_list->ret_date[i],today);
				}
			}
			flag = 1;
		}
		stud_list = stud_list->next;
	}
	while(fac_list!=NULL&&flag==0){ // traverses the list to find the member
		if(mem_id==fac_list->member_id){
			for(j=0;j<fac_list->no_books;j++){
				if(date_diff(fac_list->ret_date[i],today)>0){
					fine += 0.5*date_diff(fac_list->ret_date[i],today);
				}
			}
			flag = 1;
		}
		fac_list = fac_list->next;
	}
	if (flag==1&&print_flag==1){
		printf("Member ID %d has fine of %f rupees.\n", mem_id,fine);
	}
	else if(print_flag==1){
		printf("Member ID %d not found!!\n\n", mem_id);
	}
	return fine;
}

void max_fine(struct member* stud_list, struct member* fac_list){
	float max_fine = 0.0;
	int i = 0, mem_no = 0;
	float cur_fine = 0.01; //In case no one gets fined at all, to prevent errors in float comparison
	// and print appropriate messages, we initialise it to 0.01(min fine is 0.50)
	while(stud_list!=NULL){
		cur_fine = calc_fine(stud_list, fac_list, stud_list->member_id, 0);
		// this runs fast bcoz student id is at first pointer only, so O(1) time
		if(cur_fine>max_fine){
			max_fine = cur_fine;
			mem_no = stud_list->member_id;
		}
		stud_list = stud_list->next;
	}
	while(fac_list!=NULL){
		cur_fine = calc_fine(stud_list,fac_list,fac_list->member_id, 0);
		// this runs fast bcoz studlist is empty and faculty id is at first pointer only
		if(cur_fine>max_fine){
			max_fine = cur_fine;
			mem_no = fac_list->member_id;
		}
		fac_list = fac_list->next;
	}
	if (mem_no!=0){ //if checks if anyone has been fined at all
		printf("Max fine was received by Member No %d of %f rupees\n\n", mem_no, max_fine);
	}
	else{
		printf("No one has any fines!!\n\n");
	}
}

void insert_book(struct book** book_list){
	// book details are first stored in a temporary structure
	// then inserted in the correct place in the sorted database
	struct book* new_book = (struct book*)malloc(sizeof(struct book));
	printf("Enter book details:");
	printf("Enter book name: ");
	fgets(new_book->name, NAME_SIZE, stdin);
	printf("Enter subject name: ");
	fgets(new_book->subject, NAME_SIZE, stdin);
	printf("Enter author(s) names: ");
	fgets(new_book->authors, NAME_SIZE, stdin);
	new_book->acc_no = book_count+1;
	printf("Accession number assigned is: %d\n", book_count+1);
	printf("Enter Year Of Publication: ");
	scanf("%d", &new_book->year_pub);
	printf("Enter Hall No, Rack No, Row No: ");
	scanf("%d %d %d", &new_book->hall_no, &new_book->rack_no, &new_book->row_no);
	new_book->no_issued = 0;
	printf("Enter number of copies acquired: ");
	scanf("%d", &new_book->no_unissued);
	int flag = 0; // whether the book is inserted
	struct book* left, *right; // left and right of the book to be inserted
	left = NULL;
	right = *book_list;
	if(right==NULL){ // empty list initialised
		*book_list = new_book;
		flag = 1;
	}
	else{
		while(right!=NULL&&flag==0){
			if(strcmp(new_book->subject, right->subject)<0){
				if(left==NULL){ // new book at top of list
					*book_list = new_book;
					new_book->next = right;
				}
				else{
					left->next = new_book;
					new_book->next = right;
				}
				flag = 1;
			}
			else if(strcmp(new_book->subject, right->subject)==0){
				if(strcmp(new_book->name, right->name)<0){
					if(left==NULL){ // new book at top of list
						*book_list = new_book;
						new_book->next = right;
					}
					else{
						left->next = new_book;
						new_book->next = right;
					}
					flag = 1;
				}
				else if(strcmp(new_book->name, right->name)==0){
					if(strcmp(new_book->authors, right->authors)<0){
						if(left==NULL){ // new book at top of list
							*book_list = new_book;
							new_book->next = right;
						}
						else{
							left->next = new_book;
							new_book->next = right;
						}
						flag = 1;
					}
					else if(strcmp(new_book->authors, right->authors)==0){
						if(new_book->year_pub<right->year_pub){
							if(left==NULL){ // new book at top of list
								*book_list = new_book;
								new_book->next = right;
							}
							else{
								left->next = new_book;
								new_book->next = right;
							}
							flag = 1;
						}
					}
				}
			}	
			left = right;
			right = right->next;
		}
	}
	if(flag==0){ // if book is to be inserted at last, in non-empty list
		left->next = new_book;
	}
	book_count++;
	printf("Book Inserted in the database!!\n\n");
}

// split linked list into two halves for sorting
void split(struct member* source, struct member** front, struct member** middle) 
{ 
    struct member* fast; 
    struct member* slow; 
    slow = source; 
    fast = source->next; 
    // Moves fast 2 places ahead, slow 1 place ahead
    while (fast != NULL) { 
        fast = fast->next; 
        if (fast != NULL) { 
            slow = slow->next; 
            fast = fast->next; 
        } 
    } 
    // Now front points to first element, middle to middle element
    *front = source; 
    *middle = slow->next; 
    slow->next = NULL; 
} 

struct member* borrow_merge(struct member* a, struct member* b){ 
    struct member* retptr = NULL; // returned head
    // Base cases
    if (a == NULL){
        return (b);
    }
    else if (b == NULL){
        return (a);
    }
    if (a->no_books > b->no_books) { 
        retptr = a; 
        retptr->next = borrow_merge(a->next, b); 
    }
    else if(a->no_books == b->no_books){
        if(strcmp(a->name,b->name)<0){
            retptr = a; 
            retptr->next = borrow_merge(a->next, b);
        }
        else{
            retptr = b; 
            retptr->next = borrow_merge(a, b->next);
        }
    }
    else { 
        retptr = b; 
        retptr->next = borrow_merge(a, b->next); 
    } 
    return retptr; 
} 

// sorts the linked lists of type struct member
void borrow_mem_sort(struct member** mem_list){ 
    struct member* head = *mem_list; 
    struct member* a; 
    struct member* b; 
    // base case
    if ((head == NULL) || (head->next == NULL)) { 
        return; 
    } 
    // Split head into two lists for mergesort
    split(head, &a, &b); 
    borrow_mem_sort(&a); 
    borrow_mem_sort(&b); 
    *mem_list = borrow_merge(a, b); 
} 

void borrow_sort(struct member** stud_list, struct member** fac_list){
	// We sort faculty and student lists separately first
	// bcoz we have to maintain them separately
	// both lists are first sorted on no of books(descending)
	// for tie-breaking use alphabetical order of name
	borrow_mem_sort(fac_list);
	borrow_mem_sort(stud_list);
	// We will now display them simultaneously using criteria in question
	struct member *fac_head = *fac_list; // to traverse the lists with ease without modification
	struct member *stud_head = *stud_list;
	int i = 1; // for displaying order
	while(fac_head!=NULL&&stud_head!=NULL){
		if(stud_head->no_books > fac_head->no_books){ // student has more books than faculty
			printf("%d. %d books borrowed by Student Member ", i, stud_head->no_books);
			fputs(stud_head->name, stdout);
			printf("\n");
			stud_head = stud_head->next;
		}
		else{ // student has equal or lesser books than faculty
			printf("%d. %d books borrowed by Faculty Member ", i, fac_head->no_books);
			fputs(fac_head->name, stdout);
			printf("\n");
			fac_head = fac_head->next;
		}
		i++;
	}
	// only one of below cases execute
	while(stud_head!=NULL){ // if any students remain to be listed
		printf("%d. %d books borrowed by Student Member ", i, stud_head->no_books);
		fputs(stud_head->name, stdout);
		printf("\n");
		stud_head = stud_head->next;
		i++;
	}
	while(fac_head!=NULL){ // if any faculty remain to be listed
		printf("%d. %d books borrowed by Faculty Member ", i, fac_head->no_books);
		fputs(fac_head->name, stdout);
		printf("\n");
		fac_head = fac_head->next;
		i++;
	}
}

void print_menu(){
	printf("=========================================\n\n");
	printf("    Welcome to VNIT Library Program\n\n");
	printf("Select the operation you want to perform:\n");
	printf("1. Add New Student/Faculty Member\n");
	printf("2. See who has issued maximum books\n");
	printf("3. List of all borrowers in descending order\n");
	printf("4. Insert a new book in  the database\n");
	printf("5. Calculate fine of a Library Member\n");
	printf("6. Get info of member with Maximum fine for late return\n");
	printf("0. Exit program\n");
	printf("Enter your choice: ");
}

int main(){
	int flag = 1;
	struct book* book_list = NULL;   // head of book list linked list
	struct member* stud_list = NULL; // head of student list linked list
	struct member* fac_list = NULL;  // head of faculty list linked list
	while (flag!=0){
		print_menu();
		scanf("%d", &flag);
		printf("\n");
		switch(flag){
			case 1:
				new_member(&stud_list, &fac_list);
				break;
			case 2:
				max_issue(stud_list, fac_list);
				break;
			case 3:
				borrow_sort(&stud_list, &fac_list);
				break;
			case 4:
				insert_book(&book_list);
				break;
			case 5:
				calc_fine(stud_list, fac_list,0, 1);
				break;
			case 6:
				max_fine(stud_list, fac_list);
				break;
			case 0:
				break;
			default:
				printf("\nInvalid Choice!\n\n");
		}
	}
	printf("\nExiting program!!\n\n");
	return 0;
}