/* iaed24 - ist1110306 - project */
#ifndef MYHEADER_H
#define MYHEADER_H

#define MAX_BUFSIZ 8192
#define MAX_PARKS 20
#define ZERO 0

typedef struct date
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
} Date;
/* primeira ideia para carros :(
typedef struct car
{
    char matricula[9];
    Date entrada;
    Date saida;
    struct car *nextcar;
} Car;

typedef struct carList{
    Car *head; 
    Car *tail;
} CarList
*/
/*segunda ideia para carros*/
typedef struct movement 
{
    char matricula[9];
    Date entrada;
    Date saida;
    struct movement *next;
} Movement;

typedef struct car
{
    /* Data */
    char matricula[9];
    Movement *carMove;
    /* Next */
    struct car *next;
} Car;

typedef struct park
{
    int maxCapacity;
    int emptySpaces;
    float X;
    float Y;
    float Z;
    char *name;
    Car *FirstCarOfList;  
    Movement *FirstMoveOfList;
} Park;

typedef struct sys
{
    char *buffer;
    Park **parkPtrArray;
    int createdParks;
    Date *currentDate;
    
} Sys;

/* Park related functions */
Sys sysCreator(char *buffer, Park **parkPtrArray, int createdParks, Date *currentDate);
void createPark(Sys *system, char *Name, int capacity, float x, float y, float z);
void pCommand(Sys *system);
void parkInformation(Sys *system);
int pErrors(Sys *system, char *Name, int capacity, float x, float y, float z);
int cleanupParks(Sys *system);
/* Car related functions */
void eCommand(Sys *system);
int findParkByName(Sys *system , char *inputName);
Car *addCarToPark(Sys *system, char *matricula,int parkPos);
void printCarsInParks(Sys *system);
Movement *addMov(Sys *sys, char *matricula, int parkPos, Date *entry, Date *saida);
void eChanges(Sys *system, Car *car, Movement *mov, Date *date, int ParkPos);
int eErrors(Sys *system, char *inputName, char *matricula, Date *date);
void printMovementsInPark(Park *park);
void printAllMovements(Sys *system);
int validMatricula(char *matricula);
int isValidDate(Date *date);
int isEarlier(Sys *system, Date *date2);
int dateChecker(Sys *system, Date *date);
int searchMatricula(Sys *system, char *matricula);
#endif /* MYHEADER_H */
