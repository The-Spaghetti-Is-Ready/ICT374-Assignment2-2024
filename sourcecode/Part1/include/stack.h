/**
 * Implements a stack of strings using a double linked list
 */

typedef struct node {
    char *data;
    struct node *next;
    struct node *prev;
} Node;

typedef struct stack {
    Node *top;
    Node *bottom;
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

// clear the stack
void clear_stack(Stack *stack);