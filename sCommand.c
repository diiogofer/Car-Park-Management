#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void sCommand(Sys *system){
    int day, month, year, hour, min;
    char matricula[9], name[MAX_BUFSIZ];
    if((sscanf(system->buffer, "s \"%[^\"]\" %s %d-%d-%d %d:%d", 
    name, matricula, &day, &month, &year, &hour, &min) == 7) || 
    (sscanf(system->buffer, "s %s %s %d-%d-%d %d:%d", 
    name, matricula, &day, &month, &year, &hour, &min) == 7)){
        Date exitDate = {year, month, day, hour, min};
        int parkPos = sErrors(system, name, matricula);
        if(parkPos == ERROR){return;}
        Entrie *sCarEntrie = removeCar(system, parkPos, matricula, &exitDate);
        if (sCarEntrie != NULL) {
            Date *entrie = &sCarEntrie->entrada;
            Exit *exit = addExit(system, matricula, parkPos, &exitDate);
            sCarEntrie->saida = exit;
            float paymentValue = payment(system, parkPos, entrie, &exitDate);
            exit->Payment = paymentValue;
            printf("%s %02d-%02d-%02d %02d:%02d %02d-%02d-%02d %02d:%02d %.2f\n"
            ,matricula, entrie->day, entrie->month, entrie->year, entrie->hour, 
            entrie->minute, day, month, year, hour, min, paymentValue);
        }
        else{return;}
    }
}

int sErrors(Sys *system, char *inputName, char *matricula){
    /* invalid park name */
    int parkPosition = findParkByName(system, inputName);
    if(parkPosition == ERROR){
        printf("%s: no such parking.\n", inputName);
        return ERROR;
    }

    /* Invalid licence plate */
    else if(validMatricula(matricula) == ERROR){
        printf("%s: invalid licence plate.\n", matricula); 
        return ERROR;
    }
    /* Other errors are verified in removeCar to evitar repeated loops */
    else{
        return parkPosition;
    }
}
Exit *addExit(Sys *sys, char *matricula, int parkPos, Date *saida) {
    if (parkPos != ERROR) {
        Exit *newExit = malloc(sizeof(Exit));
        if (newExit == NULL) {
            printf("Allocation Error.\n");
            exit(0);
        }
        strcpy(newExit->matricula, matricula);
        newExit->Payment = ZERO;
        newExit->saida = *saida;
        newExit->next = NULL;

        if (sys->parkPtrArray[parkPos]->FirstExit == NULL) {
            sys->parkPtrArray[parkPos]->FirstExit = newExit;
            sys->parkPtrArray[parkPos]->LastExit = newExit;
        } else {
            sys->parkPtrArray[parkPos]->LastExit->next = newExit;
            sys->parkPtrArray[parkPos]->LastExit = newExit;
        }
        return newExit;
    }
    return NULL;
}

Entrie *removeCar(Sys *system, int parkPos, char *matricula, Date *exit){
    Car *currentCar = system->parkPtrArray[parkPos]->FirstCar;
    Car *prevCar = NULL;
    Entrie *removedEntrie = NULL; //ponteiro para o movimento do parque a apagar
    // Procurar o carro com a matrícula especificada na lista de carros
    while (currentCar != NULL && strcmp(currentCar->carEntrie->matricula, matricula) != 0) {
        prevCar = currentCar;
        currentCar = currentCar->next;
    }
    // Se o carro for encontrado na lista
    if (currentCar != NULL) {
        // Verificar a data de saída do carro
        if (isValidDate(exit) != ERROR && isEarlier(system, exit) != ERROR){
            system->currentDate->day = exit->day;
            system->currentDate->month = exit->month;
            system->currentDate->year = exit->year;
            system->currentDate->hour = exit->hour;
            system->currentDate->minute = exit->minute;
            // Se o carro for o primeiro da lista
            if (prevCar == NULL) {
                system->parkPtrArray[parkPos]->FirstCar = currentCar->next;
            } 
            else {
                // Se o carro estiver no meio ou no final da lista
                prevCar->next = currentCar->next;
            }
            removedEntrie = currentCar->carEntrie;
            // Liberar a memória do carro removido
            free(currentCar);
            // Atualizar o contador de espaços vazios no parque
            (system->parkPtrArray[parkPos]->emptySpaces)++;
            // printf("%s removed from park %s\n", matricula, system->parkPtrArray[parkPos]->name);
        } 
        else {
            printf("invalid date.\n");
        }
    } 
    else {
        printf("%s: invalid vehicle exit.\n", matricula);
    }
    return removedEntrie;
}


