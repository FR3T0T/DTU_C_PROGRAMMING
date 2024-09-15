#include <stdio.h>

int main()
{
    // a, b, d og sum vil blive defineret senere i koden fra input.
    int a, b, d, sum;

        // Instruks udskift & scanf til definering af a og b.
        printf("Indtast to heltal [adskilt af mellemrum: a b]: ");
        scanf("%d %d", &a, &b);

    // Difinere sum ved at summere a og b.
    sum = a + b;

        // Her udskrives summen.
        printf("Summen er: %d\n", sum);

    // Her opsummere jeg hvis summen er større en b.
    if (sum > b)
    {
        // Differancen difineres, så ledes:
        d = sum - b;

            // Den opsummerende tekst udskrives.
            printf("Summen er større end B. Differencen mellem summen og B må værre A: %d\n", d);
    }

    // Afslutning af main()
    return 0; 
}