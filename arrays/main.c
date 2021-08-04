#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define MAX_B_ISSUE_F 4
#define MAX_B_ISSUE_S 2
#define NAME_SIZE 50
#define DEPT_SIZE 4
#define MAX_ARRAY_SIZE 50
#define MAX_FAC_DAYS 30
#define MAX_STUD_DAYS 14
enum designation_type {Student, Faculty};

struct date{
	int day;
	int month;
	int year;
};

struct member{
	char name[NAME_SIZE];
	char dept[DEPT_SIZE]; //three letter short form for departments
	enum designation_type designation;
	int member_id;
	int no_books; //no of books issued
	int iss_book_id[MAX_B_ISSUE_F]; //Accession number of books issued
	struct date ret_date[MAX_B_ISSUE_F]; //return date of books issued
	
} students[MAX_ARRAY_SIZE], faculty[MAX_ARRAY_SIZE];

int stud_count = 0, fac_count = 0;

struct book{
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
} books[MAX_ARRAY_SIZE];

int book_count = 0;

struct mem_sort{
	char *name;
	int no_books;
	enum designation_type designation;
} member_sort[2*MAX_ARRAY_SIZE]; // useful when sorting to not disturb original array

void new_member(){
	int i;
	printf("Enter details:\n");
	printf("Enter 0 for faculty, any other number for student: ");
	scanf("%d", &i);
	if(i == 0){
		printf("Enter name: ");
		fgets(faculty[fac_count].name, NAME_SIZE, stdin); // reads full name
		// we used fgets as scanf stops reading at space
		printf("Enter Department's 3 letter name: ");
		fgets(faculty[fac_count].dept, DEPT_SIZE, stdin);
		faculty[fac_count].designation = Faculty;
		printf("Enter member ID: ");
		scanf("%d", &faculty[fac_count].member_id);
		faculty[fac_count].no_books = 0; //initialisation necessary for this element
		fac_count++;
		printf ("New Faculty member added in the database!!\n");
	}
	else{
		printf("Enter name: ");
		fgets(students[stud_count].name, NAME_SIZE, stdin); // reads full name
		// we used fgets as scanf stops reading at space
		printf("Enter Department's 3 letter name: ");
		fgets(students[stud_count].dept, DEPT_SIZE, stdin);
		students[stud_count].designation = Student;
		printf("Enter member ID: ");
		scanf("%d", &students[stud_count].member_id);
		stud_count++;
		printf ("New Student member added in the database!!\n");
	}
}

void max_issue(){
	int max_val = 0, max_index = 0;
	int i;
	for (i=0;i<fac_count;i++){
		if (faculty[i].no_books > max_val){
			max_index = i;
			max_val = faculty[i].no_books;
		}
	}
	printf("The faculty who borrowed maximum books is ");
	fputs(faculty[max_index].name, stdout); //prints string with spaces
	printf(" and they issued %d books.\n", max_val);
	//printing for faculty done
	max_val = 0;
	max_index = 0;
	for (i=0;i<stud_count;i++){
		if (students[i].no_books > max_val){
			max_index = i;
			max_val = students[i].no_books;
		}
	}
	printf("The student who borrowed maximum books is ");
	fputs(students[max_index].name, stdout); //prints string with spaces
	printf(" and they issued %d books.\n", max_val);
}

