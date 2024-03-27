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
    if (park->carList.head == NULL) return NULL;

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
    return NULL;
}





int yearToMinutes(int *DaysMonthVec){
    int iter, minutes = ZERO;
    for(iter = ZERO; iter < 12; iter++){
        minutes += DaysMonthVec[iter] * DAYTOMIN;
    }
    return minutes;
}

int traverseYears(int ComparisonYear, Date *date, int *DaysMonthVec){
    int minutes = ZERO;
    if(ComparisonYear == date->year) return minutes;

    for(;ComparisonYear < date->year; ComparisonYear++){
        /* Accumulate minutes for each year */
        minutes += yearToMinutes(DaysMonthVec);
    }

    return minutes;
}

int traverseMonths(Date *date, int *DaysMonthVec){
    int iter, minutes = ZERO;
    
    for(iter = ZERO; iter < date->month - 1; iter++){
        /* Accumulate minutes for each month */
        minutes += DaysMonthVec[iter] * DAYTOMIN;
    }
    
    return minutes;
}

int timeInMinutesFunc(Date *date, int ComparisonYear, int *DaysMonthVec){
    int timeInMinutes = ZERO;
    /* Convert years to minutes */
    timeInMinutes = traverseYears(ComparisonYear, date, DaysMonthVec);
    /* Add hours and minutes */
    timeInMinutes += date->hour * HOURTOMIN + date->minute;
    /* Add days */
    timeInMinutes += (date->day - 1) * DAYTOMIN;
    /* Convert months to minutes */
    timeInMinutes += traverseMonths(date, DaysMonthVec);
    return timeInMinutes;
}

int datesDiff(Date *date1, Date *date2){
    int timeInMinutes1, timeInMinutes2;
    /* Array of days in each month */
    int DaysMonthVec[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    /* Set the comparison year as the year before the first date */
    int ComparisonYear = date1->year - 1;
    /* Convert dates to minutes */
    timeInMinutes1 = timeInMinutesFunc(date1, ComparisonYear, DaysMonthVec);
    timeInMinutes2 = timeInMinutesFunc(date2, ComparisonYear, DaysMonthVec);
    /* Return the difference in minutes */
    return timeInMinutes2 - timeInMinutes1;
}

float payment(Sys *system, int parkPos, Date *entrie, Date *exit){
    float TaxX = system->parkPtrArray[parkPos]->X;
    float TaxY = system->parkPtrArray[parkPos]->Y;
    float TaxZ = system->parkPtrArray[parkPos]->Z;
    int diffMinutes = datesDiff(entrie, exit);
    float totalPayment = ZERO;

    int days = diffMinutes / DAYTOMIN;
    int remainingMinutes = diffMinutes % (DAYTOMIN);
    float expected15Payment = ZERO;
    int counter15min = ZERO;
    
    /* Calculate payment for each 15-minute interval */
    for(int iter = ZERO; iter < remainingMinutes; iter+=15){
        if(counter15min < 4){
            expected15Payment += TaxX;
        }
        else{expected15Payment += TaxY;}
        counter15min++;
    }

    /* Determine the total payment */
    if(expected15Payment > TaxZ){
        totalPayment += TaxZ;
    }
    else{totalPayment += expected15Payment;}
    totalPayment += TaxZ * days;
return totalPayment;
}
