# Tarefa 4: Programas memory-bound vs Programas compute-bound

Implemente dois programas paralelos em C com OpenMP: um limitado por memória, com somas simples em vetores, e outro limitado por CPU, com cálculos matemáticos intensivos. Paralelize com `#pragma omp parallel for` e meça o tempo de execução variando o número de threads. Analise quando o desempenho melhora, estabiliza ou piora, e reflita sobre como o multithreading de hardware pode ajudar em programas memory-bound, mas atrapalhar em programas compute-bound pela competição por recursos.

## Implementação

- `t04_1_multitarefas_mem.c`: soma element-wise de dois vetores grandes (`double`) e escreve em um terceiro vetor. A medição é feita por `test_func` que executa a rotina `RUNS` vezes e retorna o tempo médio (ms) usando `CLOCK_MONOTONIC`.

```c
void soma_simples(double* a, double* b, double* c, int size){
#pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        c[i] = a[i] + b[i];
    }
}
```

- `t04_2_multitarefas_cpu.c`: carga de trabalho intensiva em CPU, executando funções matemáticas (`sin`, `cos`, `sqrt`, `log`, `exp`) em loop grande. Uso de `#pragma omp parallel for`.

```c
#pragma omp parallel for
for (long long i = 1; i <= iteracoes; ++i) {
    double x = (double)i * 0.000001;
    resultado += sin(x) * cos(x) + sqrt(x + 1.0) + log(x + 1.0) + exp(sin(x) * 0.000001);
}
```

## Metodologia

Os testes foram feitos variando `OMP_NUM_THREADS` (1, 3, 6, 12, 24) para fixar o número de threads; cada execução imprime o tempo observado pelo programa.

## Resultados — Memory-bound

| Threads | Tempo (ms) |
|--------:|-----------:|
| 1       | 148.458    | 
| 3       | 75.402     | 
| 6       | 51.647     | 
| 12      | 55.159     | 
| 24      | 57.176     | 

Os resultados memory-bound mostraram o mesmo valor final pois a operação é determinística, mas o tempo de execução diminuiu significativamente de 1 para 3 e 6 threads, indicando que o programa se beneficia do paralelismo para esconder latências de memória.

## Resultados — Cpu-bound

| Threads | Tempo (ms) | Resultado |
|--------:|-----------:|------------|
| 1       | 16107.331  | 661965548.4630025625
| 3       | 8710.520   | 558949445.2603884935
| 6       | 4314.778   | 383760875.9162637591
| 12      | 2380.348   | 238139971.6851179600
| 24      | 2292.051   | 158852852.221796184

O programa cpu-bound mostrou uma redução significativa no tempo de execução à medida que o número de threads aumentou, mas o resultado final diminui com mais threads, e quando se chega ao número máximo de threads físicas o tempo de execução estabiliza, indicando que o programa se beneficia do paralelismo no tempo, mas os resultados se deterioram de acordo com o aumento do número de threads.



# Implementação Completa
## Memory-bound
```c
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

```
## Cpu-bound
```c
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

```

