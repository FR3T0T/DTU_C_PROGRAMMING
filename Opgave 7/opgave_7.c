/*********************************************************************
* Opgave 7, Sundhedsprofil opgave
*
* Navn: Frederik Tots
* Dato: 11/12-2024
*
* Beskrivelse: Program til at oprette og beregne en sundhedsprofil for en person.
* Programmet indsamler personlige data (navn, fødselsdato, højde, vægt) og
* beregner sundhedsrelaterede værdier som BMI, maksimal puls og træningspuls-
* intervaller. Alle data gemmes i en struct og udskrives formateret.
*********************************************************************/

// Inkluderer nødvendige header-filer
#include <stdio.h>  // Standard input/output funktioner
#include <time.h>   // Tidsfunktioner til aldersberegning

// Dansk sprog i terminalen
#include <wchar.h>    // For wide character support
#include <windows.h>  // For Windows console functions

// Definition af struct til sundhedsdata
// Denne struct indeholder alle beregnede sundhedsværdier
struct Sundhedsdata
{
    float bmi;              // Body Mass Index
    int maksimumPuls;       // Maksimal puls (220 - alder)
    float minPulsInterval;  // Minimum træningspuls (50% af maks)
    float maxPulsInterval;  // Maksimum træningspuls (85% af maks)
};

// Definition af hovedstruct til patientens komplette sundhedsprofil
struct Sundhedsprofil
{
    char fornavn[50];       // Array til fornavn (max 49 tegn + null-terminator)
    char efternavn[50];     // Array til efternavn (max 49 tegn + null-terminator)
    int fodselsdag[3];      // Array med [dag, måned, år]
    float hoejde;           // Højde i meter
    float vaegt;            // Vægt i kilogram
    int alder;              // Beregnet alder
    struct Sundhedsdata sundhedsdata;  // Nested struct med sundhedsdata
};

// Funktion til at beregne personens præcise alder
int beregnAlder(int dag, int maaned, int aar)
{
    // Henter aktuel systemtid
    time_t t = time(NULL);
    struct tm tm;
    
    // Platform-specifik håndtering af lokaletid
    #ifdef _WIN32
        localtime_s(&tm, &t);  // Windows-specifik tidshåndtering
    #else
        tm = *localtime(&t);   // Linux/Unix tidshåndtering
    #endif
    
    // Konverterer systemtid til læsbare værdier
    int nuAar = tm.tm_year + 1900;  // År siden 1900
    int nuMaaned = tm.tm_mon + 1;   // Måneder starter fra 0
    int nuDag = tm.tm_mday;         // Aktuel dag
    
    // Beregner basal alder (år fra fødsel til nu)
    int alder = nuAar - aar;
    
    // Justerer alder hvis fødselsdagen i år ikke er nået endnu
    if (maaned > nuMaaned || (maaned == nuMaaned && dag > nuDag))
    {
        alder--;  // Trækker et år fra hvis fødselsdagen ikke er nået
    }
    
    return alder;
}

// Funktion til at beregne hjertedata baseret på alder
struct Sundhedsdata beregnHjertedata(int alder)
{
    struct Sundhedsdata data;
    data.maksimumPuls = 220 - alder;  // Standardformel for maksimal puls
    data.minPulsInterval = data.maksimumPuls * 0.5;   // 50% af maksimal puls
    data.maxPulsInterval = data.maksimumPuls * 0.85;  // 85% af maksimal puls
    return data;
}

// Funktion til at beregne BMI ud fra højde og vægt
float beregnBMI(float hoejde, float vaegt)
{
    return vaegt / (hoejde * hoejde);  // BMI formel: vægt / højde²
}

// Hovedfunktion
int main()
{
    // Set console to use UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    // Opretter en instans af sundhedsprofil
    struct Sundhedsprofil profil;
    
    // Indsamler brugerdata gennem console input
    printf("Indtast fornavn: ");
    scanf_s("%s", profil.fornavn, sizeof(profil.fornavn));  // Sikker input med størrelsesbegrænsning
    
    printf("Indtast efternavn: ");
    scanf_s("%s", profil.efternavn, sizeof(profil.efternavn));
    
    printf("Indtast fødselsdag (dd mm åååå): ");
    scanf_s("%d %d %d", &profil.fodselsdag[0], &profil.fodselsdag[1], &profil.fodselsdag[2]);
    
    printf("Indtast højde i meter (fx 1.70): ");
    scanf_s("%f", &profil.hoejde);
    
    printf("Indtast vægt i kg: ");
    scanf_s("%f", &profil.vaegt);
    
    // Beregner alder baseret på indtastet fødselsdato
    profil.alder = beregnAlder(profil.fodselsdag[0], 
                              profil.fodselsdag[1], 
                              profil.fodselsdag[2]);
    
    // Beregner og gemmer alle sundhedsdata
    profil.sundhedsdata = beregnHjertedata(profil.alder);
    profil.sundhedsdata.bmi = beregnBMI(profil.hoejde, profil.vaegt);
    
    // Udskriver den komplette sundhedsprofil formateret
    printf("\n--- Sundhedsprofil ---\n");
    printf("%s %s\n", profil.fornavn, profil.efternavn);
    printf("%.2f m\n", profil.hoejde);           // 2 decimaler for højde
    printf("%.1f kg\n", profil.vaegt);           // 1 decimal for vægt
    printf("Alder: %d år\n", profil.alder);
    printf("BMI: %.1f kg/m^2\n", profil.sundhedsdata.bmi);
    printf("Maks. puls: %d\n", profil.sundhedsdata.maksimumPuls);
    printf("Pulsinterval: %.1f - %.1f\n", 
           profil.sundhedsdata.minPulsInterval,
           profil.sundhedsdata.maxPulsInterval);
    
    return 0;  // Programmet afsluttes Succesfuldt
}