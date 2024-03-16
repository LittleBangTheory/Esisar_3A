/******************************************************************************
 * Implementation du module gestion de stock d'un magasin
 * avec une table de hachage
 ******************************************************************************/
#ifndef __STORE_H__
#include "store.h"
#define __STORE_H__
#endif

/*----------------------------------------------------------------------------
 * Variable globale contenant le tableau
 *----------------------------------------------------------------------------*/
Item hash_table[TABLE_SIZE];
Item *index_table[TABLE_SIZE];

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*----------------------------------------------------------------------------
 * Cette fonction initialise le tableau hash_table
 * en positionnant tous les elements à NULL_ITEM
 *----------------------------------------------------------------------------*/
void init()
{
    
    int i;
    for(i=0;i<TABLE_SIZE;i++)
    {
        hash_table[i].status = NULL_ITEM;
        hash_table[i].price = 0.00f;
        hash_table[i].code = 0;
    }
}

/*----------------------------------------------------------------------------
 * Cette fonction calcule la valeur de hachage pour le produit itemCode
 *----------------------------------------------------------------------------*/
uint32_t hashkey(uint32_t itemCode,uint32_t nbTry){
    return (((itemCode%TABLE_SIZE)+nbTry*(1+itemCode%(TABLE_SIZE -1)))%TABLE_SIZE); 
}

/**
 * \fn unsigned int hashIndex(const char *buffer, int size)
 * \brief Hache un libellé en nombre
 * \param buffer le libellé à hacher
 * \param size la taille du libellé
*/
unsigned int hashIndex(const char *buffer, int size) {
    unsigned int h = 0;
    for (int i=0; i<size; i++){
        h = ( h * 1103515245u ) + 12345u + buffer[i];
    }
    return h/2;
}

/*----------------------------------------------------------------------------
 * Cette fonction insère le produit indiqué dans la table de hachage.
 * Si le produit est inséré avec succès, alors la fonction retourne SUCCESS (0)
 * Si le produit existe déjà dans la table, alors la fonction retourne INSERT_ALREADY_EXIST (-1),
 * et la table de hachage n'est pas modifiée
 * Si la table est pleine, alors la fonction retourne TABLE_FULL (-2).
 *----------------------------------------------------------------------------*/
int insertItem(uint32_t itemCode, char* itemName, float itemPrice){ 
    int index;
    int mem_index = -1;
    for(int i = 0; i < TABLE_SIZE; i++){
        // Calcul du hash
        index = hashkey(itemCode,i);
        // Si le produit existe déjà
        if (hash_table[index].code == itemCode && hash_table[index].status == USED_ITEM){
            return INSERT_ALREADY_EXIST;
        }
        // Si on trouve un emplacement deleted, on le garde en mémoire jusqu'au prochain emplacement vide
        else if (hash_table[index].status == DELETED_ITEM && mem_index == -1){
            mem_index = index;
        } 
        // Si on trouve un emplacement vide
        else if(hash_table[index].status == NULL_ITEM){
            // Si on avait trouvé un emplacement deleted, on l'utilise, sinob on utilise l'emplacement vide
            if (mem_index != -1){
                index = mem_index;
            }
            hash_table[index].status = USED_ITEM;
            hash_table[index].code = itemCode;
            strcpy(hash_table[index].name,itemName);
            hash_table[index].price = itemPrice;
            return SUCCESS;
        }
    }
    // Si on n'a pas trouvé d'emplacement vide ou deleted, on s'arrête
    if (mem_index == -1){
        return TABLE_FULL;
    } 
    // Si on a pas trouvé d'emplacement vide, on utilise l'emplacement deleted
    else {
        hash_table[mem_index].status = USED_ITEM;
        hash_table[mem_index].code = itemCode;
        strcpy(hash_table[mem_index].name,itemName);
        hash_table[mem_index].price = itemPrice;
        return SUCCESS;
    }
}

/*----------------------------------------------------------------------------
 * fonction de suppression d'un produit du magasin
 * Si le produit est supprimé avec succès, alors la fonction retourne SUCCESS (0)
 * Si le produit n'existe pas, alors la fonction retourne DELETE_NO_ROW (-4)
 *----------------------------------------------------------------------------*/
int suppressItem(unsigned int itemCode){
    int index;
    for(int i = 0; i < TABLE_SIZE; i++){
        // Calcul du hash
        index = hashkey(itemCode,i);

        // On cherche le produit
        if (hash_table[index].code == itemCode){
            hash_table[index].status = DELETED_ITEM;
            return SUCCESS;
        }
    }
    return DELETE_NO_ROW;
}

