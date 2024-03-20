#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(){
    Park *parkPtrArray[MAX_PARKS];
    char buffer[MAX_BUFSIZ];
    memset(parkPtrArray, 0, sizeof(parkPtrArray)); /* Pointers to NULL */
	memset(buffer, '\0', MAX_BUFSIZ);
    Date date = {ZERO, ZERO, ZERO, ZERO, ZERO};
    Sys system = sysCreator(buffer, parkPtrArray, 0, &date);
    while (1){
        if (fgets(buffer, MAX_BUFSIZ, stdin) == NULL || 
            system.buffer[0] == 'q'){
			break;
		}
		switch (system.buffer[0]){
            case 'p':
                pCommand(&system);
                break;
            case 'e':
                eCommand(&system);
                    break;
            case 's':
                sCommand(&system);
                break;
            case 'v':
                vCommand(&system);
                break;
            case 'f':
                printf("f pressed\n");
                break;
            case 'r':
                printf("r pressed\n");
                break; 
            case 'x':
                printAllExits(&system);
                break;    
            case 'y':
                printAllCarMatriculas(&system);
                break;
            case 'z':
                printAllEntradaDetails(&system);
                break;
        }
        memset(buffer, '\0', MAX_BUFSIZ);
	}
    /*    cleanup(&system);*/
    return 0;
}