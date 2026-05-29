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
