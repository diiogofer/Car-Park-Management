#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void insertion(Park *array[], int left, int right) {
    int currentI, prevI;
    /* Iterate through the array */
    for (currentI = left + 1; currentI <= right; currentI++){
        /* Store the current element */
        Park *current = array[currentI];
        prevI = currentI - 1;
        /* Shift elements if necessary */
        while (prevI >= left && strcmp(current->name, array[prevI]->name) < 0){
            array[prevI + 1] = array[prevI];
            prevI--;
        }
        /* Insert the current element into its correct position */
        array[prevI + 1] = current;
    }
}
void printCarMovByPlate(Sys *system, Park **parkPtrArray, char *license){
    int totalEntriesCounter = ZERO;
    for (int iter = ZERO; iter < system->createdParks; iter++){
        Mov *currentMov = parkPtrArray[iter]->movList.head;
        int entriesCounter = ZERO, exitsCounter = ZERO;
        while(currentMov != NULL){ /* Iterate through the movement list */
            if(strcmp(currentMov->license, license) == ZERO){
                if(currentMov->identifier == 'e'){ /* entry movements */
                    printf("%s %02d-%02d-%02d %02d:%02d", 
                    parkPtrArray[iter]->name, currentMov->movDate.day, 
                    currentMov->movDate.month, currentMov->movDate.year, 
                    currentMov->movDate.hour, currentMov->movDate.minute);
                    entriesCounter++ ,totalEntriesCounter++;
                } 
                else if(currentMov->identifier == 's'){ /* exit movements */
                    printf(" %02d-%02d-%02d %02d:%02d\n", 
                    currentMov->movDate.day, currentMov->movDate.month, 
                    currentMov->movDate.year, currentMov->movDate.hour, 
                    currentMov->movDate.minute);
                    exitsCounter++;
                }
            }
            currentMov = currentMov->next;
        } /* If the number of entries does not match the number of exits */
        if(entriesCounter != exitsCounter) printf("\n");
    }
    if(totalEntriesCounter == ZERO){
        printf("%s: no entries found in any parking.\n", license);
    }
}

void vCommand(Sys *system) {
    char matricula[9];
    
    /* Read the license plate from the buffer */
    if (sscanf(system->buffer, "v %s", matricula) == 1 &&
        validLicensePlate(matricula) != ERROR){
        /* Create a sorted copy of the park pointer array */
        Park *tempParkPtrArray[system->createdParks];
        for (int i = 0; i < system->createdParks; i++) {
            tempParkPtrArray[i] = system->parkPtrArray[i];
        }
        insertion(tempParkPtrArray, ZERO, system->createdParks - 1);

        /* Print the movements of the car sorted alphabetically by park names */
        printCarMovByPlate(system, tempParkPtrArray, matricula);
    }
    /* Invalid license plate, print error message */
    else{printf("%s: invalid licence plate.\n", matricula);}
}
