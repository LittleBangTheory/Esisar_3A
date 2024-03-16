/*******************************************************************************
 * OS302 - shm - Exercice 2                                                    *
 * Utilisation des segments de memoire partagee                                *
 ******************************************************************************/

#include "segment_memoire.h"

#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <string.h>

#define NOM "exo2.c"
#define SLEEP_TIME 2
#define TAILLE 100
#define ID 1

void empty_funct(){}

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

/**
 * \fn int afficher_info_segment(int shmid)
 * \brief Affiche toutes les informations concernant un segment de memoire
 * \param shmid l'identificateur du segment
 * \return -1 en cas d'erreur 
 * int shmctl (int shmid, int op, struct shmid_ds *p_shmid)
 * struct shmid_ds {
    struct ipc_perm shm_perm;		Appartenace et permissions
    size_t          shm_segsz;		Taille segment en octets
    time_t          shm_atime;		Heure dernier attachement
    time_t          shm_dtime;		Heure dernier détachement
    time_t          shm_ctime;		Heure dernière modification
    pid_t           shm_cpid;		PID du créateur
    pid_t           shm_lpid;		PID du dernier shmat(2)/shmdt(2)
    shmatt_t        shm_nattch;		Nombre d'attachements actuels   
	};
*/
int afficher_info_segment(int shmid){
	// Initialisation de la structure de reception
	struct shmid_ds* buffer = malloc(sizeof(struct shmid_ds));

	// Reception de l'information
	int index = shmctl(shmid, IPC_STAT, buffer);

	// Affichage des informations
	printf("##### Informations sur la structure : #####\n");
	printf("Taille segment en octets\t\t%zu\n", buffer->shm_segsz);
	printf("Heure dernier attachement\t\t%ld\n", buffer->shm_atime);
	printf("Heure dernier détachement\t\t%ld\n", buffer->shm_dtime);
	printf("Heure dernière modification\t\t%ld\n", buffer->shm_ctime);
	printf("PID du créateur\t\t\t\t%d\n", buffer->shm_cpid);
	printf("PID du dernier shmat(2)/shmdt(2)\t%d\n", buffer->shm_lpid);
	printf("Nombre attachements actuels\t\t%ld\n\n",buffer->shm_nattch);

	// Retour
	return index;
}

int detruire_segment(int id){
	struct shmid_ds p_shmid; // Structure temporaire non allouée afin d'eviter des erreurs, détruite à la fin de la fonction
	int res = shmctl(id,IPC_RMID,&p_shmid); // Appel a shmctl avec IPC_RMID pour demander la suppression du segment id.
	return res; // Renvoi du code de retour de shmctl
}

int main(int argc, char const *argv[])
{
	if(argc != 2){
		printf("Utilisation : ./exo2 <chaine>\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	char *mem;
	int shmid;
	int taille_segment = strlen(argv[1]);

	/* Création de :
	 * Une clef à partir de NOM et ID
	 * Un segment d'id shmid à partir de la clef et de TAILLE */
	shmid = cree_segment(NOM, taille_segment, ID);
	if(shmid == -1){
		printf("Echec de la création, fin du programme\n");
		exit(EXIT_FAILURE);
	}

	switch(pid = fork()) {
	case -1: // ECHEC
		perror("fork");
		return -1;
	case 0: // FILS
		sleep(SLEEP_TIME);
		/*s'attacher au segment et affichage de son contenu */
		mem = shmat(shmid, NULL,SHM_RDONLY);
		if(mem ==  (void *)-1){
			printf("## Fils n'a pas réussi a s'attacher ##\n");
			exit(EXIT_FAILURE);
		}
		printf("Message reçu :\t%s\n",mem);
		kill(getppid(),SIGUSR1);
		break;
	default: // PERE
		signal(SIGUSR1,empty_funct);
		// Attachement du segment de memoire partagee
		mem = shmat(shmid, NULL, 0);

		// Ecriture dans le segment
		strncpy(mem, argv[1], taille_segment);
		
		// Attente de la fin du fils
		pause();

		// Détachement du segment mémoire
		shmdt(mem);

		// Destruction du segment
		if(detruire_segment(shmid) == -1){
			printf("Echec destruction, fin du programme\n");
			exit(EXIT_FAILURE);
		}
		else{
			printf("### Destruction : OK ###\n");
		}
	}

	return 0;
}
