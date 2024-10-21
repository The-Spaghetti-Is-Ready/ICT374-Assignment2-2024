#include "include/stack.h"
#include <stdlib.h>
#include <stdio.h>

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

     newNode -> data = str;
     newNode->next = stack->top;
     stack->top = newNode;
     stack -> size += 1;
}

char *pop_stack(Stack *stack)
{
     Node *np;
     char *str;

     if (empty_stack(stack))
         return NULL;

     np = stack->top;
     stack->top = np->next;
     stack->size -= 1;

     str = np->data;
     free(np);
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