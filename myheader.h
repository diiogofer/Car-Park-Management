/* iaed24 - ist1110306 - project */
#ifndef MYHEADER_H
#define MYHEADER_H

#define MAX_BUFSIZ 8192
#define MAX_PARKS 20
#define ZERO 0
#define ERROR -1
#define SUCCESS 1

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

typedef struct moviment 
{
    char identifier; // pode ser o caracter 's' ou o caracter 'e'
    char license[9];
    Date movDate;
    float payment;
    struct moviment *next;
} Mov;

typedef struct car
{
    Mov *carEntry;
    struct car *next;
} Car;

typedef struct car_node {
    Car *head; 
    Car *tail; 
} CarList;

typedef struct mov_node {
    Mov *head; 
    Mov *tail; 
} MovList;

typedef struct park {
    int maxCapacity;
    int emptySpaces;
    float X;
    float Y;
    float Z;
    char *name;
    CarList carList; 
    MovList movList; 
} Park;

typedef struct sys
{
    char *buffer;
    Park **parkPtrArray;
    int createdParks;
    Date *currentDate;
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
int validMatricula(char *matricula);
int searchMatricula(Sys *system, char *matricula);
int isValidDate(Date *date);
int isEarlier(Sys *system, Date *date2);
/* APAGAR MAIS TARDE */
void printCarMatriculas(Sys *system);
void printMovMatriculas(Sys *system);

/**/
void sCommand(Sys *system);
Car *sErrors(Sys *system, int ParkPos, char *name, char *license, Date *exit);
Mov *removeCar(Sys *system, int parkPosition, Car *carToRemove);

int anoToMinutes(int *DaysMonthVec);
int percorreAnos(int ComparisonYear, Date *date, int *DaysMonthVec);
int percorreMeses(Date *date, int *DaysMonthVec);
int tempoEmMinutosFunc(Date *date, int ComparisonYear,int *DaysMonthVec);
int datesDiff(Date *date1, Date *date2);
float payment(Sys *system, int parkPos, Date *entrie, Date *exit);
Car *findCarInPark(Park *park, char *license);

void vCommand(Sys *system);
void insertion(Park *a[], int l, int r);
void printCarMovByPlate(Sys *system, Park **parkPtrArray, char *matricula);

void fCommand(Sys *system);
int fErrors(Sys *System, int parkPos, Date *date, char *ParkName);
void printExitsByPlate(Sys *system, int parkPosition, Date *date);
void printDailyEarnings(Sys *system, int parkPosition);

void rCommand(Sys *system);
void organizeParkPtrArray(Sys *system);
void freeAllParks(Sys *system);
void freePark(Sys *system, int ParkPos);

#endif /* MYHEADER_H */
