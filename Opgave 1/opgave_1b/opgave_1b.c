#include <stdio.h>
// Dansk sprog i terminalen
#include <wchar.h>    // For wide character support
#include <windows.h>  // For Windows console functions

int main()
{
    // Set console to use UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    int a, b, sum;
    printf("Indtast to heltal [adskilt af mellemrum: a b]: ");
    scanf("%d %d", &a, &b);
    
    sum = a + b;
    printf("Summen er: %d\n", sum);
    
    if (sum > b)
    {
        a = sum - b;  // Gem difference i a som specificeret i opgaven
        printf("Summen er større end B. A er nu: %d\n", a);
    }
    
    return 0; 
}