/*----------------------------------------------------------------------------
 * Pour chaque produit, cette fonction affiche sur une ligne
 * le code du produit
 * son libellé
 * son prix
 * son index dans la table de hashage
 * sa valeur de hash
 *----------------------------------------------------------------------------*/
void dumpItems(){
    int empty = 1;
    printf("CODE \t LIBELLE \t PRIX \t\t INDEX \t STATUS\n");
    for(int i = 0; i < TABLE_SIZE; i++){
        // On cherche le produit
        if (hash_table[i].status == USED_ITEM){
            printf("%d \t %s \t %f \t %d \t %d\n", hash_table[i].code, hash_table[i].name, hash_table[i].price, i, hash_table[i].status);
            empty = 0;
        }
    }
    if (empty) printf("La table est vide\n");
    printf("\n");
}

/*----------------------------------------------------------------------------
 * Cette fonction trouve le produit dont le code est itemCode.
 * Cette fonction retourne NULL si le produit n'existe pas.
 * Cette fonction retourne un pointeur vers le produit si le produit existe.
 *----------------------------------------------------------------------------*/
Item *getItem(unsigned int itemCode){
    int index;
    for(int i = 0; i < TABLE_SIZE; i++){
        // Calcul du hash
        index = hashkey(itemCode,i);

        // On cherche le produit
        if (hash_table[index].code == itemCode){
            return &hash_table[index];
        }
        // Si on trouve un emplacement vide, on s'arrête
        else if (hash_table[index].status == NULL_ITEM){
            return NULL;
        }
    }
    return NULL;
}

/*----------------------------------------------------------------------------
 * la fonction de recherche d'un prix d'un produit:
 *----------------------------------------------------------------------------*/
float getPrice(int itemCode){
    int index;
    for(int i = 0; i < TABLE_SIZE; i++){
        // Calcul du hash
        index = hashkey(itemCode,i);

        // On cherche le produit
        if (hash_table[index].code == itemCode){
            return (hash_table[index].price);
        }
    }
    return DELETE_NO_ROW;
}

/*----------------------------------------------------------------------------
 *  fonction de mise à jour d'un produit :
 * Si le produit est mis à jour avec succès, alors la fonction retourne SUCCESS (0)
 * Si le produit n'existe pas, alors la fonction retourne UPDATE_NO_ROW (-5)
 *----------------------------------------------------------------------------*/
int updateItem(unsigned int itemCode, char *itemName, float itemPrice)
{
    int index;
    for(int i = 0; i < TABLE_SIZE; i++){
        // Calcul du hash
        index = hashkey(itemCode,i);

        // On cherche le produit
        if (hash_table[index].code == itemCode){
            strcpy(hash_table[index].name,itemName);
            hash_table[index].price = itemPrice;
            return SUCCESS;
        }
    }
    return UPDATE_NO_ROW;
}

/*----------------------------------------------------------------------------
 * la fonction de réorganisation in situ:
    * On positionne sur chaque clef dans la table un bit pour indiquer si la clef « sale » ou « propre. » 
    * Au début, toutes les clefs sont sales. 
    * On balaye le tableau jusqu'à la prochaine clef sale et on la rehash pour lui trouver une nouvelle location. 
    * Si elle atterrit dans une case vide
        * Sa réinsertion est terminée. 
    * Si elle atterrit sur une case sale
        * On échange la clef sale avec la clef maintenant propre
        * On réinsère la clef que nous venons de retirer du tableau. 
    * Si la clef atterrit sur une clef propre
        * On utilise la résolution de collision pour lui trouver une nouvelle adresse.
 *----------------------------------------------------------------------------*/
