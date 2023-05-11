#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "utils_v2.h"
#include "header.h"

#define BUFFER_SIZE 5000

int connectToZombies(char* serverIP, int serverPort) {

    int sockfd;
    sockfd = ssocket();

    if(sconnect(serverIP, serverPort, sockfd) != -1){
         printf("Connected to server %s on port %d\n", serverIP, serverPort);
         return sockfd;
    } else {
        // Echec de la connexion au serveur
        return 0;
    }
}


void listenToTheZombies(void* array, void* lSize) {
    printf("listen\n");
}


int main(int argc, char** argv) {

    int* array = NULL; 
    int lSize = 0;  
    int pSize = 0; 
    char command[BUFFER_SIZE];

    //malloc ?

    char* servers[] = {
        "127.0.0.1",
    };

    int serversLength = sizeof(servers) / sizeof(char*);

    /* Connect to all the zombies and store their fd in an array*/
    for (int i = 0; i < serversLength; i++) {
        for(int j = 0; j < NUM_PORTS; j++) {

            int sockfd = connectToZombies(servers[i], PORTS[j]);
            if(sockfd != 0){
                if (lSize >= pSize) {
                
                    pSize += 10; 
                    int* newArray = (int*)realloc(array, pSize * sizeof(int));
                    if (newArray == NULL) {
                        printf("Error realloc.\n");
                        free(array);
                        return 0;
                    }
                    array = newArray;
                }
                array[lSize] = sockfd;
                lSize++;
            }        
        }
    }
    printf("Number of connection established : %d\n", lSize);

    printf("\nLes entiers saisis sont :\n");
    for (int i = 0; i < lSize; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // return childId
    fork_and_run2(listenToTheZombies, &array, &lSize);

    /* Programme père */

    
    // Send characters to the server
    printf("Entrez une commande :\n");
    while (1) {
        fgets(command, BUFFER_SIZE, stdin);
        command[strcspn(command, "\n")] = 0;

        for(int i = 0 ; i < lSize ; i++){
            swrite(array[i], &command, strlen(command));
        }
    }

    free(array);    
    return 0;
    

    /* Close tout les sockfd !*/
}
