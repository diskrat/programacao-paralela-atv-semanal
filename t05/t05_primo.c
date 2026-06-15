#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>


#define N 1e7
#define RUNS 1
int serial_prime(int n) {
    int count = 0;
    for (int i = 2; i <= n; ++i) {
        int is_prime = 1;
        int limit = (int)sqrt(i);
        for (int j = 2; j <= limit; ++j) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        count += is_prime;
    }
    return count;
}

int parallel_prime(int n) {
    int count = 0;
    #pragma omp parallel for 
    for (int i = 2; i <= n; ++i) {
        int is_prime = 1;
        int limit = (int)sqrt(i);
        for (int j = 2; j <= limit; ++j) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        count += is_prime;
    }
    return count;
}

double test_func(int size, int (*func)(int), int *result, int runs) {
    struct timespec start, end;
    double time_sum = 0;
    for (int i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        *result = func(size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
        time_sum += elapsed;
    }
    return time_sum / runs;
}

int main(void) {
    int n = N;
    int result_serial = 0;
    int result_parallel = 0;

    double time_taken_serial = test_func(n,serial_prime,&result_serial,RUNS);
    printf("serial:\t\t %.0f ms;\tValor: %d\n",time_taken_serial,result_serial);
    double time_taken_parallel = test_func(n,parallel_prime,&result_parallel,RUNS);
    printf("paralela:\t %.0f ms;\tValor: %d\n",time_taken_parallel,result_parallel);
    
    
    return 0;
}
