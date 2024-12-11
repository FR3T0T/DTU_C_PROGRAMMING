/*********************************************************************
* Opgave 3 (Del A)
* 
* Navn: Frederik Tots
* Dato: 10/12-2024
*
* Beskrivelse: Program til at finde antallet af bits i en int-datatype.
* Programmet bruger en algoritme der fordobler et tal gentagne gange,
* indtil overflow sker, og tæller dermed antallet af bits der kan
* repræsenteres i en int på det aktuelle system.
*********************************************************************/

#include <stdio.h>

int find_int_bits()
{
    int n = 1;
    int bits = 0;
    
    while (1)
    {
        int old_n = n;
        n = n * 2;
        
        if (n <= old_n) // Sammenligner ny værdi med gammel værdi
        {
            break;
        }
        bits++;
    }
    
    printf("Antal bits i int: %d\n", bits + 1);  // +1 fordi vi starter med 1
    return bits + 1;
}

int main()
{
    find_int_bits();
    return 0;
}