void rebuildTable(){
    // Initialisation du bit de saleté
    for(int i = 0; i < TABLE_SIZE; i++){
        if (hash_table[i].status == USED_ITEM){
            hash_table[i].dirty = DIRTY_ITEM;
        }
    }

    int j;

    // On parcourt la table
    for(int i = 0; i < TABLE_SIZE; i++){
        j = 0;
        // Si la case origine est sale
        if (hash_table[i].dirty == DIRTY_ITEM){
            while(j != -1){
                // On la rehash
                int index = hashkey(hash_table[i].code,j);
                printf("Deplacement de l'élément de code %d à la case %d\n",hash_table[i].code,index);
                // Si on est déjà au bon endroit
                if(index == i){
                    hash_table[index].dirty = CLEAN_ITEM;
                    j = -1;
                }
                // Si la case destination est vide
                else if (hash_table[index].status == NULL_ITEM || hash_table[index].status == DELETED_ITEM){
                    // On la réinsère
                    hash_table[index].status = USED_ITEM;
                    hash_table[index].code = hash_table[i].code;
                    strcpy(hash_table[index].name, hash_table[i].name);
                    hash_table[index].price = hash_table[i].price;
                    hash_table[index].dirty = CLEAN_ITEM;

                    // On marque la case à i comme DELETED
                    hash_table[i].status = DELETED_ITEM;
                    j = -1;
                    printf("Case vide, insertion réussie\n");
                }
                // Si la case destination est sale
                else if (hash_table[index].dirty == DIRTY_ITEM){
                    // On échange les cases
                    Item temp = hash_table[i];
                    hash_table[i] = hash_table[index];
                    hash_table[index] = temp;
                    hash_table[index].dirty = CLEAN_ITEM;
                    j = 0;
                    printf("Case sale, on échange, on insère l'élément de code %d\n", hash_table[i].code);
                }
                // Si la case est propre
                else{
                    j++;
                }
            }
        }
    }
}

/**
 * \fn Result *findItem(char* itemName)
 * \brief Fonction de recherche d'un produit dans la table de hachage (pointeurs)
 * \param itemName le nom du produit à rechercher
*/
Result* findItem(char* itemName){ // L'idée est de parcourir toute la table.
    Result* head = NULL;
    Result* node = NULL;
    Result* res = NULL;
    for (size_t i = 0; i < TABLE_SIZE; i++) // Temps de recherche en O(n) avec n la taille de la table de hashage puisque l'on compare chaque element pour voir si il correspond.
    {
        if (strcmp(itemName,hash_table[i].name)==0 && hash_table[i].status==1) // SI les noms correspondent
        {
            if (head==NULL) // Premier HIT
            {
                head=malloc(sizeof(Result));
                head->item=&hash_table[i];
                res=head; // On garde la tête en memoire pour le retour
            }
            else // Dexieme hit ou plus, head vient dêtre remplie, node est vide
            {
                node = malloc(sizeof(Result));
                node->item=&hash_table[i];
                node->next=NULL;
                head->next=node;
                head=node;
            }
        }
    }
    return res;
}
/*----------------------------------------------------------------------------
 * Cette fonction initialise le tableau de pointeurs index_table
 * en positionnant tous les pointeurs vers la vraie hash_table
 *----------------------------------------------------------------------------*/
void init_index_table()
{
    int ind;
    int try;
    for (size_t i = 0; i < TABLE_SIZE; i++)
    {
        try=0;
        ind = hashkey(hashIndex(hash_table[i].name,strlen(hash_table[i].name)),try);
        while (index_table[ind] != NULL) // Tant que la place n'est pas libre
        {
            try++;
            ind = hashkey(hashIndex(hash_table[i].name,strlen(hash_table[i].name)),try);
        }// La place est libre
        index_table[ind] = &hash_table[i];
        
    }
    
}
/**
 * \fn Result *findItemWithIndex(char* itemName)
 * \brief Fonction de recherche d'un produit dans la table de hachage 
 * \param itemName le nom du produit à rechercher
*/
Result *findItemWithIndex(char* itemName){ //Fonction améliorée de findItem car au lieu de parcourir chaque elements, on va seulement regarder les emplacements où l'item a pu être ajouté grâce à son hash.
    init_index_table(); // Initialisation tableau de pointeurs
    Result* head = NULL;
    Result* node = NULL;
    Result* res = NULL;
    int index = hashIndex(itemName,strlen(itemName));
    index = hashkey(index,0);
    int j = 0;
    if (index_table[index]==NULL) return NULL;
    while(index_table[index]->status == USED_ITEM){
        if (strcmp(itemName,index_table[index]->name)==0) // SI les noms correspondent ; temps de recherche en O(1) dans le meilleur des cas et en O(n) dans le pire des cas, ce dernier étant extrêmement improbable car il faudrait qu'à chaque HIT l'élément soit occupé. Peut arriver si la table n'est pas nettoyée assez souvent. 
        {
            if (head==NULL) // Premier HIT
            {
                head=malloc(sizeof(Result));
                head->item=index_table[index];
                res=head; // On garde la tête en memoire pour le retour
            }
            else // Dexieme hit ou plus, head vient dêtre remplie, node est vide
            {
                node = malloc(sizeof(Result));
                node->item=index_table[index];
                node->next=NULL;
                head->next=node;
                head=node;
            }
        }
        j++;
        index = hashIndex(itemName,strlen(itemName));
        index = hashkey(index,j);
    }
    return res;
}
