/*********************************************************************
* Opgave 9, Sorteret Database
*
* Navn: Frederik Tots
* Dato: 11/12-2024
*
* Beskrivelse: Program til at håndtere en sorteret database over personer.
* Programmet læser data fra en CSV-fil ind i et array af Records, hvor
* hver record indeholder navn, adresse, alder og telefonnummer. 
* 
* Programmet implementerer:
* - Bubble sort algoritme til sortering af databasen efter telefonnummer
* - Binær søgning for effektiv søgning efter telefonnumre
* - Robust fejlhåndtering ved fil-operationer og brugerinput
* - Dansk tegnsæt support via UTF-8
*
* Data gemmes i en Record struct og programmet bruger et array af disse
* structs som den primære datastruktur. Implementationen inkluderer
* sikkerhedstjek for at undgå buffer overflows og invalide data.
* 
* Programmet er udviklet som en udvidelse af opgave 8, hvor lineær
* søgning er erstattet med binær søgning i en sorteret liste.
*********************************************************************/

// Header filer der inkluderes for forskellige funktionaliteter
#include <stdio.h>    // Standard input/output operationer
#include <stdlib.h>   // Standard biblioteksfunktioner (f.eks. atoi)
#include <string.h>   // String manipulation funktioner
#include <stdbool.h>  // Boolean type og værdier
#include <stddef.h>   // Standard definitioner

// Dansk sprog i terminalen
#include <wchar.h>    // For wide character support
#include <windows.h>  // For Windows console functions

// Konstanter der definerer størrelsesbegrænsninger
#define LISTSIZE 100    // Maksimalt antal records i databasen
#define MAX_LINE 256    // Maksimal længde på en linje i CSV filen

// Definition af Record strukturen til at holde person-data
struct Record
{
    char name[50];      // Person navn - max 49 tegn + null terminator
    char address[50];   // Adresse - max 49 tegn + null terminator 
    int age;           // Alder som heltal
    unsigned int phone; // Telefonnummer som unsigned int
};

// Global array til at holde alle records og tæller for antal records
struct Record list[LISTSIZE];
int recordCount = 0;

// Indlæser data fra CSV fil ind i den globale list array
// Returnerer true hvis succesfuld, false hvis der er fejl
bool readDatabase(const char* filename)
{
    FILE* file = fopen(filename, "r");
    char line[MAX_LINE];
    char* token;
    
    // Check om filen kunne åbnes
    if (!file)
    {
        printf("Fejl ved aabning af fil %s\n", filename);
        return false;
    }
    
    // Spring header linjen over i CSV filen
    if (fgets(line, MAX_LINE, file) == NULL)
    {
        printf("Fejl: Filen er tom eller kan ikke læses\n");
        fclose(file);
        return false;
    }
    
    // Reset recordCount for sikkerhed
    recordCount = 0;
    
    // Læs hver linje og split op i felter ved komma
    while (fgets(line, MAX_LINE, file) && recordCount < LISTSIZE)
    {
        // Fjern newline karakteren fra enden af linjen
        line[strcspn(line, "\n")] = 0;
        
        // Split linjen op ved kommaer og kopier data ind i den aktuelle record
        token = strtok(line, ",");
        if (!token)
        {
            printf("Advarsel: Ugyldig linje fundet i filen (mangler navn)\n");
            continue;
        }
        strncpy(list[recordCount].name, token, 49);
        list[recordCount].name[49] = '\0';  // Sikr null-terminering
        
        token = strtok(NULL, ",");
        if (!token)
        {
            printf("Advarsel: Ugyldig linje fundet i filen (mangler adresse)\n");
            continue;
        }
        strncpy(list[recordCount].address, token, 49);
        list[recordCount].address[49] = '\0';  // Sikr null-terminering
        
        token = strtok(NULL, ",");
        if (!token)
        {
            printf("Advarsel: Ugyldig linje fundet i filen (mangler alder)\n");
            continue;
        }
        list[recordCount].age = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token)
        {
            printf("Advarsel: Ugyldig linje fundet i filen (mangler telefonnummer)\n");
            continue;
        }
        list[recordCount].phone = (unsigned int)atol(token);
        
        recordCount++;
    }
    
    fclose(file);
    
    // Check om vi har læst nogle records
    if (recordCount == 0)
    {
        printf("Fejl: Ingen gyldige records blev læst fra filen\n");
        return false;
    }
    
    if (recordCount == LISTSIZE)
    {
        printf("Advarsel: Maksimalt antal records (%d) nået. Nogle records kan være udeladt.\n", LISTSIZE);
    }
    
    printf("Succes: %d records blev indlæst fra filen\n", recordCount);
    return true;
}

