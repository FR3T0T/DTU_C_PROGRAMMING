#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool er_heltal(const char* s) {
    char* endptr;
    strtol(s, &endptr, 10);
    return (*s != '\0' && *endptr == '\0');
}

bool er_delelig_med_5(int n) {
    return n % 5 == 0;
}

int main() {
    char input[100];
    printf("Indtast et heltal: ");
    fgets(input, sizeof(input), stdin);

    // Fjern newline character fra input
    input[strcspn(input, "\n")] = 0;

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