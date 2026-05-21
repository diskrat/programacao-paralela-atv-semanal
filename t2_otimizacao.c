#include <time.h>
#include <stdio.h>

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
void compare_time(int size){
    int A[size],runs = 10;
    double time_taken[runs];
    struct timespec start, end;
    
    for(int i=0;i<size;i++){
        A[i] = i * 2 + 1;
    }
    
    double time_sum = 0;
    for(int i=0; i< runs; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        soma_simples(A, size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_taken[i] = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
    }
    for(int j=0; j< runs; j++){
        time_sum += time_taken[j];
    }
    double time_average = time_sum / runs;
    printf("Vetor de tamanho %d levou %f milissegundos em média (%d execuções) usando soma simples.\n", size, time_average, runs);

    time_sum = 0;
    // Medir tempo para soma_alternada
    for(int i=0; i< runs; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        soma_alternada(A, size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_taken[i] = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
    }
    for(int j=0; j< runs; j++){
        time_sum += time_taken[j];
    }
    time_average = time_sum / runs;
    printf("Vetor de tamanho %d levou %f milissegundos em média (%d execuções) usando soma alternada.\n\n", size, time_average, runs);
}


int main(){
    int size =100;
    for (int i=0;i <7;i++){
        compare_time(size);
        size+=100;
    }    
    return 0;
}
