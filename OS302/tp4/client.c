#include "calcul.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/**
 * \fn int calculation_mode(char const *argv[])
 * \brief Fonction de calcul
 * \param argv[] : tableau des arguments pour récupérer les opérandes et l'opérateur
 * \return EXIT_SUCCESS si le calcul s'est bien passé, EXIT_FAILURE sinon
*/
int calculation_mode(char const *argv[]){
	// Declaration des variables
	int msg_id;
	msg_struct msg;

	// Ouverture de la file de message
	key_t clef = ftok("calcul.h", 1);
	msg_id = msgget(clef, 0);

	// Verification de l'ouverture de la file de message
	if(msg_id == -1){
		printf("Erreur lors de la création de la file de message\n");
		return EXIT_FAILURE;
	} else {
		printf("File de message ouverte avec succès\n");
	}

	// Affichage de l'operation effectuée
	printf("CLIENT %d: preparation du message contenant l'operation suivante: %d %c %d\n", getpid(), atoi(argv[2]), argv[3][0], atoi(argv[4]));

	// Initialisation du message
	msg.type = 1;
	// Un client est identifié par son pid
	msg.client_c.pid = getpid();
	msg.client_c.op1 = atoi(argv[2]);
	msg.client_c.op = argv[3][0];
	msg.client_c.op2 = atoi(argv[4]);
	// Initialisation du resultat à 0 pour éviter les erreurs de lecture
	msg.client_c.result = 0;
	
	/* Envoie du message
	* options = 0
	* on récupère la taille du message grace au sizeof() de la structure client_charact, donc on ne prend pas en compte le type du message dans la taille
	*/
	msgsnd(msg_id, &msg, sizeof(msg.client_c), 0);

	// Reception de la reponse du serveur, d'identifiant le pid du client
	msgrcv(msg_id, &msg, sizeof(msg.client_c), getpid(), 0);
	// Affichage du resultat
	printf("CLIENT: resultat recu depuis le serveur %d : %d\n", msg.client_c.pid, msg.client_c.result);

	return EXIT_SUCCESS;
}

/**
 * \fn int translation_mode(char const *argv[])
 * \brief Fonction de traduction
 * \param argv[] : tableau des arguments pour récupérer le texte à traduire
 * \return EXIT_SUCCESS si la traduction s'est bien passée, EXIT_FAILURE sinon
*/
int translation_mode(char const *argv[]){
	// Declaration des variables
	int msg_id_send, msg_id_receive;
	msg_struct msg;

	// Creation des clefs (1 ou 2 = identifiant)
	key_t clef_send = ftok("calcul.h", 1);
	key_t clef_receive = ftok("calcul.h", 2);
	// Ouverture de la file de message (0 = options)
	msg_id_send = msgget(clef_send, 0);
	msg_id_receive = msgget(clef_receive, 0);

	// Verification de l'ouverture de la file de message
	if(msg_id_send == -1 || msg_id_receive == -1){
		printf("Erreur lors de la création de la file de message\n");
		return EXIT_FAILURE;
	} else {
		printf("File de message ouverte avec succès\n");
	}

	// Affichage de l'operation effectuée
	printf("CLIENT %d: preparation du message contenant le texte suivant : %s\n", getpid(), argv[2]);

	// Initialisation du message
	msg.type = 2;
	// Un client est identifié par son pid
	msg.client_t.pid = getpid();

	// Recopie de la chaine de caractere
	strncpy(msg.client_t.string, argv[2], strlen(argv[2]));

	/* Envoie du message
	* options = 0
	* on récupère la taille du message grace au sizeof() de la structure client_charact, donc on ne prend pas en compte le type du message dans la taille
	*/
	printf("Message send : %d\n",msgsnd(msg_id_send, &msg, sizeof(client_translation), 0));

	// Reception de la reponse du serveur, d'identifiant le pid du client
	printf("Message received : %ld\n",msgrcv(msg_id_receive, &msg, sizeof(client_translation), getpid(), 0));
	// Affichage du resultat
	printf("CLIENT: reponse recu depuis le serveur %d : %s\n", msg.client_t.pid, msg.client_t.string);

	return EXIT_SUCCESS;
}

/**
 * \fn int main(int argc, char const *argv[])
 * \brief Fonction principale
 * \param argc : nombre d'arguments
 * \param argv[] : tableau des arguments
 * \return EXIT_SUCCESS si le programme s'est bien passé, EXIT_FAILURE sinon
*/
int main(int argc, char const *argv[])
{
	// Verification des arguments
	if(argc == 5 && strcmp(argv[1],"-c") == 0){
		// Verification de l'absence de division par 0 
		if(atoi(argv[4]) == 0 && argv[3][0] == '/'){
			printf("Division par 0 impossible\n");
			return EXIT_FAILURE;
		}
		printf("Calculation mode\n");
		calculation_mode(argv);
	} else if(argc == 3 && strcmp(argv[1],"-t") == 0){
		if(strlen(argv[2]) > 80){
			printf("Texte trop long : longueur maximale = 80 caractères\n");
			return EXIT_FAILURE;
		}
		printf("Translation mode\n");
		translation_mode(argv);
	} else {
		printf("Usage:\nCalculation mode : %s -c operande1 {+|-|x|/} operande2\nTranslation mode : %s -t \"text\"\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
