#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"
#include "../ed/queue.h"
#include "../ed/stack.h"
#include "../ed/heap.h"
#include "../ed/hash.h"

typedef struct{
    double custo_inicio;
    Celula cel;
    struct LabNode *anterior;
}LabNode;

Celula *celula_create(int x, int y)
{
    Celula *c = malloc(sizeof(Celula));
    c->x = x;
    c->y = y;
    return c;
}

void celula_destroy(Celula *c)
{
    free(c);
}

int _lab_node_hash(HashTable *h, void *key)
{
    Celula c = ((LabNode *)key)->cel;
    // 83 e 97 sao primos e o operador "^" é o XOR bit a bit
    return ((c.x * 83) ^ (c.y * 97)) % hash_table_size(h);
}

int _lab_node_cmp(void *c1, void *c2)
{
    Celula a = ((LabNode *)c1)->cel;
    Celula b = ((LabNode *)c2)->cel;

    if (a.x == b.x && a.y == b.y)
        return 0;
    else
        return 1;
}

double _calcula_distancia(Celula cel1, Celula cel2){
    return sqrt(pow(cel1.x - cel2.x, 2) + pow(cel1.y - cel2.y, 2));
}

LabNode *_lab_node_construct(Celula cel, LabNode *prev){
    LabNode *labnode  = malloc(sizeof(LabNode));
    if(prev)
        labnode->custo_inicio = prev->custo_inicio + _calcula_distancia(cel, prev->cel);
    else
        labnode->custo_inicio = 0;
    labnode->cel = cel;
    labnode->anterior = (struct LabNode *)prev;

    return labnode;
}

void _lab_node_destroy(LabNode *node){
    free(node);
}

ResultData _default_result()
{
    ResultData result;

    result.caminho = NULL;
    result.custo_caminho = 0;
    result.nos_expandidos = 0;
    result.tamanho_caminho = 0;
    result.sucesso = 0;

    return result;
}

int _is_valid(Labirinto *l, Celula atual){
    if ((atual.x > labirinto_n_colunas(l) - 1) || (atual.y > labirinto_n_linhas(l) - 1) || (atual.x < 0) || (atual.y < 0))
        return 0;
    if(labirinto_obter(l, atual.y, atual.x) != LIVRE && labirinto_obter(l, atual.y, atual.x) != FIM)
        return 0;

    return 1;
}

LabNode *_atualiza_fronteira(Labirinto *l, LabNode *node, int idx){
    Celula cel = node->cel;
    
    switch (idx)
    {
    case 0:
        cel.y -= 1;
        break;
    case 1:
        cel.y -=1;
        cel.x +=1;
        break;
    case 2:
        cel.x +=1;
        break;
    case 3:
        cel.x +=1;
        cel.y +=1;
        break;
    case 4:
        cel.y +=1;
        break;
    case 5:
        cel.x -=1;
        cel.y +=1;
        break;
    case 6:
        cel.x -=1;
        break;
    case 7:
        cel.x -=1;
        cel.y -=1;
        break;
    default:
        printf("IDX INVALIDO.\n");
    }

    if(!_is_valid(l, cel))
        return NULL;

    labirinto_atribuir(l, cel.y, cel.x, FRONTEIRA);
    LabNode *pop = _lab_node_construct(cel, node);
    return pop;
}

