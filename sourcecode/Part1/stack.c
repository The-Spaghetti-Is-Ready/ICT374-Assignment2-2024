#include "include/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Stack *create_stack()
{
     Stack *s = (Stack *)malloc(sizeof(Stack));
     if (s==NULL) {
         fprintf(stderr, "Stack malloc error\n");
         exit(EXIT_FAILURE);
     }

     s->top = NULL;
     s->bottom = NULL;
     s->size = 0;
     return s;
}

void push_stack(Stack *stack, char *str)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Node malloc error\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = strdup(str); // Allocate memory and copy the string
    if(newNode->data == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        free(newNode);
        exit(EXIT_FAILURE);
    }

    newNode->next = stack->top;
    newNode->prev = NULL;

    if (stack->top != NULL) {
        stack->top->prev = newNode;
    }
    stack->top = newNode;

    if (stack->bottom == NULL) {
        stack->bottom = newNode;
    }

    stack->size += 1;
}

char *pop_stack(Stack *stack)
{
    if(empty_stack(stack)) {
        return NULL;
    }

    Node *np = stack->top;
    char *str = np->data;

    stack->top = np->next;
    if (stack->top != NULL) {
        stack->top->prev = NULL;
    } else {
        stack->bottom = NULL;
    }
   
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

