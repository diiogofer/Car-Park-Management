#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void eCommand(Sys *system){
    int day, month, year, hour, min;
    char matricula[9], nome[MAX_BUFSIZ];
    Car *car;
    Movement *mov;
    if(sscanf(system->buffer, "e \"%[^\"]\" %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7 ){
        Date entrada = { year, month, day, hour, min};
        int parkPos = eErrors(system, nome, matricula, &entrada);
        if(parkPos != -1){
            car = addCarToPark(system, matricula, parkPos);
            Date saida = {ZERO, ZERO, ZERO, ZERO, ZERO};
            mov = addMov(system, matricula, parkPos, &entrada, &saida);
            eChanges(system, car, mov, &entrada, parkPos);
        }
    }
    else if(sscanf(system->buffer, "e %s %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7){
        Date entrada = { year, month, day, hour, min};
        int parkPos = eErrors(system, nome, matricula, &entrada);
        if(parkPos != -1){
            car = addCarToPark(system, matricula, parkPos);
            Date saida = {ZERO, ZERO, ZERO, ZERO, ZERO};
            mov = addMov(system, matricula, parkPos, &entrada, &saida);
            eChanges(system, car, mov, &entrada, parkPos);
        }
    }
    else{return;}
}

int eErrors(Sys *system, char *inputName, char *matricula, Date *date){
    /* invalid park name */
    int parkPosition = findParkByName(system, inputName);
    if(parkPosition == -1){
        printf("%s: no such parking.\n", inputName);
        return -1;
    }
    /* parking is full */
    else if(system->parkPtrArray[parkPosition]->emptySpaces == 0){
        printf("%s: parking is full.\n", inputName);
        return -1;
    }
    /* Invalid licence plate */
    else if(validMatricula(matricula) == -1){
        printf("%s: invalid licence plate.\n", matricula); 
        return -1;
    }
    /* Caso onde veiculo está dentro de um parque */
    else if(searchMatricula(system, matricula) == -1){
        printf("%s: invalid vehicle entry.\n", matricula);
        return -1;
    }

    /* invalid date */
    else if (dateChecker(system, date) == -1){
        printf("invalid date.\n");
        return -1;
    }
    else{
        return parkPosition;
    }
}

Car *addCarToPark(Sys *system, char *matricula, int parkPos){
    /* Creating the car */
    Car *newCar = malloc(sizeof(Car));
    if (newCar == NULL) {
        printf("Allocation Error.\n");
        exit(0);
        }
    strcpy(newCar->matricula, matricula);
    newCar->carMove = NULL;
    newCar->next = NULL;
    /* Adding the car to the park */
    if(system->parkPtrArray[parkPos]->FirstCarOfList == NULL){
        system->parkPtrArray[parkPos]->FirstCarOfList = newCar;
        system->parkPtrArray[parkPos]->LastAddedCar = newCar;
    }
    else{
        system->parkPtrArray[parkPos]->LastAddedCar->next = newCar;
        system->parkPtrArray[parkPos]->LastAddedCar = newCar;
    }
    return newCar;
}

Movement *addMov(Sys *sys, char *matricula, int parkPos, Date *entrada, Date *saida){
    if(parkPos != -1){
        Movement *newMovement = malloc(sizeof(Movement));
        if (newMovement == NULL) {
            printf("Allocation Error\n");
            exit(0);
        }
        strcpy(newMovement->matricula, matricula);
        newMovement->entrada = *entrada;
        newMovement->saida = *saida;
        newMovement->next = NULL;

        if (sys->parkPtrArray[parkPos]->FirstMoveOfList == NULL) {
            /*Atualiza o ponteiro que aponta para o primeiro parque */
            sys->parkPtrArray[parkPos]->FirstMoveOfList = newMovement;
            /* O ponteiro que aponta para o ulitmo parque = ao do primeiro */
            sys->parkPtrArray[parkPos]->LastAddedMovement = newMovement;
        } 
        else {
            sys->parkPtrArray[parkPos]->LastAddedMovement->next = newMovement;
            sys->parkPtrArray[parkPos]->LastAddedMovement = newMovement;
        }
        return newMovement;
    }
    return NULL;
}


void eChanges(Sys *system, Car *car, Movement *mov, Date *date, int parkPos){
    /* Associate Movement To Car*/
    car->carMove = mov;
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


int findParkByName(Sys *system , char *inputName){
    int iter = 0;
    while(iter < system->createdParks){
        if(strcmp(system->parkPtrArray[iter]->name, inputName) == 0)
            return iter;
        else{
            iter++;
        }
    }
    return -1;
}

void printAllMovements(Sys *system) {
    for (int i = 0; i < system->createdParks; i++) {
        printMovementsInPark(system->parkPtrArray[i]);
    }
}

int validMatricula(char *matricula){
    int i;
    int charPairCounter = ZERO;
    int numberPairCounter = ZERO;
    int hyphen1Pos = 2, hyphen2Pos = 5; 
    if(matricula[hyphen1Pos] != '-' || matricula[hyphen2Pos] != '-'){
        return -1;
    }
    for(i = ZERO; i <= hyphen2Pos+1; i+=(hyphen1Pos+1)){
        int j = i;
        if(isupper(matricula[j])){
            if(isupper(matricula[j+1]))
                charPairCounter++;
            else{ return -1;}
        }
        else if( isdigit(matricula[j])){
            if(isdigit(matricula[j+1])){
                numberPairCounter++;
            }
            else{return -1;}
        }
        else{return -1;}
    }
    if((numberPairCounter == 1 && charPairCounter == 2) ||
        (numberPairCounter == 2 && charPairCounter == 1)){
        return 1;
        }
    else{return -1;}  
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
    else if (system->currentDate->year > date2->year) return -1;

    if (system->currentDate->month < date2->month) return 1;
    else if (system->currentDate->month > date2->month) return -1;

    else if (system->currentDate->day < date2->day) return 1;
    if (system->currentDate->day > date2->day) return -1;

    if (system->currentDate->hour < date2->hour) return 1;
    else if (system->currentDate->hour > date2->hour) return -1;

    if (system->currentDate->minute < date2->minute) return 1;
    else if (system->currentDate->minute > date2->minute) return -1;
    
    return 0;
}

int dateChecker(Sys *system, Date *date) {
    if (isValidDate(date) != -1 && isEarlier(system, date) != -1) {
        return 1;
    }
    else {
        return -1;
    }
}

void printCarsInParks(Sys *system) {
    for (int i = 0; i < system->createdParks; i++) {
        printf("Carros no Parque %s:\n", system->parkPtrArray[i]->name);
        Car *currentCar = system->parkPtrArray[i]->FirstCarOfList;
        while (currentCar != NULL) {
            printf("- Matrícula: %s\n", currentCar->matricula);
            currentCar = currentCar->next;
        }
        printf("\n");
    }
}

void printMovementsInPark(Park *park) {
    printf("Movimentos no Parque %s:\n", park->name);
    Movement *currentMovement = park->FirstMoveOfList;
    while (currentMovement != NULL) {
        printf("- Matrícula: %s\n", currentMovement->matricula);
        printf("  Data de entrada: %02d-%02d-%04d %02d:%02d\n", 
               currentMovement->entrada.day, currentMovement->entrada.month, 
               currentMovement->entrada.year, currentMovement->entrada.hour, 
               currentMovement->entrada.minute);
        printf("  Data de saída: %02d-%02d-%04d %02d:%02d\n", 
               currentMovement->saida.day, currentMovement->saida.month, 
               currentMovement->saida.year, currentMovement->saida.hour, 
               currentMovement->saida.minute);
        currentMovement = currentMovement->next;
    }
    printf("\n");
}

int searchMatricula(Sys *system, char *matricula) {
    for (int iter = 0; iter < system->createdParks; iter++) {
        Car *currentCar = system->parkPtrArray[iter]->FirstCarOfList;
        while (currentCar != NULL) {
            if (strcmp(currentCar->carMove->matricula, matricula) == 0) {
                return -1;
            }
            currentCar = currentCar->next;
        }
    }
    return 1;
}


