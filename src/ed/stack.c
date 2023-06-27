#include "stack.h"
#include "deque.h"
#include <stdlib.h>

struct Stack{
    Deque *deque; 
};

Stack *stack_construct(){
    Stack *s = malloc(sizeof(Stack));
    s->deque = deque_construct(NULL);
    return s;
}

void stack_push(Stack *stack, void *data){
    deque_push_front(stack->deque, data);
}

bool stack_empty(Stack *stack){
    return(deque_size(stack->deque) == 0);
}

void *stack_pop(Stack *stack){
    void *pop = deque_pop_front(stack->deque);
    return pop;
}

void stack_destroy(Stack *stack){
    deque_destroy(stack->deque);
    free(stack);
}

// TODO!