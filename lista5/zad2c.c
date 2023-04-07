#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    static int counter = 0;
    counter++;
    printf("Handled SIGUSR1: %d\n", counter);
}

int main() {
    signal(SIGUSR1, signal_handler);

    for (int i = 0; i < 10000; i++) {
        kill(getpid(), SIGUSR1);
    }

    return 0;
}