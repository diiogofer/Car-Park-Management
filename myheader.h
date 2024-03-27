/**
 * @file MYHEADER_H
 * @brief Header file containing data structures and function prototypes.
 * Author: Diogo Fernandes - ist1110306
 */

#ifndef MYHEADER_H
#define MYHEADER_H

#define MAX_BUFSIZ 8192
#define MAX_PARKS 20
#define ZERO 0
#define ERROR -1
#define SUCCESS 1
#define LICENSELEN 8
#define DAYTOMIN 1440
#define HOURTOMIN 60


/* Data Structures */

/**
 * @brief Represents a date with year, month, day, hour, and minute.
 */
typedef struct date {
    int year;    /**< The year. */
    int month;   /**< The month. */
    int day;     /**< The day. */
    int hour;    /**< The hour. */
    int minute;  /**< The minute. */
} Date;

/**
 * @brief Represents a movement, either entry or exit, in a parking system.
 */
typedef struct moviment {
    char identifier; /**< The identifier, either 's' (exit) or 'e' (entry). */
    char license[9]; /**< The license plate of the vehicle. */
    Date movDate;    /**< The date and time of the movement. */
    float payment;   /**< The payment made for the movement. */
    struct moviment *next; /**< Pointer to the next movement. */
} Mov;

/**
 * @brief Represents a car in the parking system.
 */
typedef struct car {
    Mov *carEntry;    /**< Pointer to the movement associated with the car. */
    struct car *next; /**< Pointer to the next car. */
} Car;

/**
 * @brief Represents a list of cars.
 */
typedef struct car_node {
    Car *head; /**< Pointer to the first car in the list. */
    Car *tail; /**< Pointer to the last car in the list. */
} CarList;

/**
 * @brief Represents a list of movements.
 */
typedef struct mov_node {
    Mov *head; /**< Pointer to the first movement in the list. */
    Mov *tail; /**< Pointer to the last movement in the list. */
} MovList;

/**
 * @brief Represents a parking lot.
 */
typedef struct park {
    int maxCapacity;   /**< The maximum capacity of the parking lot. */
    int emptySpaces;   /**< The number of empty spaces in the parking lot. */
    float X;           /**< The X-coordinate of the parking lot. */
    float Y;           /**< The Y-coordinate of the parking lot. */
    float Z;           /**< The Z-coordinate of the parking lot. */
    char *name;        /**< The name of the parking lot. */
    CarList carList;   /**< The list of cars in the parking lot. */
    MovList movList;   /**< The list of movements in the parking lot. */
} Park;

/**
 * @brief Represents the system.
 */
typedef struct sys {
    char *buffer;          /**< The buffer used for input/output. */
    Park **parkPtrArray;   /**< Array of pointers to parking lots. */
    int createdParks;      /**< The number of created parking lots. */
    Date *currentDate;     /**< Pointer to the current date. */
} Sys;


/*Function Declarations*/
Sys sysCreator(char *buf, Park **parksPtr, int createdParks, Date *date);
void pCommand(Sys *system);
void addPark(Sys *system, char *Name, int Max, float x, float y, float z);
void parkInformation(Sys *system);
int pErrors(Sys *system, char *Name, int capacity, float x, float y, float z);

void eCommand(Sys *system);
void AddMovtoList(Park *park, char identifier, char *license, Date *entryDate);
void AddCar(Park *park, Mov *carEntry);
void updateDate(Sys *system, Date *newDate);
int findParkByName(Sys *system , char *inputName);
int eErrors(Sys *system, char *inputName, char *matricula, Date *date);
int validLicensePlate(char *license);
int searchMatricula(Sys *system, char *matricula);
int isValidDate(Date *date);
int isEarlier(Sys *system, Date *date2);

/**/
void sCommand(Sys *system);
Car *sErrors(Sys *system, int ParkPos, char *name, char *license, Date *exit);
Mov *removeCar(Sys *system, int parkPosition, Car *carToRemove);

int yearToMinutes(int *DaysMonthVec);
int traverseYears(int ComparisonYear, Date *date, int *DaysMonthVec);
int traverseMonths(Date *date, int *DaysMonthVec);
int timeInMinutesFunc(Date *date, int ComparisonYear,int *DaysMonthVec);
int datesDiff(Date *date1, Date *date2);
float payment(Sys *system, int parkPos, Date *entrie, Date *exit);
Car *findCarInPark(Park *park, char *license);

void vCommand(Sys *system);
void insertion(Park *a[], int l, int r);
void printCarMovByPlate(Sys *system, Park **parkPtrArray, char *license);


void fCommand(Sys *system);
int fErrors(Sys *System, int parkPos, Date *date, char *ParkName);
void printExitsByPlate(Sys *system, int parkPosition, Date *date);
void printDailyEarnings(Sys *system, int parkPosition);

void rCommand(Sys *system);
void organizeParkPtrArray(Sys *system);
void freeAllParks(Sys *system);
void freePark(Sys *system, int ParkPos);

#endif /* MYHEADER_H */
