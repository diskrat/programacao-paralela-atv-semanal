#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1e7

typedef int (*rand_func)(void*);


int rand_wrapper(void *seed){
    (void)seed;
    return rand();
}
int rand_r_wrapper(void *seed){
    return rand_r((unsigned int*)seed);
}

double count_accumulator(long shots,rand_func rand_f,unsigned int seed){
    long count_global = 0;
    long count = 0;
    #pragma omp parallel default(none) shared(shots,count_global) firstprivate(count)
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

double count_vector(long shots,rand_func rand_f,unsigned int seed){
    int max_threads = omp_get_max_threads;
    long *arr = calloc(max_threads,sizeof(long));
    long count_global = 0;
    #pragma omp parallel default(none) shared(shots,count_global,arr)
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

/*-------- TO DO --------*/
// Implementar time function

int main(void) {
    
    return 0;
}
