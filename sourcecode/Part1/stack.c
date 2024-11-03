#include "include/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Stack *create_stack()
{
     Stack *s;

     s = malloc(sizeof(Stack));

     if (s==NULL) {
         fprintf(stderr, "No more memory - quit\n");
         exit(1);
     }

     s->top = NULL;
     s->size = 0;
     return s;
}

void push_stack(Stack *stack, char *str)
{
    Node *newNode;

    newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "No more memory - quit!\n");
        exit(1);
    }
    newNode->data = strdup(str); // Allocate memory and copy the string
    if(newNode->data == NULL) {
        fprintf(stderr, "No more memory - quit!\n");
        free(newNode);
        exit(1);
    }

    newNode->next = stack->top;
    stack->top = newNode;
    stack->size += 1;
}

char *pop_stack(Stack *stack)
{
    Node *np;
    char *str;

    if (empty_stack(stack))
        return NULL;

    np = stack->top;
    stack->top = np->next;
    str = np->data;
    free(np); // Free the node itself
    stack->size -= 1;
    return str;
}

int size_of_stack(Stack *stack)
{
    return stack->size;
}

int empty_stack(Stack *stack)
{
    return stack->size == 0;
}

// clear the stack 
void clear_stack(Stack *stack)
{
    while (!empty_stack(stack)) {
        char *str = pop_stack(stack);
        free(str); // Free the memory allocated for the string
    }
}

