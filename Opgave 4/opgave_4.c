// Opgave 4
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 100  // Definerer maksimal størrelse for arrayet

// Funktion til at indlæse tal fra brugeren
void indlaes_tal(int *array, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Indtast tal %d: ", i + 1);
        scanf("%d", &array[i]);  // Indlæser tal og gemmer i arrayet
    }
}

// Funktion til at beregne summen af alle tal i arrayet
int beregn_sum(int *array, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += array[i];  // Adderer hvert tal til summen
    }
    return sum;
}

// Funktion til at beregne gennemsnittet
float beregn_gennemsnit(int sum, int n)
{
    return (float)sum / n;  // Konverterer sum til float for at få præcist resultat
}

// Funktion til at finde det laveste tal
int find_laveste(int *array, int n)
{
    int laveste = array[0];  // Antager første element er lavest
    for (int i = 1; i < n; i++)
    {
        if (array[i] < laveste)
        {
            laveste = array[i];  // Opdaterer laveste hvis et mindre tal findes
        }
    }
    return laveste;
}

// Funktion til at finde det højeste tal
int find_hoejeste(int *array, int n)
{
    int hoejeste = array[0];  // Antager første element er højest
    for (int i = 1; i < n; i++)
    {
        if (array[i] > hoejeste)
        {
            hoejeste = array[i];  // Opdaterer højeste hvis et større tal findes
        }
    }
    return hoejeste;
}

// Funktion til at finde og udskrive dubletter
void find_dubletter(int *array, int n)
{
    bool fundet_dublet = false;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (array[i] == array[j])
            {
                if (!fundet_dublet)
                {
                    printf("Dubletter fundet:\n");
                    fundet_dublet = true;
                }
                printf("%d ", array[i]);  // Udskriver dublet
                break;  // Går videre til næste tal i ydre loop
            }
        }
    }
    if (!fundet_dublet)
    {
        printf("Ingen dubletter fundet.\n");
    }
    else
    {
        printf("\n");
    }
}

int main()
{
    int n;
    int array[MAX_SIZE];

    // Indlæs antal tal med fejlhåndtering
    do
    {
        printf("Indtast antal tal (max 100): ");
        scanf("%d", &n);
        if (n > MAX_SIZE)
        {
            printf("Fejl: Maksimalt 100 tal tilladt\n");
        }
        else if (n < 0)
        {
            printf("Fejl: Antal tal skal være positivt\n");
        }
    }
    while (n > MAX_SIZE || n < 0);

    if (n == 0)
    {
        printf("Ingen tal at analysere\n");
        return 0;
    }

    indlaes_tal(array, n);

    int sum = beregn_sum(array, n);
    float gennemsnit = beregn_gennemsnit(sum, n);
    int laveste = find_laveste(array, n);
    int hoejeste = find_hoejeste(array, n);

    // Udskriver resultater
    printf("\nResultater:\n");
    printf("Sum: %d\n", sum);
    printf("Gennemsnit: %.2f\n", gennemsnit);
    printf("Laveste tal: %d\n", laveste);
    printf("Højeste tal: %d\n", hoejeste);
    
    find_dubletter(array, n);

    return 0;
}



























// Opgave 4B
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 100  // Definerer maksimal størrelse for arrayet

// Funktion til at indlæse tal fra brugeren
void indlaes_tal(int *array, int n)
{
    for (int i = 0; i < n; i++) {
        printf("Indtast tal %d: ", i + 1);
        scanf("%d", &array[i]);  // Indlæser tal og gemmer i arrayet
    }
}

// Funktion til at beregne summen af alle tal i arrayet
int beregn_sum(int *array, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += array[i];  // Adderer hvert tal til summen
    }
    return sum;
}

// Funktion til at beregne gennemsnittet
float beregn_gennemsnit(int sum, int n)
{
    return (float)sum / n;  // Konverterer sum til float for at få præcist resultat
}

// Funktion til at finde det laveste tal (antager sorteret array)
int find_laveste(int *array, int n)
{
    return array[0];  // Efter sortering er det første element det laveste
}

// Funktion til at finde det højeste tal (antager sorteret array)
int find_hoejeste(int *array, int n)
{
    return array[n-1];  // Efter sortering er det sidste element det højeste
}

