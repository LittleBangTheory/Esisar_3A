#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

#define ASCENSEUR 0
#define SEGMENT 1

void empty_funct(){}

void sortie(){
    printf("Le fils de PID %d est bien arrivé\n", getpid());
    exit(0);
}

/** 
 * \brief Cree un segment de memoire
 * \param nom le nom du fichier associe
 * \param taille la taille du segment memoire a creer
 * \param id associe au nom pour la construction d'une cle identifiant pour le segment memoire (on utilisant la fonction ftok)
 * \return l'identificateur du segment, ou -1 en cas d'erreur 
 * int shmget(key_t cle, int taille, int option)
*/
int cree_segment(char* nom, size_t taille, int id){
	// Création de la clef avec le fichier et l'id passés en paramètre
	printf("### Création de la clef segment_key sur le fichier %s, et d'id %d ###\n", nom, id);
	key_t segment_key = ftok(nom, id);

	// Création du segment
	printf("### Création d'un segment de mémoire partagé avec la clef %d, de taille %ld ###\n\n", segment_key, taille);
	int segment_id = shmget(segment_key, taille, IPC_CREAT | 0660);

	// Renvoie de l'identifiant segment
	return segment_id;
}

int detruire_segment(int id){
	struct shmid_ds p_shmid; // Structure temporaire non allouée afin d'eviter des erreurs, détruite à la fin de la fonction
	int res = shmctl(id,IPC_RMID,&p_shmid); // Appel a shmctl avec IPC_RMID pour demander la suppression du segment id.
	return res; // Renvoi du code de retour de shmctl
}

int main(int argc, char const *argv[]){

    // Lecture des arguments
    if(argc != 2){
        printf("Utilisation : ./main <nombre_ouvriers>\n");
        exit(0);
    } 
    int n = atoi(argv[1]);

    // Création du semaphore
    int sem_id;
    key_t sem_key = ftok("main.c", 0);
    // Identifiant key, 1 = un sémaphore, IPC_CREAT = créé le sémaphore
    sem_id = semget(sem_key, 2, IPC_CREAT | 0666);

    // Initialisation du sémaphore de l'ascenseur à 2, et de celui du segment de mémoire à 1
    if(semctl(sem_id, ASCENSEUR, SETVAL, 2) == -1 || semctl(sem_id, SEGMENT, SETVAL, 1) == -1){
        printf("Echec à l'initialisation du sémaphore\n");
        exit(EXIT_FAILURE);
    }

    // Création du segment de mémoire partagée
    int shmid;
    char nom[7]="main.c";
    int taille_segment = 5;
    int id = 1;
    shmid = cree_segment(nom, taille_segment, id);


    //Creation des fils
    char *mem;
    int i = 0;
    pid_t son = 1;
    while(i<n && son!=0){
        i++;
        son = fork();
    }
    // Code des fils
    if (son == 0){
        signal(SIGUSR1,sortie);
        printf("Je suis le fils de PID : %d et de PPID : %d\n",getpid(),getppid());

        struct sembuf ascenseur_buffer = {ASCENSEUR, -1, 0};
        struct sembuf segment_buffer = {SEGMENT, -1, 0};

        // Modifier les sémaphores de l'ascenseur puis du buffer (l'ordre est important !)
        semop(sem_id, &ascenseur_buffer, 1);
        semop(sem_id, &segment_buffer, 1);

		mem = shmat(shmid, NULL,0);
		if(mem ==  (void *)-1){
			printf("## Fils n'a pas réussi a s'attacher ##\n");
			exit(EXIT_FAILURE);
		}

        // Ecriture dans le segment
        char str[5];
        sprintf(str, "%d", getpid()); 
		strncpy(mem, str, 5);
    }
    //Code du père
    else{
        int first_child, second_child;
        struct sembuf segment_buffer = {SEGMENT, 1, 0};
        struct sembuf ascenseur_buffer = {ASCENSEUR, 2, 0};

        signal(SIGUSR2,empty_funct);
        // Attachement du segment de memoire partagee
		mem = shmat(shmid, NULL, 0);

        // En attente du premier fils
        pause();

        for (int i = 0; i < n; i+=2){
            // Gestion premier fils
            first_child = atoi(mem);

            // Libération du segment mémoire
            semop(sem_id, &segment_buffer, 1);

            // En attente du deuxième  fils
            pause();
            second_child = atoi(mem);

            // Fin des deux fils et libération de l'ascenceur et du segment 
            printf("Montée de %d et %d\n", first_child, second_child);
            kill(first_child,SIGUSR1);
            kill(second_child,SIGUSR1);
            semop(sem_id, &ascenseur_buffer, 1);
            semop(sem_id, &segment_buffer, 1);
            pause();
        }

        // Si il y avait un nombre impaire de fils
        if (i != n){
            // Montée du dernier fils
            first_child = atoi(mem);
            printf("Montée de %d\n", first_child);
            kill(first_child,SIGUSR1);

            // Redéfinition de la structure buffer de l'ascenseur pour ajouter une place (pas deux, car un seul fils est monté cette fois)
            ascenseur_buffer = {ASCENSEUR, 1, 0};

            // Libération de l'ascenseur et du segment
            semop(sem_id, &ascenseur_buffer, 1);
            semop(sem_id, &segment_buffer, 1);
        }

        // Détachement du segment
        shmdt(mem);

		// Destruction du segment
		if(detruire_segment(shmid) == -1){
			printf("Echec destruction, fin du programme\n");
			exit(EXIT_FAILURE);
		}
		else{
			printf("### Destruction : OK ###\n");
		}

        // Sortie du programme (pas besoin de détacher/supprimer/etc les sémaphores)
    }
}