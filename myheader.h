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

typedef struct exit{
    char matricula[9];
    Date saida;
    float Payment;
    struct exit *next;
} Exit;

typedef struct entrie 
{
    char matricula[9];
    Date entrada;
    Exit *saida;
    struct entrie *next;
} Entrie;

typedef struct car
{
    Entrie *carEntrie;
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
    Car *FirstCar;  
    Car *LastCar;
    Entrie *FirstEntrie;
    Entrie *LastEntrie;
    Exit *FirstExit;
    Exit *LastExit;
} Park;

typedef struct sys
{
    char *buffer;
    Park **parkPtrArray;
    int createdParks;
    Date *currentDate;
} Sys;

/*Function Declarations*/
Sys sysCreator(char *buf, Park **parksPtr, int createdParks, Date *actualDate);
void pCommand(Sys *system);
void createPark(Sys *system, char *Name, int Max, float x, float y, float z);
void parkInformation(Sys *system);
int pErrors(Sys *system, char *Name, int capacity, float x, float y, float z);

void eCommand(Sys *system);
Car *addCarToPark(Sys *system, int parkPos);
Entrie *addEntrie(Sys *sys, char *matricula, int parkPos, Date *entrada);
void eChanges(Sys *system, Car *car, Entrie *entrie, Date *date, int parkPos);
int eErrors(Sys *system, char *inputName, char *matricula, Date *date);
int findParkByName(Sys *system , char *inputName);
int validMatricula(char *matricula);
int isValidDate(Date *date);
int isEarlier(Sys *system, Date *date2);
int searchMatricula(Sys *system, char *matricula);

/*Apagar mais tarde*/
void printCarMatriculas(Park *park);
void printEntradaDetails(Park *park);
void printAllCarMatriculas(Sys *system);
void printAllEntradaDetails(Sys *system);

void sCommand(Sys *system);
int sErrors(Sys *system, char *inputName, char *matricula);
Exit *addExit(Sys *sys, char *matricula, int parkPos, Date *saida);
Entrie *removeCar(Sys *system, int parkPos, char *matricula, Date *exit);
int anoToMinutes(int *DaysMonthVec);
int percorreAnos(int ComparisonYear, Date *date, int *DaysMonthVec);
int percorreMeses(Date *date, int *DaysMonthVec);
int tempoEmMinutosFunc(Date *date, int ComparisonYear,int *DaysMonthVec);
int datesDiff(Date *date1, Date *date2);
float payment(Sys *system, int parkPos, Date *entrie, Date *exit);

/* Apagar mais tarde */
void printAllExits(Sys *system);
void printExitsInPark(Park *park);


void insertion(Park *a[], int l, int r);
void printCarMovByPlate(Sys *system, Park **parkPtrArray, char *matricula);
void vCommand(Sys *system);
#endif /* MYHEADER_H */
