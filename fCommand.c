// #include "myheader.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <ctype.h>
// #include <string.h>

// void fCommand(Sys *sys){
//     int day, month, year;
//     char name[BUFSIZ];

//     if((sscanf(sys->buffer, "f \"%[^\"]\" %d-%d-%d", 
//         name, day, month, year) == 4) || 
//         (scanf(sys->buffer, "f %s %d-%d-%d", 
//         name, day, month, year) == 4)){
            
//             /*bloco de comandos*/
//         printf("Olá\n");
//         }
//     else if((sscanf(sys->buffer, "f \"%[^\"]\"", name) == 1) 
//         || (sscanf(sys->buffer, "f %s", name) == 1)){
        
//         /* outra cena fixe */

//     }
// }

// void exitsByDate(Sys *system, int parkPos, int year, int month, int day) {

//     Exit *currentExit = system->parkPtrArray[parkPos]->FirstExit;
//     while (currentExit != NULL) {
//         if (currentExit->saida.year == year &&
//             currentExit->saida.month == month &&
//             currentExit->saida.day == day) {
//             printf("%s %02d:%02d %.2f\n", currentExit->matricula,
//             currentExit->saida.hour, currentExit->saida.minute, 
//             currentExit->Payment);
//         }
//         currentExit = currentExit->next;
//     }
// }




