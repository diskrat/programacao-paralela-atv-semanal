#include <time.h>
#include <stdio.h>
#include <linux/time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define RUNS 1

double cpu_intensive(long long iter){
	double result = 0.0;
	#pragma omp parallel for
	for (long long i = 1; i <= iter; ++i) {
		double x = (double)i * 0.000001;
		result += sin(x) * cos(x) + sqrt(x + 1.0) + log(x + 1.0) + exp(sin(x) * 0.000001);
	}
	return result;
}

double test_func(double (*func)(long long), long long iter, int runs) {
    struct timespec start, end;
    double time_sum = 0;
    for (int i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        double result = func(iter);
        clock_gettime(CLOCK_MONOTONIC, &end);
		printf("resultado: %.10f\n", result);
        double elapsed = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
        time_sum += elapsed;
    }
    return time_sum / runs;
}

int main(void) {
	const long long iteracoes = 64 * 1024 * 1024;
	double total_time = test_func(cpu_intensive,iteracoes,RUNS);
	printf("TEMPO:%.3f ms\n",total_time);
	return 0;
}

