#include <stdio.h>

int get_valid_input(const char* prompt) {
    int input;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n'); // Ryd input buffer
            printf("Fejl: Indtast venligst et heltal.\n");
        } else if (input < 0 || input > 100) {
            printf("Fejl: Tallet skal være mellem 0 og 100.\n");
        } else {
            return input;
        }
    }
}

int main(void) {
    int a, b, c, d, sum, grade;

    printf("Indtast 4 heltal mellem 0 og 100.\n");

    a = get_valid_input("Indtast første tal: ");
    b = get_valid_input("Indtast andet tal: ");
    c = get_valid_input("Indtast tredje tal: ");
    d = get_valid_input("Indtast fjerde tal: ");

    sum = a + b + c + d;
    grade = sum / 4; // Gennemsnit af de fire tal

    printf("Samlet antal point: %d\n", sum);
    printf("Gennemsnit: %d\n", grade);

    if (grade >= 90)
        printf("Karakter: 12\n");
    else if (grade >= 80)
        printf("Karakter: 10\n");
    else if (grade >= 70)
        printf("Karakter: 7\n");
    else if (grade >= 60)
        printf("Karakter: 4\n");
    else if (grade >= 40)
        printf("Karakter: 02\n");
    else
        printf("Karakter: 00\n");

    return 0;       
}