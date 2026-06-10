#include <time.h>
#include <stdio.h>
#include <linux/time.h>
#include <stdlib.h>
#include <limits.h>
#define RUNS 20


void soma_simples(int* a, int* b, int* c, int size){
#pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        c[i] = a[i] + b[i];
    }
}

double test_func(int size, void (*func)(int*,int*,int*, int), int* A, int* B, int* C, int runs) {
    struct timespec start, end;
    double time_sum = 0;
    for (int i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        func(A,B,C, size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
        time_sum += elapsed;
    }
    return time_sum / runs;
}

int main(void) {
    const long N = 64 * 1024 * 1024;
    int *a = malloc(N * sizeof(int));
    int *b = malloc(N * sizeof(int));
    int *c = malloc(N * sizeof(int));
    if (!a || !b || !c) {
        fprintf(stderr, "falha na alocação de memória\n");
        return 1;
    }


    for (size_t i = 0; i < N; ++i) {
        a[i] = i * 1;
        b[i] = i * 2;
        c[i] = 0;
    }
    double total_time = test_func(N,soma_simples,a,b,c,RUNS);

     unsigned long total = 0;
     for (size_t i = 0; i < N; i++) {
         total += c[i];
     }
     printf("TEMPO:%.3f ms; resultado: %ld\n",total_time, total);
    
    free(a);
    free(b);
    free(c);
    return 0;
}
