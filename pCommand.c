#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

Sys sysCreator(char *buf, Park **parksPtr, int createdParks, Date *actualDate){
    Sys system = {buf, parksPtr, createdParks, actualDate};
    return system;
}

void pCommand(Sys *system){
	int capacity;
	float TaxX, TaxY, TaxZ;
	char parkName[MAX_BUFSIZ];
	
	if ((sscanf(system->buffer, "p \"%[^\"]\" %d %f %f %f", 
        parkName, &capacity, &TaxX, &TaxY, &TaxZ) == 5) ||
        (sscanf(system->buffer, "p %s %d %f %f %f", 
        parkName, &capacity, &TaxX, &TaxY, &TaxZ) == 5)){
		
        createPark(system, parkName, capacity, TaxX, TaxY, TaxZ);
	}
	/* Other case: p */
	else{parkInformation(system);}
}
void createPark(Sys *sys, char *Name, int Max, float x, float y, float z){
    if (pErrors(sys, Name, Max, x, y, z) == ERROR){return;}
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
        newPark->FirstCar = NULL;
        newPark->LastCar = NULL;
        newPark->FirstEntrie = NULL;
        newPark->LastEntrie = NULL;
        newPark->FirstExit = NULL;
        newPark->LastExit = NULL;
        sys->parkPtrArray[sys->createdParks++] = newPark;

    }
}
void parkInformation(Sys *system) {
    int iter;
    for (iter = ZERO; iter < system->createdParks; iter++) {
        printf("%s ", system->parkPtrArray[iter]->name);
        printf("%d ", system->parkPtrArray[iter]->maxCapacity);
        printf("%d\n", system->parkPtrArray[iter]->emptySpaces);
    }
}

int pErrors(Sys *system, char *Name, int capacity, float x, float y, float z){
    int iter;
    for (iter = ZERO; iter < system->createdParks; iter++) {
        if (strcmp(Name, system->parkPtrArray[iter]->name) == ZERO){
            printf("%s: parking already exists.\n", Name);
            return ERROR;
        }
    }
    /* invalid capacity */
    if (capacity <= ZERO) {
        printf("%d: invalid capacity.\n", capacity);
        return ERROR;
    }
    /* invalid cost? (Z > Y > X) */
    else if (x <= ZERO || y <= ZERO || z <= ZERO || !(x < y && y < z)) {
        printf("invalid cost.\n");
        return ERROR;
    }
    /* number of created parks must be <= MAX_PARKS*/
    else if (system->createdParks >= MAX_PARKS) {
        printf("too many parks.\n");
        return ERROR;
    }
    else {
        return ZERO;
    }
}


