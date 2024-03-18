#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

Sys sysCreator(char *buffer, Park **parkPtrArray, int createdParks,Date *currentDate){
    Sys system;
    system.buffer = buffer;
    system.parkPtrArray = parkPtrArray;
    system.createdParks = createdParks;
    system.currentDate = currentDate;
    return system;
}

void pCommand(Sys *system)
{
	int capacity;
	float TaxX, TaxY, TaxZ;
	char parkName[MAX_BUFSIZ];
	/* Fist case: between '\"' . */
	if (sscanf(system->buffer, "p \"%[^\"]\" %d %f %f %f", 
        parkName, &capacity, &TaxX, &TaxY, &TaxZ) == 5){
		
        createPark(system, parkName, capacity, TaxX, TaxY, TaxZ);
	}
	/* Second case: one word.*/
	else if(sscanf(system->buffer, "p %s %d %f %f %f", 
            parkName, &capacity, &TaxX, &TaxY, &TaxZ) == 5){
		
        createPark(system, parkName, capacity, TaxX, TaxY, TaxZ);
	}
	/* Other case: p */
	else{parkInformation(system);}
}

void createPark(Sys *system, char *Name, int Max, float x, float y, float z){
    /* Command p Errors */
    if (pErrors(system, Name, Max, x, y, z) == 1){
		return;
    }
	else{
        Park *newPark = malloc(sizeof(Park)); /*MALOC TEM DE FAZER FREE!!!*/
		if (newPark == NULL){
			printf("Allocation Error.\n");
			exit(0);
		}

        newPark->name = malloc((strlen(Name) + 1) * sizeof(char)); /*fazerFREE*/
		if (newPark->name == NULL){
			printf("Allocation Error\n");
			free(newPark); /*Free do parque caso a nome não tenha dado certo */
			exit(0);
        }
        /* changing park info */
        strcpy(newPark->name, Name);
        newPark->maxCapacity = Max;
        newPark->emptySpaces = Max;
        newPark->X = x;
        newPark->Y = y;
        newPark->Z = z;
        /* List pointers to NULL */
        newPark->FirstCarOfList = NULL;
        newPark->FirstMoveOfList = NULL;
        /* Incrementation of createdParks */
        system->parkPtrArray[system->createdParks] = newPark;
        (system->createdParks)++;
    }
}

void parkInformation(Sys *system) {
    int iter;
    for (iter = 0; iter < system->createdParks; iter++) {
        printf("%s ", system->parkPtrArray[iter]->name);
        printf("%d ", system->parkPtrArray[iter]->maxCapacity);
        printf("%d\n", system->parkPtrArray[iter]->emptySpaces);
    }
}

/*
void parkInformation(Sys *system) {
    int iter;
    for (iter = 0; iter < system->createdParks; iter++) {
        printf("Parque %d:\n", iter + 1);
        printf("Nome do parque: %s\n", system->parkPtrArray[iter]->name);
        printf("Capacidade max: %d\n", system->parkPtrArray[iter]->maxCapacity);
        printf("X: %.2f\n", system->parkPtrArray[iter]->X);
        printf("Y: %.2f\n", system->parkPtrArray[iter]->Y);
        printf("Z: %.2f\n", system->parkPtrArray[iter]->Z);
        printf("Empty: %d\n", system->parkPtrArray[iter]->emptySpaces);
        printf("\n");
    }
}
*/
int pErrors(Sys *system, char *Name, int capacity, float x, float y, float z){
    int i;
    /* Tratamento de Erros (pErrors == 1? Wrong : Normal;) */
    /* Park name exists? (strcmp == 0 ? EqualName : DifferentName)*/
    for (i = 0; i < system->createdParks; i++) {
        if (strcmp(Name, system->parkPtrArray[i]->name) == 0) {
            printf("%s: parking already exists.\n", Name);
            return 1;
        }
    }
    /* invalid capacity */
    if (capacity <= ZERO) {
        printf("%d: invalid capacity.\n", capacity);
        return 1;
    }
    /* invalid cost? (Z > Y > X) */
    else if (!(x < y && y < z)) {
        printf("invalid cost.\n");
        return 1;
    }
    /* number of created parks must be <= MAX_PARKS*/
    else if (system->createdParks >= MAX_PARKS) {
        printf("too many parks.\n");
        return 1;
    }
    else {
        return 0;
    }
}

int cleanupParks(Sys *system) {
    int i;

    for (i = 0; i < system->createdParks; i++) {
        free(system->parkPtrArray[i]->name);
        free(system->parkPtrArray[i]);
    }
     return 0;
}

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
    /* Invalid license plate */
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
    }
    else{
        Car *current = system->parkPtrArray[parkPos]->FirstCarOfList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newCar;
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
            sys->parkPtrArray[parkPos]->FirstMoveOfList = newMovement;
        } 
        else {
            Movement *current = sys->parkPtrArray[parkPos]->FirstMoveOfList;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newMovement;
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

int validMatricula(char *matricula) {
    char letra1, letra2, letra3, letra4;
    int num1, num2;
    // Verifica o formato "XX-99-XX"
    if (sscanf(matricula, "%c%c-%2d-%c%c", 
        &letra1, &letra2, &num1, &letra3, &letra4) == 5){
        if (isupper(letra1) && isupper(letra2) && 
            isupper(letra3) && isupper(letra4)
            && num1 >= 00 && num1 <= 99) {
                return 1; // Matrícula válida
        }
    }
    // Verifica o formato "99-XX-99"
    if(sscanf(matricula, "%2d-%c%c-%2d", &num1, &letra1, &letra2, &num2) == 4){
        if (isupper(letra1) && isupper(letra2) && num2 >= 00 && num2 <= 99) {
            return 1; // Matrícula válida
        }
    }

    return -1; // Matrícula inválida
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
