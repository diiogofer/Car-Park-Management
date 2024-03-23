#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void fCommand(Sys *system){
    int day, month, year;
    char name[BUFSIZ];

    if((sscanf(system->buffer, "f \"%[^\"]\" %d-%d-%d", 
        name, &day, &month, &year) == 4) || 
        (sscanf(system->buffer, "f %s %d-%d-%d", 
        name, &day, &month, &year) == 4)){
            int parkPos = findParkByName(system, name);
            Date date = {year, month, day, ZERO, ZERO};
            if(fErrors(system, parkPos, &date, name) != ERROR){
                printExitsByPlate(system, parkPos, &date);
            }
        }
    else if((sscanf(system->buffer, "f \"%[^\"]\"", name) == 1) 
        || (sscanf(system->buffer, "f %s", name) == 1)){
        int parkPos = findParkByName(system, name);
        Date date = {ZERO, ZERO, ZERO, ZERO, ZERO};
        if(fErrors(system, parkPos, &date, name) != ERROR){
            printDailyEarnings(system, parkPos);
        }
    }
}

int fErrors(Sys *system, int parkPos, Date *date, char *ParkName){
    if(parkPos == ERROR){
        printf("%s: no such parking.\n", ParkName);
        return ERROR;
    }

    if(isEarlier(system, date) == SUCCESS){
        printf("invalid date.\n");
        return ERROR;
    }
    return SUCCESS;
}

void printExitsByPlate(Sys *system, int parkPosition, Date *date) {
    Park *park = system->parkPtrArray[parkPosition];
    Mov *currentMov = park->movList.head;

    while (currentMov != NULL) {
        if (currentMov->identifier == 's' && 
            date->day == currentMov->movDate.day &&
            date->month == currentMov->movDate.month && 
            date->year == currentMov->movDate.year) {
            printf("%s ", currentMov->license);
            printf("%02d:%02d ", currentMov->movDate.hour, 
                                currentMov->movDate.minute);
            printf("%.2f\n", currentMov->payment);
        }
        currentMov = currentMov->next;
    }
}

void printDailyEarnings(Sys *system, int parkPosition) {
    Park *park = system->parkPtrArray[parkPosition];
    Mov *currentMov = park->movList.head;
    if (currentMov == NULL) {
        return;
    }
    int prevDay = -1, prevMonth = -1, prevYear = -1;
    float dailyTotal = 0;
    while (currentMov != NULL) {
        if (currentMov->identifier == 's') { // Apenas movimentos de saída
            if (currentMov->movDate.day == prevDay && 
                currentMov->movDate.month == prevMonth && 
                currentMov->movDate.year == prevYear) {
                dailyTotal += currentMov->payment;
            } 
            else {
                if (prevDay != -1) { // Ignorar a primeira entrada
                    printf("%02d-%02d-%02d %.2f\n", 
                    prevDay, prevMonth, prevYear, dailyTotal);
                }
                dailyTotal = currentMov->payment;
                prevDay = currentMov->movDate.day;
                prevMonth = currentMov->movDate.month;
                prevYear = currentMov->movDate.year;
            }
        }
        currentMov = currentMov->next;
    }
    if (prevDay != -1) {
        printf("%02d-%02d-%02d %.2f\n", prevDay, prevMonth, prevYear, dailyTotal);
    }
}

