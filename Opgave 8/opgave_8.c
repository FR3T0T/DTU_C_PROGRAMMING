#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dansk sprog i terminalen
#include <wchar.h>    // For wide character support
#include <windows.h>  // For Windows console functions

#define MAX_LEN 50        // Maksimal længde for tekstinput
#define MAX_PERSONS 100   // Maksimalt antal personer i databasen
#define DB_FILE "database.csv"  // Databasens filnavn

// Struct der holder data i separate felter for bedre strukturering,
// selvom opgaveeksemplet viser "Lucky,Luke,11111111" format
typedef struct
{
    char fname[MAX_LEN];    
    char lname[MAX_LEN];    
    char phone[MAX_LEN];    
} Person;

Person* db[MAX_PERSONS];    // Database array af person-pegepinde
int count = 0;              // Aktuelle antal personer i databasen

void safe_input(char* dest, int max_len)
{
    char buffer[MAX_LEN * 2];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strncpy(dest, buffer, max_len - 1);
    dest[max_len - 1] = 0;
}

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

void delete_person()
{
    char phone[MAX_LEN];
    printf("Indtast telefonnummer på person der skal slettes: ");
    while(getchar() != '\n');
    safe_input(phone, MAX_LEN);
    
    for (int i = 0; i < count; i++)
    {
        if (!strcmp(db[i]->phone, phone))
        {
            printf("Sletter: %s %s, %s\n", 
                   db[i]->fname, db[i]->lname, db[i]->phone);
            free(db[i]);
            // Flyt resterende elementer
            for(int j = i; j < count - 1; j++)
            {
                db[j] = db[j + 1];
            }
            count--;
            return;
        }
    }
    printf("Person med dette telefonnummer blev ikke fundet!\n");
}

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
    if (!f) {
        printf("Kunne ikke åbne fil til gemning!\n");
        return;
    }
    for (int i = 0; i < count; i++)
        fprintf(f, "%s,%s,%s\n", db[i]->fname, db[i]->lname, db[i]->phone);
    fclose(f);
    printf("Database gemt.\n");
}

void load_db()
{
    FILE* f = fopen(DB_FILE, "r");
    if (!f) {
        printf("Kunne ikke åbne databasefilen. Starter med tom database.\n");
        return;
    }
    
    char line[MAX_LEN * 3];
    while (count < MAX_PERSONS && fgets(line, sizeof(line), f))
    {
        if (!(db[count] = malloc(sizeof(Person)))) {
            printf("Hukommelsesfejl under indlæsning!\n");
            break;
        }
        if (sscanf(line, "%[^,],%[^,],%s",
            db[count]->fname, db[count]->lname, db[count]->phone) == 3)
            count++;
        else {
            printf("Fejl i filformat på linje %d\n", count + 1);
            free(db[count]);
        }
    }
    fclose(f);
    printf("Indlæst %d poster.\n", count);
}

void show_all()
{
    if (count == 0) {
        printf("\nDatabasen er tom.\n");
        return;
    }
    printf("\nAlle personer:\n");
    for (int i = 0; i < count; i++)
        printf("%d. %s %s, %s\n", i+1, 
               db[i]->fname, db[i]->lname, db[i]->phone);
}

void cleanup()
{
    while(count > 0) free(db[--count]);
}

int main()
{
    // Set console to use UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    int choice;
    load_db();
    
    while (1)
    {
        printf("\nMenu:\n"
               "1. Tilføj person\n"
               "2. Søg via telefon\n"
               "3. Vis alle\n"
               "4. Slet person\n"
               "5. Gem\n"
               "6. Afslut\n"
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
            case 4: delete_person(); break;
            case 5: save_db(); break;
            case 6: save_db(); cleanup(); return 0;
            default: printf("Ugyldigt valg!\n");
        }
    }
}