// Implementering af bubble sort algoritmen til sortering af records efter telefonnummer
void bubbleSort()
{
    if (recordCount < 2) return;  // Ingen grund til at sortere 0 eller 1 element
    
    bool swapped;
    int i;
    struct Record temp;  // Midlertidig record til brug ved ombytning
    
    // Ydre loop - en iteration for hver element undtagen det sidste
    for (int j = 0; j < recordCount - 1; j++)
    {
        swapped = false;
        
        // Indre loop - sammenlign naboelementer og byt om hvis nødvendigt
        for (i = 0; i < recordCount - j - 1; i++)
        {
            // Sammenlign telefonnumre og byt records hvis de er i forkert rækkefølge
            if (list[i].phone > list[i + 1].phone)
            {
                // Byt hele record strukturen
                temp = list[i];
                list[i] = list[i + 1];
                list[i + 1] = temp;
                swapped = true;
            }
        }
        
        // Hvis ingen ombytninger er foretaget er listen sorteret
        if (!swapped)
        {
            break;
        }
    }
}

// Binary search implementation til at finde record ud fra telefonnummer
int binarySearch(unsigned int searchPhone)
{
    if (recordCount == 0) return -1;  // Tom liste
    
    int low = 0;
    int high = recordCount - 1;
    int mid;

    // Fortsæt søgningen så længe der er et gyldigt søgeinterval
    while (low <= high)
    {
        mid = (low + high) / 2;
        
        // Check om det midterste element er det søgte
        if (list[mid].phone == searchPhone)
        {
            return mid;
        }
        // Hvis søgte nummer er mindre, søg i nedre halvdel
        else if (list[mid].phone > searchPhone)
        {
            high = mid - 1;
        }
        // Hvis søgte nummer er større, søg i øvre halvdel
        else
        {
            low = mid + 1;
        }
    }
    return -1;  // Returnér -1 hvis nummeret ikke blev fundet
}

// Udskriver alle felter for en given record
void printRecord(const struct Record* record)
{
    if (!record) return;  // Sikkerhedscheck
    
    printf("Navn: %s\n", record->name);
    printf("Adresse: %s\n", record->address);
    printf("Alder: %d\n", record->age);
    printf("Telefon: %u\n", record->phone);
}

// Hovedprogram - indlæser database, sorterer og håndterer søgninger
int main()
{
    // Set console to use UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    unsigned int searchPhone;
    int foundIndex;
    
    // Indlæs database
    printf("Indlæser database...\n");
    if (!readDatabase("database.csv"))
    {
        printf("Kritisk fejl: Kunne ikke indlæse databasen. Programmet afsluttes.\n");
        return 1;
    }
    
    // Sortér databasen
    printf("Sorterer database efter telefonnummer...\n");
    bubbleSort();
    printf("Sortering færdig.\n");
    
    // Hovedloop der håndterer brugerens søgninger
    while (1)
    {
        printf("\nIndtast telefonnummer for at søge! (0 for at afslutte): ");
        if (scanf("%u", &searchPhone) != 1)
        {
            printf("Fejl: Ugyldigt input. Prøv igen.\n");
            // Ryd input buffer
            while (getchar() != '\n');
            continue;
        }
        
        if (searchPhone == 0) break;  // Afslut hvis bruger indtaster 0
        
        // Søg efter nummer og vis resultatet
        foundIndex = binarySearch(searchPhone);
        if (foundIndex != -1)
        {
            printf("Record fundet:\n");
            printRecord(&list[foundIndex]);
        }
        else
        {
            printf("Telefonnummer ikke fundet i databasen.\n");
        }
    }
    
    printf("Program afsluttet.\n");
    return 0;
}