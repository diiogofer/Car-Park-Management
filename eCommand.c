#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void eCommand(Sys *system){
    int day, month, year, hour, min;
    char matricula[9], nome[MAX_BUFSIZ];

    if((sscanf(system->buffer, "e \"%[^\"]\" %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7) ||
    sscanf(system->buffer, "e %s %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7){
        Date entry = { year, month, day, hour, min};
        int parkPos = eErrors(system, nome, matricula, &entry);

        if (parkPos != ERROR) {
            Park *park = system->parkPtrArray[parkPos];
            AddMovtoList(park, 'e', matricula, &entry); // Adiciona um movimento de entrada
            AddCar(park, park->movList.tail); // Associa o carro ao último movimento de entrada
            updateDate(system, &entry); 
            printf("%s %d\n", park->name, park->emptySpaces);
        }
    }
    else{return;}
}

void AddMovtoList(Park *park, char identifier, char *license, Date *entryDate) {
    Mov *newMov = malloc(sizeof(Mov)); // Aloca memória para o novo movimento
    if (newMov == NULL) {
        printf("Allocation Error.\n");
        exit(0);
    }
    newMov->identifier = identifier;
    newMov->payment = ZERO;
    strcpy(newMov->license, license);
    newMov->movDate = *entryDate;
    newMov->next = NULL;

    // Verifica se a lista de movimentos está vazia
    if (park->movList.head == NULL) {
        park->movList.head = newMov;
        park->movList.tail = newMov;
    } else {
        park->movList.tail->next = newMov;
        park->movList.tail = newMov;
    }
}

void AddCar(Park *park, Mov *carEntry) {
    Car *newCar = malloc(sizeof(Car)); // Aloca memória para o novo carro
    if (newCar == NULL) {
        printf("Allocation Error.\n");
        exit(1);
    }

    newCar->carEntry = carEntry;
    newCar->next = NULL;

    // Verifica se a lista de carros está vazia
    if (park->carList.head == NULL) {
        park->carList.head = newCar;
        park->carList.tail = newCar;
    } else {
        park->carList.tail->next = newCar;
        park->carList.tail = newCar;
    }
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
    while(iter < system->createdParks){
        if(strcmp(system->parkPtrArray[iter]->name, inputName) == ZERO)
            return iter;
        else{
            iter++;
        }
    }
    return ERROR;
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
    else if(validLicensePlate(matricula) == ERROR){
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

int validLicensePlate(char *license) {
    int charPairCounter = 0;
    int numberPairCounter = 0;
    int hyphen1Pos = 2, hyphen2Pos = 5;

    if (strlen(license) != 8) return ERROR;
    if (license[hyphen1Pos] != '-' || license[hyphen2Pos] != '-') return ERROR;

    for (int iter = 0; iter < 8; iter += 3) {
        int j = iter;
        if (isupper(license[j])) {
            if (!isupper(license[j + 1])) return ERROR;
            charPairCounter++;
        } else if (isdigit(license[j])) {
            if (!isdigit(license[j + 1])) return ERROR;
            numberPairCounter++;
        } else {
            return ERROR;
        }
    }

    if ((numberPairCounter == 1 && charPairCounter == 2) ||
        (numberPairCounter == 2 && charPairCounter == 1)) {
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

