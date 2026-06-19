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