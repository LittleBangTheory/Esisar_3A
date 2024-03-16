#include <stdlib.h>
#include <stdio.h>

typedef struct Client {
	int numero;
	int prixAppel;
	int nbAppel;
	struct Client * suivant;
} client;


// Nombre total de lignes dans le fichier
#ifndef NBLOGLINE
#define NBLOGLINE    10000*2
#endif

// Nombre de clients
#ifndef NBCLIENT
#define NBCLIENT    20000
#endif

/* QUESTION 1
Réalisez une structure Client permettant de stocker un numéro de téléphone (sous la forme d'un int),
un nombre d'appel, un coût total en centimes d'euros, une référence vers cette structure (structure auto référentielle).
*/
struct Client *createClient(int numero, int nbAppel, int prixAppel)
{
	client* ret = malloc(sizeof(client));
	ret->numero=numero;
	ret->prixAppel=prixAppel;
	ret->nbAppel=nbAppel;
	ret->suivant=NULL;
	return ret;
}


/* QUESTION 2
Réalisez une fonction qui insère une ligne du fichier de log dans la liste chaînée.
Le fonctionnement de cette méthode sera le suivant:
-vous naviguez dans la liste pour trouver si le client (identifié par son numéro de téléphone) est présent dans la liste
-si vous trouvez le client: vous incrémentez de 1 le nombre d'appel et vous augmentez le coût total du prix de l'appel
-si vous ne trouvez pas le client, vous l'insérez à la bonne position, et vous le créditez du prix de l'appel et de 1 appel.
La liste chaînée doit être triée en permanence: si vous voulez insérer le client 4, il faut l'insérer entre le client 2 et le client 6.
*/
int addLogLine(struct Client **liste, int numero, int prixAppel)
{
	if(*liste==NULL){*liste=createClient(numero,1,prixAppel);return 1;}
	client* node=*liste;
	client* prev=NULL;
	while(node!=NULL && numero>node->numero){
		prev=node;
		node=node->suivant;
	}
	if(node!=NULL && node->numero==numero){ //cas d'existence : OK
		node->nbAppel=node->nbAppel+1;
		node->prixAppel=node->prixAppel+prixAppel;
	}
	else if (prev!=NULL){
		prev->suivant=createClient(numero,1,prixAppel);
		prev->suivant->suivant=node;
	}
	else{
		*liste=createClient(numero,1,prixAppel);
		(*liste)->suivant=node;
	}

	return 1;
}

void dumpListe(struct Client *liste){
	if (liste==NULL) {
		printf("FIN LISTE\n\n");
		return;
	}
	printf("numero : %d\n",liste->numero);
	printf("prixAppel : %d\n",liste->prixAppel);
	printf("nbAppel : %d\n",liste->nbAppel);
	printf("________________\n");
	dumpListe(liste->suivant);
}

int main()
{
    client *liste=NULL;

    int i;
    int numeroTel;
    int prixAppel;


		/*TESTS


		liste=createClient(1,1,1);
		liste->suivant=createClient(3,1,1);
		affiche(liste);
		addLogLine(&liste,3,90);
		affiche(liste);
		addLogLine(&liste,2,90);
		affiche(liste);

		*/

    // Aide au calcul du pourcentage d'avancement
    int pas = NBLOGLINE/100;

    printf("****** Facturation appels telephoniques ******\n");


    for(i=0;i<NBLOGLINE;i++)
    {

        // Génération d'un numéro de telephone portable
        numeroTel = 600000000+(rand() % NBCLIENT);

        // Donne un prix d'appel compris entre 0.01 et 4 euros
        prixAppel = (rand() % 400)+1;

        // Ajout de cette ligne de log dans la liste des clients
   	if (!addLogLine(&liste,numeroTel,prixAppel)) break;
        //
        // printf("numero=%d prix = %d\n",numeroTel,prixAppel);


        // Affichage du pourcentage d'avancement
        if ((i % pas)==0)
        {
             printf("Done  = %d %%...\n",i/pas);
        }
    }

   dumpListe(liste);
   printf("======= Facturation appels telephoniques ======\n");

   return 0;
}
