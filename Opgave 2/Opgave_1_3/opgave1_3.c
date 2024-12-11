/*********************************************************************
* Opgave 2 (Del. 1-3)
*
* Navn: Frederik Tots
* Dato: 11/12-2024

* Beskrivelse: Program til beregning af karakterer baseret på fire opgaver i biologi.
* Programmet tager fire pointtal som input (0-100) og beregner gennemsnittet
* samt den tilsvarende karakter efter 7-trinsskalaen.
*********************************************************************/

#include <stdio.h>

// Funktion til at håndtere og validere brugerinput
int get_valid_input(const char* prompt)
{
    int input;
    while (1)
    {
        printf("%s", prompt);
        // Tjek om input er et tal
        if (scanf("%d", &input) != 1)
        {
            while (getchar() != '\n'); // Ryd input buffer
            printf("Fejl: Indtast venligst et tal mellem 0 og 100.\n");
        } 
        // Tjek om tallet er inden for det gyldige interval
        else if (input < 0 || input > 100)
        {
            printf("Fejl: Tallet skal være mellem 0 og 100.\n");
            while (getchar() != '\n'); // Ryd input buffer
        } 
        else
        {
            while (getchar() != '\n'); // Ryd eventuelle resterende tegn i bufferen
            return input;
        }
    }
}

int main(void)
{
    // Variabler til point og beregninger
    int point1, point2, point3, point4, sum, gennemsnit;

    // Velkomstbesked og instruktioner
    printf("Velkommen til karakterberegning\n");
    printf("--------------------------------\n");
    printf("Indtast venligst point (0-100) for de fire opgaver.\n\n");

    // Indhent point for alle fire opgaver
    point1 = get_valid_input("Indtast point for første opgave: ");
    point2 = get_valid_input("Indtast point for anden opgave: ");
    point3 = get_valid_input("Indtast point for tredje opgave: ");
    point4 = get_valid_input("Indtast point for fjerde opgave: ");

    // Beregn sum og gennemsnit
    sum = point1 + point2 + point3 + point4;
    gennemsnit = sum / 4;

    // Vis resultaterne
    printf("\nResultater:\n");
    printf("--------------------------------\n");
    printf("Samlet antal point: %d\n", sum);
    printf("Gennemsnit: %d\n", gennemsnit);
    printf("Karakter: ");

    // Bestem og vis karakteren baseret på gennemsnittet
    if (gennemsnit >= 90)
    {
        printf("12\n");
    }
    else if (gennemsnit >= 80)
    {
        printf("10\n");
    }
    else if (gennemsnit >= 70)
    {
        printf("7\n");
    }
    else if (gennemsnit >= 60)
    {
        printf("4\n");
    }
    else if (gennemsnit >= 40)
    {
        printf("02\n");
    }
    else
    {
        printf("00\n");
    }

    return 0;       
}