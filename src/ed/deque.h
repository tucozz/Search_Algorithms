
#ifndef _DEQUE_H_
#define _DEQUE_H_

typedef struct Deque Deque;
typedef void* data_type;
typedef void(*Destroy_fn)(void *k1);

// criacao do deque
Deque *deque_construct(Destroy_fn destroy_fn);

// funcoes para insercao na direita e esquerda (devem ser feitas em O(1), com eventual realloc)
void deque_push_back(Deque *d, data_type val);
void deque_push_front(Deque *d, data_type val);

// funcoes para remocao na direita e esquerda (devem ser feitas em O(1))
data_type deque_pop_back(Deque *d);
data_type deque_pop_front(Deque *d);

// numero de elementos
int deque_size(Deque *d);

// obtem o i-esimo elemento
data_type deque_get(Deque *d, int idx);

// libera o espaco alocado para o deque
void deque_destroy(Deque *d);

#endif