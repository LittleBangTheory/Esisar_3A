# Exercice 2
## execl
execl : `int execl (char *fic, char * arg0, [arg1, ... argn])`
* `fic` est le nom du fichier exécutable qui sera chargé dans la zone de code du processus qui appelle `execl`. Si ce fichier n'est pas dans le répertoire courant, il faut donner son nom complet (chemin absolu).
* Les paramètres suivants sont des pointeurs sur des chaînes de caractères contenant les arguments passés à ce programme (cf. argv en C).
    * La convention UNIX impose que le premier soit le nom du fichier lui-même et que le dernier soit un pointeur nul.
* Par exemple, si on veut charger le fichier appelé prog qui se trouve dans le répertoire courant et qui n'utilise
aucun argument passé sur la ligne de commande :
``execl ("prog", "prog", (char *)0)``


## fexec
Pour utiliser fexec : `./fexec <fichier executable>`
* code C compilé
* script bash (`/bin/ls`)
* ...

# Exercice 3
## UID :
* Chaque processus possède un UID (identifiant) qui détermine ses privilèges
* Deux types 
    * UID Réel : UID de l'utilisateur qui lance le programme
    * UID Effectif (`euid`): Droits réels accordés au fichier
* En général, `uid` = `euid`, sauf en cas de modification de l'UID effectif avec `setuid`
* `getuid()` et `geteuid()` permettent de récupérer les UID
* chaque utilisateur appartient aussi à un groupe 
    * `getgid()` et `getegid()`

## `getpwuid()`
The ``getpwuid()`` function returns a pointer to a structure, defined in `<pwd.h>` as follows:

    struct passwd {
        char   *pw_name;       /* username */
        char   *pw_passwd;     /* user password */
        uid_t   pw_uid;        /* user ID */
        gid_t   pw_gid;        /* group ID */
        char   *pw_gecos;      /* user information */
        char   *pw_dir;        /* home directory */
        char   *pw_shell;      /* shell program */
    };

Usage example : 

    struct passwd *pwd;
    pwd = getpwuid(getuid());
