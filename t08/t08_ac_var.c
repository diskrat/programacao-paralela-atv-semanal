#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 1e7
#define RUNS 1
typedef int (*rand_func)(unsigned int*);


int rand_wrapper(unsigned int *seed){
    (void)seed;
    return rand();
}
int rand_r_wrapper(unsigned int *seed){
    return rand_r(seed);
}

double count_accumulator(long shots,rand_func rand_f,unsigned int *seed){
    long count_global = 0;
    long count = 0;
    #pragma omp parallel default(none) shared(shots,count_global,rand_f) firstprivate(count,seed)
    {
            
        #pragma omp for 
        for (long i = 0; i < shots; i++) {
            double x = (double)rand_f(seed) / RAND_MAX;
            double y = (double)rand_f(seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                {
                    count++;
                }
            }
        }
        #pragma omp critical
        count_global+= count;
    }
    return 4.0 * (double)count_global / (double)shots;
}

double count_vector(long shots,rand_func rand_f,unsigned int *seed){
    int max_threads = omp_get_max_threads();
    long *arr = calloc(max_threads,sizeof(long));
    long count_global = 0;
    #pragma omp parallel default(none) shared(shots,count_global,arr,rand_f) firstprivate(seed) 
    {
            
        #pragma omp for 
        for (long i = 0; i < shots; i++) {
            double x = (double)rand_f(seed) / RAND_MAX;
            double y = (double)rand_f(seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                {
                    arr[omp_get_thread_num()]++;
                }
            }
        }
    }
    for (int i = 0; i < max_threads;i++) count_global+=arr[i];
    free(arr);
    return 4.0 * (double)count_global / (double)shots;
}

double elapsed_ms(struct timespec start, struct timespec end){
    return (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
}

double compare_time(long shots, double (*func)(long, rand_func, unsigned int*), rand_func rand_f, unsigned int *seed, double *pi_estimate){
    struct timespec start, end;
    double time_sum = 0.0;

    for (int r = 0; r < RUNS; r++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        *pi_estimate = func(shots, rand_f, seed);
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_sum += elapsed_ms(start, end);
    }

    return time_sum / RUNS;
}
int main(void) {
    unsigned int seed = 42;
    double pi_estimate = 0.0;

    double time_sum = compare_time(N, count_accumulator, rand_wrapper, &seed, &pi_estimate);
    printf("[acumulador + rand()]   pi ~= %.12f, tempo medio = %f ms (%d execucoes)\n",
            pi_estimate, time_sum, RUNS);

    time_sum = compare_time(N, count_vector, rand_wrapper, &seed, &pi_estimate);
    printf("[vetor + rand()]        pi ~= %.12f, tempo medio = %f ms (%d execucoes)\n",
            pi_estimate, time_sum, RUNS);

    time_sum = compare_time(N, count_accumulator, rand_r_wrapper, &seed, &pi_estimate);
    printf("[acumulador + rand_r()] pi ~= %.12f, tempo medio = %f ms (%d execucoes)\n",
            pi_estimate, time_sum, RUNS);

    time_sum = compare_time(N, count_vector, rand_r_wrapper, &seed, &pi_estimate);
    printf("[vetor + rand_r()]      pi ~= %.12f, tempo medio = %f ms (%d execucoes)\n",
            pi_estimate, time_sum, RUNS);
    
    return 0;
}