ResultData a_star(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();

    LabNode *atual = _lab_node_construct(inicio, NULL);
    HashTable *heap_hash = hash_table_construct(383, _lab_node_hash, _lab_node_cmp);
    Heap *fronteira = heap_construct(heap_hash);
    Stack *expandidos = stack_construct(NULL);
    while (atual->cel.y != fim.y || atual->cel.x != fim.x){
        //PARA DEBUG
        //labirinto_print(l);
        //printf("\n");
        for(int i = 0; i < 8; i++){
            LabNode *node = _atualiza_fronteira(l, atual, i);
            double funcao_f = 0;
            if(node){
                funcao_f = node->custo_inicio + _calcula_distancia(node->cel, fim);
                heap_push(fronteira, node, funcao_f);
            }
        }
        result.nos_expandidos++;
        labirinto_atribuir(l, atual->cel.y, atual->cel.x, EXPANDIDO);
        stack_push(expandidos, atual);
        if(heap_empty(fronteira)){
            //limpa tudo antes de ir embora
            //libera os nos expandidos
            while(!stack_empty(expandidos)){
                LabNode *pop = (LabNode *)stack_pop(expandidos);
                _lab_node_destroy(pop); 
            }
            stack_destroy(expandidos);

            //libera a fronteira
            HashTableIterator *it = hash_table_iterator(heap_hash);

            while (!hash_table_iterator_is_over(it))
            {
                HashTableItem *item = hash_table_iterator_next(it);
                LabNode *node = (LabNode *)item->key;
                int *pos = (int *)item->val;
                _lab_node_destroy(node);
                free(pos);
            }

            hash_table_iterator_destroy(it);
            hash_table_destroy(heap_hash);
            heap_destroy(fronteira);

            return _default_result();
        }
        else
            atual = heap_pop(fronteira);
    }
    result.nos_expandidos++;
    stack_push(expandidos, atual);
    result.sucesso = 1;
    result.custo_caminho = atual->custo_inicio;

    //identifica o caminho principal
    Stack *stack = stack_construct(NULL);
    LabNode *current = atual;
    while(current != NULL){
        stack_push(stack, current);
        current = (LabNode *)current->anterior;
        result.tamanho_caminho++;
    }
    result.caminho = calloc(result.tamanho_caminho, sizeof(Celula));
    int idx = 0;
    while(!stack_empty(stack)){
        result.caminho[idx++] = ((LabNode *)stack_pop(stack))->cel;
    }
    stack_destroy(stack);

    //libera os nos expandidos
    while(!stack_empty(expandidos)){
        LabNode *pop = (LabNode *)stack_pop(expandidos);
        _lab_node_destroy(pop); 
    }
    stack_destroy(expandidos);

    //libera a fronteira
    HashTableIterator *it = hash_table_iterator(heap_hash);

    while (!hash_table_iterator_is_over(it))
    {
        HashTableItem *item = hash_table_iterator_next(it);
        LabNode *node = (LabNode *)item->key;
        int *pos = (int *)item->val;
        _lab_node_destroy(node);
        free(pos);
    }

    hash_table_iterator_destroy(it);
    hash_table_destroy(heap_hash);
    heap_destroy(fronteira);
    
    return result;
}

