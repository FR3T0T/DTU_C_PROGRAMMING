/*******************************************************************************
* Person Database Management System
*
* Brugen af Cp-Pilot, ChatGPT og ClaudeAI er blevet brugt til fejlfinding af kode.
*
* Systemformaal:
* - Haandterer en database af personer med kontaktinformation
* - Tillader tilfoejelse og soegning af personer
* - Gemmer data persistent i CSV-fil format
* - Implementerer sikker input haandtering
*
* Funktionalitet:
* - CRUD operationer paa persondata
* - CSV fil baseret data persistence
* - Beskyttelse mod buffer overflow
* - Validering af telefonnumre
*
* Udviklingsplatform: Standard C
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************************
* System Konstanter og Konfiguration
****************************************************************************/
#define MAX_LEN 50        // Maksimal laengde for tekstinput
#define MAX_PERSONS 100   // Maksimalt antal personer i databasen
#define DB_FILE "database.csv"  // Databasens filnavn

/****************************************************************************
* Data Strukturer
****************************************************************************/
/************************************************/
/* Person Struktur                              */
/* Indeholder:                                  */
/*   - fornavn  (max MAX_LEN karakterer)        */
/*   - efternavn (max MAX_LEN karakterer)       */
/*   - telefon   (8 cifre, valideret)           */
/************************************************/
typedef struct
{
    char fname[MAX_LEN];    
    char lname[MAX_LEN];    
    char phone[MAX_LEN];    
} Person;

/****************************************************************************
* Globale Variable
****************************************************************************/
Person* db[MAX_PERSONS];    // Database array af person-pegepinde
int count = 0;              // Aktuelle antal personer i databasen

/****************************************************************************
* Input/Output Funktioner
****************************************************************************/
/************************************************/
/* safe_input                                   */
/* Formaal:                                     */
/*   Sikker indlaesning af bruger input         */
/* Input:                                       */
/*   dest:    Destination buffer                */
/*   max_len: Maksimal laengde paa input        */
/* Beskyttelse:                                 */
/*   - Buffer overflow prevention               */
/*   - Null-terminering garanteret              */
/************************************************/
void safe_input(char* dest, int max_len)
{
    char buffer[MAX_LEN * 2];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strncpy(dest, buffer, max_len - 1);
    dest[max_len - 1] = 0;
}

/************************************************/
/* add_person                                   */
/* Formaal:                                     */
/*   Tilfoej ny person til databasen            */
/* Validering:                                  */
/*   - Database kapacitet                       */
/*   - Hukommelsestildeling                     */
/*   - Telefonnummer format (8 cifre)           */
/************************************************/
void add_person()
{
    if (count >= MAX_PERSONS)
    {
        printf("Database fuld!\n");
        return;
    }
    
    if (!(db[count] = malloc(sizeof(Person))))
    {
        printf("Hukommelsesfejl!\n");
        return;
    }
    
    while(getchar() != '\n');
    printf("Fornavn: ");
    safe_input(db[count]->fname, MAX_LEN);
    printf("Efternavn: ");
    safe_input(db[count]->lname, MAX_LEN);
    printf("Telefon (8 cifre): ");
    safe_input(db[count]->phone, MAX_LEN);
    
    if(strlen(db[count]->phone) != 8 || strspn(db[count]->phone, "0123456789") != 8)
    {
        printf("Ugyldigt telefonnummer!\n");
        free(db[count]);
        return;
    }
    count++;
}

/************************************************/
/* Database Operationer                         */
/* find_person: Soeger efter person via telefon */
/* save_db: Gemmer database til CSV fil         */
/* load_db: Indlaeser database fra CSV fil      */
/* show_all: Viser alle personer i databasen    */
/* cleanup: Frigoer allokeret hukommelse        */
/************************************************/
void find_person()
{
    char phone[MAX_LEN];
    printf("Indtast telefon: ");
    while(getchar() != '\n');
    safe_input(phone, MAX_LEN);
    
    for (int i = 0; i < count; i++)
        if (!strcmp(db[i]->phone, phone))
        {
            printf("Fundet: %s %s, %s\n", 
                   db[i]->fname, db[i]->lname, db[i]->phone);
            return;
        }
    printf("Ikke fundet!\n");
}

void save_db()
{
    FILE* f = fopen(DB_FILE, "w");
    if (!f) return;
    for (int i = 0; i < count; i++)
        fprintf(f, "%s,%s,%s\n", db[i]->fname, db[i]->lname, db[i]->phone);
    fclose(f);
    printf("Gemt.\n");
}

void load_db()
{
    FILE* f = fopen(DB_FILE, "r");
    char line[MAX_LEN * 3];
    if (!f) return;
    
    while (count < MAX_PERSONS && fgets(line, sizeof(line), f))
    {
        if (!(db[count] = malloc(sizeof(Person)))) break;
        if (sscanf(line, "%[^,],%[^,],%s",
            db[count]->fname, db[count]->lname, db[count]->phone) == 3)
            count++;
        else
            free(db[count]);
    }
    fclose(f);
    printf("Indlaest %d poster.\n", count);
}

void show_all()
{
    printf("\nAlle personer:\n");
    for (int i = 0; i < count; i++)
        printf("%d. %s %s, %s\n", i+1, 
               db[i]->fname, db[i]->lname, db[i]->phone);
}

void cleanup()
{
    while(count > 0) free(db[--count]);
}

/****************************************************************************
* Hovedprogram
****************************************************************************/
int main()
{
    int choice;
    load_db();
    
    while (1)
    {
        printf("\nMenu:\n"
               "1. Tilfoej person\n"
               "2. Soeg via telefon\n"
               "3. Vis alle\n"
               "4. Gem\n"
               "5. Afslut\n"
               "\nValg: ");
               
        if (scanf("%d", &choice) != 1)
        {
            while(getchar() != '\n');
            printf("Ugyldigt input!\n");
            continue;
        }
        
        switch (choice)
        {
            case 1: add_person(); break;
            case 2: find_person(); break;
            case 3: show_all(); break;
            case 4: save_db(); break;
            case 5: save_db(); cleanup(); return 0;
            default: printf("Ugyldigt valg!\n");
        }
    }
}