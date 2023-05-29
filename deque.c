#include "deque.h"
#include <stdlib.h>

struct _Deque{
    data_type **map;
    int bloco_inicial;
    int bloco_final;
    int tamanho_bloco;
    int inicio;
    int fim;
};

Deque *deque_construct(){
    Deque *d = (Deque *)malloc(sizeof(Deque));

    d->tamanho_bloco = 5;
    d->map = calloc(d->tamanho_bloco, sizeof(Deque *));
    d->map[d->tamanho_bloco/2] = calloc(d->tamanho_bloco, sizeof(data_type));
    d->bloco_final = d->bloco_inicial = d->tamanho_bloco/2;
    d->inicio = d->fim = 0;
    

    return d;
}

void deque_destroy(Deque *d){

}

void deque_push_front(Deque *d){

}

void deque_push_back(Deque *d){

}

data_type deque_pop_back(Deque *d){

}

data_type deque_pop_front(Deque *d){

}

int deque_size(Deque *d){

}