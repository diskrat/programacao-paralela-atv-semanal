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
