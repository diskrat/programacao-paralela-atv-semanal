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