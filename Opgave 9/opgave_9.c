// Co-Pilot, Chat-GPT og Claude er blevet brugt til fejlfiding.
// Header filer der inkluderes for forskellige funktionaliteter
#include <stdio.h>    // Standard input/output operationer
#include <stdlib.h>   // Standard biblioteksfunktioner (f.eks. atoi)
#include <string.h>   // String manipulation funktioner
#include <stdbool.h>  // Boolean type og værdier
#include <stddef.h>   // Standard definitioner

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
void readDatabase(const char* filename)
{
    FILE* file = fopen(filename, "r");
    char line[MAX_LINE];
    char* token;
    
    // Check om filen kunne åbnes
    if (!file)
    {
        printf("Fejl ved aabning af fil %s\n", filename);
        return;
    }
    
    // Spring header linjen over i CSV filen
    fgets(line, MAX_LINE, file);
    
    // Læs hver linje og split op i felter ved komma
    while (fgets(line, MAX_LINE, file) && recordCount < LISTSIZE)
    {
        // Fjern newline karakteren fra enden af linjen
        line[strcspn(line, "\n")] = 0;
        
        // Split linjen op ved kommaer og kopier data ind i den aktuelle record
        token = strtok(line, ",");
        if (token) strncpy(list[recordCount].name, token, 49);
        
        token = strtok(NULL, ",");
        if (token) strncpy(list[recordCount].address, token, 49);
        
        token = strtok(NULL, ",");
        if (token) list[recordCount].age = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) list[recordCount].phone = (unsigned int)atol(token);
        
        recordCount++;
    }
    fclose(file);
}

// Implementering af bubble sort algoritmen til sortering af records efter telefonnummer
void bubbleSort()
{
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
    printf("Navn: %s\n", record->name);
    printf("Adresse: %s\n", record->address);
    printf("Alder: %d\n", record->age);
    printf("Telefon: %u\n", record->phone);
}

// Hovedprogram - indlæser database, sorterer og håndterer søgninger
int main()
{
    unsigned int searchPhone;
    int foundIndex;
    
    // Indlæs data og sortér
    readDatabase("database.csv");
    printf("Database indlaest. Sorterer efter telefonnummer...\n");
    bubbleSort();
    
    // Hovedloop der håndterer brugerens søgninger
    while (1)
    {
        printf("\nIndtast telefonnummer at soege efter (0 for at afslutte): ");
        scanf("%u", &searchPhone);
        
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
            printf("Telefonnummer ikke fundet.\n");
        }
    }
    return 0;
}