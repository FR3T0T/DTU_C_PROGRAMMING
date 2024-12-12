/*********************************************************************
* Opgave 5: Pointers
* 
* Navn: Frederik Tots
* Dato: 11/12-2024
*
* Beskrivelse: Program til at tælle forekomster af specifikke karakterer 
* i en tekst. Programmet implementerer en funktion der tæller hvor mange
* gange bestemte bogstaver (f.eks. vokaler) forekommer i en given tekst.
*
* Hovedfunktionalitet:
* - Tæller specifikke karakterer i en tekst ved brug af pointere
* - Håndterer både små og store bogstaver
* - Inkluderer test med prædefineret eksempel
* - Tillader brugerindtastning af tekst (op til 100 tegn)
* 
* Implementation er baseret på kapitel 7 om pointere og inkluderer
* sikker input-håndtering samt const-kvalificerede parametre for
* at beskytte input-data.
*********************************************************************/

#include <stdio.h>   // Inkluderer standard input/output bibliotek
#include <string.h>  // Inkluderer string-håndteringsbibliotek

// Dansk sprog i terminalen
#include <wchar.h>    // For wide character support
#include <windows.h>  // For Windows console functions

/**
 * Tæller antallet af karakterer fra 'text' som findes i 'set'.
 * 
 * @param text Inputteksten der skal analyseres.
 * @param set Mængden af karakterer der skal tælles.
 * @return Antallet af karakterer fra 'set' fundet i 'text'.
 */
int countCharactersInSet(const char* text, const char* set)
{
    int count = 0;  // Initialiserer tælleren
    
    // Løkke der kører indtil slutningen af 'text' (nul-terminatoren)
    while (*text != '\0')
    {
        const char* setPtr = set;  // Peger på starten af 'set' for hver ny karakter i 'text'
        
        // Indre løkke der tjekker hvert tegn i 'set'
        while (*setPtr != '\0')
        {
            // Hvis det aktuelle tegn i 'text' matcher et tegn i 'set'
            if (*text == *setPtr)
            {
                count++;  // Øger tælleren
                break;    // Afbryder den indre løkke, da vi har fundet et match
            }
            setPtr++;  // Går videre til næste tegn i 'set'
        }
        text++;  // Går videre til næste tegn i 'text'
    }
    
    return count;  // Returnerer det samlede antal matches
}

int main()
{
    // Set console to use UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    const int maxlength = 100;                    // Definerer maksimal længde for inputtekst
    char mytext[maxlength];                       // Array til at indeholde inputteksten
    const char* vowels = "aeiouyæøåAEIOUYÆØÅ";         // Streng med alle vokaler (små og store), inkl. 'y'

    // Test med eksemplet fra opgaven
    int example = countCharactersInSet("Denmark is a democratic country", vowels);
    printf("Antal vokaler i eksempelteksten \"Denmark is a democratic country\": %d\n\n", example);

    // Prompter brugeren om at indtaste tekst
    printf("Indtast en tekst (max %d tegn): ", maxlength - 1);
    
    // Læser tekst fra konsollen, inkluderer plads til nul-terminatoren
    fgets(mytext, maxlength, stdin);

    // Fjerner newline-tegnet hvis det er til stede
    mytext[strcspn(mytext, "\n")] = 0;

    // Kalder funktionen for at tælle vokaler
    int vowelCount = countCharactersInSet(mytext, vowels);

    // Udskriver resultatet
    printf("Antal vokaler i \"%s\": %d\n", mytext, vowelCount);

    return 0;  // Indikerer succesfuld afslutning af programmet
}