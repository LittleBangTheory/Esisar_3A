# TPs de CS353, partie C (TP 1-2-3)
## Authors
* Julien Da Costa
* Gabin Chognot

## Contient
* TP1: `c_part/tp1/` - Listes chainées
* TP2: `c_part/tp2/` - Arbres binaires de recherche
* TP3: `c_part/tp3/` - Table de hachage

# Listes chainées et arbres binaires de recherche
## Compilation
`gcc liste.c -o liste` dans le dossier `c_part/tp1/` ou `c_part/tp2/` pour compiler les fichiers .c

## Analyse de complexité
* Possible avec time.sh, en ayant changé les valeurs NBLOGLINE et NBCLIENT au préalable dans les fichiers .c dans TP1 et TP2
* Exemple de résultats avec NBLOGLINE = 20000 et NBCLIENT = 20000 
```
Compilation...
...Done
---------- TP1 ----------

real    0m0.412s
user    0m0.412s
sys     0m0.000s
======= Facturation appels telephoniques ======
---------- TP2 ----------

real    0m0.009s
user    0m0.009s
sys     0m0.000s
```
On constate que la méthode avec les arbres binaires est bien plus rapide qu'avec les listes chainées.
Cela est bien en adéquation avec le cours où nous avons vu que la complexité avec les listes chainées est en n . n . p alors que celle avec les arbres dépend de la hauteur de l'arbre donc en log_2(n).

Avec `time.sh`, ce sont les champs real et user qui nous intéressent puisque real désigne le temps réél et user le temps CPU. On voit aussi que le programme avec les arbres binaires est ici 45 fois plus rapide qu'avec des listes chainées. 

# Tables de hachage
## Compilation
``` bash
# Pour compiler le main :
make 

# Pour nettoyer les fichiers .o et executables :
make clean
```

## Utilisation
``` bash
# DEUX MODES
# Mode de test des fonctionnalités :
./main -f

# Mode de test du temps d'execution :
./main -t
```

Note : Pour tester le mode de test du temps d'execution, il faut impérativement modifier la valeur de `TABLE_SIZE` dans `store.h` et recompiler avec `make`!! Cette valeur vaut 10 par défaut, mais pour avoir des résultats intéressants, il faut la mettre à 2000000 ou plus.

## Analyse 

Toutes les fonctions de `store.c` sont commentées pour expliquer leur fonctionnement.

`main.c` contient une fonction de test des différentes fonctionnalitées demandées dans le TP3.

*Note : on utilise une table réduite à une taille de 10 pour permettre de tester facilement les fonctions d'overflow et de rebuild. Mais ces fonctions marchent également avec des tailles de plusieurs milliers d'éléments.*