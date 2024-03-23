#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Função de ordenação por inserção para strings
void insertion(Park *a[], int l, int r) {
    int i, j;
    for (i = l + 1; i <= r; i++) {
        Park *v = a[i];
        j = i - 1;
        while (j >= l && strcmp(v->name, a[j]->name) < 0) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = v;
    }
}
void printCarMovByPlate(Sys *system, Park **parkPtrArray, char *matricula) {
    int totalEntriesCounter = 0;
    for (int iter = 0; iter < system->createdParks; iter++) {
        Mov *currentMov = parkPtrArray[iter]->movList.head;
        int entriesCounter = 0, exitsCounter = 0;
        while (currentMov != NULL) {
            if (strcmp(currentMov->license, matricula) == 0) {
                if (currentMov->identifier == 'e') {
                    printf("%s ", parkPtrArray[iter]->name);
                    printf("%02d-%02d-%02d %02d:%02d", 
                           currentMov->movDate.day, currentMov->movDate.month,
                           currentMov->movDate.year, currentMov->movDate.hour,
                           currentMov->movDate.minute);
                    entriesCounter++;
                    totalEntriesCounter++;
                } 
                else if (currentMov->identifier == 's') {
                    printf(" %02d-%02d-%02d %02d:%02d\n", 
                           currentMov->movDate.day, currentMov->movDate.month, 
                           currentMov->movDate.year, currentMov->movDate.hour, 
                           currentMov->movDate.minute);
                    exitsCounter++;
                }
            }
            currentMov = currentMov->next;
        }
        if(entriesCounter != exitsCounter){printf("\n");}
    }
    if (totalEntriesCounter == 0) {
        printf("%s: no entries found in any parking.\n", matricula);
    }
}

void vCommand(Sys *system) {
    char matricula[9];
    
    // Lê a matrícula do buffer
    if (sscanf(system->buffer, "v %s", matricula) == 1 &&
        validMatricula(matricula) != ERROR){
        // Cria uma cópia ordenada dos ponteiros do vetor original
        Park *tempParkPtrArray[system->createdParks];
        for (int i = 0; i < system->createdParks; i++) {
            tempParkPtrArray[i] = system->parkPtrArray[i];
        }
        insertion(tempParkPtrArray, ZERO, system->createdParks - 1);

        // Imprime os movimentos do carro por ordem alfabética dos parques
        printCarMovByPlate(system, tempParkPtrArray, matricula);
    }
    else{printf("%s: invalid licence plate.\n", matricula);}
}
