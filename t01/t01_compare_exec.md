# Tarefa 1: localidade temporal e espacial de cache

Implemente duas versões da multiplicação de matriz por vetor (MxV) em C: uma com acesso à matriz por linhas (laço interno variando a coluna) e outra por colunas (laço interno variando a linha). Meça o tempo de execução de cada versão usando uma função apropriada e execute testes com matrizes de diferentes tamanhos. Identifique a partir de que tamanho os tempos passam a divergir significativamente e explique por que isso ocorre, relacionando suas observações ao uso da memória cache e ao padrão de acesso à memória. 

## Implementação
```c
void multiply_line(int size, int X[], int** A, int Y[]) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Y[i] += A[i][j] * X[j];
        }
    }
}

void multiply_column(int size, int X[], int** A, int Y[]) {
    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            Y[i] += A[i][j] * X[j];
        }
    }
}
```

## Resultados experimentais

| Tamanho | Linha (ms) | Coluna (ms) | Razão (col/linha) |
|---------|------------|-------------|-------------------|
| 10      | 0.000356   | 0.000389    | 1.1               |
| 20      | 0.001294   | 0.001419    | 1.1               |
| 30      | 0.003227   | 0.003152    | 1.0               |
| 40      | 0.005052   | 0.005540    | 1.1               |
| 50      | 0.008147   | 0.008964    | 1.1               |
| 60      | 0.011306   | 0.013274    | 1.2               |
| 70      | 0.015361   | 0.016924    | 1.1               |
| 80      | 0.020059   | 0.022099    | 1.1               |
| 90      | 0.026040   | 0.031414    | 1.2               |
| 100     | 0.033097   | 0.036101    | 1.1               |
| 200     | 0.142067   | 0.144908    | 1.0               |
| 400     | 0.522071   | 0.651815    | 1.2               |
| 600     | 1.225480   | 1.507834    | 1.2               |
| 800     | 2.157612   | 3.130938    | 1.5               |
| 1000    | 3.333062   | 5.238328    | 1.6               |
| 1500    | 7.516113   | 11.189799   | 1.5               |
| 2000    | 13.965362  | 27.552186   | 2.0               |
| 2500    | 20.556311  | 53.493252   | 2.6               |
| 3000    | 29.238816  | 79.137947   | 2.7               |
| 3500    | 39.573331  | 114.397314  | 2.9               |
| 4000    | 51.607173  | 153.710950  | 3.0               |

## Análise dos resultados

Para matrizes pequenas (até 100), o tempo de execução das duas versões é praticamente igual, pois ambas cabem inteiramente na cache L1d. Entre 100 e 400, a diferença ainda é pequena, pois a matriz cabe na L2. A partir de 800, a diferença começa a crescer, pois a matriz já não cabe mais nas caches menores e o acesso por linhas passa a ser muito mais eficiente.

A divergência significativa ocorre a partir de 800x800, onde a razão já chega a 1.5, e se acentua conforme o tamanho cresce, chegando a 3x para 4000x4000. Isso está de acordo com os limites calculados para as caches do processador.

## Explicação: cache e padrão de acesso

O motivo dessa diferença está relacionado ao funcionamento da memória cache dos processadores. Em C, matrizes são armazenadas em memória de forma linear por linhas (row-major). Quando acessamos a matriz por linhas, os elementos acessados estão próximos na memória, aproveitando melhor a localidade espacial do cache. Isso faz com que mais dados necessários já estejam carregados na cache, acelerando o processamento.

Já no acesso por colunas, os acessos saltam entre posições distantes na memória, causando mais cache misses e obrigando o processador a buscar dados na RAM com mais frequência, o que é muito mais lento. Por isso, para matrizes grandes, o acesso por linhas é muito mais eficiente.

## Detalhes do processador e caches

**Caches relevantes para uma thread:**
- L1d (dados): 32 KiB por núcleo
- L2: 512 KiB por núcleo
- L3: 4 MiB (compartilhado)

