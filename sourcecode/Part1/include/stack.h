/**
 * Implements a stack of strings - lab05 example code
 */

typedef struct node {
    char *data;
    struct node *next;
} Node;

typedef struct stack {
    Node *top;
    int size;
} Stack;

// create a stack and return a pointer to the stack
Stack *create_stack();

// Push the string onto the stack
void push_stack(Stack *stack, char *str);

// pop off the string at the top of the stack and return it
char *pop_stack(Stack *stack);

// return the string at the top of the stack
int size_of_stack(Stack *stack);

// return if the stack is empty
int empty_stack(Stack *stack);
