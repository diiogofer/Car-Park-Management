/* iaed24 - ist1110306 - project */
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
        Sys system = sysCreator(buffer, parkPtrArray, 0);
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
                                printf("e pressed\n");
                                break;
                        case 's':
                                printf("s pressed\n");
                                break;
                        case 'v':
                                printf("v pressed\n");
                                break;
                        case 'f':
                                printf("f pressed\n");
                                break;
                        case 'r':
                                printf("r pressed\n");
                                break;
                        }
                        memset(buffer, '\0', MAX_BUFSIZ);
	}
        int cleanupParks(Sys *system);
        return 0;
}