ResultData breadth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();

    LabNode *atual = _lab_node_construct(inicio, NULL);
    Queue *fronteira = queue_construct(NULL);
    Stack *expandidos = stack_construct(NULL);
    while (atual->cel.y != fim.y || atual->cel.x != fim.x){
        //PARA DEBUG
        //labirinto_print(l);
        //printf("\n");
        for(int i = 0; i < 8; i++){
            LabNode *node = _atualiza_fronteira(l, atual, i);
            if(node)
                queue_push(fronteira, node);
        }
        result.nos_expandidos++;
        labirinto_atribuir(l, atual->cel.y, atual->cel.x, EXPANDIDO);
        stack_push(expandidos, atual);
        if(queue_empty(fronteira)){
            //limpa tudo antes de ir embora
            //libera os nos expandidos
            while(!stack_empty(expandidos)){
                LabNode *pop = (LabNode *)stack_pop(expandidos);
                _lab_node_destroy(pop); 
            }
            stack_destroy(expandidos);

            //libera a fronteira
            while(!queue_empty(fronteira)){
                LabNode *pop = (LabNode *)queue_pop(fronteira);
                _lab_node_destroy(pop); 
            }
            queue_destroy(fronteira);

            return _default_result();
        }
        else
            atual = queue_pop(fronteira);
    }
    result.nos_expandidos++;
    stack_push(expandidos, atual);
    result.sucesso = 1;
    result.custo_caminho = atual->custo_inicio;

    Stack *stack = stack_construct(NULL);
    LabNode *current = atual;
    while(current != NULL){
        stack_push(stack, current);
        current = (LabNode *)current->anterior;
        result.tamanho_caminho++;
    }

    //identifica o caminho principal
    result.caminho = calloc(result.tamanho_caminho, sizeof(Celula));
    int idx = 0;
    while(!stack_empty(stack)){
        result.caminho[idx++] = ((LabNode *)stack_pop(stack))->cel;
    }
    stack_destroy(stack);

    //libera os nos expandidos
    while(!stack_empty(expandidos)){
        LabNode *pop = (LabNode *)stack_pop(expandidos);
        _lab_node_destroy(pop); 
    }
    stack_destroy(expandidos);

    //libera a fronteira
    while(!queue_empty(fronteira)){
        LabNode *pop = (LabNode *)queue_pop(fronteira);
        _lab_node_destroy(pop); 
    }
    queue_destroy(fronteira);
    
    return result;
}

ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();

    LabNode *atual = _lab_node_construct(inicio, NULL);
    Stack *fronteira = stack_construct(NULL);
    Stack *expandidos = stack_construct(NULL);
    while (atual->cel.y != fim.y || atual->cel.x != fim.x){
        //PARA DEBUG
        //labirinto_print(l);
        //printf("\n");
        for(int i = 0; i < 8; i++){
            LabNode *node = _atualiza_fronteira(l, atual, i);
            if(node)
                stack_push(fronteira, node);
        }
        result.nos_expandidos++;
        labirinto_atribuir(l, atual->cel.y, atual->cel.x, EXPANDIDO);
        stack_push(expandidos, atual);
        if(stack_empty(fronteira)){
            //limpa tudo antes de ir embora
            //libera os nos expandidos
            while(!stack_empty(expandidos)){
                LabNode *pop = (LabNode *)stack_pop(expandidos);
                _lab_node_destroy(pop); 
            }
            stack_destroy(expandidos);

            //libera a fronteira
            while(!stack_empty(fronteira)){
                LabNode *pop = (LabNode *)stack_pop(fronteira);
                _lab_node_destroy(pop); 
            }
            stack_destroy(fronteira);

            return _default_result();
        }
        else
            atual = stack_pop(fronteira);
    }
    result.nos_expandidos++;
    stack_push(expandidos, atual);
    result.sucesso = 1;
    result.custo_caminho = atual->custo_inicio;

    Stack *stack = stack_construct(NULL);
    LabNode *current = atual;
    while(current != NULL){
        stack_push(stack, current);
        current = (LabNode *)current->anterior;
        result.tamanho_caminho++;
    }

    //identifica o caminho principal
    result.caminho = calloc(result.tamanho_caminho, sizeof(Celula));
    int idx = 0;
    while(!stack_empty(stack)){
        result.caminho[idx++] = ((LabNode *)stack_pop(stack))->cel;
    }
    stack_destroy(stack);

    //libera os nos expandidos
    while(!stack_empty(expandidos)){
        LabNode *pop = (LabNode *)stack_pop(expandidos);
        _lab_node_destroy(pop); 
    }
    stack_destroy(expandidos);

    //libera a fronteira
    while(!stack_empty(fronteira)){
        LabNode *pop = (LabNode *)stack_pop(fronteira);
        _lab_node_destroy(pop); 
    }
    stack_destroy(fronteira);

    return result;
}

ResultData dummy_search(Labirinto *l, Celula inicio, Celula fim)
{
    int max_path_length = 0;
    float dx, dy;

    ResultData result = _default_result();

    max_path_length = abs(fim.x - inicio.x) + abs(fim.y - inicio.y);
    result.caminho = (Celula *)malloc(sizeof(Celula) * max_path_length);
    result.sucesso = 1;

    Celula atual = inicio;
    result.caminho[result.tamanho_caminho++] = atual;
    result.nos_expandidos++;

    while ((atual.x != fim.x) || (atual.y != fim.y))
    {
        dx = fim.x - atual.x;
        dy = fim.y - atual.y;

        if (dx != 0)
            dx /= fabs(dx);

        if (dy != 0)
            dy /= fabs(dy);

        atual.x += (int)dx;
        atual.y += (int)dy;

        if (labirinto_obter(l, atual.y, atual.x) == OCUPADO || (atual.x > labirinto_n_colunas(l) - 1) || (atual.y > labirinto_n_linhas(l) - 1) || (atual.x < 0) || (atual.y < 0))
        {
            result.sucesso = 0;
            free(result.caminho);
            result.caminho = NULL;
            return result;
        }

        result.caminho[result.tamanho_caminho++] = atual;
        result.nos_expandidos++;
        result.custo_caminho += sqrt(pow(dx, 2) + pow(dy, 2));
    }

    return result;
}
