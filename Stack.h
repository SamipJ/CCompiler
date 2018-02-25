// SAMIP JASANI 2015A7PS0127P

#include "parser.h"

typedef struct stack
{
    int capacity;
    int index;
    Rhs *array;
} stack;

typedef stack *Stack;

Stack createStack(int capacity);
Rhs pop(Stack stackH);
void push(Stack stackH, Rhs item);
int isEmpty(Stack stackH);
int isFull(Stack stackH);
