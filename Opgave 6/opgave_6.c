/*
// Opgave 6A
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Hjælpefunktion til at konvertere første bogstav til stort
void capitalizeFirstLetter(char* str)
{
    if (str[0] != '\0')
    {
        str[0] = toupper(str[0]);
    }
}

int main() {
    // Arrays med ord
    const char* artikel[] = {"en", "et", "den", "det"};
    const char* navneord[] = {"kat", "hund", "bil", "bog", "hus", "computer"};
    const char* verbum[] = {"spiser", "løber", "hopper", "sover", "leger", "arbejder"};
    const char* praeposition[] = {"på", "under", "ved", "bag", "over", "foran"};

    // Antal ord i hver kategori
    const int antal_artikel = sizeof(artikel) / sizeof(artikel[0]);
    const int antal_navneord = sizeof(navneord) / sizeof(navneord[0]);
    const int antal_verbum = sizeof(verbum) / sizeof(verbum[0]);
    const int antal_praeposition = sizeof(praeposition) / sizeof(praeposition[0]);

    // Initialiser random seed
    srand(time(NULL));

    // Buffer til den samlede sætning
    char saetning[200];

    // Generer 20 sætninger
    for (int i = 0; i < 20; i++)
    {
        // Nulstil sætningen
        saetning[0] = '\0';

        // Vælg tilfældige ord og sammensæt sætningen
        strcat(saetning, artikel[rand() % antal_artikel]);
        strcat(saetning, " ");
        strcat(saetning, navneord[rand() % antal_navneord]);
        strcat(saetning, " ");
        strcat(saetning, verbum[rand() % antal_verbum]);
        strcat(saetning, " ");
        strcat(saetning, praeposition[rand() % antal_praeposition]);
        strcat(saetning, " ");
        strcat(saetning, artikel[rand() % antal_artikel]);
        strcat(saetning, " ");
        strcat(saetning, navneord[rand() % antal_navneord]);

        // Gør første bogstav stort og tilføj punktum
        capitalizeFirstLetter(saetning);
        strcat(saetning, ".");

        // Udskriv sætningen
        printf("%s\n", saetning);
    }

    return 0;
}
*/


// Opgave 6B
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_SAETNING_LAENGDE 200
#define ANTAL_AFSNIT 4
#define SAETNINGER_PER_AFSNIT 5

// Hjælpefunktion til at konvertere første bogstav til stort
void capitalizeFirstLetter(char* str)
{
    if (str[0] != '\0')
    {
        str[0] = toupper(str[0]);
    }
}

// Funktion til at generere en enkelt sætning
void genererSaetning(char* saetning, const char** artikel, int antal_artikel, 
                     const char** navneord, int antal_navneord,
                     const char** verbum, int antal_verbum,
                     const char** praeposition, int antal_praeposition)
{
    saetning[0] = '\0';
    
    strcat(saetning, artikel[rand() % antal_artikel]);
    strcat(saetning, " ");
    strcat(saetning, navneord[rand() % antal_navneord]);
    strcat(saetning, " ");
    strcat(saetning, verbum[rand() % antal_verbum]);
    strcat(saetning, " ");
    strcat(saetning, praeposition[rand() % antal_praeposition]);
    strcat(saetning, " ");
    strcat(saetning, artikel[rand() % antal_artikel]);
    strcat(saetning, " ");
    strcat(saetning, navneord[rand() % antal_navneord]);
    
    capitalizeFirstLetter(saetning);
    strcat(saetning, ".");
}

int main()
{
    // Arrays med ord
    const char* artikel[] = {
                            "en",
                            "et",
                            "den",
                            "det"
                            };

    const char* navneord[] = {
                                "kat",
                                "hund",
                                "bil",
                                "bog",
                                "hus",
                                "computer",
                                "fugl",
                                "blomst",
                                "telefon",
                                "kage",
                                "stol",
                                "bord",
                                "vindue",
                                "dør"
                             };

    const char* verbum[] = {
                            "spiser",
                            "løber",
                            "hopper",
                            "sover",
                            "leger",
                            "arbejder",
                            "danser",
                            "synger",
                            "griner",
                            "læser",
                            "skriver",
                            "maler"
                            };

    const char* praeposition[] = {
                                    "på",
                                    "under",
                                    "ved",
                                    "bag",
                                    "over",
                                    "foran",
                                    "mellem",
                                    "omkring",
                                    "nær",
                                    "inden for"
                                 };
    
    // Tilfældige forfattere
    const char* forfattere[] = {
                                "H.C. Andersen",
                                "Karen Blixen",
                                "Peter Høeg",
                                "Tove Ditlevsen",
                                "Tom Kristensen",
                                "Suzanne Brøgger",
                                "Dan Turèll",
                                "Jesper Wung-Sung"
                                };
    
    // Tilfældige titler (første del + anden del)
    const char* titel_del1[] = {
                                "Den mystiske",
                                "Den sidste",
                                "Den gamle",
                                "Den glemte",
                                "Den hemmelige"
                                };

    const char* titel_del2[] = {
                                "fortælling",
                                "historie",
                                "dagbog",
                                "hemmelighed",
                                "drøm"
                                };

    // Antal ord i hver kategori
    const int antal_artikel = sizeof(artikel) / sizeof(artikel[0]);
    const int antal_navneord = sizeof(navneord) / sizeof(navneord[0]);
    const int antal_verbum = sizeof(verbum) / sizeof(verbum[0]);
    const int antal_praeposition = sizeof(praeposition) / sizeof(praeposition[0]);
    const int antal_forfattere = sizeof(forfattere) / sizeof(forfattere[0]);
    const int antal_titel_dele = sizeof(titel_del1) / sizeof(titel_del1[0]);

    srand(time(NULL));

    // Vælg tilfældig forfatter og titel
    const char* forfatter = forfattere[rand() % antal_forfattere];
    const char* novelle_titel = malloc(100);
    sprintf((char*)novelle_titel, "%s %s", 
            titel_del1[rand() % antal_titel_dele], 
            titel_del2[rand() % antal_titel_dele]);

    // Udskriv titel og forfatter
    printf("\n%s\n", novelle_titel);
    printf("Af %s\n\n", forfatter);

    // Generer novellen
    for (int afsnit = 0; afsnit < ANTAL_AFSNIT; afsnit++)
    {
        // Generer sætninger for dette afsnit
        for (int i = 0; i < SAETNINGER_PER_AFSNIT; i++)
        {
            char saetning[MAX_SAETNING_LAENGDE];
            genererSaetning(saetning, artikel, antal_artikel, 
                           navneord, antal_navneord,
                           verbum, antal_verbum,
                           praeposition, antal_praeposition);
            printf("%s ", saetning);
        }
        printf("\n\n"); // Nyt afsnit
    }

    free((void*)novelle_titel);
    return 0;
}