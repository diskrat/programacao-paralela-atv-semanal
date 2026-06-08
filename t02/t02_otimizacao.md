# Tarefa 2: Otimização e comparação de somas

Implemente três laços em C para investigar os efeitos do paralelismo ao nível de instrução (ILP): 1. inicialize um vetor com um cálculo simples; 2. some seus elementos de forma acumulativa, criando dependência entre as iterações; e 3. quebre essa dependência utilizando múltiplas variáveis. Compare o tempo de execução das versões compiladas com diferentes níveis de otimização (O0, O2, O3) e analise como o estilo do código e as dependências influenciam o desempenho.

## Implementação
```c
void soma_simples(double A[], int size) {
    double soma = 0;
    for (int i = 0; i < size; i++) {
        soma += A[i];
    }
}

void soma_alternada(double A[], int size) {
    double soma_par = 0;
    double soma_impar = 0;
    for (int i = 0; i < size; i += 2) {
        soma_par += A[i];
        soma_impar += A[i + 1];
    }
    double soma = soma_par + soma_impar;
}

```

## Resultados

A tabela abaixo mostra os tempos médios extraídos das saídas dos binários compilados com `-O0`, `-O2` e `-O3`.

| Tamanho | O0 simples | O0 alternada | O2 simples | O2 alternada | O3 simples | O3 alternada |
|---------|------------|--------------|------------|--------------|------------|--------------|
| 200 | 0.000467 ms | 0.000273 ms | 0.000026 ms | 0.000022 ms | 0.000024 ms | 0.000024 ms |
| 400 | 0.001050 ms | 0.000486 ms | 0.000022 ms | 0.000022 ms | 0.000021 ms | 0.000022 ms |
| 600 | 0.001559 ms | 0.000723 ms | 0.000023 ms | 0.000022 ms | 0.000021 ms | 0.000021 ms |
| 800 | 0.002065 ms | 0.000947 ms | 0.000023 ms | 0.000022 ms | 0.000022 ms | 0.000023 ms |
| 1000 | 0.002569 ms | 0.001179 ms | 0.000024 ms | 0.000022 ms | 0.000023 ms | 0.000021 ms |
| 1200 | 0.003077 ms | 0.001404 ms | 0.000022 ms | 0.000022 ms | 0.000022 ms | 0.000022 ms |
| 1400 | 0.003578 ms | 0.001636 ms | 0.000023 ms | 0.000022 ms | 0.000023 ms | 0.000022 ms |

## Análise

A análise dos resultados revela que dependências entre iterações limitam o paralelismo a nível de instrução, pois forçam a execução sequencial das operações. Ao utilizar múltiplos acumuladores independentes, é possível expor mais ILP, permitindo que o processador execute múltiplas operações simultaneamente e aproveite melhor o pipeline. Isso se reflete no ganho de desempenho observado sem otimizações do compilador. Com otimizações ativadas, o compilador reduziu as operações a nops, mostrando que o codigo foi otimizado para um resultado constante.

# Codigo completo para teste
```c
#include <time.h>
#include <stdio.h>
#include <linux/time.h>

void soma_simples(int A[],int size){
    int soma = 0;
    for(int i = 0 ; i <size;i++){
        soma += A[i];
    }
}
void soma_alternada(int A[],int size){
    int soma_par = 0;
    int soma_impar = 0;
    
    for(int i = 0 ; i <size;i+=2){
        soma_par +=A[i];
        soma_impar+=A[i+1];
    }
    int soma = soma_par + soma_impar;
}

double test_func(int size, void (*func)(int[], int), int A[], int runs) {
    struct timespec start, end;
    double time_sum = 0;
    for (int i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        func(A, size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
        time_sum += elapsed;
    }
    return time_sum / runs;
}
void compare_time(int size){
    int A[size];
    int runs = 10;

    for(int i=0;i<size;i++){
        A[i] = i * 2 + 1;
    }

    double time_average = test_func(size, soma_simples, A, runs);
    printf("Vetor de tamanho %d levou %f milissegundos em média (%d execuções) usando soma simples.\n", size, time_average, runs);

    double time_average_alt = test_func(size, soma_alternada, A, runs);
    printf("Vetor de tamanho %d levou %f milissegundos em média (%d execuções) usando soma alternada.\n\n", size, time_average_alt, runs);
}


int main(){
    int step = 200;
    int size =step;
    for (int i=0;i <7;i++){
        compare_time(size);
        size+=step;
    }    
    return 0;
}

```

