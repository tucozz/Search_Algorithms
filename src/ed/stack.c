
#include "stack.h"
#include "deque.h"

struct Stack{
    Deque *deque; 
};

Stack *stack_construct(void(*destroy_fn)(data_type)){
    Stack *s = malloc(sizeof(Stack));
    s->deque = deque_construct(destroy_fn);
    return s;
}

void stack_push(Stack *stack, void *data){
    deque_push_back(stack->deque, data);
}

bool stack_empty(Stack *stack){
    return(deque_size(stack->deque) == 0);
}

void *stack_pop(Stack *stack){
    void *pop = deque_pop_back(stack->deque);
    return pop;
}

void stack_destroy(Stack *stack){
    deque_destroy(stack->deque);
    free(stack);
}

// TODO!