# Tarefa 6: Estimativa de PI

Implemente em C a estimativa estocástica de π. Paralelize com #pragma omp parallel for e explique o resultado incorreto. Corrija a condição de corrida utilizando o #pragma omp critical e reestruturando com #pragma omp parallel seguido de #pragma omp for e aplicando as cláusulas private, firstprivate, lastprivate e shared. Teste diferentes combinações e explique como cada cláusula afeta o comportamento do programa. Comente também como a cláusula default(none) pode ajudar a tornar o escopo mais claro em programas complexos.


# Estimativa com condição de corrida
O resultado incorreto ocorre devido à condição de corrida na variável `count`, que é compartilhada entre as threads. Quando múltiplas threads tentam atualizar `count` simultaneamente, elas podem sobrescrever o mesmo resultado, levando a um resultado final incorreto.

## Implementação
```c
long n = N;
long count = 0;

#pragma omp parallel for
for (long i = 0; i < n; i++) {
    double x = (double)rand() / RAND_MAX;
    double y = (double)rand() / RAND_MAX;
    if (x * x + y * y <= 1.0) count++; 
}
double pi = 4.0 * (double)count / (double)n;
```
# Correção com #pragma omp critical
### Shared
A cláusula `shared` indica que a variável `count` é compartilhada entre todas as threads, o que é necessário para que todas as threads possam atualizar o mesmo contador. Usando `#pragma omp critical`, garantimos que apenas uma thread possa atualizar `count`, evitando a condição de corrida.
### Private
A cláusula `private` cria uma cópia privada da variável para cada thread. A variável `count` seria privada, e inicializada com lixo, o que ao final do loop resultaria em um valor igual a zero pois o valor de `count` não seria atualizado ao final da paralelização.
### Firstprivate
A cláusula `firstprivate` também cria uma cópia privada da variável para cada thread, mas inicializa essa cópia com o valor da variável original. O resultado seria o mesmo que `private`, pois cada thread teria sua própria cópia de `count` e não haveria uma atualização global do contador.
### Lastprivate
A cláusula `lastprivate` cria uma cópia privada da variável para cada thread, mas ao final do loop, a thread a atualizar `count` no valor `n-1` do loop teria seu valor atribuído à variável original. No entanto, isso não resolveria a condição de corrida, pois as outras threads ainda estariam atualizando `count` simultaneamente, e o valor final seria incorreto.
### Default(none)
A cláusula `default(none)` força o programador a declarar explicitamente o escopo de cada variável, seja como `shared`, `private`, `firstprivate` ou `lastprivate`. Isso ajuda a tornar o código mais claro e evita erros de escopo, e atribuição acidental de variáveis, especialmente em programas complexos onde muitas variáveis estão envolvidas.
## Implementação 
```c
long n = N;
long count = 0;

#pragma omp parallel shared(count)
//#pragma omp parallel // quando nao for shared
{
    
    #pragma omp for 
    //#pragma omp for private(count)
    //#pragma omp for firstprivate(count)
    //#pragma omp for lastprivate(count)
    for (long i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            #pragma omp critical
            {
                count++;
            }
        }
    }
}
double pi = 4.0 * (double)count / (double)n;
```


# Implementação com race completa
```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main(void) {
    long n = N;
    long count = 0;

    #pragma omp parallel for
    for (long i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) count++; 

    }

    double pi = 4.0 * (double)count / (double)n;
    printf("pi ~ %.10f (count=%ld, n=%ld)\n", pi, count, n);
    return 0;
}

```

# Implementação corrigida com #pragma omp critical completa
```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main(void) {
    long n = N;
    long count = 0;

    #pragma omp parallel shared(count)
    //#pragma omp parallel // quando nao for shared
    {
        
        #pragma omp for 
        //#pragma omp for private(count)
        //#pragma omp for firstprivate(count)
        //#pragma omp for lastprivate(count)
        for (long i = 0; i < n; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                #pragma omp critical
                {
                    count++;
                }
            }
        }
    }
    double pi = 4.0 * (double)count / (double)n;
    printf("pi ~ %.10f (count=%ld, n=%ld)\n", pi, count, n);
    return 0;
}

```