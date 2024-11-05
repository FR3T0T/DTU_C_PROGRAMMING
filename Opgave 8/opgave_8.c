#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_PERSONS 100
#define FILENAME "database.csv"

// Struct for storing person information
typedef struct
{
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char phoneNumber[MAX_PHONE_LENGTH];
} Person;

// Global array of pointers to Person structs
Person* database[MAX_PERSONS];
int personCount = 0;

// Function prototypes
void addPerson(const char* firstName, const char* lastName, const char* phoneNumber);
Person* findPersonByPhone(const char* phoneNumber);
void loadDatabase();
void saveDatabase();
void printPerson(const Person* person);
void printAllPersons();
void cleanup();

// Add new person to database
void addPerson(const char* firstName, const char* lastName, const char* phoneNumber)
{
    if (personCount >= MAX_PERSONS)
    {
        printf("Database is full!\n");
        return;
    }
    
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL)
    {
        printf("Memory allocation error!\n");
        return;
    }
    
    strncpy(newPerson->firstName, firstName, MAX_NAME_LENGTH - 1);
    strncpy(newPerson->lastName, lastName, MAX_NAME_LENGTH - 1);
    strncpy(newPerson->phoneNumber, phoneNumber, MAX_PHONE_LENGTH - 1);
    
    newPerson->firstName[MAX_NAME_LENGTH - 1] = '\0';
    newPerson->lastName[MAX_NAME_LENGTH - 1] = '\0';
    newPerson->phoneNumber[MAX_PHONE_LENGTH - 1] = '\0';
    
    database[personCount++] = newPerson;
    printf("Person added to database.\n");
}

// Find person by phone number
Person* findPersonByPhone(const char* phoneNumber)
{
    for (int i = 0; i < personCount; i++)
    {
        if (strcmp(database[i]->phoneNumber, phoneNumber) == 0)
        {
            return database[i];
        }
    }
    return NULL;
}

// Load database from file
void loadDatabase()
{
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Could not open file for reading.\n");
        return;
    }
    
    char line[MAX_NAME_LENGTH * 2 + MAX_PHONE_LENGTH + 3];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char phoneNumber[MAX_PHONE_LENGTH];
    
    while (fgets(line, sizeof(line), file) != NULL && personCount < MAX_PERSONS)
    {
        // Remove newline if present
        line[strcspn(line, "\n")] = 0;
        
        // Parse the line
        if (sscanf(line, "%[^,],%[^,],%s", firstName, lastName, phoneNumber) == 3)
        {
            addPerson(firstName, lastName, phoneNumber);
        }
    }
    
    fclose(file);
    printf("Database loaded from file.\n");
}

// Save database to file
void saveDatabase()
{
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        printf("Could not open file for writing.\n");
        return;
    }
    
    for (int i = 0; i < personCount; i++)
    {
        fprintf(file, "%s,%s,%s\n", 
                database[i]->firstName,
                database[i]->lastName,
                database[i]->phoneNumber);
    }
    
    fclose(file);
    printf("Database saved to file.\n");
}

// Print a single person
void printPerson(const Person* person)
{
    if (person != NULL)
    {
        printf("Name: %s %s, Phone: %s\n",
               person->firstName,
               person->lastName,
               person->phoneNumber);
    }
}

// Print all persons in database
void printAllPersons()
{
    printf("\nAll persons in database:\n");
    for (int i = 0; i < personCount; i++)
    {
        printf("%d. ", i + 1);
        printPerson(database[i]);
    }
    printf("\n");
}

// Free allocated memory
void cleanup()
{
    for (int i = 0; i < personCount; i++)
    {
        free(database[i]);
    }
    personCount = 0;
}

int main()
{
    int choice;
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char phoneNumber[MAX_PHONE_LENGTH];
    
    // Load existing database at startup
    loadDatabase();
    
    while (1) {
        printf("\nPerson Database Menu:\n");
        printf("1. Add person\n");
        printf("2. Search for person (phone number)\n");
        printf("3. Show all persons\n");
        printf("4. Save database\n");
        printf("5. Exit\n");
        printf("Select (1-5): ");
        
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid choice!\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice)
        {
            case 1:
                printf("Enter first name: ");
                scanf("%s", firstName);
                printf("Enter last name: ");
                scanf("%s", lastName);
                printf("Enter phone number: ");
                scanf("%s", phoneNumber);
                addPerson(firstName, lastName, phoneNumber);
                break;
                
            case 2:
                printf("Enter phone number: ");
                scanf("%s", phoneNumber);
                Person* found = findPersonByPhone(phoneNumber);
                if (found != NULL)
                {
                    printf("Person found:\n");
                    printPerson(found);
                }
                else
                {
                    printf("No person found with this phone number.\n");
                }
                break;
                
            case 3:
                printAllPersons();
                break;
                
            case 4:
                saveDatabase();
                break;
                
            case 5:
                saveDatabase();
                cleanup();
                printf("Program terminated.\n");
                return 0;
                
            default:
                printf("Invalid choice! Choose between 1-5.\n");
        }
    }
    
    return 0;
}