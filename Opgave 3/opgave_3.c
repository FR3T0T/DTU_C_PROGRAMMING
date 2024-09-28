/*
// Opgave 3a
#include <stdio.h>

int count_int_bits() {
    int n = 1;
    int bits = 0;
    int previous = 0;

    while (n > previous) {
        previous = n;
        n *= 2;
        bits++;
    }

    return bits;
}

int main() {
    int result = count_int_bits();
    printf("Antal bits i int: %d\n", result);
    return 0;
}
*/


/*
// Opgave 3b
#include <stdio.h>
#include <math.h>

double newton_raphson_sqrt(double N, int iterations) {
    if (N < 0) {
        printf("Fejl: Negativt input ikke tilladt.\n");
        return -1;
    }

    double x = 1; // Start gæt
    for (int i = 0; i < iterations; i++) {
        x = 0.5 * (x + N / x);
    }
    return x;
}

int main() {
    double N;
    int iterations = 10; // Antal iterationer

    printf("Indtast et positivt tal for at beregne dets kvadratrod: ");
    scanf("%lf", &N);

    double result = newton_raphson_sqrt(N, iterations);
    if (result != -1) {
        printf("Beregnet kvadratrod af %.4f er %.4f\n", N, result);
        printf("Faktisk kvadratrod (sqrt funktion): %.4f\n", sqrt(N));
        printf("Forskel: %.4e\n", fabs(result - sqrt(N)));
    }

    return 0;
}
*/


#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

double integrate_sin(int n) {
    double dx = PI / n;
    double sum = 0.0;
    
    for (int i = 0; i < n; i++) {
        double x = i * dx;
        sum += sin(x) * dx;
    }
    
    return sum;
}

int main() {
    int n_values[] = {10, 100, 1000, 10000, 100000};
    int n_count = sizeof(n_values) / sizeof(n_values[0]);
    
    printf("Numerisk integration af sin(x) fra 0 til pi:\n\n");
    printf("   N      Approksimeret værdi    Fejl\n");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < n_count; i++) {
        int n = n_values[i];
        double result = integrate_sin(n);
        double error = fabs(2.0 - result);  // Den teoretiske værdi er 2
        
        printf("%6d %20.12f %12.2e\n", n, result, error);
    }
    
    printf("\nTeoretisk værdi: 2.000000000000\n");
    
    return 0;
}