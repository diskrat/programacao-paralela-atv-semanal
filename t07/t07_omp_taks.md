# Tarefa 7: omp tasks

Implemente um programa em C que cria uma lista encadeada com nós, cada um, contendo o nome de um arquivo fictício. Dentro de uma região paralela, percorra a lista e crie uma tarefa com `#pragma omp task` para processar cada nó. Cada tarefa deve imprimir o nome do arquivo e o identificador da thread que a executou. Após executar o programa, reflita: todos os nós foram processados? Algum foi processado mais de uma vez ou ignorado? O comportamento muda entre execuções? Como garantir que cada nó seja processado uma única vez e por apenas uma tarefa?
# Reflexão: 
Usando `#pragma omp task` sem `pragma omp single` faz cada thread criar tarefas para processar os nós, repetindo o trabalho multiplas vezes. Não foi observado a não execução de algum nó, com ou sem o uso da diretiva `single`. Entre execuções a ordem de execução e a thread encarregada de processar cada nó varia, para garantir que cada no seja executado apenas uma vez, é necessário usar a diretiva `single` para garantir que apenas uma thread crie as tarefas, evitando a criação de tarefas duplicadas.

# Implementação:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

typedef struct node {
    char *name;
    struct node *next;
} node;

static node *make_list(int n) {
    node *head = NULL;
    for (int i = n; i >= 1; --i) {
        node *p = malloc(sizeof *p);
        p->name = malloc(32);
        sprintf(p->name, "file%d.txt", i);
        p->next = head;
        head = p;
    }
    return head;
}

int main(void) {
    node *head = make_list(12);

    #pragma omp parallel
    {
        #pragma omp single
        {
            
            for (node *p = head; p != NULL; p = p->next) {
                #pragma omp task     
                {
                    printf("file=%s thread=%d\n", p->name, omp_get_thread_num());
                }
            }
            
        }
        #pragma omp taskwait
    }

    return 0;
}

```