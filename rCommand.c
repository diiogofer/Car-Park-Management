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
                for (int iter = ZERO; iter < system->createdParks; iter++) {
                tempParkPtrArray[iter] = system->parkPtrArray[iter];
                }
                insertion(tempParkPtrArray, ZERO, system->createdParks - 1);
                for (int iter = ZERO; iter < system->createdParks; iter++){
                    printf("%s\n", tempParkPtrArray[iter]->name);
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
    int newIndex = ZERO;
    for (int iter = ZERO; iter < system->createdParks; iter++){
        if (system->parkPtrArray[iter] != NULL){
            newArray[newIndex] = system->parkPtrArray[iter];
            newIndex++;
        }
    }
    // Preenche as posições restantes do novo vetor com NULL
    for (int iter = newIndex; iter < MAX_PARKS; iter++){
        newArray[iter] = NULL;
    }
    // Substitui o vetor parkPtrArray pelo novo vetor temporário
    for (int iter = ZERO; iter < MAX_PARKS; iter++){
        system->parkPtrArray[iter] = newArray[iter];
    }
}





