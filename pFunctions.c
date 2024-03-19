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
        newPark->LastAddedCar = NULL;
        newPark->FirstMoveOfList = NULL;
        newPark->LastAddedMovement = NULL;
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



