# Tarefa 3: Aproximação de Pi e Desempenho Computacional

Implemente um programa em C que calcule uma aproximação de $\pi$ usando uma série matemática, variando o número de iterações e medindo o tempo de execução. Compare os valores obtidos com o valor real de $\pi$ e analise como a acurácia melhora com mais processamento. Reflita sobre como esse comportamento se repete em aplicações reais que demandam resultados cada vez mais precisos, como simulações físicas e inteligência artificial.

## Implementação
```c
double calculo_pi_serie(int iteracoes){
    double soma = 0.0;
    for (int i = 0; i < iteracoes; i++){
        double sinal = (i % 2 == 0) ? 1.0 : -1.0;
        soma += sinal / (2.0 * i + 1.0);
    }
    return soma * 4.0;
}
```
## Resultados experimentais
A tabela abaixo apresenta uma amostragem da evolução do cálculo conforme o aumento escalonado de iterações, extraída dos logs de execução do programa.

| Iterações   | π Calculado     | Diferença Absoluta | Tempo  (ms)      |
|------------:|:---------------:|:------------------:|:----------------:|
| 100.000     | 3.141582653590  | 1.000000007334e-05 | 0.263893         |
| 200.000     | 3.141587653590  | 5.000000031341e-06 | 0.624950         |
| 400.000     | 3.141590153590  | 2.500000049199e-06 | 1.084034         |
| 800.000     | 3.141591403590  | 1.250000075892e-06 | 2.196243         |
| 1.600.000   | 3.141592028590  | 6.249999486840e-07 | 4.288568         |
| 3.200.000   | 3.141592341090  | 3.125001337700e-07 | 8.540586         |
| ~1e+13      | 3.1415926535897 |       ~1e-13       | nao computado    |
# Pi real : 3.1415926535897 (13 casas decimais)

## Análise dos resultados
Os resultados demonstram uma relação de proporcionalidade linear entre o volume de processamento e o tempo exigido, contrastando fortemente com os retornos marginais na acurácia matemática observada.O tempo de execução dobra quase perfeitamente ao pular de 10.000.000 para 20.000.000 de iterações, saltando de aproximadamente 26.7 ms para 53.3 ms.Contudo, a diferença absoluta em relação ao real valor de $\pi$ não decai na mesma velocidade exponencial dos estágios iniciais.Nos primeiros saltos, o erro cai abruptamente da ordem de $10^{-5}$ para $10^{-6}$ e $10^{-7}$. Nos milhões de iterações finais adicionados, a máquina despende metade de todo o tempo registrado unicamente para polir os resquícios do cálculo, empurrando o erro de forma mais demorada para a casa dos $10^{-8}$.

## Reflexão: Custo Computacional vs. Precisão

O comportamento assintótico no cálculo da série evidencia um princípio fundamental da ciência da computação: a lei dos rendimentos decrescentes. A busca por acurácia máxima é invariavelmente limitada pelo custo do processamento.

* **Modelagem e Simulações:** O aumento da resolução de malhas iterativas melhora a fidelidade do modelo, mas exige um crescimento exponencial do tempo de execução. Na prática, estabelecem-se margens de tolerância para garantir a viabilidade do projeto.
* **Inteligência Artificial:** O treinamento de modelos de aprendizado de máquina apresenta o mesmo padrão. Após a convergência inicial, épocas adicionais de treinamento consomem vastos recursos de hardware para entregar apenas ganhos fracionários de acurácia.

**Conclusão:** Na computação aplicada, a exatidão absoluta é quase sempre inviável. O desenvolvimento de sistemas de alto desempenho exige o balanceamento contínuo entre o rigor matemático e a otimização dos recursos de hardware.

# Codigo completo para teste


```c
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <linux/time.h>

#if defined(M_PI)
#else
#define M_PI 3.14159265358979323846
#endif

#define RUNS 1

double calculo_pi_serie(int iteracoes){
    double soma = 0.0;
    for (int i = 0; i < iteracoes; i++){
        double sinal = (i % 2 == 0) ? 1.0 : -1.0;
        soma += sinal / (2.0 * i + 1.0);
    }
    return soma * 4.0;
}

void compare_time(int iteracoes){
    struct timespec start, end;
    double time_sum = 0.0;
    double last_result = 0.0;

    for (int r = 0; r < RUNS; r++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        last_result = calculo_pi_serie(iteracoes);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
        time_sum += elapsed;
    }
    double time_average = time_sum / RUNS;
    double diff = fabs(last_result - M_PI);
    printf("Iteracoes %d: pi ~= %.12f, pi = %.12f, diferenca absoluta = %.12e, tempo medio = %f ms (%d execucoes)\n",
           iteracoes, last_result, M_PI, diff, time_average, RUNS);
}

int main(){
    int start = 100000;
    for (int i = 0; i < 200; i++){
        int it = start * (i + 1);
        compare_time(it);
    }
    return 0;
}

```