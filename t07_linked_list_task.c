#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <linux/time.h>
#include <omp.h>

static double timespec_diff_sec(const struct timespec *a, const struct timespec *b) {
    return (b->tv_sec - a->tv_sec) + (b->tv_nsec - a->tv_nsec) * 1e-9;
}

int main(int argc, char **argv) {
    long long iters = (argc > 1) ? atoll(argv[1]) : 5000000LL;
    printf("iterations=%lld, omp max threads=%d\n", iters, omp_get_max_threads());

    struct timespec t_start, t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    double omp_start = omp_get_wtime();

    long long total = 0;

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();
        long long local = 0;
        for (long long i = tid; i < iters; i += nth) local += i;
        #pragma omp atomic
        total += local;

        #pragma omp critical
        {
            printf("thread %d/%d partial=%lld\n", tid, nth, local);
        }
    }

    double omp_end = omp_get_wtime();
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    printf("total = %lld\n", total);
    printf("clock_gettime elapsed = %.6f s\n", timespec_diff_sec(&t_start, &t_end));
    printf("omp_get_wtime elapsed   = %.6f s\n", omp_end - omp_start);

    return 0;
}