#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void rCommand(Sys *system){
    char name[MAX_BUFSIZ];
    if(sscanf(system->buffer, "r \"%[^\"]\"", name) == 1 ||
        sscanf(system->buffer, "r %s", name) == 1){
            int ParkPos = findParkByName(system, name);
            if(ParkPos != ERROR){
                /*fazer free do parque e reorganizar o vetor de sistema*/
                freePark(system, ParkPos);
                organizeParkPtrArray(system);
                (system->createdParks)--;
                // Cria uma cópia ordenada dos ponteiros do vetor original
                Park *tempParkPtrArray[system->createdParks];
                for (int i = 0; i < system->createdParks; i++) {
                tempParkPtrArray[i] = system->parkPtrArray[i];
                }
                insertion(tempParkPtrArray, ZERO, system->createdParks - 1);
                for (int i = 0; i < system->createdParks; i++){
                    printf("%s\n", tempParkPtrArray[i]->name);
                }
                }
            else{
                printf("%s: no such parking.\n", name);
            }
    }
}

void organizeParkPtrArray(Sys *system) {
    /* Novo vetor de ponteiros para substituir o antigo */
    Park *newArray[MAX_PARKS];
    int newIndex = 0;
    for (int i = 0; i < system->createdParks; i++) {
        if (system->parkPtrArray[i] != NULL) {
            newArray[newIndex] = system->parkPtrArray[i];
            newIndex++;
        }
    }
    // Preenche as posições restantes do novo vetor com NULL
    for (int i = newIndex; i < MAX_PARKS; i++) {
        newArray[i] = NULL;
    }
    // Substitui o vetor parkPtrArray pelo novo vetor temporário
    for (int i = 0; i < MAX_PARKS; i++) {
        system->parkPtrArray[i] = newArray[i];
    }
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

