#include <stdlib.h> 
#include <stdio.h> 

typedef struct Client { 
	int numero; 
	int prixAppel; 
	int nbAppel; 
	struct Client * gauche;
	struct Client * droite;
 
} client; 

// Nombre total de lignes dans le fichier
#ifndef NBLOGLINE
#define NBLOGLINE    10000*2
#endif
// Nombre de clients
#ifndef NBCLIENT
#define NBCLIENT    20000
#endif


struct Client * creerClient(int numeroTel, int nbAppel,int cout)
{
    client* ret = malloc(sizeof(client));
	ret->numero=numeroTel;
	ret->prixAppel=cout;
	ret->nbAppel=nbAppel;
	ret->gauche=NULL;
    ret->droite=NULL;
	return ret;
}

struct Client * createSampleTree(){
    struct Client *root = creerClient(15, 1, 1);
    root->gauche = creerClient(12, 1, 1);
    root->droite = creerClient(20, 1, 1);
    root->gauche->gauche = creerClient(8, 1, 1);
    root->gauche->droite = creerClient(14, 1, 1);
    root->gauche->gauche->droite = creerClient(10, 1, 1);
    root->gauche->droite->gauche = creerClient(13, 1, 1);
    root->droite->gauche = creerClient(16, 1, 1);
    root->droite->droite = creerClient(21, 1, 1);
    root->droite->gauche->droite = creerClient(17, 1, 1);

    printf("****** Facturation appels telephoniques ******\n");
    printf("Numero : %d\n", root->numero);
    printf("Numero : %d\n", root->gauche->numero);
    printf("Numero : %d\n", root->droite->numero);
    printf("Numero : %d\n", root->gauche->gauche->numero);
    printf("Numero : %d\n", root->gauche->droite->numero);

    return root;
}

struct Client * chercher(struct Client * abr,int numeroTel){
    if(abr == NULL){
        return NULL;
    }
    else if(numeroTel == abr->numero){
        return abr;
    }
    else {
        if(numeroTel < abr->numero){
            return chercher(abr->gauche, numeroTel);
        }
        else {
            return chercher(abr->droite, numeroTel);
        }
    }
}

struct Client *inserer(struct Client ** abr, int numeroTel, int prixAppel){
    // Si l'arbre est vide, on crée un nouveau client
    if(*abr == NULL){
        *abr = creerClient(numeroTel, 1, prixAppel);
        return *abr;
    }
        if(numeroTel == (*abr)->numero){
            (*abr)->nbAppel++;
            (*abr)->prixAppel += prixAppel;
            return *abr;
        }
        else {
            if(numeroTel < (*abr)->numero){
                return inserer(&((*abr)->gauche), numeroTel, prixAppel);
            }
            else {
                return inserer(&((*abr)->droite), numeroTel, prixAppel);
            }
        }
}

struct Client *supprimerClient(struct Client ** abr, int numeroTel){
    if(*abr == NULL){
        return NULL;
    }
    else {
        if(numeroTel == (*abr)->numero){
            // Si le client n'a pas d'enfant
            if((*abr)->gauche == NULL && (*abr)->droite == NULL){
                struct Client *tmp = *abr;
                *abr = NULL;
                return tmp;
            }
            // Si le client a un enfant à droite
            else if((*abr)->gauche == NULL){
                struct Client *tmp = *abr;
                *abr = (*abr)->droite;
                return tmp;
            }
            // Si le client a un enfant à gauche
            else if((*abr)->droite == NULL){
                struct Client *tmp = *abr;
                *abr = (*abr)->gauche;
                return tmp;
            }
            // Si le client a deux enfants
            else {
                struct Client *tmp = (*abr)->droite;
                while(tmp->gauche != NULL){
                    tmp = tmp->gauche;
                }
                (*abr)->numero = tmp->numero;
                (*abr)->nbAppel = tmp->nbAppel;
                (*abr)->prixAppel = tmp->prixAppel;
                return supprimerClient(&((*abr)->droite), tmp->numero);
            }
        }
        else {
            if(numeroTel < (*abr)->numero){
                return supprimerClient(&((*abr)->gauche), numeroTel);
            }
            else {
                return supprimerClient(&((*abr)->droite), numeroTel);
            }
        }
    }
}


void parcourirInfixe(struct Client * abr){
    if(abr != NULL){
        parcourirInfixe(abr->gauche);
        printf("Numero : %d\n", abr->numero);
        parcourirInfixe(abr->droite);
    }
}	

int main() 
{
    //client *liste=createSampleTree();
    int i;
    int numeroTel;
    int prixAppel;
    client* liste = malloc(sizeof(client));

    // Aide au calcul du pourcentage d'avancement
    int pas = NBLOGLINE/100;
    for(i=0;i<NBLOGLINE;i++)
    {

        // Génération d'un numéro de telephone portable
        numeroTel = 600000000+(rand() % NBCLIENT);

        // Donne un prix d'appel compris entre 0.01 et 4 euros
        prixAppel = (rand() % 400)+1;

        // Ajout de cette ligne de log dans la liste des clients
   	if (inserer(&liste ,numeroTel,prixAppel)==NULL) break; 

        // Affichage du pourcentage d'avancement
        if ((i % pas)==0)
        {
             printf("Done  = %d %%...\n",i/pas);
        }
    }

    printf("****** Facturation appels telephoniques ******\n");

    parcourirInfixe(liste);

    printf("****** Suppression de la facturation appels telephoniques ******\n");

    for (i=0;i<NBCLIENT;i++) {
	 client *tmp=NULL; 
	if ((tmp=supprimerClient(&liste,600000000+i)) != NULL) { free(tmp); }
    }
    printf("****** Fin Facturation appels telephoniques ******\n");    
}


