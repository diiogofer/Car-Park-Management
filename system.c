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
    for (int i = 0; i < system->createdParks; i++) {
        Park *park = system->parkPtrArray[i];
        if (park != NULL) {
            freePark(system, i);
        }
    }
}


void freePark(Sys *system, int parkPos) {
    Park *park = system->parkPtrArray[parkPos];

    if (park != NULL) {
        // free da lista de carros
        Car *currentCar = park->carList.head;
        while (currentCar != NULL) {
            Car *nextCar = currentCar->next;
            free(currentCar);
            currentCar = nextCar;
        }

        // free da lista de movimentos
        Mov *currentMov = park->movList.head;
        while (currentMov != NULL) {
            Mov *nextMov = currentMov->next;
            free(currentMov);
            currentMov = nextMov;
        }

        // Libera o nome do parque
        free(park->name);

        // Libera o próprio parque
        free(park);

        // Define o ponteiro do parque como NULL para evitar acesso acidental
        system->parkPtrArray[parkPos] = NULL;
    }
}

