#include <stdio.h>

int main()
{
    // int number; vil blive defineret af input fra brugeren. 
    // char buffer[100]; læser en linje tekst sikkert (op til 99 tegn + nul-tegn).
    int number;
    char buffer[100];

    // Instruks udskrives til brugeren.
    printf("Indtast et heltal: ");

    // (ER IKKE NØDVENDIGT, JEG KUNNE OGSÅ BARE FJERNE DETTE OG GÅ DIREKTE TIL SCANF) Bruges til at læse input fra brugeren som en streng og tjekke, at der blev indtastet noget. 
    if (fgets(buffer, sizeof(buffer), stdin))
    {
        // Tjekker om det indtastede er et heltal
        if (sscanf(buffer, "%d", &number) == 1)
        {
            // Tjekker om tallet er deleligt med 5
            if (number % 5 == 0)
            {
                // Udskrift hvis tallet er deleligt med 5.
                printf("%d er deleligt med 5.\n", number);
            }
            else
            {
                // Udskrift hvis tallet IKKE er deleligt med 5.
                printf("%d er ikke deleligt med 5.\n", number);
            }
        }
        else
        {
            // Udskrift hvis det indtastede ikke kan fortolkes som et heltal.
            printf("Fejl: Det indtastede er ikke et heltal.\n");
        }
    }

    // Afslutning af main()
    return 0;
}