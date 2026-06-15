#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1e7

int main(void) {
    long n = N;
    long count = 0;

    #pragma omp parallel for
    for (long i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) count++; 

    }

    double pi = 4.0 * (double)count / (double)n;
    printf("pi ~ %.10f (count=%ld, n=%ld)\n", pi, count, n);
    return 0;
}