// Hjælpefunktion til at bytte to elementer
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

// Partitionsfunktion til QuickSort
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];  // Vælger sidste element som pivot
    int i = (low - 1);  // Index for mindre element
    
    for (int j = low; j <= high - 1; j++)
    {
        // Hvis nuværende element er mindre end pivot
        if (arr[j] < pivot) {
            i++;  // Incrementer index for mindre element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Rekursiv QuickSort funktion
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);  // Før partition
        quickSort(arr, pi + 1, high);  // Efter partition
    }
}

// Rekursiv binær søgefunktion
int binarySearch(int arr[], int l, int r, int x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        if (arr[mid] == x)
            return mid;
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);
        return binarySearch(arr, mid + 1, r, x);
    }
    return -1;  // Element ikke fundet
}

// Funktion til at finde og udskrive dubletter
void find_dubletter(int *array, int n)
{
    int dublet_count = 0;
    printf("Dubletter fundet:\n");
    
    for (int i = 0; i < n - 1; i++)
    {
        if (i > 0 && array[i] == array[i-1]) continue; // Spring over hvis vi allerede har talt denne dublet
        
        int result = binarySearch(array, i+1, n-1, array[i]);
        if (result != -1) {
            int count = 2; // Vi har fundet mindst én dublet
            dublet_count++;
            
            // Tæl alle forekomster
            for (int j = result + 1; j < n && array[j] == array[i]; j++)
            {
                count++;
            }
            for (int j = i + 1; j < result && array[j] == array[i]; j++)
            {
                count++;
            }
            
            printf("%d (forekommer %d gange)\n", array[i], count);
        }
    }
    
    if (dublet_count == 0)
    {
        printf("Ingen dubletter fundet.\n");
    }
    else
    {
        printf("Antal forskellige dubletter: %d\n", dublet_count);
    }
}

int main() {
    int n;
    int array[MAX_SIZE];

    // Indlæs antal tal med fejlhåndtering
    do {
        printf("Indtast antal tal (max 100): ");
        scanf("%d", &n);
        if (n > MAX_SIZE)
        {
            printf("Fejl: Maksimalt 100 tal tilladt\n");
        } else if (n < 0)
        {
            printf("Fejl: Antal tal skal være positivt\n");
        }
    }
    while (n > MAX_SIZE || n < 0);

    if (n == 0)
    {
        printf("Ingen tal at analysere\n");
        return 0;
    }

    indlaes_tal(array, n);

    quickSort(array, 0, n-1);  // Sorterer arrayet

    int sum = beregn_sum(array, n);
    float gennemsnit = beregn_gennemsnit(sum, n);
    int laveste = find_laveste(array, n);
    int hoejeste = find_hoejeste(array, n);

    // Udskriver resultater
    printf("\nResultater:\n");
    printf("Sum: %d\n", sum);
    printf("Gennemsnit: %.2f\n", gennemsnit);
    printf("Laveste tal: %d\n", laveste);
    printf("Højeste tal: %d\n", hoejeste);
    
    find_dubletter(array, n);

    return 0;
}

























// Opgave 4C
#include <stdio.h>

// Funktion til at konvertere et unsigned char til binær repræsentation
void unsigned_char_to_binary(unsigned char num)
{
    int binary[8] = {0}; // Initialiser array med nuller
    int i = 0;

    // Konverter til binær
    while (num > 0 && i < 8)
    {
        binary[i] = num % 2;  // Gem rest ved division med 2 (enten 0 eller 1)
        num = num / 2;        // Divider tallet med 2
        i++;                  // Gå til næste bit
    }

    // Udskriv binær repræsentation
    printf("Binær repræsentation: ");
    for (int j = 7; j >= 0; j--)
    {
        printf("%d", binary[j]);  // Udskriv bits fra mest signifikante til mindst signifikante
    }
    printf("\n");
}

int main()
{
    unsigned char num;
    
    printf("Indtast et tal mellem 0 og 255: ");
    scanf("%hhu", &num);  // Læs et unsigned char (8-bit heltal)

    unsigned_char_to_binary(num);  // Kald funktionen til at konvertere og udskrive binær repræsentation

    return 0;
}














