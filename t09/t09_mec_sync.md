# Tarefa 9: Mecanismos de sincronização 

Escreva um programa que cria tarefas para realizar N inserções em duas listas encadeadas, cada uma associada a uma thread. Cada tarefa deve escolher aleatoriamente em qual lista inserir um número. Garanta a integridade das listas evitando condição de corrida e, sempre que possível use regiôes críticas nomeadas para que a inserção em uma lista não bloqueie a outra. Em seguida, generalize o programa para um número de listas definido pelo usuário. explique por que, nesse caso, regiões críticas nomeadas não são suficientes e por que o uso de locks explícitos se torna necessário.


# Regiões críticas nomeadas as vezes não são o suficiente
O uso de regiões críticas nomeadas funciona quando o número de recursos é conhecido e não definido dinamicamente. A nomeação de uma região crítica é apenas um nome dado à região crítica e não uma associação direta ao recurso. Não é possível usar arrays de regiões críticas nomeadas. O uso de locks explícitos se torna necessário, pois é possível criar um array de locks, inicializar cada lock e associá-los a cada recurso dinamicamente. E ao final destruir os locks.

```c
omp_lock_t lock
omp_init_lock(&lock)
omp_set_lock(&lock)
omp_unset_lock(&lock)
omp_destroy_lock(&lock)

```

# Implementação 2 Listas
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

typedef struct node {
    int val;
    struct node *next;
} node;

void push(node** head, int val) {
    node *new_node;
    new_node = (node*) malloc(sizeof(node));

    new_node->val = val;    
    new_node->next = *head;
    *head = new_node;
}

void print_list(node* head, const char* name) {
    printf("%s: ", name);
    while (head != NULL) {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

void cleanup(node* head) {
    node* current = head;
    while (current != NULL) {
        node* temp = current;
        current = current->next;
        free(temp); 
    }
}

int main(void) {
    int N = 100; 
    node *lista1 = NULL;
    node *lista2 = NULL;
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i++) {           
                #pragma omp task firstprivate(i)
                {
                    unsigned int seed = omp_get_thread_num() + i + time(NULL);   
                    int escolha = rand_r(&seed) % 2;
                    if (escolha == 0) {
                        #pragma omp critical (lock_lista1)
                        {
                            push(&lista1, i);
                        }
                    } else {
                        
                        #pragma omp critical (lock_lista2)
                        {
                            push(&lista2, i);
                        }
                    }
                } 
            }
        } 
    } 
    print_list(lista1, "Lista 1");
    print_list(lista2, "Lista 2");
    cleanup(lista1);
    cleanup(lista2);
    return 0;
}
```

# Implementação N Listas
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NUM_LISTAS 3
#define N 10000

typedef struct node {
    int val;
    struct node *next;
} node;

void push(node** head, int val) {
    node *new_node;
    new_node = (node*) malloc(sizeof(node));

    new_node->val = val;    
    new_node->next = *head;
    *head = new_node;
}

void print_list(node* head) {
    printf("Lista: ");
    while (head != NULL) {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

void free_list(node* head) {
    node* current = head;
    while (current != NULL) {
        node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    node* listas[NUM_LISTAS] = {NULL};

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i++) {
                #pragma omp task firstprivate(i)
                {
                    unsigned int seed = omp_get_thread_num() + i + time(NULL);
                    int escolha = rand_r(&seed) % NUM_LISTAS;

                    #pragma omp critical (lock_lista[escolha])
                    {
                        push(&listas[escolha], i);
                    }
                }
            }
        }
    }
    for (int i = 0; i < NUM_LISTAS; i++) {
        print_list(listas[i]);
        free_list(listas[i]);
    }
    return 0;
}
```