#ifndef UTILITY_H  
#define UTILITY_H

typedef struct date{ // defined in header to avoid a bug
	int day;
	int month;
	int year;
};


int date_diff(struct date, struct date);
struct date get_cur_date();

#endif