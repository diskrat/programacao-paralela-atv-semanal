#include <stdio.h>
#include <math.h>
#include <omp.h>

#define N 1e7

int main(void) {
    int n = N;
    int count = 0;

    for (int i = 2; i <= n; ++i) {
        int is_prime = 1;
        int limit = (int)sqrt(i);
        for (int j = 2; j <= limit; ++j) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        count += is_prime;
    }

    printf("%d\n", count);
    return 0;
}
