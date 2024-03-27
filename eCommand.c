#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void eCommand(Sys *system){
    int day, month, year, hour, min;
    char matricula[9], nome[MAX_BUFSIZ];

    /* Parse input to extract vehicle information */
    if((sscanf(system->buffer, "e \"%[^\"]\" %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7) ||
    sscanf(system->buffer, "e %s %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7){
        Date entry = { year, month, day, hour, min};
        
        /* Check for errors in the entrance operation */
        int parkPos = eErrors(system, nome, matricula, &entry);
        if (parkPos != ERROR) {
            Park *park = system->parkPtrArray[parkPos];
            
            /* Add an entrance movement to the park's movement list */
            AddMovtoList(park, 'e', matricula, &entry); 
            /* Add a new car to the park's car list */
            AddCar(park, park->movList.tail); 
            /* Update the system's current date */
            updateDate(system, &entry); 
            
            /* Print the name of the park and the number of empty spaces */
            printf("%s %d\n", park->name, park->emptySpaces);
        }
    }
    return;
}

void AddMovtoList(Park *park, char identifier, char *license, Date *entryDate){
    /* Allocate memory for the new movement */
    Mov *newMov = malloc(sizeof(Mov));
    if (newMov == NULL) exit(0);

    /* Initialize the new movement with provided information */
    newMov->identifier = identifier;
    newMov->payment = ZERO;
    strcpy(newMov->license, license);
    newMov->movDate = *entryDate;
    newMov->next = NULL;

    /* empty movement list */
    if(park->movList.head == NULL){
        park->movList.head = newMov;
        park->movList.tail = newMov;
    } 
    /* Add the new movement to the end of the existing movement list */
    else{
        park->movList.tail->next = newMov;
        park->movList.tail = newMov;
    }
}

void AddCar(Park *park, Mov *carEntry) {
    /* Allocate memory for the new car */
    Car *newCar = malloc(sizeof(Car));
    if(newCar == NULL) exit(1);

    /* Set the car entry information */
    newCar->carEntry = carEntry;
    newCar->next = NULL;

    /* empty car list */
    if(park->carList.head == NULL) {
        park->carList.head = newCar;
        park->carList.tail = newCar;
    }

    /* Add the new car to the end of the existing car list */
    else{
        park->carList.tail->next = newCar;
        park->carList.tail = newCar;
    }

    /* Decrement empty spaces number */
    park->emptySpaces--;
}

void updateDate(Sys *system, Date *entry) {
    system->currentDate->year = entry->year;
    system->currentDate->month = entry->month;
    system->currentDate->day = entry->day;
    system->currentDate->hour = entry->hour;
    system->currentDate->minute = entry->minute;
}

int findParkByName(Sys *system , char *inputName){
    int iter = ZERO;

    /* Loop through the array to check if the names match */
    while(iter < system->createdParks){
        if(strcmp(system->parkPtrArray[iter]->name, inputName) == ZERO) 
            return iter;
        else{
            iter++;
        }
    }

    return ERROR;
}

int eErrors(Sys *system, char *inputName, char *license, Date *date){
    /* Check if the specified parking exists */
    int parkPosition = findParkByName(system, inputName);
    if(parkPosition == ERROR){
        printf("%s: no such parking.\n", inputName);
        return ERROR;
    }
    /* Check if the parking is full */
    else if(system->parkPtrArray[parkPosition]->emptySpaces == ZERO){
        printf("%s: parking is full.\n", inputName);
        return ERROR;
    }
    /* Validate the license plate */
    else if(validLicensePlate(license) == ERROR){
        printf("%s: invalid licence plate.\n", license); 
        return ERROR;
    }
    /* Check if the vehicle is already inside the parking */
    else if(searchMatricula(system, license) == ERROR){
        printf("%s: invalid vehicle entry.\n", license);
        return ERROR;
    }
    /* Validate the date */
    else if (isValidDate(date) == ERROR || isEarlier(system, date) == ERROR){
        printf("invalid date.\n");
        return ERROR;
    }
    else{
        return parkPosition;
    }
}

int validLicensePlate(char *license){
    int iter;
    int charPairCounter = ZERO, numberPairCounter = ZERO;
    int hyphen1Pos = 2, hyphen2Pos = 5; 
    /* Check the length and if the hyphens are in the correct positions */
    if(strlen(license) != LICENSELEN) return ERROR;
    if(license[hyphen1Pos] != '-' || license[hyphen2Pos] != '-') return ERROR;
    /* Iterate over the pairs of characters in the license plate */
    for(iter = ZERO; iter <= hyphen2Pos+1; iter += (hyphen1Pos + 1)){
        int pairIter = iter;
        if(isupper(license[pairIter]) && isupper(license[pairIter+1]))
            charPairCounter++;
        else if(isdigit(license[pairIter]) && isdigit(license[pairIter + 1]))
                numberPairCounter++;
        else {return ERROR;}
    }
    /* Check if the number of character and number pairs is correct */
    if((numberPairCounter == 1 && charPairCounter == 2) ||
        (numberPairCounter == 2 && charPairCounter == 1)){
        return SUCCESS;
    }
    return ERROR;
}


int searchMatricula(Sys *system, char *matricula) {
    for (int iter = 0; iter < system->createdParks; iter++) {
        Car *currentCar = system->parkPtrArray[iter]->carList.head;
        while (currentCar != NULL) {
            if (strcmp(currentCar->carEntry->license, matricula) == 0) {
                return ERROR;
            }
            currentCar = currentCar->next;
        }
    }
    return SUCCESS;
}


int isValidDate(Date *date) {
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

