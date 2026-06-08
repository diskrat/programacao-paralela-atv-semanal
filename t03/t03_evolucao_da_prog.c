#include <time.h>
#include <stdio.h>
#include <math.h>
#include <linux/time.h>

#if defined(M_PI)
#else
#define M_PI 3.14159265358979323846
#endif

#define RUNS 1

double calculo_pi_serie(int iteracoes){
    double soma = 0.0;
    for (int i = 0; i < iteracoes; i++){
        double sinal = (i % 2 == 0) ? 1.0 : -1.0;
        soma += sinal / (2.0 * i + 1.0);
    }
    return soma * 4.0;
}

void compare_time(int iteracoes){
    struct timespec start, end;
    double time_sum = 0.0;
    double last_result = 0.0;

    for (int r = 0; r < RUNS; r++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        last_result = calculo_pi_serie(iteracoes);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
        time_sum += elapsed;
    }
    double time_average = time_sum / RUNS;
    double diff = fabs(last_result - M_PI);
    printf("Iteracoes %d: pi ~= %.12f, pi = %.12f, diferenca absoluta = %.12e, tempo medio = %f ms (%d execucoes)\n",
           iteracoes, last_result, M_PI, diff, time_average, RUNS);
}

int main(){
    int start = 100000;
    for (int i = 0; i < 200; i++){
        int it = start * (i + 1);
        compare_time(it);
    }
    return 0;
}
