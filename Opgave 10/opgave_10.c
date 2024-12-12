/*********************************************************************
* Opgave 10, Hægtet Liste Database
*
* Navn: Frederik Tots
* Dato: 11/12-2024
*
* Beskrivelse: Program til at håndtere en sorteret database over personer
* ved brug af en hægtet liste (linked list). Programmet læser data fra en 
* CSV-fil ind i en dynamisk allokeret liste af Records, hvor hver record 
* indeholder navn, adresse og telefonnummer.
* 
* Programmet implementerer:
* - Dynamisk hukommelsesallokering med malloc() for fleksibel listestørrelse
* - Automatisk sortering ved indsættelse baseret på navn
* - Effektiv sletning af elementer uden at skulle omorganisere data
* - Robust håndtering af danske tegn via UTF-8 encoding
* - Menu-baseret brugerinterface til databaseoperationer
*
* Data gemmes i en Record struct med en pointer til næste element,
* hvilket muliggør en dynamisk og fleksibel datastruktur. Implementationen
* inkluderer sikkerhedstjek for NULL-pointers og memory leaks.
* 
* Programmet er udviklet som en videreudvikling fra tidligere opgaver,
* hvor et statisk array er erstattet med en hægtet liste for bedre
* hukommelsesudnyttelse og fleksibilitet.
*
* Filformat: Data gemmes i CSV-format med komma-separerede værdier:
* <navn>, <adresse>, <telefonnummer>
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dansk sprog i terminalen
#include <wchar.h>    // For wide character support
#include <windows.h>  // For Windows console functions

// Record node der indeholder persondata:
struct Record
{
   char name[50];
   char address[50]; 
   unsigned int phone;
   struct Record *nextPtr;
};

typedef struct Record Record;
typedef Record *recordPtr;

// Hjælpefunktioner 
void stripNewline(char *str)
{
   str[strcspn(str, "\n")] = 0;  // Fjerner newline fra string
}

/* Funktionsprototyper */
recordPtr createNode(const char *name, const char *address, unsigned int phone);
void insertSorted(recordPtr *headPtr, recordPtr newNode);
void printList(recordPtr currentPtr);
void saveToFile(recordPtr head, const char *filename);
recordPtr loadFromFile(const char *filename);
void deleteRecord(recordPtr *headPtr, const char *name);
void freeList(recordPtr head);

// Hovedprogram med menu-system og brugerinteraktion.
int main()
{
   // Set console to use UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);


   recordPtr head = NULL;
   const char *filename = "mydatabase.txt";
   int choice;
   char buffer[1024];
   char addressBuffer[1024];

   do
   {
       printf("\n1. Indlæs database fra fil");
       printf("\n2. Tilføj ny post");
       printf("\n3. Slet post");
       printf("\n4. Vis alle poster");
       printf("\n5. Gem til fil");
       printf("\n0. Afslut");
       printf("\nVælg handling: ");
       scanf("%d", &choice);
       getchar();

       switch(choice)
       {
           case 1:
               head = loadFromFile(filename);
               printf("Database indlæst.\n");
               break;

           case 2:
           {
               unsigned int phone;
               
               printf("Indtast navn: ");
               fgets(buffer, sizeof(buffer), stdin);
               stripNewline(buffer);
               
               printf("Indtast adresse: ");
               fgets(addressBuffer, sizeof(addressBuffer), stdin);
               stripNewline(addressBuffer);
               
               printf("Indtast telefon: ");
               scanf("%u", &phone);
               getchar();
               
               recordPtr newNode = createNode(buffer, addressBuffer, phone);
               if (newNode != NULL)
               {
                   insertSorted(&head, newNode);
                   printf("Post tilføjet.\n");
               }
               break;
           }

           case 3:
               printf("Indtast navn der skal slettes: ");
               fgets(buffer, sizeof(buffer), stdin);
               stripNewline(buffer);
               deleteRecord(&head, buffer);
               break;

           case 4:
               printList(head);
               break;

           case 5:
               saveToFile(head, filename);
               printf("Database gemt.\n");
               break;
       }
   }
   while (choice != 0);

   freeList(head);
   return 0;
}