**Limites aproximados para matrizes quadradas de int (4 bytes):**
- L1d: até ~89x89
- L2: até ~253x253
- L3: até ~1024x1024

**Contextualização:**
Os resultados mostram que, enquanto a matriz cabe nas caches menores, o padrão de acesso tem pouco impacto. Quando o tamanho ultrapassa o limite da L2 e principalmente da L3, o acesso por linhas se mantém eficiente, enquanto o acesso por colunas sofre forte penalização devido ao aumento de cache misses e acesso à RAM. Isso explica o crescimento da razão entre os tempos conforme o tamanho da matriz aumenta.

### Cálculo do limite de cache para uma thread

Como apenas uma thread está sendo usada, o limite relevante para a matriz é o cache L1d e L2 de um núcleo:
- L1d: 32 KiB por núcleo
- L2: 512 KiB por núcleo (3 MiB / 6 núcleos)

Uma matriz quadrada de inteiros (4 bytes) ocupa:
- Para N x N: 4 * N * N bytes

Limite para caber na L1d:
- 4 * N^2 <= 32 * 1024 → N ~ 89

Limite para caber na L2:
- 4 * N^2 <= 512 * 1024 → N ~ 253

Limite para caber na L3 (4 MiB compartilhado):
- 4 * N^2 <= 4 * 1024 * 1024 → N ~ 1024

Ou seja, matrizes até ~89x89 cabem na L1d, até ~253x253 cabem na L2 de um núcleo, e até ~1024x1024 cabem na L3. Acima disso, o acesso à RAM domina o tempo de execução, tornando o padrão de acesso ainda mais crítico para o desempenho.

**Resumo:**
- O salto de desempenho ocorre quando a matriz deixa de caber nas caches L1/L2/L3, tornando o padrão de acesso fundamental para o desempenho.

# Codigo completo para teste

```c
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RUNS 20

void multiply_line(int size, int X[], int** A, int Y[]) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Y[i] += A[i][j] * X[j];
        }
    }
}
void multiply_column(int size, int X[], int** A, int Y[]) {
    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            Y[i] += A[i][j] * X[j];
        }
    }
}

double test_func(int size, void (*func)(int, int[], int**, int[]), int X[],
int** A, int Y[], int runs) {
    struct timespec start, end;
    double time_sum = 0;
    for (int i = 0; i < runs; i++) {
        for (int k = 0; k < size; k++) Y[k] = 0;
        clock_gettime(CLOCK_MONOTONIC, &start);
        func(size, X, A, Y);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
        time_sum += elapsed;
    }
    return time_sum / runs;
}

void create_matrix(int size) {
    int* X = malloc(size * sizeof(int));
    int* Y = calloc(size, size * sizeof(int));
    int** A = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        A[i] = malloc(size * sizeof(int));
    }
    for (int i = 0; i < size; i++) {
        X[i] = 1;
        for (int j = 0; j < size; j++) {
            A[i][j] = 1;
        }
    }
    double time_average_linha = test_func(size, multiply_line, X, A, Y, RUNS);
    printf("Matriz %d levou %f ms em média (%d exec) usando acesso por linhas.\n", size, time_average_linha, RUNS);
    double time_average_coluna = test_func(size, multiply_column, X, A, Y, RUNS);
    printf("Matriz %d levou %f ms em média (%d exec) usando acesso por colunas.\n", size, time_average_coluna, RUNS);

    printf("x%.1f\n\n",time_average_coluna/time_average_linha);

    free(X);
    free(Y);
    for (int i = 0; i < size; i++) {
        free(A[i]);
    }
    free(A);
}

int main() {
    int size = 10;
    for (int i = 0; i < 10; i++)
    {
        create_matrix(size);
        size+=10;
    }
    size = 200;
    for (int i = 0; i < 5; i++) {
        create_matrix(size);
        size += 200;
    }
    size = 1500;
    for (int i = 0; i < 6; i++) {
        create_matrix(size);
        size += 500;
    }
    return 0;
}

```