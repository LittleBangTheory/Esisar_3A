#ifndef __CALCUL_H__
#define __CALCUL_H__

/**
 * \struct client_calculation
 * \brief Structure contenant les informations d'une requete de calcul
 * \param pid : pid du client
 * \param op1 : premier operande
 * \param op : operateur
 * \param op2 : second operande
 * \param result : resultat de l'operation
*/
typedef struct client_calculation {
    int pid;
    int op1;
    char op;
    int op2;
    int result;
}client_calculation;

/**
 * \struct client_translation
 * \brief Structure contenant les informations d'une requete de traduction
 * \param pid : pid du client
 * \param string : chaine de caractere a traduire (100 caracteres max)
*/
typedef struct client_translation {
    int pid;
    char string[80];
}client_translation;

/**
 * \struct msg_struct
 * \brief Structure contenant les informations d'une requete
 * \param type : type de la requete
 * \param client_c : requete de calcul (non initialisee si requete de traduction)
 * \param client_t : requete de traduction (non initialisee si requete de calcul)
*/
typedef struct msg_struct {
    long type;
    client_calculation client_c;
    client_translation client_t;
}msg_struct;

#endif /*__CALCUL_H__*/
