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
    for(int i=0;i<size;i++){
        for(int j= 0;j<size;j++){
            Y[i]+=A[j][i]*X[j];
        }
    }
}
void create_matrix(int size){
    int X[size],Y[size];
    int A[size][size];
    double time_taken;
    struct timespec start, end;
    
    for(int i=0;i<size;i++){
        X[i]=1;
        for(int j=0;j<size;j++){
            A[i][j]=1;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &start);
    multiply_line(size,X,A,Y);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_taken = ((end.tv_nsec - start.tv_nsec) + (end.tv_nsec - start.tv_nsec))/1e6;
    printf("Matrix size %d took %f miliseconds to process using lines.\n", size, time_taken);

    clock_gettime(CLOCK_MONOTONIC, &start);
    multiply_column(size,X,A,Y);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_taken = ((end.tv_nsec - start.tv_nsec) + (end.tv_nsec - start.tv_nsec))/1e6;
    printf("Matrix size %d took %f miliseconds to process. using column.\n\n\n", size, time_taken);
}


int main(){
    
    

    
    int size =200;
    for (int i=0;i <7;i++){
        create_matrix(size);
        size+=200;
    }
    create_matrix(1450);
    
    
    

    

    

    return 0;
}
