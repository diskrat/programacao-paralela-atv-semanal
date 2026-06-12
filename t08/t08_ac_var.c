#include <stdio.h>
#include <stdlib.h>

#define N 10000000



int main(void) {
    long n = N;
    long count_global = 0;
    long count = 0;
    #pragma omp parallel default(none) shared(n,count_global) private(count)
    {
        count = 0;
            
        #pragma omp for 
        for (long i = 0; i < n; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                {
                    count++;
                }
            }
        }
        #pragma omp critical
        count_global+= count;
    }
    double pi = 4.0 * (double)count_global / (double)n;
    printf("pi ~ %.10f (count=%ld, n=%ld)\n", pi, count_global, n);
    return 0;
}
