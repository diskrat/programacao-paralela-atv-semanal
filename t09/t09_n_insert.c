#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

typedef struct node {
    int val;
    struct node *next;
} node;


void push (node** head,int val){
    node *new_node;
    new_node = (node*) malloc(sizeof(node));

    new_node->val = val;    
    new_node->next = *head;
    *head = new_node;
}


int main(void) {

    return 0;
}
