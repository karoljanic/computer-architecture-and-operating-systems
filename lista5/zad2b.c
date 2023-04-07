#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    if(geteuid() != 0) {
        printf("Sudo is needed.\n");
        exit(1);
    }

    if(kill(1, SIGUSR1) == 0) { 
        printf("SIGUSR1 sent!\n");
    }

    if(kill(1, SIGKILL) == 0) { 
        printf("SIGKILL sent!\n");
    }

    return 0;
}

// sudo gcc zad2b.c
// sudo chmod u+s a.out