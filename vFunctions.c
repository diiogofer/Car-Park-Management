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
// Função auxiliar para imprimir o movimento do carro
void printCarMovement(const char *parkName, Date *entrada, Date *saida) {
    if (saida->day != 0 || saida->month != 0 || saida->year != 0 ||
        saida->hour != 0 || saida->minute != 0) {
        printf("%s %02d-%02d-%02d %02d:%02d %02d-%02d-%02d %02d:%02d\n",
               parkName, entrada->day, entrada->month, entrada->year,
               entrada->hour, entrada->minute, saida->day, saida->month,
               saida->year, saida->hour, saida->minute);
    } else {
        printf("%s %02d-%02d-%02d %02d:%02d\n",
               parkName, entrada->day, entrada->month, entrada->year,
               entrada->hour, entrada->minute);
    }
}

// Função para imprimir os movimentos do carro por matrícula
void printCarMovementsByPlate(Sys *system, Park **parkPtrArray, char *matricula){
    int entriesCounter = 0;
    for (int i = 0; i < system->createdParks; i++) {
        Movement *currentMovement = parkPtrArray[i]->FirstMoveOfList;
        while (currentMovement != NULL) {
            if (strcmp(currentMovement->matricula, matricula) == 0) {
                printCarMovement(parkPtrArray[i]->name,
                                 &currentMovement->entrada,
                                 &currentMovement->saida);
                entriesCounter++;
            }
            currentMovement = currentMovement->next;
        }
    }
    if (entriesCounter == 0) {
        printf("%s: no entries found in any parking.\n", matricula);
    }
}


void vCommand(Sys *system) {
    char matricula[9];
    
    // Lê a matrícula do buffer
    if (sscanf(system->buffer, "v %s", matricula) == 1 &&
        validMatricula(matricula) != -1){
    // Cria uma cópia ordenada dos ponteiros do vetor original
    Park *tempParkPtrArray[system->createdParks];
    for (int i = 0; i < system->createdParks; i++) {
        tempParkPtrArray[i] = system->parkPtrArray[i];
    }
    insertion(tempParkPtrArray, ZERO, system->createdParks - 1);

    // Imprime os movimentos do carro por ordem alfabética dos parques
    printCarMovementsByPlate(system, tempParkPtrArray, matricula);
    }
    else{printf("%s: invalid licence plate.\n", matricula);}
}

