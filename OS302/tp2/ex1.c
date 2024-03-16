#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void empty_funct(int){}

int main(void){
    // Ecrire un programme qui crée deux processus, père et fils. Le père affiche les nombres entiers impairs compris entre 1 et 100, alors que le fils affiche les entiers pairs compris dans le même intervalle. Synchroniser les processus à l'aide des signaux pour que le résultat d'affichage soit : 1 2 3 ... 100
    pid_t pid_pere = getpid();

    // On capture le signal SIGUSR1
    signal(SIGUSR1, empty_funct);

    printf("Je suis le processus pere de pid %d\n", getpid());
    // On cree un processus fils
    pid_t pid_fils = fork();

    switch (pid_fils) {
    // Si le pid du fils est 0, on est dans le processus fils
    case 0:
        for(int j = 2; j <= 100; j+=2){
            printf("%d\n", j);
            kill(pid_pere, SIGUSR1);
            pause();
        }
        printf("end of child process\n");
		return EXIT_SUCCESS;
    // Si le pid du fils est -1, il y a eu une erreur au fork
    case -1:
        perror("Le fork a echoue");
		return EXIT_FAILURE;
    // Sinon, on est dans le processus pere
    default:
        for(int i = 1; i <= 100; i+=2){
            printf("%d\n", i);
            kill(pid_fils, SIGUSR1);
            pause();
        }
        kill(pid_fils, SIGUSR1);
        printf("end of parent process\n");
    }
    return EXIT_SUCCESS;
}