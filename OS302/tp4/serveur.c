#include "calcul.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Declaration des variables globales
int msg_id_send;
int msg_id_receive;

/** 
 * \fn void raz_msg()
 * \brief Fonction de suppression de la file de message
 * Supprime la file de message à la reception de n'importe quel signal
 */
void raz_msg() {
	printf("Suppression de la file de message!\n");
	msgctl(msg_id_send, IPC_RMID, NULL);
	msgctl(msg_id_receive, IPC_RMID, NULL);
	exit(0);
}

/**
 * \fn int calculation_mode()
 * \brief Fonction de calcul
 * \return EXIT_SUCCESS si le calcul s'est bien passé, EXIT_FAILURE sinon
*/
int calculation_mode(){
	msg_struct msg;
	int i_sig;
	int result;

	// Ouverture de la file de message
	key_t clef_send = ftok("calcul.h", 1);

	/* Creation de la file de message
	* IPC_CREAT = creation de la file de message
	* 0660 = droits d'acces (dialogue avec les processus d’un même ensemble d’applications)
	*/
	msg_id_send = msgget(clef_send,IPC_CREAT | 0660);
	// Verification de l'ouverture de la file de message
	if(msg_id_send == -1){
		printf("Erreur lors de la création de la file de message\n");
		return EXIT_FAILURE;
	} else {
		printf("File de message créée avec succès\nSERVEUR: pret!\n");
	}

	/* Pour tous les signaux, on appelle la fonction raz_msg
	* NSIG = nombre de signaux du systeme
	*/
	for (i_sig = 0 ; i_sig < NSIG ; i_sig++) {
		signal(i_sig, raz_msg);
	}
	msg_struct reception;
	while (1) {
		/* reception */
		/* Ouverture de la file
		* reception = structure de reception (vide au début)
		* sizeof(client_charact) = taille de la structure de reception 
		* 1 = type de message à recevoir
		* 0 = bloquant
		*/
		if(msgrcv(msg_id_send, &reception,sizeof(client_calculation),1L,0) == -1){
			perror("Erreur lors de la reception du message\n");
			return EXIT_FAILURE;
		}
		printf("SERVEUR: reception d'une requete de la part de: %d, contenant l'operation suivante:\n %d %c %d\n\n", reception.client_c.pid, reception.client_c.op1, reception.client_c.op, reception.client_c.op2);

		/* preparation de la reponse */
		msg.type=reception.client_c.pid;
		msg.client_c.pid=getpid();		
		
		// Calcul du resultat
		switch (reception.client_c.op)
		{
		case '/':
			result=reception.client_c.op1 / reception.client_c.op2;
			break;
		case 'x':
			result=reception.client_c.op1 * reception.client_c.op2;
			break;
		case '+':
			result=reception.client_c.op1 + reception.client_c.op2;
			break;
		case '-':
			result=reception.client_c.op1 - reception.client_c.op2;
			break;
		default:
			printf("Erreur opération non reconnue\n");
			exit(1);
			break;
		}

		msg.client_c.result=result;

		/* envoi de la reponse */
		msgsnd(msg_id_send, &msg, sizeof(client_calculation),0);
	}

	return EXIT_SUCCESS;
}

/**
 * \fn int translation_mode()
 * \brief Fonction de traduction du message en majuscule
 * \return EXIT_SUCCESS si la traduction s'est bien passée, EXIT_FAILURE sinon
*/
int translation_mode(){
	msg_struct msg, reception;
	int i_sig;

	// Ouverture des deux files de message
	key_t clef_receive = ftok("calcul.h", 1);
	key_t clef_send = ftok("calcul.h", 2);

	/* Creation de la file de message
	* IPC_CREAT = creation de la file de message
	* 0660 = droits d'acces (dialogue avec les processus d’un même ensemble d’applications)
	*/
	msg_id_receive = msgget(clef_receive,IPC_CREAT | 0660);
	msg_id_send = msgget(clef_send,IPC_CREAT | 0660);
	
	// Verification de l'ouverture de la file de message
	if(msg_id_send == -1 || msg_id_receive == -1){
		printf("Erreur lors de la création de la file de message\n");
		return EXIT_FAILURE;
	} else {
		printf("File de message créée avec succès\nSERVEUR: pret!\n");
	}

	/* Pour tous les signaux, on appelle la fonction raz_msg
	* NSIG = nombre de signaux du systeme
	*/
	for (i_sig = 0 ; i_sig < NSIG ; i_sig++) {
		signal(i_sig, raz_msg);
	}

	while (1) {
		/* Ouverture de la file
		* reception = structure de reception (vide au début)
		* sizeof(client_charact) = taille de la structure de reception 
		* 1 = type de message à recevoir
		* 0 = bloquant
		*/
		if(msgrcv(msg_id_receive, &reception,sizeof(client_translation),2L,0) == -1){
			perror("Erreur lors de la reception du message\n");
			return EXIT_FAILURE;
		}
		printf("SERVEUR: reception d'une requete de type %ld de la part de %d : %s\n", reception.type, reception.client_t.pid, reception.client_t.string);

		/* preparation de la reponse */
		msg.type=reception.client_t.pid;
		msg.client_t.pid=getpid();

		// Arrêt du serveur si on reçoit un '@'
		if(reception.client_t.string[0] == '@' && reception.client_t.string[1] == '\0'){
			printf("Réception d'un '@', arrêt du serveur...\n");
			strcpy(msg.client_t.string,"Arrêt du serveur");
			msgsnd(msg_id_send, &msg, sizeof(client_translation),0);
			exit(EXIT_SUCCESS);
		}
		else {
			int i = 0;
			// On traduit la chaine en majuscule
			for(i = 0; reception.client_t.string[i] != '\0'; i++){
				msg.client_t.string[i] = toupper(reception.client_t.string[i]);
			}
			msg.client_t.string[i] = '\0';

			/* envoi de la reponse */
			printf("SERVEUR: envoie de la reponse au client %d : %s\n", msg.client_t.pid, msg.client_t.string);
			msgsnd(msg_id_send, &msg, sizeof(client_translation),0);
		}
	}

	return EXIT_SUCCESS;
}

/**
 * \fn int main(int argc, char const *argv[])
 * \brief Fonction principale du serveur
 * \param argc Nombre d'arguments
 * \param argv Tableau des arguments
 * \return EXIT_SUCCESS si le serveur s'est bien lancé, EXIT_FAILURE sinon
*/
int main(int argc, char const *argv[]){
	if(argc == 2 && strcmp(argv[1],"-c") == 0){
		printf("Calculation mode\n");
		calculation_mode();
	} else if(argc == 2 && strcmp(argv[1],"-t") == 0){
		printf("Translation mode\n");
		translation_mode();
	} else {
		printf("Usage:\nCalculation mode : %s -c\nTranslation mode : %s -t\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
