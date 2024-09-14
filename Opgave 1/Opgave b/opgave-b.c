#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool er_heltal(const char* s) {
    if (*s == '\0') return false;  // Tom streng
    if (*s == '-') s++;  // Spring over minus-tegnet hvis det er der
    while (*s) {
        if (!isdigit(*s)) return false;
        s++;
    }
    return true;
}

bool er_delelig_med_5(int n) {
    return n % 5 == 0;
}

int main() {
    char input[100];
    printf("Indtast et heltal: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Fejl ved læsning af input.\n");
        return 1;
    }

    // Fjern newline character fra input hvis den er der
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }

    if (er_heltal(input)) {
        int tal = atoi(input);
        if (er_delelig_med_5(tal)) {
            printf("%d er delelig med 5.\n", tal);
        } else {
            printf("%d er ikke delelig med 5.\n", tal);
        }
    } else {
        printf("Fejl: Du har ikke indtastet et gyldigt heltal.\n");
    }

    return 0;
}