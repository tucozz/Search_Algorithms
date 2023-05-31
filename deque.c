#include "deque.h"
#include <stdlib.h>

struct _Deque{
    data_type **map;
    void(*destroy_fn)(data_type);
    int bloco_inicial;
    int bloco_final;
    int tamanho_bloco;
    int inicio;
    int fim;
};

Deque *deque_construct(void(*destroy_fn)(data_type)){
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
    //seleciona um a um os blocos alocados
    for(int i = d->bloco_inicial; i < d->bloco_final; i++){
        int idx_init = 0;
        int idx_fim = d->tamanho_bloco - 1;

        //confere se o bloco ta cheio ou se deve começar / terminar de outro lugar
        if(i == d->bloco_inicial)
            idx_init = d->inicio;
        if(i == d->bloco_final)
            idx_fim = d->fim;

        //vai destruindo todos os itens dentro do bloco
        for(int j = idx_init; j < idx_fim; i++){
            d->destroy_fn(d->map[i][j]);
        }
        //ao acabar, libera o ponteiro para o bloco
        free(d->map[i]);
    }
    //e por fim libera o mapa
    free(d->map);
}

void deque_push_front(Deque *d, data_type data){
    // Verifica se o deque está vazio
    if (d->inicio == d->fim && d->map[d->bloco_inicial][d->inicio] == NULL) {
        d->map[d->bloco_inicial][d->inicio] = data;
        return;
    }
    
    // Verifica se é necessário alocar um novo bloco no início
    if (d->inicio == 0) {
        if (d->bloco_inicial == 0) {
            // É necessário realocar o mapa para acomodar mais blocos
            int novo_tamanho = d->tamanho_bloco * 2;
            data_type **novo_mapa = calloc(novo_tamanho, sizeof(data_type *));
            
            // Desloca os blocos existentes para a nova posição no mapa
            for (int i = d->bloco_inicial; i <= d->bloco_final; i++) {
                novo_mapa[i + d->tamanho_bloco] = d->map[i];
            }
            
            // Atualiza os ponteiros do deque
            d->bloco_inicial += d->tamanho_bloco;
            d->bloco_final += d->tamanho_bloco;
            d->tamanho_bloco = novo_tamanho;
            d->map = novo_mapa;
        }
        
        // Aloca um novo bloco no início
        d->bloco_inicial--;
        d->map[d->bloco_inicial] = calloc(d->tamanho_bloco, sizeof(data_type));
        d->inicio = d->tamanho_bloco - 1;

        // Insere o novo elemento no início do deque
        // sem decrementar o inicio
        d->map[d->bloco_inicial][d->inicio] = data;
    }
    else{
        // Insere o novo elemento no início do deque
        // e decrementa o inicio
        d->inicio--;
        d->map[d->bloco_inicial][d->inicio] = data;
    }
}

void deque_push_back(Deque *d, data_type data){
    // Verifica se o deque está vazio
    if (d->inicio == d->fim && d->map[d->bloco_final][d->fim] == NULL) {
        d->map[d->bloco_final][d->fim] = data;
        return;
    }
    
    // Verifica se é necessário alocar um novo bloco no final
    if (d->fim == d->tamanho_bloco - 1) {
        if (d->bloco_final == d->tamanho_bloco - 1) {
            // É necessário realocar o mapa para acomodar mais blocos
            int novo_tamanho = d->tamanho_bloco * 2;
            data_type **novo_mapa = calloc(novo_tamanho, sizeof(data_type *));
            
            // Copia os blocos existentes para a nova posição no mapa
            for (int i = d->bloco_inicial; i <= d->bloco_final; i++) {
                novo_mapa[i] = d->map[i];
            }
            
            // Atualiza os ponteiros do deque
            d->tamanho_bloco = novo_tamanho;
            d->map = novo_mapa;
        }
        
        // Aloca um novo bloco no final
        d->bloco_final++;
        d->map[d->bloco_final] = calloc(d->tamanho_bloco, sizeof(data_type));
        d->fim = 0;
    }
    else {
        // Incrementa o índice do fim
        d->fim++;
    }
    
    // Insere o novo elemento no final do deque
    d->map[d->bloco_final][d->fim] = data;
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

void deque_print(Deque *d, void(*print_fn)(data_type)){
    for(int i = d->bloco_inicial; i <= d->bloco_final; i++){
        int idx_init = 0;
        int idx_fim = d->tamanho_bloco - 1;

        //confere se o bloco ta cheio ou se deve começar / terminar de outro lugar
        if(i == d->bloco_inicial)
            idx_init = d->inicio;
        if(i == d->bloco_final)
            idx_fim = d->fim;

        //vai printando todos os itens dentro do bloco
        for(int j = idx_init; j <= idx_fim; j++){
            print_fn(d->map[i][j]);
        }
    }
}