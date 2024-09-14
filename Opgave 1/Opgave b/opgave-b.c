#include <stdio.h>

int main() {
    int a, b, sum;

    // Indlæs to tal fra tastaturet
    printf("Indtast to tal (adskilt af mellemrum): ");
    scanf("%d %d", &a, &b);

    // Beregn summen
    sum = a + b;

    // Udskriv summen
    printf("Summen er: %d\n", sum);

    // Tjek om sum er større end b
    if (sum > b) {
        // Beregn difference og gem i a
        a = sum - b;
        printf("Sum er større end b. Ny værdi af a: %d\n", a);
    }

    return 0;
}
