
#include "heap.h"
#include "hash.h"
#include <stdlib.h>

typedef struct
{
    void *data;
    double priority;
}HeapNode;

struct Heap
{
    HashTable *hash;
    HeapNode *nodes;
    int size;
    int capacity;
};

static int node_idx_parent(int idx){return (int)(idx - 1)/2;}

static int node_idx_left_child(int idx){return 2*idx + 1;}

static int node_idx_right_child(int idx){return 2*idx + 2;}

static void _heapify_idx_swap(Heap *heap, int idx1, int idx2){
    //muda na hashtable primeiro
    void *hash_idx1 = hash_table_get(heap->hash, heap->nodes[idx1].data); 
    void *hash_idx2 = hash_table_get(heap->hash, heap->nodes[idx2].data); 

    hash_table_set(heap->hash, heap->nodes[idx1].data, hash_idx2);
    hash_table_set(heap->hash, heap->nodes[idx2].data, hash_idx1);

    //depois muda no heap mesmo
    HeapNode aux = heap->nodes[idx1];
    heap->nodes[idx1] = heap->nodes[idx2];
    heap->nodes[idx2] = aux;
}

static int _heapify(Heap *h, int idx_init){
    if(h->size == 0)
        return idx_init;
    
    int current = idx_init;
    while(current != 0 && h->nodes[node_idx_parent(current)].priority > h->nodes[current].priority){
        _heapify_idx_swap(h, current, node_idx_parent(current));
        current = node_idx_parent(current);
    }

    return current;
}

static int _heapify_down(Heap *h, int idx_init){
    int current = idx_init;

    while(node_idx_right_child(current) < h->size && (h->nodes[node_idx_left_child(current)].priority < h->nodes[current].priority
                            || h->nodes[node_idx_right_child(current)].priority < h->nodes[current].priority)){

        if(h->nodes[node_idx_left_child(current)].priority < h->nodes[node_idx_right_child(current)].priority){
            _heapify_idx_swap(h, node_idx_left_child(current), current);
            current = node_idx_left_child(current);
        }
        else if (h->nodes[node_idx_left_child(current)].priority > h->nodes[node_idx_right_child(current)].priority){
            _heapify_idx_swap(h, node_idx_right_child(current), current);
            current = node_idx_right_child(current);
        }
    }

    return current;
}

Heap *heap_construct(HashTable *hash){
    Heap *h = malloc(sizeof(Heap));
    h->size = 0;
    h->capacity = 10;
    h->nodes = calloc(h->capacity, sizeof(HeapNode));
    h->hash = hash;

    return h;
}

void *heap_push(Heap *heap, void *data, double priority){
    //checa pra ver se o valor ja existe
    void *old = hash_table_get(heap->hash, data);

    //se ja existir, atualiza o valor e ajeita o heap
    //esse heap vai apenas atualizar se o valor for menor do que o atual. descomente o else if para que ele atualize tambem.
    if(old){
        if(heap->nodes[*(int *)old].priority > priority){
            heap->nodes[*(int *)old].priority = priority;
            _heapify(heap, *(int *)old);
        }
            
        else if(heap->nodes[*(int *)old].priority < priority){
            //heap->nodes[*(int *)old].priority = priority;
            //_heapify_down(heap, *(int *)old);
        }

        //retorna o void *data que foi enviado para que ele possa ser desalocado
        return data;
    }

    //se o valor não existir, insere normalmente no final e faz heapify
    if(heap->size == heap->capacity){
        heap->capacity = 2*heap->capacity;
        realloc(heap->nodes, heap->capacity);
    }
    HeapNode node;
    node.data = data;
    node.priority = priority;

    heap->nodes[heap->size++] = node;

    int hash_new_idx = _heapify(heap, heap->size - 1);
    int *int_ptr = malloc(sizeof(int));
    *int_ptr = hash_new_idx;
    void *prev = hash_table_set(heap->hash, data, int_ptr);
    if(prev)
        free(prev);

    return NULL;
}

bool heap_empty(Heap *heap){return heap->size == 0;}

void *heap_min(Heap *heap){return heap->nodes[0].data;}

double heap_min_priority(Heap *heap){return heap->nodes[0].priority;}

void *heap_pop(Heap *heap){
    heap->size--;
    void *pop = heap->nodes[0].data;

    void *hash_pop = hash_table_pop(heap->hash, heap->nodes[0].data);
    free(hash_pop);

    heap->nodes[0] = heap->nodes[heap->size];
    int *zero = malloc(sizeof(int));
    *zero = 0;
    int deletazero = 1;
    if(hash_table_num_elems(heap->hash) != 0){
        void *prev = hash_table_set(heap->hash, heap->nodes[0].data, zero);
        deletazero = 0;
        free(prev);
    }
    if(deletazero)
        free(zero);
    _heapify_down(heap, 0);
    return pop;
}

void heap_destroy(Heap *heap){
    free(heap->nodes);
    free(heap);
}