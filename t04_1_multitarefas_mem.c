#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
    const size_t N = 128 * 1024 * 1024; // 1 billion elements (~8 GiB per array)
    const int repetitions = 10;

    double *a = malloc(N * sizeof(double));
    double *b = malloc(N * sizeof(double));
    double *c = malloc(N * sizeof(double));
    if (!a || !b || !c) {
        fprintf(stderr, "falha na alocação de memória\n");
        return 1;
    }

    #pragma omp parallel for
    for (size_t i = 0; i < N; ++i) {
        a[i] = (double)i * 1.0;
        b[i] = (double)i * 2.0;
        c[i] = 0.0;
    }

    double start = omp_get_wtime();

    for (int rep = 0; rep < repetitions; ++rep) {
        #pragma omp parallel for
        for (size_t i = 0; i < N; ++i) {
            c[i] = a[i] + b[i];
        }
    }

    double elapsed = omp_get_wtime() - start;

    double checksum = 0.0;
    #pragma omp parallel for 
    for (size_t i = 0; i < N; ++i) {
        checksum += c[i];
    }

    printf("N=%zu repetitions=%d threads=%d elapsed=%.6f s checksum=%.0f\n",
           N, repetitions, omp_get_max_threads(), elapsed, checksum);

    free(a);
    free(b);
    free(c);
    return 0;
}
