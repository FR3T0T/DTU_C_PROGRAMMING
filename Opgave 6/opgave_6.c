/*********************************************************************
* Opgave 6A: Tilfældige sætninger
* 
* Navn: Frederik Tots
* Dato: 11/12-2024
*
* Beskrivelse: Program til at generere tilfældige sætninger ved hjælp af
* arrays af pointere til char. Programmet sammensætter grammatisk 
* korrekte sætninger ved at vælge tilfældige ord fra fire forskellige 
* ordklasser.
*
* Hovedfunktionalitet:
* - Genererer 20 tilfældige sætninger
* - Bruger fire arrays med danske ord (artikler, navneord, verber, præpositioner)
* - Sammenkæder ord i korrekt grammatisk rækkefølge
* - Formaterer sætninger med stort begyndelsesbogstav og punktum
* - Håndterer danske tegn gennem UTF-8 encoding
* 
* Implementation er baseret på kapitel 7.10 om arrays of pointers og
* inkluderer sikker string-håndtering med strcat_s og strcpy_s samt
* tilfældig talsgenerering med rand() funktionen.
*********************************************************************/

#define _CRT_SECURE_NO_WARNINGS  // For at kunne bruge strcat_s og strcpy_s
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Dansk sprog i terminalen
#include <wchar.h>    // For wide character support
#include <windows.h>  // For Windows console functions

// Hjælpefunktion til at konvertere første bogstav til stort
void capitalizeFirstLetter(char* str)
{
    if (str[0] != '\0')
    {
        str[0] = toupper(str[0]);
    }
}

int main()
{
    // Set console to use UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    // Arrays med ord fra opgavebeskrivelsen
    const char* artikel[] = {"den", "en", "nogle", "enhver", "det", "flere"};
    const char* navneord[] = {"personer", "studerende", "arbejdsløse", "by", "cykel", "s-tog"};
    const char* verbum[] = {"kørte", "hoppede", "løb", "gik", "sad", "bevægede"};
    const char* praeposition[] = {"til", "fra", "over", "under", "på", "ved siden af"};

    // Antal ord i hver kategori
    const int antal_artikel = sizeof(artikel) / sizeof(artikel[0]);
    const int antal_navneord = sizeof(navneord) / sizeof(navneord[0]);
    const int antal_verbum = sizeof(verbum) / sizeof(verbum[0]);
    const int antal_praeposition = sizeof(praeposition) / sizeof(praeposition[0]);

    // Initialiser random seed
    srand((unsigned int)time(NULL));

    // Buffer til den samlede sætning
    char saetning[300];
    const size_t bufferSize = sizeof(saetning);

    // Generer 20 sætninger
    for (int i = 0; i < 20; i++)
    {
        // Nulstil sætningen
        strcpy_s(saetning, bufferSize, "");

        // Vælg tilfældige ord og sammensæt sætningen
        strcpy_s(saetning, bufferSize, artikel[rand() % antal_artikel]);
        strcat_s(saetning, bufferSize, " ");
        strcat_s(saetning, bufferSize, navneord[rand() % antal_navneord]);
        strcat_s(saetning, bufferSize, " ");
        strcat_s(saetning, bufferSize, verbum[rand() % antal_verbum]);
        strcat_s(saetning, bufferSize, " ");
        strcat_s(saetning, bufferSize, praeposition[rand() % antal_praeposition]);
        strcat_s(saetning, bufferSize, " ");
        strcat_s(saetning, bufferSize, artikel[rand() % antal_artikel]);
        strcat_s(saetning, bufferSize, " ");
        strcat_s(saetning, bufferSize, navneord[rand() % antal_navneord]);

        // Gør første bogstav stort og tilføj punktum
        capitalizeFirstLetter(saetning);
        strcat_s(saetning, bufferSize, ".");

        // Udskriv sætningen
        printf("%s\n", saetning);
    }

    return 0;
}