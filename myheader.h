/* iaed24 - ist1110306 - project */
#ifndef MYHEADER_H
#define MYHEADER_H

#define MAX_BUFSIZ 8192
#define MAX_PARKS 20
#define ZERO 0
/*Abstrações*/
typedef char* Item;
#define key(A) (A)
#define less(A, B) (strcmp(key(A), key(B)) < 0)
#define exch(A, B) { Item t = A; A = B; B = t; }
typedef struct date
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
} Date;

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
    Car *LastAddedCar;
    Movement *FirstMoveOfList;
    Movement *LastAddedMovement;
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
/* Exit Functions */
void sCommand(Sys *system);
int sErrors(Sys *system, char *inputName, char *matricula);
Movement *removeCarFromPark(Sys *system, int parkPos, char *matricula, Date *exit);
int anoToMinutes(int *DaysMonthVec);
int percorreAnos(int ComparisonYear, Date *date, int *DaysMonthVec);
int percorreMeses(Date *date, int *DaysMonthVec);
int tempoEmMinutosFunc(Date *date, int ComparisonYear,int *DaysMonthVec);
int datesDiff(Date *date1, Date *date2);
float payment(Sys *system, int parkPos, Date *entrie, Date *exit);

void insertion(Park *a[], int l, int r);
void printCarMovementsByPlate(Sys *system, Park **parkPtrArray, char *matricula);
void printCarMovement(const char *parkName, Date *entrada, Date *saida);
void vCommand(Sys *system);
#endif /* MYHEADER_H */
