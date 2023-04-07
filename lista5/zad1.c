#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    if (setuid(0) < 0) {
        perror("setuid");
        return 1;
    }

    execl("/bin/bash", NULL);

    return 0;
}

// sudo gcc zad1.c
// sudo chmod u+s a.out
