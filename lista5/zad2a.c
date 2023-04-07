#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int signals[62] = { 
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 34, 
    35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64
};

void signal_handler(int signum) {
    printf("Handled signal with number: %d\n", signum);
}

int main() {
    for (int i = 0; i < 62; i++) {
        signal(signals[i], signal_handler);
    }

    for (int i = 0; i < 62; i++) {
        printf("Generating signal %d\n", signals[i]);
        if(signals[i] != 9 && signals[i] != 19) // 9 -> kill signal, 19 -> stopped signal
            kill(getpid(), signals[i]);
    }

    return 0;
}