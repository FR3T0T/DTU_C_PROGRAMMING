#include <stdio.h>
#include <stdlib.h>

int get_valid_input(const char* prompt, int min, int max) {
    int input;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n'); // Ryd input buffer
            printf("Fejl: Indtast venligst et heltal.\n");
        } else if (input < min || input > max) {
            printf("Fejl: Tallet skal være mellem %d og %d.\n", min, max);
        } else {
            return input;
        }
    }
}

int main(void) {
    int antal_tests, sum = 0, *scores;
    double gennemsnit;

    antal_tests = get_valid_input("Indtast antal tests (1-10): ", 1, 10);

    // Dynamisk allokering af hukommelse til scores
    scores = (int*)malloc(antal_tests * sizeof(int));
    if (scores == NULL) {
        printf("Fejl ved allokering af hukommelse.\n");
        return 1;
    }

    printf("Indtast %d testscores mellem 0 og 100.\n", antal_tests);

    for (int i = 0; i < antal_tests; i++) {
        char prompt[50];
        sprintf(prompt, "Indtast score for test %d: ", i + 1);
        scores[i] = get_valid_input(prompt, 0, 100);
        sum += scores[i];
    }

    gennemsnit = (double)sum / antal_tests;

    printf("Samlet antal point: %d\n", sum);
    printf("Gennemsnit: %.2f\n", gennemsnit);

    printf("Karakter: ");
    if (gennemsnit >= 90)
        printf("12\n");
    else if (gennemsnit >= 80)
        printf("10\n");
    else if (gennemsnit >= 70)
        printf("7\n");
    else if (gennemsnit >= 60)
        printf("4\n");
    else if (gennemsnit >= 40)
        printf("02\n");
    else
        printf("00\n");

    // Frigør allokeret hukommelse
    free(scores);

    return 0;       
}