#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const size_t N = 64 * 1024 * 1024;

    double *a = malloc(N * sizeof(double));
    double *b = malloc(N * sizeof(double));
    double *c = malloc(N * sizeof(double));
    if (!a || !b || !c) {
        fprintf(stderr, "falha na alocação de memória\n");
        return 1;
    }


    for (size_t i = 0; i < N; ++i) {
        a[i] = (double)i * 1.0;
        b[i] = (double)i * 2.0;
        c[i] = 0.0;
    }
#pragma omp parallel for
    for (size_t i = 0; i < N; ++i) {
        c[i] = a[i] + b[i];
    }

    double checksum = 0.0;

    for (size_t i = 0; i < N; ++i) {
        checksum += c[i];
    }
    printf("checksum: %f\n", checksum);
    free(a);
    free(b);
    free(c);
    return 0;
}