int anoToMinutes(int *DaysMonthVec){
        int i, minutes = 0;
        for(i = 0; i < 12; i++){
                minutes += DaysMonthVec[i] * 1440;
        }
        return minutes;
}

int percorreAnos(int ComparisonYear, Date *date, int *DaysMonthVec){
    int minutes = 0;
    if(ComparisonYear == date->year){return minutes;}

    for(;ComparisonYear < date->year; ComparisonYear++){
        minutes += anoToMinutes(DaysMonthVec);
    }
        return minutes;
}

int percorreMeses(Date *date, int *DaysMonthVec){
    int i, minutos = 0;
    for(i = 0; i < date->month - 1; i++){
        minutos += DaysMonthVec[i] * 24 * 60;
    }
    return minutos;
}

int tempoEmMinutosFunc(Date *date, int ComparisonYear,int *DaysMonthVec){
    int tempoEmMinutos = 0;
    tempoEmMinutos = percorreAnos(ComparisonYear, date, DaysMonthVec);
    tempoEmMinutos += date->hour * 60 + date->minute;
    tempoEmMinutos += (date->day - 1) * 24 * 60;
    tempoEmMinutos += percorreMeses(date, DaysMonthVec);
    return tempoEmMinutos;
}

int datesDiff(Date *date1, Date *date2){
    int tempoEmMinutos1, tempoEmMinutos2;
    int DaysMonthVec[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int ComparisonYear = date1->year - 1;
    tempoEmMinutos1 = tempoEmMinutosFunc(date1, ComparisonYear, DaysMonthVec);
    tempoEmMinutos2 = tempoEmMinutosFunc(date2, ComparisonYear, DaysMonthVec);
    return tempoEmMinutos2 - tempoEmMinutos1;
}

float payment(Sys *system, int parkPos, Date *entrie, Date *exit){
    float X = system->parkPtrArray[parkPos]->X;
    float Y = system->parkPtrArray[parkPos]->Y;
    float Z = system->parkPtrArray[parkPos]->Z;
    int diffMinutes = datesDiff(entrie, exit);
    float totalPayment = 0.0;

    int days = diffMinutes / (24*60);
    int remainingMinutes = diffMinutes % (24*60);
    float expected15Payment = 0;
    int counter15min = 0;
    for(int i = 0; i < remainingMinutes; i+=15){
        if(counter15min < 4){
            expected15Payment += X;
        }
        else{expected15Payment += Y;}
        counter15min++;
    }
    if(expected15Payment > Z){
        totalPayment += Z;
    }
    else{totalPayment += expected15Payment;}
    totalPayment += Z * days;
return totalPayment;
}



void printAllExits(Sys *system) {
    for (int i = 0; i < system->createdParks; i++) {
        printf("Parque %s:\n", system->parkPtrArray[i]->name);
        printExitsInPark(system->parkPtrArray[i]);
    }
}

void printExitsInPark(Park *park) {
    Exit *currentExit = park->FirstExit;
    float totalPayment = 0.0;

    while (currentExit != NULL) {
        printf("- Matrícula: %s\n", currentExit->matricula);
        printf("  Data de saída: %02d-%02d-%02d %02d:%02d\n", 
               currentExit->saida.day, currentExit->saida.month, currentExit->saida.year, 
               currentExit->saida.hour, currentExit->saida.minute);
        printf("  Pagamento: %.2f\n", currentExit->Payment);

        totalPayment += currentExit->Payment;

        currentExit = currentExit->next;
    }

    printf("Total Payment do Parque %s: %.2f\n\n", park->name, totalPayment);
}