// Opretter ny node med kopi af input data
recordPtr createNode(const char *name, const char *address, unsigned int phone)
{
   recordPtr newNode = malloc(sizeof(Record));
   if (newNode != NULL)
   {
       strncpy(newNode->name, name, sizeof(newNode->name) - 1);
       newNode->name[sizeof(newNode->name) - 1] = '\0';
       
       strncpy(newNode->address, address, sizeof(newNode->address) - 1);
       newNode->address[sizeof(newNode->address) - 1] = '\0';
       
       newNode->phone = phone;
       newNode->nextPtr = NULL;
   }
   return newNode;
}

// Frigør node
void freeNode(recordPtr node)
{
   free(node);
}

// Indsætter node i sorteret rækkefølge efter navn
void insertSorted(recordPtr *headPtr, recordPtr newNode)
{
   if (*headPtr == NULL)
   {
       *headPtr = newNode;
       return;
   }
   
   recordPtr current = *headPtr;
   recordPtr previous = NULL;
   
   while (current != NULL && strcmp(current->name, newNode->name) < 0)
   {
       previous = current;
       current = current->nextPtr;
   }
   
   if (previous == NULL)
   {
       newNode->nextPtr = *headPtr;
       *headPtr = newNode;
   }
   else
   {
       previous->nextPtr = newNode;
       newNode->nextPtr = current;
   }
}

// Viser alle poster i databasen
void printList(recordPtr currentPtr)
{
   if (currentPtr == NULL)
   {
       printf("Listen er tom.\n");
       return;
   }
   
   printf("\nPoster i databasen:\n");

   while (currentPtr != NULL)
   {
       printf("Navn: %s\n", currentPtr->name);
       printf("Adresse: %s\n", currentPtr->address);
       printf("Telefon: %u\n\n", currentPtr->phone);
       currentPtr = currentPtr->nextPtr;
   }
}

// Gemmer database til fil i TXT-format
void saveToFile(recordPtr head, const char *filename)
{
   FILE *file = fopen(filename, "w");
   if (file == NULL)
   {
       printf("Fejl ved åbning af fil til skrivning!\n");
       return;
   }
   
   for (recordPtr current = head; current != NULL; current = current->nextPtr)
   {
       fprintf(file, "%s, %s, %u\n", 
               current->name, 
               current->address, 
               current->phone);
   }
   
   fclose(file);
}

// Indlæser database fra CSV-fil
recordPtr loadFromFile(const char *filename)
{
   FILE *file = fopen(filename, "r");
   if (file == NULL)
   {
       printf("Fejl ved åbning af fil til læsning!\n");
       return NULL;
   }
   
   recordPtr head = NULL;
   char line[1024];
   
   while (fgets(line, sizeof(line), file))
   {
       char *token = strtok(line, ",");
       if (!token) continue;
       char *name = token;
           
       token = strtok(NULL, ",");
       if (!token) continue;
       while (*token == ' ') token++;
       char *address = token;
           
       token = strtok(NULL, ",");
       if (!token) continue;
       unsigned int phone = (unsigned int)strtoul(token, NULL, 10);
           
       recordPtr newNode = createNode(name, address, phone);
       if (newNode)
       {
           insertSorted(&head, newNode);
       }
   }
   
   fclose(file);
   return head;
}

// Sletter post fra databasen baseret på navn
void deleteRecord(recordPtr *headPtr, const char *name)
{
   recordPtr current = *headPtr;
   recordPtr previous = NULL;
   
   while (current != NULL && strcmp(current->name, name) != 0)
   {
       previous = current;
       current = current->nextPtr;
   }
   
   if (current == NULL)
   {
       printf("Post ikke fundet.\n");
       return;
   }
   
   if (previous == NULL)
   {
       *headPtr = current->nextPtr;
   }
   else
   {
       previous->nextPtr = current->nextPtr;
   }
   
   freeNode(current);
   printf("Post slettet.\n");
}

// Frigør al allokeret hukommelse i listen
void freeList(recordPtr head)
{
   while (head != NULL)
   {
       recordPtr next = head->nextPtr;
       freeNode(head);
       head = next;
   }
}