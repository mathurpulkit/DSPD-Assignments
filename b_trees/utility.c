#include<time.h>
#include"utility.h"


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
