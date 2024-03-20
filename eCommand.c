#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void eCommand(Sys *system){
    int day, month, year, hour, min;
    char matricula[9], nome[MAX_BUFSIZ];
    Car *car;
    Entrie *entriePtr;
    if((sscanf(system->buffer, "e \"%[^\"]\" %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7) ||
    sscanf(system->buffer, "e %s %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7){
        Date entrie = { year, month, day, hour, min};
        int parkPos = eErrors(system, nome, matricula, &entrie);
        if(parkPos != ERROR){
            car = addCarToPark(system, parkPos);
            entriePtr = addEntrie(system, matricula, parkPos, &entrie);
            eChanges(system, car, entriePtr, &entrie, parkPos);
        }
    }
    else{return;}
}

Car *addCarToPark(Sys *system, int parkPos){
    /* Creating the car */
    Car *newCar = malloc(sizeof(Car));
    if (newCar == NULL) {
        printf("Allocation Error.\n");
        exit(0);
        }
    newCar->carEntrie = NULL;
    newCar->next = NULL;
    /* Adding the car to the park */
    if(system->parkPtrArray[parkPos]->FirstCar == NULL){
        system->parkPtrArray[parkPos]->FirstCar = newCar;
        system->parkPtrArray[parkPos]->LastCar = newCar;
    }
    else{
        system->parkPtrArray[parkPos]->LastCar->next = newCar;
        system->parkPtrArray[parkPos]->LastCar = newCar;
    }
    return newCar;
}

Entrie *addEntrie(Sys *sys, char *matricula, int parkPos, Date *entrada){
    if(parkPos != ERROR){
        Entrie *newEntrie = malloc(sizeof(Entrie));
        if (newEntrie == NULL) {
            printf("Allocation Error\n");
            exit(0);
        }
        strcpy(newEntrie->matricula, matricula);
        newEntrie->entrada = *entrada;
        newEntrie->saida = NULL;
        newEntrie->next = NULL;
        
        if (sys->parkPtrArray[parkPos]->FirstEntrie == NULL) {
            /*Atualiza o ponteiro que aponta para o primeiro parque */
            sys->parkPtrArray[parkPos]->FirstEntrie = newEntrie;
            /* O ponteiro que aponta para o ulitmo parque = ao do primeiro */
            sys->parkPtrArray[parkPos]->LastEntrie = newEntrie;
        } 
        else {
            sys->parkPtrArray[parkPos]->LastEntrie->next = newEntrie;
            sys->parkPtrArray[parkPos]->LastEntrie = newEntrie;
        }
        return newEntrie;
    }
    return NULL;
}



void eChanges(Sys *system, Car *car, Entrie *entrie, Date *date, int parkPos){
    /* Associate Entrie To Car*/
    car->carEntrie = entrie;
    /* Changing system last date */
    system->currentDate->day = date->day;
    system->currentDate->month = date->month;
    system->currentDate->year = date->year;
    system->currentDate->hour = date->hour;
    system->currentDate->minute = date->minute;
    /* Decrementing counters in parks */
    Park *currentPark = system->parkPtrArray[parkPos];
    (currentPark->emptySpaces)--;
    /* Printing park information */
    printf("%s %d\n", currentPark->name, currentPark->emptySpaces);
}


int eErrors(Sys *system, char *inputName, char *matricula, Date *date){
    /* invalid park name */
    int parkPosition = findParkByName(system, inputName);
    if(parkPosition == ERROR){
        printf("%s: no such parking.\n", inputName);
        return ERROR;
    }
    /* parking is full */
    else if(system->parkPtrArray[parkPosition]->emptySpaces == ZERO){
        printf("%s: parking is full.\n", inputName);
        return ERROR;
    }
    /* Invalid licence plate */
    else if(validMatricula(matricula) == ERROR){
        printf("%s: invalid licence plate.\n", matricula); 
        return ERROR;
    }
    /* Caso onde veiculo está dentro de um parque */
    else if(searchMatricula(system, matricula) == ERROR){
        printf("%s: invalid vehicle entry.\n", matricula);
        return ERROR;
    }
    /* invalid date */
    else if (isValidDate(date) == ERROR || isEarlier(system, date) == ERROR){
        printf("invalid date.\n");
        return ERROR;
    }
    else{
        return parkPosition;
    }
}

int findParkByName(Sys *system , char *inputName){
    int iter = ZERO;
    while(iter < system->createdParks){
        if(strcmp(system->parkPtrArray[iter]->name, inputName) == ZERO)
            return iter;
        else{
            iter++;
        }
    }
    return ERROR;
}

int validMatricula(char *matricula){
    int i;
    int charPairCounter = ZERO;
    int numberPairCounter = ZERO;
    int hyphen1Pos = 2, hyphen2Pos = 5; 
    if(strlen(matricula) != 8){return ERROR;}
    if(matricula[hyphen1Pos] != '-' || matricula[hyphen2Pos] != '-'){
        return ERROR;
    }
    for(i = ZERO; i <= hyphen2Pos+1; i+=(hyphen1Pos+1)){
        int j = i;
        if(isupper(matricula[j])){
            if(isupper(matricula[j+1]))
                charPairCounter++;
            else{ return ERROR;}
        }
        else if(isdigit(matricula[j])){
            if(isdigit(matricula[j+1])){
                numberPairCounter++;
            }
            else{return ERROR;}
        }
        else{return ERROR;}
    }
    if((numberPairCounter == 1 && charPairCounter == 2) ||
        (numberPairCounter == 2 && charPairCounter == 1)){
        return SUCCESS;
        }
    else{return ERROR;}  
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

    if (system->currentDate->year < date2->year) return 1;
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

int searchMatricula(Sys *system, char *matricula) {
    for (int iter = ZERO; iter < system->createdParks; iter++) {
        Car *currentCar = system->parkPtrArray[iter]->FirstCar;
        while (currentCar != NULL) {
            if (strcmp(currentCar->carEntrie->matricula, matricula) == ZERO) {
                return ERROR;
            }
            currentCar = currentCar->next;
        }
    }
    return SUCCESS;
}

void printCarMatriculas(Park *park) {
    Car *currentCar = park->FirstCar;
    while (currentCar != NULL) {
        printf("Matrícula do carro: %s\n", currentCar->carEntrie->matricula);
        currentCar = currentCar->next;
    }
}

void printEntradaDetails(Park *park) {
    Entrie *currentEntrie = park->FirstEntrie;
    while (currentEntrie != NULL) {
        printf("Matrícula: %s, Data de entrada: %d/%d/%d %d:%d\n", 
               currentEntrie->matricula, 
               currentEntrie->entrada.day, currentEntrie->entrada.month, currentEntrie->entrada.year,
               currentEntrie->entrada.hour, currentEntrie->entrada.minute);
        currentEntrie = currentEntrie->next;
    }
}

void printAllCarMatriculas(Sys *system) {
    for (int i = 0; i < system->createdParks; i++) {
        printf("Parque %s:\n", system->parkPtrArray[i]->name);
        printCarMatriculas(system->parkPtrArray[i]);
    }
}

void printAllEntradaDetails(Sys *system) {
    for (int i = 0; i < system->createdParks; i++) {
        printf("Parque %s:\n", system->parkPtrArray[i]->name);
        printEntradaDetails(system->parkPtrArray[i]);
    }
}