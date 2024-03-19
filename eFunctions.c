#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void eCommand(Sys *system){
    int day, month, year, hour, min;
    char matricula[9], nome[MAX_BUFSIZ];
    Car *car;
    Movement *mov;
    if(sscanf(system->buffer, "e \"%[^\"]\" %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7 ){
        Date entrada = { year, month, day, hour, min};
        int parkPos = eErrors(system, nome, matricula, &entrada);
        if(parkPos != -1){
            car = addCarToPark(system, matricula, parkPos);
            Date saida = {ZERO, ZERO, ZERO, ZERO, ZERO};
            mov = addMov(system, matricula, parkPos, &entrada, &saida);
            eChanges(system, car, mov, &entrada, parkPos);
        }
    }
    else if(sscanf(system->buffer, "e %s %s %d-%d-%d %d:%d", 
    nome, matricula, &day, &month, &year, &hour, &min) == 7){
        Date entrada = { year, month, day, hour, min};
        int parkPos = eErrors(system, nome, matricula, &entrada);
        if(parkPos != -1){
            car = addCarToPark(system, matricula, parkPos);
            Date saida = {ZERO, ZERO, ZERO, ZERO, ZERO};
            mov = addMov(system, matricula, parkPos, &entrada, &saida);
            eChanges(system, car, mov, &entrada, parkPos);
        }
    }
    else{return;}
}

int eErrors(Sys *system, char *inputName, char *matricula, Date *date){
    /* invalid park name */
    int parkPosition = findParkByName(system, inputName);
    if(parkPosition == -1){
        printf("%s: no such parking.\n", inputName);
        return -1;
    }
    /* parking is full */
    else if(system->parkPtrArray[parkPosition]->emptySpaces == 0){
        printf("%s: parking is full.\n", inputName);
        return -1;
    }
    /* Invalid license plate */
    else if(validMatricula(matricula) == -1){
        printf("%s: invalid licence plate.\n", matricula); 
        return -1;
    }
    /* Caso onde veiculo está dentro de um parque */
    else if(searchMatricula(system, matricula) == -1){
        printf("%s: invalid vehicle entry.\n", matricula);
        return -1;
    }

    /* invalid date */
    else if (dateChecker(system, date) == -1){
        printf("invalid date.\n");
        return -1;
    }
    else{
        return parkPosition;
    }
}

Car *addCarToPark(Sys *system, char *matricula, int parkPos){
    /* Creating the car */
    Car *newCar = malloc(sizeof(Car));
    if (newCar == NULL) {
        printf("Allocation Error.\n");
        exit(0);
        }
    strcpy(newCar->matricula, matricula);
    newCar->carMove = NULL;
    newCar->next = NULL;
    /* Adding the car to the park */
    if(system->parkPtrArray[parkPos]->FirstCarOfList == NULL){
        system->parkPtrArray[parkPos]->FirstCarOfList = newCar;
        system->parkPtrArray[parkPos]->LastAddedCar = newCar;
    }
    else{
        system->parkPtrArray[parkPos]->LastAddedCar->next = newCar;
        system->parkPtrArray[parkPos]->LastAddedCar = newCar;
    }
    return newCar;
}

Movement *addMov(Sys *sys, char *matricula, int parkPos, Date *entrada, Date *saida){
    if(parkPos != -1){
        Movement *newMovement = malloc(sizeof(Movement));
        if (newMovement == NULL) {
            printf("Allocation Error\n");
            exit(0);
        }
        strcpy(newMovement->matricula, matricula);
        newMovement->entrada = *entrada;
        newMovement->saida = *saida;
        newMovement->next = NULL;

        if (sys->parkPtrArray[parkPos]->FirstMoveOfList == NULL) {
            /*Atualiza o ponteiro que aponta para o primeiro parque */
            sys->parkPtrArray[parkPos]->FirstMoveOfList = newMovement;
            /* O ponteiro que aponta para o ulitmo parque = ao do primeiro */
            sys->parkPtrArray[parkPos]->LastAddedMovement = newMovement;
        } 
        else {
            sys->parkPtrArray[parkPos]->LastAddedMovement->next = newMovement;
            sys->parkPtrArray[parkPos]->LastAddedMovement = newMovement;
        }
        return newMovement;
    }
    return NULL;
}


void eChanges(Sys *system, Car *car, Movement *mov, Date *date, int parkPos){
    /* Associate Movement To Car*/
    car->carMove = mov;
    /* Changing system last date */
    system->currentDate->day = date->day;
    system->currentDate->month = date->month;
    system->currentDate->year = date->year;
    system->currentDate->hour = date->hour;
    system->currentDate->minute = date->minute;
    /* Decrementing counters in parks */
    Park *currentPark = system->parkPtrArray[parkPos];
    (currentPark->emptySpaces)--;

    /* Printing park information */
    printf("%s %d\n", currentPark->name, currentPark->emptySpaces);
}


