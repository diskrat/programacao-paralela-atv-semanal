#include <time.h>
#include <stdio.h>

void multiply_line(int size,int X[],int A[][size],int Y[]){
    for(int i=0;i<size;i++){
        for(int j= 0;j<size;j++){
            Y[i]+=A[i][j]*X[j];
        }
    }
}
void multiply_column(int size,int X[],int A[][size],int Y[]){
    for(int j=0;j<size;j++){
        for(int i= 0;i<size;i++){
            Y[i]+=A[i][j]*X[j];
        }
    }
}
void create_matrix(int size){
    int X[size], Y[size];
    int A[size][size];
    int runs = 100;
    double time_taken[runs];
    struct timespec start, end;

    for(int i=0;i<size;i++){
        X[i]=1;
        for(int j=0;j<size;j++){
            A[i][j]=1;
        }
    }

    // Medir tempo para multiply_line
    double time_sum = 0;
    for(int i=0; i<runs; i++){
        for(int k=0;k<size;k++) Y[k]=0;
        clock_gettime(CLOCK_MONOTONIC, &start);
        multiply_line(size,X,A,Y);
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_taken[i] = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
    }
    for(int i=0;i<runs;i++) time_sum += time_taken[i];
    double time_average = time_sum / runs;
    printf("Matrix size %d levou %f milissegundos em média (%d execuções) usando acesso por linhas.\n", size, time_average, runs);

    // Medir tempo para multiply_column
    time_sum = 0;
    for(int i=0; i<runs; i++){
        for(int k=0;k<size;k++) Y[k]=0;
        clock_gettime(CLOCK_MONOTONIC, &start);
        multiply_column(size,X,A,Y);
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_taken[i] = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
    }
    for(int i=0;i<runs;i++) time_sum += time_taken[i];
    time_average = time_sum / runs;
    printf("Matrix size %d levou %f milissegundos em média (%d execuções) usando acesso por colunas.\n\n", size, time_average, runs);
}


int main(){
    int size =200;
    for (int i=0;i <7;i++){
        create_matrix(size);
        size+=200;
    }
    
    return 0;
}
