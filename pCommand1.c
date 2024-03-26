#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

Sys sysCreator(char *buffer, Park **parksPtr, int numParks, Date *actualDate){
    /* Initialize a system struct and return it*/
    Sys system = {buffer, parksPtr, numParks, actualDate};
    return system;
}

void pCommand(Sys *system){
	int capacity;
	float TaxX, TaxY, TaxZ;
	char parkName[MAX_BUFSIZ];

    /* Analysing input */
	if ((sscanf(system->buffer, "p \"%[^\"]\" %d %f %f %f", 
        parkName, &capacity, &TaxX, &TaxY, &TaxZ) == 5) ||
        (sscanf(system->buffer, "p %s %d %f %f %f", 
        parkName, &capacity, &TaxX, &TaxY, &TaxZ) == 5)){
		
        /* Creating a park */
        addPark(system, parkName, capacity, TaxX, TaxY, TaxZ);
	}

	/* Showing all parks information */
	else{
        parkInformation(system);
    }
}

void addPark(Sys *sys, char *Name, int Max, float TaxX, float TaxY, float TaxZ){
    /* Check for errors in park creation */
    if (pErrors(sys, Name, Max, TaxX, TaxY, TaxZ) == ERROR) return;
    
    /* Allocate memory for the new park */
    Park *newPark = malloc(sizeof(Park)); 
	if (newPark == NULL){
	    exit(0);
	}
    /* Allocate memory for the park name */
    newPark->name = malloc((strlen(Name) + 1) * sizeof(char));
	if (newPark->name == NULL){
	    free(newPark);
		exit(0);
    }
    
    /* Initialize park information */
    strcpy(newPark->name, Name);
    newPark->maxCapacity = Max;
    newPark->emptySpaces = Max;
    newPark->X = TaxX;
    newPark->Y = TaxY;
    newPark->Z = TaxZ;
    newPark->carList.head = NULL;
    newPark->carList.tail = NULL;
    newPark->movList.head = NULL;
    newPark->movList.tail = NULL;
    // Add the new park to the system
    sys->parkPtrArray[sys->createdParks++] = newPark;
}


void parkInformation(Sys *system){
    int parkIndex;
    /* Iterate over each park in the system */
    for (parkIndex= ZERO; parkIndex < system->createdParks; parkIndex++){
        printf("%s ", system->parkPtrArray[parkIndex]->name);
        printf("%d ", system->parkPtrArray[parkIndex]->maxCapacity);
        printf("%d\n", system->parkPtrArray[parkIndex]->emptySpaces);
    }
}

int pErrors(Sys *system, char *Name, int capacity, float x, float y, float z){
    int parkIndex;
    /* Check if the park name already exists */
    for (parkIndex = ZERO; parkIndex < system->createdParks; parkIndex++){
        if (strcmp(Name, system->parkPtrArray[parkIndex]->name) == ZERO){
            printf("%s: parking already exists.\n", Name);
            return ERROR;
        }
    }
    /* Check for invalid capacity */
    if (capacity <= ZERO) {
        printf("%d: invalid capacity.\n", capacity);
        return ERROR;
    }
    /* Check for invalid cost parameters */
    else if (x <= ZERO || y <= ZERO || z <= ZERO || !(x < y && y < z)) {
        printf("invalid cost.\n");
        return ERROR;
    }
    /* Check if the maximum number of parks has been reached */
    else if (system->createdParks >= MAX_PARKS) {
        printf("too many parks.\n");
        return ERROR;
    }
    /* No errors found */
    else {
        return ZERO;
    }
}
