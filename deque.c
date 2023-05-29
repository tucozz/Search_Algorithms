#include "deque.h"
#include <stdlib.h>

struct _Deque{
    data_type **map;
    void(*destroy_fn)(data_type*);
    int bloco_inicial;
    int bloco_final;
    int tamanho_bloco;
    int inicio;
    int fim;
};

Deque *deque_construct(void(*destroy_fn)(data_type*)){
    Deque *d = (Deque *)malloc(sizeof(Deque));

    d->destroy_fn = destroy_fn;
    d->tamanho_bloco = 5;
    d->map = calloc(d->tamanho_bloco, sizeof(Deque *));
    d->map[d->tamanho_bloco/2] = calloc(d->tamanho_bloco, sizeof(data_type));
    d->bloco_final = d->bloco_inicial = d->tamanho_bloco/2;
    d->inicio = d->fim = 0;
    

    return d;
}

void deque_destroy(Deque *d){
    for(int i = d->bloco_inicial; i < d->bloco_final; i++){
        int idx_init = 0;
        int idx_fim = d->tamanho_bloco - 1;

        if(i == d->bloco_inicial)
            idx_init = d->inicio;
        if(i == d->bloco_final)
            idx_fim = d->fim;

        for(int j = idx_init; j < idx_fim; i++){
            d->destroy_fn(d->map[i][j]);
        }
        free(d->map[i]);
    }
    free(d->map);
}

void deque_push_front(Deque *d){

}

void deque_push_back(Deque *d){

}

data_type deque_pop_back(Deque *d){

}

data_type deque_pop_front(Deque *d){

}

data_type deque_get(Deque *d, int idx){
    int POS_ITEM = d->inicio + idx;
    int BLOCO_IDX = (int)(POS_ITEM/d->tamanho_bloco);
    int ITEM_IDX = POS_ITEM%d->tamanho_bloco;

    return d->map[BLOCO_IDX][ITEM_IDX];
}

int deque_size(Deque *d){
    return (d->bloco_final - d->bloco_inicial)
    * d->tamanho_bloco + (d->fim - d->inicio);
}