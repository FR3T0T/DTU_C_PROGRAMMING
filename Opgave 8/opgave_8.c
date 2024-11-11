#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50
#define MAX_PERSONS 100
#define DB_FILE "database.csv"

typedef struct {
    char fname[MAX_LEN];
    char lname[MAX_LEN];
    char phone[MAX_LEN];
} Person;

Person* db[MAX_PERSONS];
int count = 0;

// Sikker input funktion
void safe_input(char* dest, int max_len) {
    char buffer[MAX_LEN * 2];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strncpy(dest, buffer, max_len - 1);
    dest[max_len - 1] = 0;
}

void add_person() {
    if (count >= MAX_PERSONS) {
        printf("Database full!\n");
        return;
    }
    
    if (!(db[count] = malloc(sizeof(Person)))) {
        printf("Memory error!\n");
        return;
    }
    
    while(getchar() != '\n'); // Clear buffer
    printf("First name: ");
    safe_input(db[count]->fname, MAX_LEN);
    printf("Last name: ");
    safe_input(db[count]->lname, MAX_LEN);
    printf("Phone (8 digits): ");
    safe_input(db[count]->phone, MAX_LEN);
    
    // Validate phone number
    if(strlen(db[count]->phone) != 8 || strspn(db[count]->phone, "0123456789") != 8) {
        printf("Invalid phone number!\n");
        free(db[count]);
        return;
    }
    count++;
}

void find_person() {
    char phone[MAX_LEN];
    printf("Enter phone: ");
    while(getchar() != '\n');
    safe_input(phone, MAX_LEN);
    
    for (int i = 0; i < count; i++)
        if (!strcmp(db[i]->phone, phone)) {
            printf("Found: %s %s, %s\n", 
                   db[i]->fname, db[i]->lname, db[i]->phone);
            return;
        }
    printf("Not found!\n");
}

void save_db() {
    FILE* f = fopen(DB_FILE, "w");
    if (!f) return;
    for (int i = 0; i < count; i++)
        fprintf(f, "%s,%s,%s\n", db[i]->fname, db[i]->lname, db[i]->phone);
    fclose(f);
    printf("Saved.\n");
}

void load_db() {
    FILE* f = fopen(DB_FILE, "r");
    char line[MAX_LEN * 3];
    if (!f) return;
    
    while (count < MAX_PERSONS && fgets(line, sizeof(line), f)) {
        if (!(db[count] = malloc(sizeof(Person)))) break;
        if (sscanf(line, "%[^,],%[^,],%s", 
            db[count]->fname, db[count]->lname, db[count]->phone) == 3)
            count++;
        else
            free(db[count]);
    }
    fclose(f);
    printf("Loaded %d records.\n", count);
}

void show_all() {
    printf("\nAll persons:\n");
    for (int i = 0; i < count; i++)
        printf("%d. %s %s, %s\n", i+1, 
               db[i]->fname, db[i]->lname, db[i]->phone);
}

void cleanup() {
    while(count > 0) free(db[--count]);
}

int main() {
    int choice;
    load_db();
    
    while (1) {
        printf("\nMenu:\n"
               "1. Add person\n"
               "2. Search by phone\n"
               "3. Show all\n"
               "4. Save\n"
               "5. Exit\n"
               "Choice: ");
               
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Invalid input!\n");
            continue;
        }
        
        switch (choice) {
            case 1: add_person(); break;
            case 2: find_person(); break;
            case 3: show_all(); break;
            case 4: save_db(); break;
            case 5: save_db(); cleanup(); return 0;
            default: printf("Invalid choice!\n");
        }
    }
}