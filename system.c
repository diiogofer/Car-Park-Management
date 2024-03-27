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

