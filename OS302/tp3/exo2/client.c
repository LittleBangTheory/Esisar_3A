/* Objectifs :
* Ouvre en ecriture seule le tube /tmp/fifo. Echec si le tube n'existe pas
* Ecrit un texte passé en paramètre dans le tube
*/
#include <stdio.h>
#include <stdlib.h>
#define TAILLE 100

int main(int argc, char const *argv[])
{
    if(argc!=2){printf("Usage : ./client <message>\n");exit(1);}
    FILE *fic=NULL;
    fic=fopen("/tmp/fifo","w");
    if(fic==NULL){printf("Erreur ouverture !\n");exit(1);}
    //int fputs(const char *s, FILE *stream

    fputs(argv[1],fic);
    //fseek(fic,0,0);
    fputs("\0",fic);


    fclose(fic);
    return 0;
}