int findParkByName(Sys *system , char *inputName){
    int iter = 0;
    while(iter < system->createdParks){
        if(strcmp(system->parkPtrArray[iter]->name, inputName) == 0)
            return iter;
        else{
            iter++;
        }
    }
    return -1;
}


void printAllMovements(Sys *system) {
    for (int i = 0; i < system->createdParks; i++) {
        printMovementsInPark(system->parkPtrArray[i]);
    }
}

// int validMatricula(char *matricula) {
//     char letra1, letra2, letra3, letra4;
//     int num1, num2;
//     // Verifica o formato "XX-99-XX"
//     if (sscanf(matricula, "%c%c-%02d-%c%c", 
//         &letra1, &letra2, &num1, &letra3, &letra4) == 5){
//         if (isupper(letra1) && isupper(letra2) && 
//             isupper(letra3) && isupper(letra4)
//             && num1 >= 10 && num1 <= 99) {
//                 return 1; // Matrícula válida
//         }
//     }
//     // Verifica o formato "99-XX-99"
//     if(sscanf(matricula, "%02d-%c%c-%02d", &num1, &letra1, &letra2, &num2) == 4){
//         if (isupper(letra1) && isupper(letra2) && num2 >= 10 && num2 <= 99) {
//             return 1; // Matrícula válida
//         }
//     }

//     return -1; // Matrícula inválida
// }
int validMatricula(char *matricula){
    /* "AA-11-BB\0" 
    11-AA-11
    11-11-AA
    AA-AA-11 */
    int i;
    int charPairCounter = ZERO;
    int numberPairCounter = ZERO;
    int hyphen1Pos = 2, hyphen2Pos = 5; 
    if(matricula[hyphen1Pos] != '-' || matricula[hyphen2Pos] != '-'){
        return -1;
    }
    for(i = ZERO; i <= hyphen2Pos+1; i+=(hyphen1Pos+1)){
        int j = i;
        if(isupper(matricula[j])){
            if(isupper(matricula[j+1]))
                charPairCounter++;
            else{ return -1;}
        }
        else if( isdigit(matricula[j])){
            if(isdigit(matricula[j+1])){
                numberPairCounter++;
            }
            else{return -1;}
        }
        else{return -1;}
    }
    if((numberPairCounter == 1 && charPairCounter == 2) ||
        (numberPairCounter == 2 && charPairCounter == 1)){
        return 1;
        }
    else{return -1;}  
}

int isValidDate(Date *date) {
    /* Static vector with month days */
    int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int validDate = 1;
    if (date->year < 0 || date->year > 9999) validDate = -1;
    if (date->month < 1 || date->month > 12) validDate = -1;
    if (date->day < 1 || date->day > monthDays[(date->month)-1]) validDate = -1;
    if (date->hour < 0 || date->hour > 23) validDate = -1;
    if (date->minute < 0 || date->minute > 59) validDate = -1;
    return validDate;
}

int isEarlier(Sys *system, Date *date2) {

    if (system->currentDate->year < date2->year) return 1;
    else if (system->currentDate->year > date2->year) return -1;

    if (system->currentDate->month < date2->month) return 1;
    else if (system->currentDate->month > date2->month) return -1;

    else if (system->currentDate->day < date2->day) return 1;
    if (system->currentDate->day > date2->day) return -1;

    if (system->currentDate->hour < date2->hour) return 1;
    else if (system->currentDate->hour > date2->hour) return -1;

    if (system->currentDate->minute < date2->minute) return 1;
    else if (system->currentDate->minute > date2->minute) return -1;
    
    return 0;
}

int dateChecker(Sys *system, Date *date) {
    if (isValidDate(date) != -1 && isEarlier(system, date) != -1) {
        return 1;
    }
    else {
        return -1;
    }
}

void printCarsInParks(Sys *system) {
    for (int i = 0; i < system->createdParks; i++) {
        printf("Carros no Parque %s:\n", system->parkPtrArray[i]->name);
        Car *currentCar = system->parkPtrArray[i]->FirstCarOfList;
        while (currentCar != NULL) {
            printf("- Matrícula: %s\n", currentCar->matricula);
            currentCar = currentCar->next;
        }
        printf("\n");
    }
}

void printMovementsInPark(Park *park) {
    printf("Movimentos no Parque %s:\n", park->name);
    Movement *currentMovement = park->FirstMoveOfList;
    while (currentMovement != NULL) {
        printf("- Matrícula: %s\n", currentMovement->matricula);
        printf("  Data de entrada: %02d-%02d-%04d %02d:%02d\n", 
               currentMovement->entrada.day, currentMovement->entrada.month, 
               currentMovement->entrada.year, currentMovement->entrada.hour, 
               currentMovement->entrada.minute);
        printf("  Data de saída: %02d-%02d-%04d %02d:%02d\n", 
               currentMovement->saida.day, currentMovement->saida.month, 
               currentMovement->saida.year, currentMovement->saida.hour, 
               currentMovement->saida.minute);
        currentMovement = currentMovement->next;
    }
    printf("\n");
}

