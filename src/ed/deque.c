#include "deque.h"
#include <stdlib.h>
#include <stdio.h>

struct Deque{
    data_type **map;
    Destroy_fn destroy_fn;
    int bloco_inicial;
    int bloco_final;
    int tamanho_bloco;
    int tamanho_mapa;
    int inicio;
    int fim;
};

Deque *deque_construct(Destroy_fn destroy_fn){
    Deque *d = (Deque *)malloc(sizeof(Deque));

    d->destroy_fn = destroy_fn;
    d->tamanho_bloco = 5;
    d->tamanho_mapa = 5;
    d->map = calloc(d->tamanho_mapa, sizeof(data_type *));
    //d->map[d->tamanho_mapa/2] = calloc(d->tamanho_bloco, sizeof(data_type));
    d->bloco_final = d->bloco_inicial = d->tamanho_mapa/2;
    d->inicio = d->fim = 0;
    

    return d;
}

static void _map_destroy(Deque *d){
    if(deque_size(d)){
        //seleciona um a um os blocos alocados
        for(int i = d->bloco_inicial; i <= d->bloco_final; i++){
            if(i == d->bloco_final && d->fim == 0 && d->bloco_final != d->bloco_inicial)
                break;
            int idx_init = 0;
            int idx_fim = d->tamanho_bloco - 1;

            //confere se o bloco ta cheio ou se deve começar / terminar de outro lugar
            if(i == d->bloco_inicial)
                idx_init = d->inicio;
            if(i == d->bloco_final)
                idx_fim = d->fim;

            //vai destruindo todos os itens dentro do bloco
            for(int j = idx_init; j <= idx_fim; j++){
                d->destroy_fn(d->map[i][j]);
            }
            //ao acabar, libera o ponteiro para o bloco
            free(d->map[i]);
        }
    }
    //e por fim libera o mapa
    free(d->map);
}

void deque_destroy(Deque *d){
    _map_destroy(d);
    free(d);
}

static void _centraliza_mapa(Deque *d){
    //verifica se o mapa centralizado tem que ser maior do que o atual
    if(d->bloco_final >= d->tamanho_mapa - 1 && d->bloco_inicial == 0)
        d->tamanho_mapa = d->tamanho_mapa * 2;
                
    data_type **novo_mapa = calloc(d->tamanho_mapa, sizeof(data_type *));
            
    //seleciona qual deve ser o inicio para que o mapa fique centralizado
    int novo_inicio = (d->tamanho_mapa - (d->bloco_final - d->bloco_inicial))/2;

    // Desloca os blocos existentes para a nova posição no mapa
    for (int i = d->bloco_inicial, j = novo_inicio; i <= d->bloco_final; i++, j++) {
        if(!(i == d->bloco_final && d->fim == 0))
            novo_mapa[j] = d->map[i];
    }
            
    // Atualiza os ponteiros do deque
    d->bloco_final = novo_inicio + (d->bloco_final - d->bloco_inicial);
    d->bloco_inicial = novo_inicio;
    free(d->map);
    d->map = novo_mapa;
}

void deque_push_front(Deque *d, data_type data){
    // Verifica se o deque está vazio
    if (deque_size(d) == 0) {
        if(d->map[d->bloco_inicial] == NULL){
            d->map[d->tamanho_mapa/2] = calloc(d->tamanho_bloco, sizeof(data_type));
            d->bloco_final = d->bloco_inicial = d->tamanho_mapa/2;
            d->inicio = d->fim = 0;
        }

        d->map[d->bloco_inicial][d->inicio] = data;
        d->fim++;
        return;
    }
    
    // Verifica se é necessário alocar um novo bloco no início
    if (d->inicio == 0) {
        if (d->bloco_inicial == 0) {
            // É necessário realocar o mapa para acomodar mais blocos
            _centraliza_mapa(d);
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
    if(deque_size(d) == 0){
        if(d->map[d->bloco_inicial] == NULL){
            d->map[d->tamanho_mapa/2] = calloc(d->tamanho_bloco, sizeof(data_type));
            d->bloco_final = d->bloco_inicial = d->tamanho_mapa/2;
            d->inicio = d->fim = 0;
        }

        d->map[d->bloco_inicial][d->fim] = data;
        d->fim++;
        return;
    }

    // Verifica se é necessário alocar um novo bloco no final
    if (d->fim == 0 && d->bloco_final != d->bloco_inicial) {
        if (d->bloco_final == d->tamanho_mapa) {
            // É necessário reajustar o mapa (ou realocar)
            _centraliza_mapa(d);
        }
        
        // Aloca um novo bloco no final
        d->map[d->bloco_final] = calloc(d->tamanho_bloco, sizeof(data_type));
    }

    d->map[d->bloco_final][d->fim] = data;

    // Insere o novo elemento no final do deque
    //verifica se o fim precisa mudar de bloco
    if(d->fim == d->tamanho_bloco - 1){
        d->bloco_final++;
        d->fim = 0;
    }
    else
        d->fim++;
}

data_type deque_pop_back(Deque *d){
    //verifica se está vazio
    if (deque_size(d) == 0) {
        exit(printf("ERRO: DEQUE VAZIO\n"));
    }

    int novo_bloco = d->bloco_final;
    int novo_fim = d->fim;

    if(d->fim == 0){
        novo_bloco--;
        novo_fim = d->tamanho_bloco - 1;
    }
    else
        novo_fim--;
        
    data_type pop = d->map[novo_bloco][novo_fim];
    d->map[novo_bloco][novo_fim] = NULL;
    
    d->fim = novo_fim;
    if(d->fim == 0){
        free(d->map[d->bloco_final]);
        d->map[d->bloco_final] = NULL;
    }
    d->bloco_final = novo_bloco;
        
    return pop;
}

data_type deque_pop_front(Deque *d){
    //verifica se está vazio
    if (deque_size(d) == 0) {
        exit(printf("ERRO: DEQUE VAZIO\n"));
    }

    data_type pop = d->map[d->bloco_inicial][d->inicio];
    d->map[d->bloco_inicial][d->inicio] = NULL;
    
    if(d->inicio == d->tamanho_bloco - 1){
        free(d->map[d->bloco_inicial]);
        d->map[d->bloco_inicial] = NULL;
        d->bloco_inicial++;
        d->inicio = -1;
        if(d->bloco_inicial == d->tamanho_mapa){
            d->bloco_final = d->bloco_inicial = d->tamanho_mapa/2;
            d->fim = 0;
        }
    }
    d->inicio++;
        
    return pop;
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

// TODO!