#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define TAILLE_MAX 2

int KILLED = 0;

void empty_funct(){}

void living(){KILLED++;}

int lire_valeur(const char *path){
    FILE *fichier;
    char chaine[TAILLE_MAX];
    int valeur;
    fichier = fopen(path, "r");
    if (fichier != NULL) {
        /* On lit au maximum TAILLE_MAX caractères du fichier, ce qui est lu est stocké dans `chaine` */
        fgets(chaine, TAILLE_MAX, fichier);
        fclose(fichier);
        valeur = atoi(chaine);
    }
    return valeur;
}

void ecrire_valeur(const char *path, int valeur){
    FILE *fichier = fopen(path, "w");
    if (fichier != NULL) {
        fprintf(fichier, "%d", valeur);
        fclose(fichier);
    }
}

int main(void){
    // On capture le signal SIGUSR1 (reveil)
    signal(SIGUSR1, empty_funct);

    // On capture le signal SIGUSR2 (fils mort ou vivant)
    signal(SIGUSR2, living);

    // On print le pid du pere
    printf("Je suis le processus pere de pid %d\n\n", getpid());


    /*---------On cree 6 processus fils--------------*/
    pid_t pids[6];
    int nprocesses = 0;
    while(nprocesses < 6)
    {
        // on cree un processus fils et on stocke son pid dans le tableau
        pids[nprocesses] = fork();
        switch (pids[nprocesses]) {
        // Si le pid du fils est 0, on est dans le processus fils
        case 0 :
            ;//On ne fait rien car on ne peut affecter directement après un :
            // on stocke son id
            int id = nprocesses+1;
            // on attend le signal du père pour jouer
            pause();
            // on lit la valeur du barillet
            int valeur_roulette = lire_valeur("barillet.txt");
            // Si la valeur du barillet est égale à son id, il se tue et réveille le père
            if (valeur_roulette == id){
                //kill itself
                printf("\nLe processus %d est mort\n\n", id);
                kill(getppid(), SIGUSR2);
                kill(getpid(), SIGKILL);
            // Sinon, il envoie un signal au père pour dire qu'il est toujours vivant
            } else {
                //send signal that still alive
                kill(getppid(), SIGUSR1);
            }
            return EXIT_SUCCESS;
        case -1 :
            perror("Le fork a echoue");
            return EXIT_FAILURE;
        default :
            //on boucle
            nprocesses++;
        }
    }


    /*-------------On joue----------------*/
    // add the number in the file
    char path[13]="barillet.txt";
    // on tire un nombre random entre 1 et 6 après avoir initialisé le pseudo-random
    srand(time(NULL));
    int nombre = (rand() % 6) + 1;
    printf("La valeur du barillet est %d\n\n", nombre);
    // on écrit le nombre dans le fichier
    ecrire_valeur(path, nombre);
    nprocesses = 0;
    while (nprocesses < 6)
    {
        // Si aucun processus n'est mort, on fait jouer les fils les uns après les autres
        if(KILLED == 0) {
            // on reveille le fils
            printf("On fait jouer le processus %d de pid %d \n", nprocesses+1, pids[nprocesses]);
            kill(pids[nprocesses], SIGUSR1);
            pause();
        } else {
            // Sinon, on tue tous les processus restants
            printf("On met fin au jeu pour le processus %d de pid %d \n", nprocesses+1, pids[nprocesses]);
            kill(pids[nprocesses], SIGKILL);
        }
        nprocesses++;
    }
    return EXIT_SUCCESS;
}

