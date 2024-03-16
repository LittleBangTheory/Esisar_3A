#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main(void)
{   
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    printf("Reel : %s(UID=%d), %s(GID=%d)\n", pwd->pw_name, getuid(), pwd->pw_name, getgid());
    printf("Effectif : %s(UID=%d), %s(GID=%d)\n", pwd->pw_name, geteuid(), pwd->pw_name, getegid());
    return 0;
}