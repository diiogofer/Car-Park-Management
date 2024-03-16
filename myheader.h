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

typedef struct park
{
    int maxCapacity;
    int emptySpaces;
    float X;
    float Y;
    float Z;
    char *name;
} Park;

typedef struct sys
{
    char *buffer;
    Park **parkPtrArray;
    int createdParks;
} Sys;

void print();
Sys sysCreator(char *buffer, Park **parkPtrArray, int createdParks);
void createPark(Sys *system, char *Name, int capacity, float x, float y, float z);
void pCommand(Sys *system);
void parkInformation(Sys *system);
int pErrors(Sys *system, char *Name, int capacity, float x, float y, float z);
int cleanupParks(Sys *system);
#endif /* MYHEADER_H */
