/*********************************************************************
* Opgave 2 (Del. 4-5)
* 
* Navn: Frederik Tots
* Dato: 11/12-2024
* 
* Beskrivelse:
* Dette program hjælper en lærer med at udregne karakterer baseret på
* pointtal fra biologiopgaver. Programmet kan:
* - Modtage vilkårligt antal pointtal (0-100)
* - Beregne gennemsnit
* - Tildele karakter efter 7-trinsskalaen
* - Håndtere fejlinput
* - Køres flere gange
*
* Input: Heltal mellem 0-100, -1 for at afslutte indtastning
* Output: Samlet point, antal tests, gennemsnit og karakter
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Beholder din oprindelige get_integer_input funktion
int get_integer_input(const char* prompt)
{
    char buffer[100];
    char* endptr;
    long input;

    while (1)
    {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Fejl 40 - Indtast venligst et gyldigt heltal.\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strchr(buffer, '.') != NULL || strchr(buffer, ',') != NULL)
        {
            printf("FEJL 1 - Kommatal er ikke tilladt. Indtast venligst et heltal!\n");
            continue;
        }

        input = strtol(buffer, &endptr, 10);

        if (*endptr != '\0')
        {
            printf("FEJL 3 - Tegn er ikke gyldigt. Indtast venligst et heltal!\n\n");
            continue;
        }

        return (int)input;
    }
}

int main(void)
{
    char fortsaet = 'j';
    
    while (fortsaet == 'j' || fortsaet == 'J')
    {
        int sum = 0, count = 0, score;
        double gennemsnit;

        printf("\n----------------- INFORMATION -----------------\n");
        printf("-     Indtast testresultaterne [0 - 100].     -\n");
        printf("-  For at afslutte indtastningen, tast '-1'.  -\n");
        printf("-----------------------------------------------\n\n");

        // Din oprindelige kode for indtastning og beregning
        while (1)
        {
            score = get_integer_input("Indtast testresultat: ");

            if (score == -1)
            {
                break;
            }
            else if (score < 0 || score > 100)
            {
                printf("FEJL 2 - Indtast venlist et heltal, inden for 0 - 100!\n");
            }
            else
            {
                sum += score;
                count++;
            }
        }

        if (count == 0)
        {
            printf("Fejl 4 - Ingen testresultater blev indtastet og registret.\n\n");
            continue;  // Gå tilbage og start forfra i stedet for at afslutte
        }

        gennemsnit = (double)sum / count;

        printf("\nSamlet antal point: %d\n", sum);
        printf("Samlet antal test: %d\n", count);
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

        // Tilføjer mulighed for at køre programmet igen
        printf("\nVil du beregne en ny karakter? (j/n): ");
        char svar;
        scanf(" %c", &svar);
        while (getchar() != '\n');  // Ryd input buffer
        fortsaet = svar;
    }

    return 0;
}