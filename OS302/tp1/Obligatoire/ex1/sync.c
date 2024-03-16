#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define SLEEP_TIME 30

int main(void)
{
    pid_t pid_fils;
    pid_t ret_wait;
	int	etat;

    // On affiche le pid du processus pere
    printf("Je suis le processus pere de pid %d\n", getpid());
    // On cree un processus fils
    pid_fils = fork();

    switch (pid_fils) {
    // Si le pid du fils est 0, on est dans le processus fils
    case 0:
        printf("*** FILS ***\n");
        // On affiche le pid du processus fils
        printf("Processus fils de pid %d\n", getpid());
        // On affiche le pid du processus pere
		printf("Pere de pid %d\n", getppid());
        printf("Je vais dormir 30 secondes ...\n");
        // On fait dormir le processus fils pendant 30 secondes
        sleep(SLEEP_TIME);
        printf("Je me reveille\n");
		printf("Je termine mon execution par un `return EXIT_SUCCESS`\n");
        // On termine le processus fils
		return EXIT_SUCCESS;
    // Si le pid du fils est -1, il y a eu une erreur au fork
    case -1:
        perror("Le fork a echoue");
		return EXIT_FAILURE;
    // Sinon, on est dans le processus pere
    default:
        printf("*** PERE ***\n");
        // On affiche le pid du processus pere
        printf("Processus pere de pid %d\n", getpid());
        // On affiche le pid du processus fils  
		printf("Fils de pid %d\n", pid_fils);
        printf("J'attends la fin de mon fils...\n");
        // On attend la fin du processus fils
        ret_wait = wait(&etat);
        // On affiche le pid du processus fils qui est termine
        printf("Mon fils de pid %d est termine\n", ret_wait);
        // On affiche l'etat de retour du processus fils
		printf("Son etat etait : %04x\n", etat);
    }
    return EXIT_SUCCESS;
}