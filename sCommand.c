#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void sCommand(Sys *system){
    int day, month, year, hour, min;
    char license[9], name[MAX_BUFSIZ];
    if((sscanf(system->buffer, "s \"%[^\"]\" %s %d-%d-%d %d:%d", 
    name, license, &day, &month, &year, &hour, &min) == 7) || 
    (sscanf(system->buffer, "s %s %s %d-%d-%d %d:%d", 
    name, license, &day, &month, &year, &hour, &min) == 7)){

        Date exitDate = {year, month, day, hour, min};
        int parkPos = findParkByName(system, name);
        Car *carToRemove = sErrors(system, parkPos, name, license, &exitDate);
        if(carToRemove == NULL){return;}
        
        Mov *associatedEntrie = removeCar(system, parkPos, carToRemove);
        Date entryDate = associatedEntrie->movDate;
        Park *park= system->parkPtrArray[parkPos]; 
        float paymentValue = ZERO;
        AddMovtoList(park, 's', license, &exitDate);
        paymentValue = payment(system, parkPos, &entryDate, &exitDate);
        park->movList.tail->payment = paymentValue;
        printf("%s %02d-%02d-%02d %02d:%02d %02d-%02d-%02d %02d:%02d %.2f\n",
        license, entryDate.day, entryDate.month, entryDate.year, 
        entryDate.hour, entryDate.minute, day, month, year, hour, min, 
        paymentValue);
        (park->emptySpaces)++;
        updateDate(system, &exitDate);
    }
    else{return;}
}



Car *findCarInPark(Park *park, char *license) {
    Car *currentCar = park->carList.head;
    while (currentCar != NULL) {
        if (strcmp(currentCar->carEntry->license, license) == 0) {
            return currentCar; // Retorna o ponteiro para o carro encontrado
        }
        currentCar = currentCar->next;
    }
    return NULL; // Retorna NULL se o carro não for encontrado no parque
}


// Função para verificar se há erros durante a saída do veículo
Car *sErrors(Sys *system, int ParkPos, char *name, char *license, Date *exit) {
    Park *park = system->parkPtrArray[ParkPos];
    /* Invalid park name */
    if (ParkPos == ERROR) {
        printf("%s: no such parking.\n", name);
        return NULL;
    }
    /* Invalid licence plate */
    else if (validLicensePlate(license) == ERROR) {
        printf("%s: invalid licence plate.\n", license); 
        return NULL;
    }
    /* Check if the car is in the park */
    Car *CarToRemove = findCarInPark(park, license);
    if (CarToRemove == NULL) {
        printf("%s: invalid vehicle exit.\n", license);
        return NULL;
    }
    /* Invalid date */
    if (isValidDate(exit) != SUCCESS || isEarlier(system, exit) != SUCCESS) {
        printf("invalid date.\n");
        return NULL;
    }
    /* Return pointer to the car */
    return CarToRemove;
}

Mov *removeCar(Sys *system, int parkPosition, Car *carToRemove) {
    Park *park = system->parkPtrArray[parkPosition];
    Mov *associatedEntry = NULL;
    if (park->carList.head == NULL) {
        printf("Segundo a lógica do código, isto nunca deve ser impresso.\n");
        return NULL;
    }
    // Se o carro a ser removido for o primeiro da lista
    if (park->carList.head == carToRemove) {
        associatedEntry = carToRemove->carEntry;
        park->carList.head = carToRemove->next;
        free(carToRemove); // Libera a memória do carro removido
        return associatedEntry;
    }
    // Caso contrário, procura o carro na lista
    Car *currentCar = park->carList.head;
    while (currentCar->next != NULL) {
        if (currentCar->next == carToRemove) {
            associatedEntry = carToRemove->carEntry;
            currentCar->next = carToRemove->next;
            if (carToRemove == park->carList.tail) {
                park->carList.tail = currentCar;
            }
            free(carToRemove); // Libera a memória do carro removido
            return associatedEntry;
        }
        currentCar = currentCar->next;
    }
    printf("Segundo a lógica do código, isto nunca deve ser impresso.\n");
    return NULL;
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
