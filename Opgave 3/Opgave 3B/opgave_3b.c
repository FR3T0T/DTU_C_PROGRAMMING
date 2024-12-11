#include <stdio.h>
#include <math.h>

double beregn_kvadratrod(double N, double tolerance)
{
    if (N < 0)
    {
        return -1.0; // Indikerer en fejl
    }
    
    double x_gammel = 1.0;
    double x_ny;
    
    do
    {
        x_ny = 0.5 * (x_gammel + N / x_gammel);
        
        // Check om vi har nået den ønskede præcision
        if (fabs(x_ny - x_gammel) < tolerance)
        {
            break;
        }
        
        x_gammel = x_ny;
    }
    while (1);
    
    return x_ny;
}

int main()
{
    double N;
    printf("Indtast et tal for at beregne dets kvadratrod: ");
    scanf("%lf", &N);

    if (N < 0)
    {
        printf("Fejl: Kan ikke beregne kvadratroden af et negativt tal.\n");
        return 1;
    }

    double resultat = beregn_kvadratrod(N, 1e-10); // Tolerance på 10^-10

    printf("N = %.10f\n", N);
    printf("Beregnet kvadratrod = %.10f\n", resultat);
    printf("Faktisk kvadratrod = %.10f\n", sqrt(N));
    printf("Difference = %.10e\n", fabs(resultat - sqrt(N)));
    printf("Verificering: x * x = %.10f\n", resultat * resultat);
    printf("Afvigelse fra N: %.10e\n", fabs(N - (resultat * resultat)));

    return 0;
}