/**
 * @file fCommand.c
 * @brief Implementation of functions related to the "f" command.
 * @author Diogo Fernandes (diiogofer - https://github.com/diiogofer)
 */

#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Executes the "f" command to display exits by plate or daily earnings.
 * 
 * @param system Pointer to the system structure.
 */
void fCommand(Sys *system){
    int day, month, year;
    char name[BUFSIZ];
    /* Check if the input format includes a date */
    if((sscanf(system->buffer, "f \"%[^\"]\" %d-%d-%d", 
        name, &day, &month, &year) == 4) || 
        (sscanf(system->buffer, "f %s %d-%d-%d", 
        name, &day, &month, &year) == 4)){

            /* Check for errors and print exits by license plate */
            int parkPos = findParkByName(system, name);
            Date date = {year, month, day, ZERO, ZERO};
            if(fErrors(system, parkPos, &date, name) != ERROR){
                printExitsByPlate(system, parkPos, &date);
            }
        }
    /* Check if the input format includes only a name */
    else if((sscanf(system->buffer, "f \"%[^\"]\"", name) == 1) 
        || (sscanf(system->buffer, "f %s", name) == 1)){
        /* Check for errors and then print daily earnings */
        int parkPos = findParkByName(system, name);
        Date date = {ZERO, ZERO, ZERO, ZERO, ZERO};
        if(fErrors(system, parkPos, &date, name) != ERROR){
            printDailyEarnings(system, parkPos);
        }
    }
}

/**
 * @brief Handles error conditions for the "f" command.
 * 
 * @param system Pointer to the system structure.
 * @param parkPos Position of the park in the system array.
 * @param date Pointer to the target date.
 * @param ParkName Name of the park.
 * @return SUCCESS if no errors are found, ERROR otherwise.
 */
int fErrors(Sys *system, int parkPos, Date *date, char *ParkName){
    /* Check if the park position is valid */
    if(parkPos == ERROR){
        printf("%s: no such parking.\n", ParkName);
        return ERROR;
        }
    /* Check if the date is earlier than the current date in the system */
    if(isEarlier(system, date) == SUCCESS){
        printf("invalid date.\n");
        return ERROR;
    }
    /* No errors found*/
    return SUCCESS;
}

/**
 * @brief Prints exits by plate for a given park and date.
 * 
 * @param system Pointer to the system structure.
 * @param parkPosition Position of the park in the system array.
 * @param date Pointer to the target date.
 */
void printExitsByPlate(Sys *system, int parkPosition, Date *date){
    Park *park = system->parkPtrArray[parkPosition];
    Mov *currentMov = park->movList.head;

    /* Iterate through the movement list of the park */
    while (currentMov != NULL) {
        /* Check if the movement is an exit and matches the given date */
        if (currentMov->identifier == 's' && 
            date->day == currentMov->movDate.day &&
            date->month == currentMov->movDate.month && 
            date->year == currentMov->movDate.year) {
            /* Expected ouput: */
            printf("%s ", currentMov->license);
            printf("%02d:%02d ", currentMov->movDate.hour, 
                                currentMov->movDate.minute);
            printf("%.2f\n", currentMov->payment);
        }
        currentMov = currentMov->next;
    }
}

/**
 * @brief Prints daily earnings for a given park.
 * 
 * @param system Pointer to the system structure.
 * @param parkPosition Position of the park in the system array.
 */

void printDailyEarnings(Sys *system, int parkPosition){
    Park *park = system->parkPtrArray[parkPosition];
    Mov *currentMov = park->movList.head;
    if (currentMov == NULL) return;
    int prevDay = ERROR, prevMonth = ERROR, prevYear = ERROR;
    float dayTotal = ZERO;
    while (currentMov != NULL) {
        if (currentMov->identifier == 's') { /* movement is an exit */
            if(currentMov->movDate.day == prevDay && 
                currentMov->movDate.month == prevMonth && 
                currentMov->movDate.year == prevYear){
                dayTotal += currentMov->payment;
            } else {
                if (prevDay != ERROR) { 
                    printf("%02d-%02d-%02d %.2f\n", 
                    prevDay, prevMonth, prevYear, dayTotal);
                } /* Reset the daily total and update the previous date */
                dayTotal = currentMov->payment;
                prevDay = currentMov->movDate.day;
                prevMonth = currentMov->movDate.month;
                prevYear = currentMov->movDate.year;
            }
        }
        currentMov = currentMov->next;
    }
    if (prevDay != ERROR){ /* Print the final daily earnings */
        printf("%02d-%02d-%02d %.2f\n", prevDay, prevMonth, prevYear, dayTotal);
    }
}

