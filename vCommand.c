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
    int entriesCounter = 0;
    
    for (int i = 0; i < system->createdParks; i++) {
        Entrie *currentEntrie = parkPtrArray[i]->FirstEntrie;
        
        while (currentEntrie != NULL) {
            if (strcmp(currentEntrie->matricula, matricula) == ZERO) {
                printf("%s ", parkPtrArray[i]->name);
                printf("%02d-%02d-%02d %02d:%02d", 
                       currentEntrie->entrada.day, currentEntrie->entrada.month,
                       currentEntrie->entrada.year, currentEntrie->entrada.hour,
                       currentEntrie->entrada.minute);

                // Se houver uma saída associada
                if (currentEntrie->saida != NULL) {
                    printf(" %02d-%02d-%02d %02d:%02d", 
                           currentEntrie->saida->saida.day, 
                           currentEntrie->saida->saida.month, 
                           currentEntrie->saida->saida.year,
                           currentEntrie->saida->saida.hour, 
                           currentEntrie->saida->saida.minute);
                }
                printf("\n");
                entriesCounter++;
            }
            currentEntrie = currentEntrie->next;
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