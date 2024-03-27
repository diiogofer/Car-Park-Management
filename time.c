/**
 * @file time.c
 * @brief Implementation of functions related to date and time management.
 * @author Diogo Fernandes - ist1110306
 */
#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Checks if a given date is valid.
 * 
 * @param date Pointer to the date structure to be validated.
 * @return SUCCESS if the date is valid, ERROR otherwise.
 */
int isValidDate(Date *date){
    /* Static vector with month days */
    int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int validDate = SUCCESS;
    if (date->year < 0 || date->year > 9999) validDate = ERROR;
    if (date->month < 1 || date->month > 12) validDate = ERROR;
    if (date->day < 1 || date->day > monthDays[(date->month)-1]) 
        validDate = ERROR;
    if (date->hour < 0 || date->hour > 23) validDate = ERROR;
    if (date->minute < 0 || date->minute > 59) validDate = ERROR;
    return validDate;
}

/**
 * @brief Compares two dates to determine if the first date is earlier than the second.
 * 
 * @param system Pointer to the parking system.
 * @param date2 Pointer to the second date structure.
 * @return SUCCESS if the first date is earlier, ERROR otherwise.
 */
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

/**
 * @brief Converts the number of days in each month to minutes for a given year.
 * 
 * @param DaysMonthVec Array containing the number of days in each month.
 * @return The total number of minutes in the year.
 */
int yearToMinutes(int *DaysMonthVec){
    int iter, minutes = ZERO;
    for(iter = ZERO; iter < 12; iter++){
        minutes += DaysMonthVec[iter] * DAYTOMIN;
    }
    return minutes;
}

/**
 * @brief Traverses the years between two dates and calculates the total minutes.
 * 
 * @param ComparisonYear The year to start the comparison from.
 * @param date Pointer to the target date.
 * @param DaysMonthVec Array containing the number of days in each month.
 * @return The total number of minutes between the two years.
 */
int traverseYears(int ComparisonYear, Date *date, int *DaysMonthVec){
    int minutes = ZERO;
    if(ComparisonYear == date->year) return minutes;

    for(;ComparisonYear < date->year; ComparisonYear++){
        /* Accumulate minutes for each year */
        minutes += yearToMinutes(DaysMonthVec);
    }

    return minutes;
}

/**
 * @brief Traverses the months in a given date and calculates the total minutes.
 * 
 * @param date Pointer to the target date.
 * @param DaysMonthVec Array containing the number of days in each month.
 * @return The total number of minutes in the months.
 */
int traverseMonths(Date *date, int *DaysMonthVec){
    int iter, minutes = ZERO;
    
    for(iter = ZERO; iter < date->month - 1; iter++){
        /* Accumulate minutes for each month */
        minutes += DaysMonthVec[iter] * DAYTOMIN;
    }
    
    return minutes;
}

/**
 * @brief Calculates the total time in minutes for a given date.
 * 
 * @param date Pointer to the target date.
 * @param ComparisonYear The year to start the comparison from.
 * @param DaysMonthVec Array containing the number of days in each month.
 * @return The total time in minutes for the given date.
 */
int timeInMinutesFunc(Date *date, int ComparisonYear, int *DaysMonthVec){
    int timeInMinutes = ZERO;
    /* Convert years to minutes */
    timeInMinutes = traverseYears(ComparisonYear, date, DaysMonthVec);
    /* Add hours and minutes */
    timeInMinutes += date->hour * HOURTOMIN + date->minute;
    /* Add days */
    timeInMinutes += (date->day - 1) * DAYTOMIN;
    /* Convert months to minutes */
    timeInMinutes += traverseMonths(date, DaysMonthVec);
    return timeInMinutes;
}


/**
 * @brief Calculates the difference in minutes between two dates.
 * 
 * @param date1 Pointer to the first date.
 * @param date2 Pointer to the second date.
 * @return The difference in minutes between the two dates.
 */
int datesDiff(Date *date1, Date *date2){
    int timeInMinutes1, timeInMinutes2;
    /* Array of days in each month */
    int DaysMonthVec[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    /* Set the comparison year as the year before the first date */
    int ComparisonYear = date1->year - 1;
    /* Convert dates to minutes */
    timeInMinutes1 = timeInMinutesFunc(date1, ComparisonYear, DaysMonthVec);
    timeInMinutes2 = timeInMinutesFunc(date2, ComparisonYear, DaysMonthVec);
    /* Return the difference in minutes */
    return timeInMinutes2 - timeInMinutes1;
}
