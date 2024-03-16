/* Objectif :
* cree un tube /tmp/fifo
* Ouvre le tube en lecture 
* Attend un texte du client
* Affiche a l'ecran le texte du client
* Se met en attente d'un nouveau texte du client
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#define SIZE 100

int main() {
    // Crée le tube
    char path[] = "/tmp/fifo";
    int mode = 0666;
    mkfifo(path, mode);
    printf("Tube créé\n\n");

    // Ouvre le tube en lecture
    FILE *tube = NULL;
    tube = fopen(path, "w+");
    if (tube == NULL) {
        printf("Erreur d'ouverture du tube\n\n");
        exit(1);
    }
    printf("Tube ouvert en lecture, en attente d'un client...\n\n");

    // Attend un texte du client
    char buffer[SIZE];
    fgets(buffer, SIZE, tube);
    printf("Buffer : %s\n\n", buffer);
    for(int i = 0; i < 10; i++) {
        fgets(buffer, SIZE, tube);
        if(buffer[0] != '\0') {
            // Affiche à l'écran le texte du client
            printf("%s\n",buffer);
            if (buffer[0] == 'q' && buffer[1] == '\0') {
                fclose(tube);
                unlink(path);
                exit(0);
            }
            // Se met en attente d'un nouveau texte du client
            fseek(tube,0,0);
            fputs("\0",tube);
        }
    }
}