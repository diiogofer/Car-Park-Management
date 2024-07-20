/**
 * @file system.c
 * @brief Implementation file for system-related functions.
 * @author Diogo Fernandes (diiogofer - https://github.com/diiogofer)
 */

#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Creates a new system struct.
 * 
 * Initializes a system struct with the provided parameters.
 * 
 * @param buffer The buffer for input/output operations.
 * @param parksPtr Array of pointers to Park structs.
 * @param numParks Number of parks in the system.
 * @param actualDate Pointer to the current date.
 * @return The newly created system struct.
 */
Sys sysCreator(char *buffer, Park **parksPtr, int numParks, Date *actualDate){
    /* Initialize a system struct and return it*/
    Sys system = {buffer, parksPtr, numParks, actualDate};
    return system;
}


/**
 * @brief Updates the current date in the system.
 * 
 * Updates the current date in the system with the provided date.
 * 
 * @param system Pointer to the system struct.
 * @param entry Pointer to the new date to be set.
 */
void updateDate(Sys *system, Date *entry) {
    system->currentDate->year = entry->year;
    system->currentDate->month = entry->month;
    system->currentDate->day = entry->day;
    system->currentDate->hour = entry->hour;
    system->currentDate->minute = entry->minute;
}

/**
 * @brief Frees memory allocated for all parks in the system.
 * 
 * Iterates through each park in the system and frees memory allocated for it.
 * 
 * @param system Pointer to the system struct.
 */
void freeAllParks(Sys *system) {
    /* Iterate through each park in the system */
    for (int i = 0; i < system->createdParks; i++) {
        Park *park = system->parkPtrArray[i];
        if (park != NULL) {
            /* Free memory for the current park */
            freePark(system, i);
        }
    }
}

/**
 * @brief Frees memory allocated for a specific park in the system.
 * 
 * Frees memory allocated for a specific park, including its cars and movements.
 * 
 * @param system Pointer to the system struct.
 * @param parkPos Position of the park in the park array.
 */
void freePark(Sys *system, int parkPos) {
    Park *park = system->parkPtrArray[parkPos];

    if (park != NULL) {
        /* Free memory for each car in the car list */
        Car *currentCar = park->carList.head;
        while (currentCar != NULL) {
            Car *nextCar = currentCar->next;
            free(currentCar);
            currentCar = nextCar;
        }

        /* Free memory for each movement in the movement list */
        Mov *currentMov = park->movList.head;
        while (currentMov != NULL) {
            Mov *nextMov = currentMov->next;
            free(currentMov);
            currentMov = nextMov;
        }

        /* Free memory for the park's name */
        free(park->name);

        /* Free memory for the park itself */
        free(park);

        /* Set the park pointer to NULL */
        system->parkPtrArray[parkPos] = NULL;
    }
}

