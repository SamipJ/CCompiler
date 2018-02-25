// SAMIP JASANI 2015A7PS0127P

#include "_Stack.h"

// C program for array implementation of stack
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// function to create a stack of given capacity. It initializes size of
// stack as 0
Stack createStack(int capacity)
{
    Stack stackH = (Stack)calloc(1, sizeof(stack));
    stackH->capacity = capacity;
    stackH->index = 0;
    stackH->array = (Rhs *)malloc(capacity * sizeof(rhside));
    return stackH;
}

// Stack is full when top is equal to the last index
int isFull(Stack stackH)
{
    return stackH->index == stackH->capacity - 1;
}

int isEmpty(Stack stackH)
{
    return stackH->index == 0;
}

// Function to add an item to stack.  It increases top by 1
void push(Stack stackH, Rhs item)
{
    if (isFull(stackH))
    {
        stackH->capacity *= 2;
        stackH->array = realloc(stackH->array, stackH->capacity * sizeof(rhside));
    }
    stackH->array[++stackH->index] = item;
}

// Function to remove an item from stack.  It decreases top by 1
Rhs pop(Stack stackH)
{
    if (isEmpty(stackH))
        return NULL;
    return stackH->array[stackH->index--];
}
// Driver program to test above functions
// int main()
// {
//     struct Stack *stack = createStack(100);

//     push(stack, 10);
//     push(stack, 20);
//     push(stack, 30);

//     printf("%d popped from stack\n", pop(stack));

//     return 0;
// }