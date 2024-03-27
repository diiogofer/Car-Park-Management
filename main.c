/**
 * @file main.c
 * @brief Contains the main function of the program.
 * @author Diogo Fernandes - ist1110306
 */

#include "myheader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief The main function of the program.
 * 
 * This function initializes the system and processes user commands until 
 * the program is terminated. User commands are read from the standard 
 * input (stdin) and routed to the appropriate functions based on the 
 * command type.
 * 
 * @return Returns 0 upon successful execution.
 */
int main(){
    /* Array of park pointers */
    Park *parkPtrArray[MAX_PARKS];
    memset(parkPtrArray, 0, sizeof(parkPtrArray));
    /* Buffer for user commands */
    char buff[MAX_BUFSIZ];
	memset(buff, '\0', MAX_BUFSIZ);
    /* Initializing system with default date */
    Date date = {ZERO, ZERO, ZERO, ZERO, ZERO};
    Sys system = sysCreator(buff, parkPtrArray, 0, &date);

    /* Processing user commands */
    while (1){
        if (fgets(buff, MAX_BUFSIZ, stdin) == NULL || system.buffer[0] == 'q') {
			break;
		}
		switch (system.buffer[0]){
            case 'p': pCommand(&system); break;
            case 'e': eCommand(&system); break;
            case 's': sCommand(&system); break;
            case 'v': vCommand(&system); break;
            case 'f': fCommand(&system); break;
            case 'r': rCommand(&system); break;     
        }
    /* Freeing allocated memory for parks */
        memset(buff, '\0', MAX_BUFSIZ);
	}
    freeAllParks(&system);
    return 0;
}