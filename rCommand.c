/**
 * @file rCommand.c
 * @brief Implementation of functions related to the "r" command.
 * @author Diogo Fernandes - ist1110306
 */

#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Executes the "r" command to remove a parking.
 * 
 * @param system Pointer to the system structure.
 */
void rCommand(Sys *system){
    char name[MAX_BUFSIZ];
    if(sscanf(system->buffer, "r \"%[^\"]\"", name) == 1 ||
        sscanf(system->buffer, "r %s", name) == 1){
        int ParkPos = findParkByName(system, name);
        if(ParkPos != ERROR){
            /* Free the park, reorganize the system's park pointer array */
            freePark(system, ParkPos);
            organizeParkPtrArray(system);
            (system->createdParks)--;
            /* Create an ordered copy of the park pointer array */
            Park *tempParkPtrArray[system->createdParks];
            for (int iter = ZERO; iter < system->createdParks; iter++) {
            tempParkPtrArray[iter] = system->parkPtrArray[iter];
            }
            insertion(tempParkPtrArray, ZERO, system->createdParks - 1);
            /* Print the names of the remaining parks */
            for (int iter = ZERO; iter < system->createdParks; iter++){
                printf("%s\n", tempParkPtrArray[iter]->name);
            }
            }
        else{
            printf("%s: no such parking.\n", name);
        }
    }
}

/**
 * @brief Organizes the park pointer array by removing NULL pointers.
 * 
 * @param system Pointer to the system structure.
 */
void organizeParkPtrArray(Sys *system) {
    /* New pointer array to replace the old one */
    Park *newArray[MAX_PARKS];
    int newIndex = ZERO;

    /* Copy non-null pointers from the original array to the new array */
    for (int iter = ZERO; iter < system->createdParks; iter++){
        if (system->parkPtrArray[iter] != NULL){
            newArray[newIndex] = system->parkPtrArray[iter];
            newIndex++;
        }
    }
    /* Fill remaining positions in the new array with NULL */
    for (int iter = newIndex; iter < MAX_PARKS; iter++){
        newArray[iter] = NULL;
    }
    /* Replace the parkPtrArray with the new temporary array */
    for (int iter = ZERO; iter < MAX_PARKS; iter++){
        system->parkPtrArray[iter] = newArray[iter];
    }
}





