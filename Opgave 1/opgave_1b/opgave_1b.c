#include <stdio.h>
int main()
{
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