# include <stdio.h>
# include <unistd.h>

int main (void) {
    printf("Coucou, ici %d !\n", getpid() );
    sleep (4);
    return 6;
}