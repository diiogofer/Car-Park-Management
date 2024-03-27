#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int isValidDate(Date *date){
    /* Static vector with month days */
    int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int validDate = 1;
    if (date->year < 0 || date->year > 9999) validDate = -1;
    if (date->month < 1 || date->month > 12) validDate = -1;
    if (date->day < 1 || date->day > monthDays[(date->month)-1]) validDate = -1;
    if (date->hour < 0 || date->hour > 23) validDate = -1;
    if (date->minute < 0 || date->minute > 59) validDate = -1;
    return validDate;
}

int isEarlier(Sys *system, Date *date2) {

    if (system->currentDate->year < date2->year) return SUCCESS;
    else if (system->currentDate->year > date2->year) return ERROR;

    if (system->currentDate->month < date2->month) return SUCCESS;
    else if (system->currentDate->month > date2->month) return ERROR;

    else if (system->currentDate->day < date2->day) return SUCCESS;
    if (system->currentDate->day > date2->day) return ERROR;

    if (system->currentDate->hour < date2->hour) return SUCCESS;
    else if (system->currentDate->hour > date2->hour) return ERROR;

    if (system->currentDate->minute < date2->minute) return SUCCESS;
    else if (system->currentDate->minute > date2->minute) return ERROR;
    
    return SUCCESS;
}

