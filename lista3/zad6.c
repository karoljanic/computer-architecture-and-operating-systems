#include <stdio.h>

// wyswietlenie "Hello World!" w 256 kolorach
// 16 pierwszych -> podstawowe
// 216 kolejnych -> czesc RGB
// ostatnie 24 -> skala szarosci

//ESC[38;5;{ID}m -> foreground 8 bits
//ESC[48;5;{ID}m -> background 8 bits

int main() {
    for(int color = 0; color < 256; color++) {
        printf("\033[38;5;%dmHello World!\033[0m\n", color);
    }
}

//mozliwe jest wyswietlenie wiekszej liczby kolorow 2^8 * 2^8 * 2^8
//w przestrzeni barw RGB, ale nie wszyskie terminale obsluguja tyle kolorow
//tput colors - liczba kolorow obslugiwanych przez terminal

//ESC[38;2;{r};{g};{b}m -> foreground 24 bits
//ESC[48;2;{r};{g};{b}m -> background 24 bits

/*
int main() {
    for(int red = 0; red < 255; red++) {
        for(int green = 0; green < 255; green++) {
            for(int blue = 0; blue < 255; blue++) {
                printf("\033[38;2;%d;%d;%dmHello World!\033[0m\n", red, green, blue);
            }
        }
    }
}
*/

// More Informations:
// https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797