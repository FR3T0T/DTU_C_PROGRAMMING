/*********************************************************************
* Opgave 4 (Del 5A)
* 
* Navn: Frederik Tots
* Dato: 11/12-2024
*
* Beskrivelse: Program til at analysere en række brugerindtastede tal.
* Programmet kan håndtere op til 100 tal og udfører følgende analyser:
* - Beregner summen af alle tal
* - Beregner gennemsnittet
* - Finder det laveste tal
* - Finder det højeste tal
* - Identificerer eventuelle dubletter
* 
* Programmet inkluderer fejlhåndtering for ugyldige inputs og kan
* håndtere situationen hvor der ikke indtastes nogen tal.
*********************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

// Dansk sprog i terminalen
#include <wchar.h>    // For wide character support
#include <windows.h>  // For Windows console functions

#define MAX_SIZE 100  // Definerer maksimal størrelse for arrayet

// Funktion til at indlæse tal fra brugeren
void indlaes_tal(int *array, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Indtast tal %d: ", i + 1);
        scanf("%d", &array[i]);  // Indlæser tal og gemmer i arrayet
    }
}

// Funktion til at beregne summen af alle tal i arrayet
int beregn_sum(int *array, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += array[i];  // Adderer hvert tal til summen
    }
    return sum;
}

// Funktion til at beregne gennemsnittet
float beregn_gennemsnit(int sum, int n)
{
    return (float)sum / n;  // Konverterer sum til float for at få præcist resultat
}

// Funktion til at finde det laveste tal
int find_laveste(int *array, int n)
{
    int laveste = array[0];  // Antager første element er lavest
    for (int i = 1; i < n; i++)
    {
        if (array[i] < laveste)
        {
            laveste = array[i];  // Opdaterer laveste hvis et mindre tal findes
        }
    }
    return laveste;
}

// Funktion til at finde det højeste tal
int find_hoejeste(int *array, int n)
{
    int hoejeste = array[0];  // Antager første element er højest
    for (int i = 1; i < n; i++)
    {
        if (array[i] > hoejeste)
        {
            hoejeste = array[i];  // Opdaterer højeste hvis et større tal findes
        }
    }
    return hoejeste;
}

// Funktion til at finde og udskrive dubletter
void find_dubletter(int *array, int n)
{
    bool fundet_dublet = false;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (array[i] == array[j])
            {
                if (!fundet_dublet)
                {
                    printf("Dubletter fundet:\n");
                    fundet_dublet = true;
                }
                printf("%d ", array[i]);  // Udskriver dublet
                break;  // Går videre til næste tal i ydre loop
            }
        }
    }
    if (!fundet_dublet)
    {
        printf("Ingen dubletter fundet.\n");
    }
    else
    {
        printf("\n");
    }
}

int main()
{
    // Set console to use UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    
    int n;
    int array[MAX_SIZE];

    // Indlæs antal tal med fejlhåndtering
    do
    {
        printf("Indtast antal tal (max 100): ");
        scanf("%d", &n);
        if (n > MAX_SIZE)
        {
            printf("Fejl: Maksimalt 100 tal tilladt\n");
        }
        else if (n < 0)
        {
            printf("Fejl: Antal tal skal være positivt\n");
        }
    }
    while (n > MAX_SIZE || n < 0);

    if (n == 0)
    {
        printf("Ingen tal at analysere\n");
        return 0;
    }

    indlaes_tal(array, n);

    int sum = beregn_sum(array, n);
    float gennemsnit = beregn_gennemsnit(sum, n);
    int laveste = find_laveste(array, n);
    int hoejeste = find_hoejeste(array, n);

    // Udskriver resultater
    printf("\nResultater:\n");
    printf("Sum: %d\n", sum);
    printf("Gennemsnit: %.2f\n", gennemsnit);
    printf("Laveste tal: %d\n", laveste);
    printf("Højeste tal: %d\n", hoejeste);

    find_dubletter(array, n);

    return 0;
}