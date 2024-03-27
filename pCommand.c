/**
 * @file Command.c
 * @brief Implementation file for command C related functions.
 * @author Diogo Fernandes - ist1110306
 */

#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Handles the 'p' command.
 * 
 * Parses the input buffer to extract park information and adds a new park to 
 * the system or displays information about existing parks.
 * 
 * @param system Pointer to the system struct.
 */
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

/**
 * @brief Adds a new park to the system.
 * 
 * Creates a new park with the provided parameters and adds it to the 
 * system's park list.
 * 
 * @param sys Pointer to the system struct.
 * @param Name Name of the park.
 * @param Max Maximum capacity of the park.
 * @param TaxX Price per 15 minutes within the first hour.
 * @param TaxY Price per 15 minutes after the first hour.
 * @param TaxZ Maximum daily price (24 hours).
 */
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
    /* Add the new park to the system */ 
    sys->parkPtrArray[sys->createdParks++] = newPark;
}

/**
 * @brief Displays information about all parks in the system.
 * 
 * Iterates through each park in the system and prints its name, maximum 
 * capacity, and empty spaces.
 * 
 * @param system Pointer to the system struct.
 */
void parkInformation(Sys *system){
    int parkIndex;
    /* Iterate over each park in the system */
    for (parkIndex= ZERO; parkIndex < system->createdParks; parkIndex++){
        printf("%s ", system->parkPtrArray[parkIndex]->name);
        printf("%d ", system->parkPtrArray[parkIndex]->maxCapacity);
        printf("%d\n", system->parkPtrArray[parkIndex]->emptySpaces);
    }
}

/**
 * @brief Checks for errors in park creation.
 * 
 * Checks for errors such as duplicate park names, invalid capacity, 
 * invalid tax values and if the MAX_PARKS value is reached.
 * 
 * @param system Pointer to the system struct.
 * @param Name Name of the park.
 * @param capacity Capacity of the park.
 * @param x Cost parameter for the first 15 minutes of parking.
 * @param y Cost parameter for each additional 15 minutes after the first hour.
 * @param z Maximum daily cost (24 hours).
 * @return Returns ERROR if any errors are found, otherwise returns ZERO.
 */
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
