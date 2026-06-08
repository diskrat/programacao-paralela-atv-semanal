#include <stdio.h>
#include <math.h>

int main(void) {
	const long long iteracoes = 64 * 1024 * 1024;
	volatile double resultado = 0.0;
#pragma omp parallel for 
	for (long long i = 1; i <= iteracoes; ++i) {
		double x = (double)i * 0.000001;
		resultado += sin(x) * cos(x) + sqrt(x + 1.0) + log(x + 1.0) +exp(sin(x) * 0.000001);
	}

	printf("resultado: %.10f\n", resultado);
	return 0;
}

