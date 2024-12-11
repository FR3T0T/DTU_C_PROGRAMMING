#include <stdio.h>

int main()
{
    int number;

    // Instruks udskrives til brugeren
    printf("Indtast et heltal: ");

    // Tjekker om input er et tal
    if (scanf("%d", &number) == 1)
    {
        // Hvis input er et heltal, tjek om det er deleligt med 5
        if (number % 5 == 0)
        {
            printf("%d er deleligt med 5.\n", number);
        }
        else
        {
            printf("%d er ikke deleligt med 5.\n", number);
        }
    }
    else
    {
        printf("Fejl: Input er ikke et tal.\n");
    }

    return 0;
}