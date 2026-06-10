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
