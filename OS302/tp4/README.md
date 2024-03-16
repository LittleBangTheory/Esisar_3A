# TP4 d'OS302

## Auteurs
* Julien Da Costa
* Gabin Chognot

## Contient
* [client.c](client.c) : le fichier client qui envoie la requête
* [serveur.c](serveur.c) : le fichier serveur qui crée la(les) file(s) et traite la requête
* [calcul.h](calcul.h) : le fichier header qui contient les structures qui stockent les éléments de l'opération
* [Makefile](Makefile) : le fichier pour générer les programmes client et serveur

## Comment utiliser
### Compilation
```
make 
make clean
```

### Mode arithmétique 
*Description : le client envoie au serveur, dans une file, une opération mathématique à effectuer, le serveur réalise le calcul et renvoie la réponse dans la même file.*

Dans deux terminaux différents :
```
./serveur -c

./client -c <op1> <op> <op2>
```

### Mode traduction 
*Description : le client envoie au serveur, dans une file, un texte passé en paramètre de l'appel, le serveur traduit le texte (transforme tout en majuscules) et renvoie le texte traduit dans une autre file.*

Dans deux terminaux différents :
```
./serveur -t

./client -t <texte>
```
---
## Comprendre le code
*Pour une analyse détaillée, regarder les commentaires du code source.*

### `int main(int argc, char const *argv[])`
Même fonction pour les deux fichiers, elle analyse les arguments passés en paramètre et appelle la fonction correspondante.

---
### Mode arithmétique :
Le serveur crée une file de message avec la clef `clef_send`. 

Le client initialise une structure `msg_struct` contenant : 
* Le type du message (0)
* Une structure `client_calculation` contenant :
    * L'opération à effectuer (op1, op, op2)
    * Le pid du client (pour pouvoir répondre au bon client)
* Une structure `client_translation`, vide car inutile dans ce mode

Le serveur receptionne l'opération, effectue le calcul et renvoie dans la même file une nouvelle structure contenant :
* Le type du message (`client_pid`)
* La réponse à l'opération

Le client reçoit la réponse et l'affiche.

---
### Mode traduction :
Même fonctionnement général que le mode arithmétique. Différences : 

Le serveur crée deux files de message avec les clefs `clef_send` et `clef_recv`, auxquelles le client se connecte. 

Le client initialise une structure `msg_struct` contenant : 
* Le type du message (0)
* Une structure `client_calculation` vide car inutile dans ce mode
* Une structure `client_translation`, contenant :
    * Le type du message (2)
    * Le texte à traduire (longueur maximale de 100 caractères)
    * Le pid du client (pour pouvoir répondre au bon client)

