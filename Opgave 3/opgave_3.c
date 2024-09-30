/*
// Opgave 3a
#include <stdio.h>
#include <limits.h>

void count_bits_in_int() {
    int n = 1;
    int bits = 0;
    
    while (1) {
        int new_n = 2 * n;
        if (new_n <= n) {
            break;  // Overflow detected
        }
        n = new_n;
        bits++;
    }
    
    printf("Antal bits i int: %d\n", bits);
}

int main() {
    count_bits_in_int();
    return 0;
}
*/



/*
// Opgave 3b
#include <stdio.h>
#include <math.h>

double newton_raphson_sqrt(double N, int iterations) {
    double x_gammel = 1.0; // Start med et vilkårligt gæt
    double x_ny;

    for (int i = 0; i < iterations; i++) {
        x_ny = 0.5 * (x_gammel + N / x_gammel);
        x_gammel = x_ny;
    }

    return x_ny;
}

int main() {
    double N;
    printf("Indtast et tal for at beregne dets kvadratrod: ");
    scanf("%lf", &N);

    if (N < 0) {
        printf("Fejl: Kan ikke beregne kvadratroden af et negativt tal.\n");
        return 1;
    }

    double resultat = newton_raphson_sqrt(N, 10); // Bruger 10 iterationer

    printf("N = %f\n", N);
    printf("Beregnet kvadratrod = %f\n", resultat);
    printf("Faktisk kvadratrod = %f\n", sqrt(N));
    printf("Difference = %e\n", fabs(resultat - sqrt(N)));
    printf("N * N = %f\n", resultat * resultat);

    return 0;
}
*/



/*
// Opgave b option
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

double sin_integral(double dx) {
    double sum = 0.0;
    for (double x = 0; x < PI; x += dx) {
        sum += sin(x) * dx;
    }
    return sum;
}

int main() {
    double dx = 0.0001; // En passende skridtlængde for god nøjagtighed
    double numerisk_resultat = sin_integral(dx);
    double teoretisk_resultat = 2.0; // Den kendte værdi af integralet

    printf("Numerisk beregnet integral: %.8f\n", numerisk_resultat);
    printf("Teoretisk værdi: %.8f\n", teoretisk_resultat);
    printf("Absolut fejl: %.8f\n", fabs(numerisk_resultat - teoretisk_resultat));
    printf("Relativ fejl: %.8f%%\n", fabs(numerisk_resultat - teoretisk_resultat) / teoretisk_resultat * 100);

    return 0;
}
*/