int searchMatricula(Sys *system, char *matricula) {
    for (int iter = 0; iter < system->createdParks; iter++) {
        Car *currentCar = system->parkPtrArray[iter]->FirstCarOfList;
        while (currentCar != NULL) {
            if (strcmp(currentCar->carMove->matricula, matricula) == 0) {
                return -1;
            }
            currentCar = currentCar->next;
        }
    }
    return 1;
}

void sCommand(Sys *system){
    int day, month, year, hour, min;
    char matricula[9], name[MAX_BUFSIZ];
    if(sscanf(system->buffer, "s \"%[^\"]\" %s %d-%d-%d %d:%d", 
    name, matricula, &day, &month, &year, &hour, &min) == 7 ){
        Date exitDate = {year, month, day, hour, min};
        int parkPos = sErrors(system, name, matricula);
        if(parkPos == -1){return;}
        Movement *sCarMov = removeCarFromPark(system, parkPos, matricula, &exitDate);
        if (sCarMov != NULL) {
            Date *entrie = &sCarMov->entrada; // Obter a data de entrada do movimento removido
            float paymentValue = payment(system, parkPos, entrie, &exitDate);
     
            printf("%s %02d-%02d-%02d %02d:%02d %02d-%02d-%02d %02d:%02d %.2f\n",
            matricula, entrie->day, entrie->month, entrie->year, entrie->hour, 
            entrie->minute, day, month, year, hour, min, paymentValue);
        }
        else{return;}
    }
    else if(sscanf(system->buffer, "s %s %s %d-%d-%d %d:%d", 
    name, matricula, &day, &month, &year, &hour, &min) == 7 ){
        Date exitDate = {year, month, day, hour, min};
        int parkPos = sErrors(system, name, matricula);
        if(parkPos == -1){return;}
        Movement *sCarMov = removeCarFromPark(system, parkPos, matricula, &exitDate);
        if (sCarMov != NULL) {
            Date *entrie = &sCarMov->entrada; // Obter a data de entrada do movimento removido
            float paymentValue = payment(system, parkPos, entrie, &exitDate);
            printf("%s %02d-%02d-%02d %02d:%02d %02d-%02d-%02d %02d:%02d %.2f\n",
            matricula, entrie->day, entrie->month, entrie->year, entrie->hour, 
            entrie->minute, day, month, year, hour, min, paymentValue);
        }
        else{return;}
    }
}

int sErrors(Sys *system, char *inputName, char *matricula){
    /* invalid park name */
    int parkPosition = findParkByName(system, inputName);
    if(parkPosition == -1){
        printf("%s: no such parking.\n", inputName);
        return -1;
    }

    /* Invalid license plate */
    else if(validMatricula(matricula) == -1){
        printf("%s: invalid licence plate.\n", matricula); 
        return -1;
    }
    /* Other errors are verified in removeCar to evitar repeated loops */
    else{
        return parkPosition;
    }
}


Movement *removeCarFromPark(Sys *system, int parkPos, char *matricula, Date *exit) {
    Car *currentCar = system->parkPtrArray[parkPos]->FirstCarOfList;
    Car *prevCar = NULL;
    Movement *removedMovement = NULL; //ponteiro para o movimento do parque a apagar
    // Procurar o carro com a matrícula especificada na lista de carros
    while (currentCar != NULL && strcmp(currentCar->carMove->matricula, matricula) != 0) {
        prevCar = currentCar;
        currentCar = currentCar->next;
    }
    // Se o carro for encontrado na lista
    if (currentCar != NULL) {
        // Verificar a data de saída do carro
        if (dateChecker(system, exit) != -1) {
            system->currentDate->day = exit->day;
            system->currentDate->month = exit->month;
            system->currentDate->year = exit->year;
            system->currentDate->hour = exit->hour;
            system->currentDate->minute = exit->minute;
            // Atualizar a data de saída do movimento associado ao carro
            currentCar->carMove->saida.day = exit->day;
            currentCar->carMove->saida.month = exit->month;
            currentCar->carMove->saida.year = exit->year;
            currentCar->carMove->saida.hour = exit->hour;
            currentCar->carMove->saida.minute = exit->minute;
            // Se o carro for o primeiro da lista
            if (prevCar == NULL) {
                system->parkPtrArray[parkPos]->FirstCarOfList = currentCar->next;
            } 
            else {
                // Se o carro estiver no meio ou no final da lista
                prevCar->next = currentCar->next;
            }
            removedMovement = currentCar->carMove;
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
    return removedMovement;
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
