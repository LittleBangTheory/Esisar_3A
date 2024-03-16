#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Écrire un programme qui prend en argument deux commandes (commande1 et commande2) et réalise une exécution équivalente à l'exécution de « commande1 | commande2 » dans un shell.

int main(int argc, char** argv){
    if (argc != 3) {
        printf("Usage : %s commande1 commande2\n", argv[0]);
        exit(1);
    }

    // Création du tube
    int tube[2];
    pipe(tube);

    if (fork() == 0) {
        // Fermeture du tube en lecture
        close(tube[0]);
        // Redirection de la sortie standard vers le tube
        dup2(tube[1], 1);
        // Fermeture du tube en écriture
        close(tube[1]);
        // Exécution de la commande 1
        execlp(argv[1], argv[1], NULL);
        exit(0);
    }

    if (fork() == 0) {
        // Fermeture du tube en écriture
        close(tube[1]);
        // Redirection de l'entrée standard vers le tube
        dup2(tube[0], 0);
        // Fermeture du tube en lecture
        close(tube[0]);
        // Exécution de la commande 2
        execlp(argv[2], argv[2], NULL);
        exit(0);
    }

    // Fermeture du tube en lecture
    close(tube[0]);
    // Fermeture du tube en écriture
    close(tube[1]);
    // Attente de la fin des fils
    wait(NULL);
    wait(NULL);
    exit(0);
}