void merge(struct mem_sort arr[], int l, int m, int r){
    int n1 = m-l+1;
    int n2 = r-m;
    struct mem_sort L[n1], R[n2];
    // Copy data to temp arrays L[] and R[]
    int i, j, k;
    for (i=0;i<n1;i++){
        L[i] = arr[l+i];
    }
    for (j=0;j<n2;j++){
        R[j] = arr[m+1+j];
    }
    i= 0;
	j = 0;
	k = l;
    // k is Initial index of merged subarray 
    while (i<n1 && j<n2){
        if(L[i].no_books > R[j].no_books){ // sorts on no of books
            arr[k] = L[i];
            i++;
        }
        else if (L[i].no_books == R[j].no_books){  
        	if(L[i].designation > R[j].designation){ //sorts on basis of designation
        		arr[k] = L[i];
        		i++;
			}
			else if(L[i].designation < R[j].designation){
				arr[k] = R[j];
				j++;
			}
			else{ //sorts on basis of alphabetical order
				if(strcmp(L[i].name, R[j].name)<0){
					arr[k] = L[i];
        			i++;
				}
				else{
					arr[k] = R[j];
					j++;
				}
			}
		}
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    // copy remaining L
    while (i<n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    // Copy remaining R
    while (j<n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void merge_sort(struct mem_sort arr[],int l,int r){
    if(l>=r){
        return;//returns recursively
    }
    int m = (l+r-1)/2;
    merge_sort(arr,l,m);
    merge_sort(arr,m+1,r);
    merge(arr,l,m,r);
}

void borrow_sort(){
	int i, j;
	for(i=0;i<fac_count;i++){ // initialising member_sort array
		member_sort[i].name = &faculty[i].name[0];
		member_sort[i].no_books = faculty[i].no_books;
		member_sort[i].designation = Faculty;
	}
	int total = fac_count + stud_count;
	for(i=fac_count;i<total;i++){
		member_sort[i].name = &students[i-fac_count].name[0];
		member_sort[i].no_books = students[i-fac_count].no_books;
		member_sort[i].designation = Student;
	}
	// initialising done
	merge_sort(member_sort, 0, total - 1);
	printf("The sorted list of borrowers is:\n");
	for(i=0;i<total;i++){
		printf("%d. %d books borrowed by ", i+1, member_sort[i].no_books);
		if (member_sort[i].designation==Faculty){
			printf("Faculty member ");
		}
		else{
			printf("Student member ");
		}
		fputs(member_sort[i].name, stdout);
		printf("\n");
	}
}

void book_shift(int start, int size){
	int i; // this function moves books forward to make room for a new one
	for(i=size-1; i>=start; i--){
		books[i+1] = books[i];
	}
}
void insert_book(){
	// book details are first stored in a temporary structure
	// then inserted in the correct place in the sorted database
	struct book temp;
	printf("Enter book details:");
	printf("Enter book name: ");
	fgets(temp.name, NAME_SIZE, stdin);
	printf("Enter subject name: ");
	fgets(temp.subject, NAME_SIZE, stdin);
	printf("Enter author(s) names: ");
	fgets(temp.authors, NAME_SIZE, stdin);
	temp.acc_no = book_count+1;
	printf("Accession number assigned is: %d\n", book_count+1);
	printf("Enter Year Of Publication: ");
	scanf("%d", &temp.year_pub);
	printf("Enter Hall No, Rack No, Row No: ");
	scanf("%d %d %d", &temp.hall_no, &temp.rack_no, &temp.row_no);
	temp.no_issued = 0;
	printf("Enter number of copies acquired: ");
	scanf("%d", &temp.no_unissued);
	int i=0, flag = 0;
	for(i=0; i<book_count&&flag==0; i++){
		if(strcmp(temp.subject, books[i].subject)<0){
			book_shift(i, book_count); //shifts books to make room for new
			books[i] = temp;
			flag = 1;
		}
		else if(strcmp(temp.subject, books[i].subject)==0){
			if(strcmp(temp.name, books[i].name)<0){
				book_shift(i, book_count); //shifts books to make room for new
				books[i] = temp;
				flag = 1;
			}
			else if(strcmp(temp.name, books[i].name)==0){
				if(strcmp(temp.authors, books[i].authors)<0){
					book_shift(i, book_count); //shifts books to make room for new
					books[i] = temp;
					flag = 1;
				}
				else if(strcmp(temp.authors, books[i].authors)==0){
					if(temp.year_pub<books[i].year_pub){
						book_shift(i, book_count); //shifts books to make room for new
						books[i] = temp;
						flag = 1;
					}
				}
			}
		}
	}
	if(flag==0){ // if book is to be inserted at last
		books[book_count]= temp;
	}
	book_count++;
	printf("Book Inserted in the database!!\n\n");
}

int date_diff(struct date a, struct date b){
	int month_days[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	int a_day, b_day; //stores days elapsed since 31 Dec 2000
	a_day = 365*(a.year-2000);
	a_day += (a.year-2001)/4; // for leap year, it ignores current year for now
	if(a.month>2&&(a.year%4==0)){ //if current year is leap
		a_day++;
	}
	a_day += month_days[a.month - 1];
	a_day += a.day;
	// we calculate the same for b
	b_day = 365*(b.year-2000);
	b_day += (b.year-2001)/4; // for leap year, it ignores current year for now
	if(b.month>2&&(b.year%4==0)){ //if current year is leap
		b_day++;
	}
	b_day += month_days[b.month - 1];
	b_day += b.day;
	return a_day - b_day;
}

struct date get_cur_date(){
	struct date ans;
	time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    ans.day = local->tm_mday;            // get day of month (1 to 31)
    ans.month = local->tm_mon + 1;       // get month of year (0 to 11)
    ans.year = local->tm_year + 1900;    // get year since 1900
    return ans;
}

float calc_fine(int mem_id, int print_flag){
	if (mem_id == 0){ // if mem_id is passed zero, it asks the user for id
		printf("Enter Member ID whose fine you want to calculate: ");
		scanf("%d", &mem_id);
	}
	int i, j, flag = 0;
	float fine = 0;
	struct date today = get_cur_date(); //gets today's date
	for(i=0;i<fac_count&&flag==0;i++){
		if(mem_id==faculty[i].member_id){
			for(j=0;j<faculty[i].no_books;j++){
				if(date_diff(faculty[i].ret_date[i],today)>0){
					fine += 0.5*date_diff(faculty[i].ret_date[i],today);
				}
			}
			flag = 1;
		}
	}
	for(i=0;i<stud_count&&flag==0;i++){
		if(mem_id==students[i].member_id){
			for(j=0;j<students[i].no_books;j++){
				if(date_diff(students[i].ret_date[i],today)>0){
					fine += 0.5*date_diff(students[i].ret_date[i],today);
				}
			}
			flag = 1;
		}
	}
	if (flag==1&&print_flag==1){
		printf("Member ID %d has fine of %f rupees.\n", mem_id,fine);
	}
	else if(print_flag==1){
		printf("Member ID %d not found!!\n\n", mem_id);
	}
	return fine;
}

void max_fine(){
	float max_fine = 0.0;
	int i = 0, mem_no = 0;
	float cur_fine;
	for(i=0;i<fac_count;i++){
		cur_fine = calc_fine(faculty[i].member_id, 0);
		if(cur_fine>max_fine){
			max_fine = cur_fine;
		}
	}
	for(i=0;i<stud_count;i++){
		cur_fine = calc_fine(students[i].member_id, 0);
		if(cur_fine>max_fine){
			max_fine = cur_fine;
		}
	}
	if (mem_no!=0){ //if checks if anyone has been fined at all
		printf("Max fine was received by Member No %d of %f rupees\n\n", mem_no, max_fine);
	}
	else{
		printf("No one has any fines!!\n\n");
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
	int flag = 1, temp;
	while (flag!=0){
		print_menu();
		scanf("%d", &flag);
		printf("\n");
		switch(flag){
			case 1:
				new_member();
				break;
			case 2:
				max_issue();
				break;
			case 3:
				borrow_sort();
				break;
			case 4:
				insert_book();
				break;
			case 5:
				calc_fine(0, 1);
				break;
			case 6:
				max_fine();
				break;
		}
	}
	printf("\n\nExiting program!!!");
	return 0;
}

