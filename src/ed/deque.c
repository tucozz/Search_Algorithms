#include "deque.h"
#include <stdlib.h>
#include "forward_list.h"

struct Deque{
    ForwardList *list;
    void(*destroy_fn)(data_type);
};

// criacao do deque
Deque *deque_construct(void(*destroy_fn)(data_type)){
    Deque *deque = malloc(sizeof(Deque));
    deque->destroy_fn = destroy_fn;
    deque->list = forward_list_construct();

    return deque;
}

// funcoes para insercao na direita e esquerda (devem ser feitas em O(1), com eventual realloc)
void deque_push_back(Deque *d, data_type val){
    forward_list_push_back(d->list, val);
}

void deque_push_front(Deque *d, data_type val){
    forward_list_push_front(d->list, val);
}

// funcoes para remocao na direita e esquerda (devem ser feitas em O(1))
data_type deque_pop_back(Deque *d){
    return forward_list_pop_back(d->list);
}

data_type deque_pop_front(Deque *d){
    return forward_list_pop_front(d->list);
}

// numero de elementos
int deque_size(Deque *d){
    return forward_list_size(d->list);
}

// obtem o i-esimo elemento
data_type deque_get(Deque *d, int idx){
    return forward_list_get(d->list, idx);
}

// libera o espaco alocado para o deque
void deque_destroy(Deque *d){
    forward_list_destroy(d->list);
    free(d);
}