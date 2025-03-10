/**
 * @file eCommand.c
 * @brief Contains implementations of functions related to the 'e' command.
 * @author Diogo Fernandes (diiogofer - https://github.com/diiogofer)
*/

#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * Processes the entrance command.
 * @param system Pointer to the system struct.
 */
void eCommand(Sys *system){
    int day, month, year, hour, min;
    char matricula[9], name[MAX_BUFSIZ];

    /* Parse input to extract vehicle information */
    if((sscanf(system->buffer, "e \"%[^\"]\" %s %d-%d-%d %d:%d", 
    name, matricula, &day, &month, &year, &hour, &min) == 7) ||
    sscanf(system->buffer, "e %s %s %d-%d-%d %d:%d", 
    name, matricula, &day, &month, &year, &hour, &min) == 7){
        Date entry = {year, month, day, hour, min};
        
        /* Check for errors in the entrance operation */
        int parkPos = eErrors(system, name, matricula, &entry);
        if (parkPos != ERROR){
            Park *park = system->parkPtrArray[parkPos];
            
            /* Add an entrance movement to the park's movement list */
            AddMovtoList(park, 'e', matricula, &entry); 
            /* Add a new car to the park's car list */
            AddCar(park, park->movList.tail); 
            /* Update the system's current date */
            updateDate(system, &entry); 
            
            /* Print the name of the park and the number of empty spaces */
            printf("%s %d\n", park->name, park->emptySpaces);
        }
    }
    return;
}

/** 
 * @brief Adds a new movement to the movement list of a park.
 * 
 * @param park Pointer to the park structure.
 * @param identifier Character indicating the type of movement 
 * ('e' for entrance, 's' for exit).
 * @param license Pointer to a string containing the license plate.
 * @param entryDate Pointer to the date structure representing 
 * the entry date and time.
 */
void AddMovtoList(Park *park, char identifier, char *license, Date *entryDate){
    /* Allocate memory for the new movement */
    Mov *newMov = malloc(sizeof(Mov));
    if (newMov == NULL) exit(0);

    /* Initialize the new movement with provided information */
    newMov->identifier = identifier;
    newMov->payment = ZERO;
    strcpy(newMov->license, license);
    newMov->movDate = *entryDate;
    newMov->next = NULL;

    /* empty movement list */
    if(park->movList.head == NULL){
        park->movList.head = newMov;
        park->movList.tail = newMov;
    } 
    /* Add the new movement to the end of the existing movement list */
    else{
        park->movList.tail->next = newMov;
        park->movList.tail = newMov;
    }
}

/** 
 * @brief Adds a new car to the car list of a park.
 * 
 * @param park Pointer to the park structure.
 * @param carEntry Pointer to the movement representing the car's entry.
 */
void AddCar(Park *park, Mov *carEntry) {
    /* Allocate memory for the new car */
    Car *newCar = malloc(sizeof(Car));
    if(newCar == NULL) exit(1);

    /* Set the car entry information */
    newCar->carEntry = carEntry;
    newCar->next = NULL;

    /* empty car list */
    if(park->carList.head == NULL) {
        park->carList.head = newCar;
        park->carList.tail = newCar;
    }

    /* Add the new car to the end of the existing car list */
    else{
        park->carList.tail->next = newCar;
        park->carList.tail = newCar;
    }

    /* Decrement empty spaces number */
    park->emptySpaces--;
}

/**
 * @brief Finds a park by its name in the system.
 * 
 * This function searches for a park with a specific name within 
 * the system's array of parks.
 * 
 * @param system Pointer to the system structure.
 * @param inputName Pointer to the name of the park to be found.
 * @return The index of the park in the system's array if found, or ERROR if 
 * not found.
 */
int findParkByName(Sys *system , char *inputName){
    int iter = ZERO;

    /* Loop through the array to check if the names match */
    while(iter < system->createdParks){
        if(strcmp(system->parkPtrArray[iter]->name, inputName) == ZERO) 
            return iter;
        else{
            iter++;
        }
    }
    return ERROR;
}

/**
 * @brief Checks for errors in the entrance operation.
 * 
 * This function verifies if there are any errors in the entrance operation,
 * such as parking availability, license plate validity, and date correctness.
 * 
 * @param system Pointer to the system structure.
 * @param inputName Pointer to the park's name.
 * @param license Pointer to the vehicle's license plate.
 * @param date Pointer to the date of entrance.
 * @return The index of the park in the system if successful, or ERROR if any 
 * error occurs.
 */
int eErrors(Sys *system, char *inputName, char *license, Date *date){
    /* Check if the specified parking exists */
    int parkPosition = findParkByName(system, inputName);
    if(parkPosition == ERROR){
        printf("%s: no such parking.\n", inputName);
        return ERROR;
    }
    /* Check if the parking is full */
    else if(system->parkPtrArray[parkPosition]->emptySpaces == ZERO){
        printf("%s: parking is full.\n", inputName);
        return ERROR;
    }
    /* Validate the license plate */
    else if(validLicensePlate(license) == ERROR){
        printf("%s: invalid licence plate.\n", license); 
        return ERROR;
    }
    /* Check if the vehicle is already inside the parking */
    else if(searchMatricula(system, license) == ERROR){
        printf("%s: invalid vehicle entry.\n", license);
        return ERROR;
    }
    /* Validate the date */
    else if (isValidDate(date) == ERROR || isEarlier(system, date) == ERROR){
        printf("invalid date.\n");
        return ERROR;
    }
    else{
        return parkPosition;
    }
}

/**
 * @brief Validates a vehicle's license plate format.
 * 
 * Checks if the license plate follows the correct format.
 * 
 * @param license Pointer to the license plate string.
 * @return SUCCESS if valid, ERROR otherwise.
 */
int validLicensePlate(char *license){
    int iter;
    int charPairCounter = ZERO, numberPairCounter = ZERO;
    int hyphen1Pos = 2, hyphen2Pos = 5; 
    
    /* Check the length and if the hyphens are in the correct positions */
    if(strlen(license) != LICENSELEN) return ERROR;
    if(license[hyphen1Pos] != '-' || license[hyphen2Pos] != '-') return ERROR;
    
    /* Iterate over the pairs of characters in the license plate */
    for(iter = ZERO; iter <= hyphen2Pos+1; iter += (hyphen1Pos + 1)){
        int pairIter = iter;
        if(isupper(license[pairIter]) && isupper(license[pairIter+1]))
            charPairCounter++;
        else if(isdigit(license[pairIter]) && isdigit(license[pairIter + 1]))
                numberPairCounter++;
        else {return ERROR;}
    }
    
    /* Check if the number of character and number pairs is correct */
    if((numberPairCounter == 1 && charPairCounter == 2) ||
        (numberPairCounter == 2 && charPairCounter == 1)){
        return SUCCESS;
    }
    return ERROR;
}

/**
 * @brief Searches for a vehicle with a given license plate 
 * in the parking system.
 * 
 * @param system Pointer to the parking system.
 * @param matricula Pointer to the license plate string to search for.
 * @return SUCCESS if the license plate is not found, ERROR if found.
 */
int searchMatricula(Sys *system, char *matricula){
    for (int iter = 0; iter < system->createdParks; iter++){
        Car *currentCar = system->parkPtrArray[iter]->carList.head;
        while (currentCar != NULL) {
            if (strcmp(currentCar->carEntry->license, matricula) == 0){
                return ERROR;
            }
            currentCar = currentCar->next;
        }
    }
    return SUCCESS;
}


