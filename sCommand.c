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
        if(carToRemove == NULL) return;
        /* Removing car from the park */
        Mov *associatedEntrie = removeCar(system, parkPos, carToRemove);
        Date entryDate = associatedEntrie->movDate;
        Park *park= system->parkPtrArray[parkPos]; 
        AddMovtoList(park, 's', license, &exitDate);
        /* Calculating the payment */
        float fee = payment(system, parkPos, &entryDate, &exitDate);
        park->movList.tail->payment = fee;
        
        printf("%s %02d-%02d-%02d %02d:%02d %02d-%02d-%02d %02d:%02d %.2f\n",
        license, entryDate.day, entryDate.month, entryDate.year, 
        entryDate.hour, entryDate.minute, day, month, year, hour, min, fee);
        /* Updates */ 
        (park->emptySpaces)++;
        updateDate(system, &exitDate);
    }
    return ;
}



Car *findCarInPark(Park *park, char *license) {
    Car *currentCar = park->carList.head;
    
    /* Loop through the car list until the end is reached */
    while (currentCar != NULL) {
        /* Check if the license plate matches */
        if (strcmp(currentCar->carEntry->license, license) == 0) {
            /* Return the pointer to the found car */
            return currentCar;
        }
        currentCar = currentCar->next;
    }

    /* Return NULL if the car is not found in the park */
    return NULL; 
}

Car *sErrors(Sys *system, int ParkPos, char *name, char *license, Date *exit){
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
    if (isValidDate(exit) != SUCCESS || isEarlier(system, exit) != SUCCESS){
        printf("invalid date.\n");
        return NULL;
    }
    /* Return pointer to the car */
    return CarToRemove;
}

Mov *removeCar(Sys *system, int parkPosition, Car *carToRemove) {
    Park *park = system->parkPtrArray[parkPosition];
    Mov *associatedEntry = NULL;
    /* If the car list is empty, return NULL */
    if (park->carList.head == NULL) return NULL;

    /* If the car to be removed is the first in the list */
    if (park->carList.head == carToRemove) {
        associatedEntry = carToRemove->carEntry;
        park->carList.head = carToRemove->next;
        free(carToRemove); /* Free the memory of the removed car */
        return associatedEntry;
    }
    /* Otherwise, search for the car in the list */
    Car *currentCar = park->carList.head;
    while (currentCar->next != NULL){
        if (currentCar->next == carToRemove){
            associatedEntry = carToRemove->carEntry;
            currentCar->next = carToRemove->next;
            /* Update the tail if the removed car was the last in the list */
            if (carToRemove == park->carList.tail){
                park->carList.tail = currentCar;
            }
            free(carToRemove); /* Free the memory of the removed car */
            return associatedEntry;
        }
        currentCar = currentCar->next;
    }
    return NULL;
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
