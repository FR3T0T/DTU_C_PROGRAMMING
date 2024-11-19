#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>  // For uint64_t

// Definerer Record strukturen med dynamiske strings
typedef struct Record {
    char *name;      // Dynamisk allokeret string
    char *address;   // Dynamisk allokeret string
    uint64_t phone;  // Mere passende for telefonnumre
    struct Record *nextPtr;
} Record;

typedef Record *recordPtr;

// Funktionsprototyper
recordPtr createNode(const char *name, const char *address, uint64_t phone);
void insertSorted(recordPtr *headPtr, recordPtr newNode);
void printList(recordPtr currentPtr);
void saveToFile(recordPtr head, const char *filename);
recordPtr loadFromFile(const char *filename);
void deleteRecord(recordPtr *headPtr, const char *name);
void freeList(recordPtr *headPtr);

// Hovedfunktion
int main() {
    recordPtr head = NULL;
    const char *filename = "mydatabase.txt";
    int choice;
    char buffer[1024];  // Temporær buffer til input
    
    do {
        printf("\n1. Load database from file");
        printf("\n2. Add new record");
        printf("\n3. Delete record");
        printf("\n4. Show all records");
        printf("\n5. Save to file");
        printf("\n0. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        getchar(); // Fjern newline

        switch(choice) {
            case 1: {
                head = loadFromFile(filename);
                printf("Database loaded.\n");
                break;
            }
            case 2: {
                uint64_t phone;
                
                printf("Enter name: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                char *name = strdup(buffer);  // Dynamisk allokering af navn
                
                printf("Enter address: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                char *address = strdup(buffer);  // Dynamisk allokering af adresse
                
                printf("Enter phone: ");
                scanf("%lu", &phone);
                
                recordPtr newNode = createNode(name, address, phone);
                if (newNode != NULL) {
                    insertSorted(&head, newNode);
                    printf("Record added.\n");
                }
                
                // Frigør de midlertidige strenge
                free(name);
                free(address);
                break;
            }
            case 3: {
                printf("Enter name to delete: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                deleteRecord(&head, buffer);
                break;
            }
            case 4: {
                printList(head);
                break;
            }
            case 5: {
                saveToFile(head, filename);
                printf("Database saved.\n");
                break;
            }
        }
    } while (choice != 0);

    freeList(&head);
    return 0;
}

// Opret ny node med dynamisk hukommelsesallokering
recordPtr createNode(const char *name, const char *address, uint64_t phone) {
    recordPtr newNode = malloc(sizeof(Record));
    if (newNode != NULL) {
        // Alloker hukommelse til strings og kopier data
        newNode->name = strdup(name);
        if (newNode->name == NULL) {
            free(newNode);
            return NULL;
        }
        
        newNode->address = strdup(address);
        if (newNode->address == NULL) {
            free(newNode->name);
            free(newNode);
            return NULL;
        }
        
        newNode->phone = phone;
        newNode->nextPtr = NULL;
    }
    return newNode;
}

// Frigør node og dens dynamisk allokerede data
void freeNode(recordPtr node) {
    if (node != NULL) {
        free(node->name);
        free(node->address);
        free(node);
    }
}

// Indsæt node i sorteret rækkefølge
void insertSorted(recordPtr *headPtr, recordPtr newNode) {
    recordPtr current;
    recordPtr previous = NULL;
    
    if (*headPtr == NULL) {
        *headPtr = newNode;
    }
    else {
        current = *headPtr;
        
        while (current != NULL && strcmp(current->name, newNode->name) < 0) {
            previous = current;
            current = current->nextPtr;
        }
        
        if (previous == NULL) {
            newNode->nextPtr = *headPtr;
            *headPtr = newNode;
        }
        else {
            previous->nextPtr = newNode;
            newNode->nextPtr = current;
        }
    }
}

// Udskriv liste
void printList(recordPtr currentPtr) {
    if (currentPtr == NULL) {
        printf("List is empty.\n");
        return;
    }
    
    printf("\nRecords in database:\n");
    while (currentPtr != NULL) {
        printf("Name: %s\n", currentPtr->name);
        printf("Address: %s\n", currentPtr->address);
        printf("Phone: %lu\n\n", currentPtr->phone);
        currentPtr = currentPtr->nextPtr;
    }
}

// Gem til fil
void saveToFile(recordPtr head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    
    recordPtr current = head;
    while (current != NULL) {
        fprintf(file, "%s, %s, %lu\n", 
                current->name, 
                current->address, 
                current->phone);
        current = current->nextPtr;
    }
    
    fclose(file);
}

// Indlæs fra fil med dynamisk hukommelsesallokering
recordPtr loadFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading!\n");
        return NULL;
    }
    
    recordPtr head = NULL;
    char line[1024];  // Større buffer til hele linjen
    char *name, *address;
    uint64_t phone;
    
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (token != NULL) {
            name = strdup(token);
            
            token = strtok(NULL, ",");
            if (token != NULL) {
                while (*token == ' ') token++; // Skip leading spaces
                address = strdup(token);
                
                token = strtok(NULL, ",");
                if (token != NULL) {
                    phone = strtoull(token, NULL, 10);
                    
                    recordPtr newNode = createNode(name, address, phone);
                    if (newNode != NULL) {
                        insertSorted(&head, newNode);
                    }
                }
            }
            free(name);
            free(address);
        }
    }
    
    fclose(file);
    return head;
}

// Slet record med proper hukommelsesfrigørelse
void deleteRecord(recordPtr *headPtr, const char *name) {
    recordPtr current = *headPtr;
    recordPtr previous = NULL;
    
    while (current != NULL && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->nextPtr;
    }
    
    if (current == NULL) {
        printf("Record not found.\n");
        return;
    }
    
    if (previous == NULL) {
        *headPtr = current->nextPtr;
    }
    else {
        previous->nextPtr = current->nextPtr;
    }
    
    freeNode(current);
    printf("Record deleted.\n");
}

// Frigør hele listen og al dynamisk allokeret hukommelse
void freeList(recordPtr *headPtr) {
    recordPtr current = *headPtr;
    recordPtr next;
    
    while (current != NULL) {
        next = current->nextPtr;
        freeNode(current);
        current = next;
    }
    
    *headPtr = NULL;
}