
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

static void _heapify_idx_swap(HeapNode *nodes, int idx1, int idx2){
    HeapNode aux = nodes[idx1];
    nodes[idx1] = nodes[idx2];
    nodes[idx2] = aux;
}

static void _heapify(Heap *h, int idx_init){
    if(h->size == 0)
        return;
    
    int current = idx_init;
    while(current != 0 && h->nodes[node_idx_parent(current)].priority > h->nodes[current].priority)
        _heapify_idx_swap(h->nodes, current, node_idx_parent(current));
}

static HeapNode _heapify_down(Heap *h, int idx_init){
    HeapNode pop = h->nodes[0];
    h->size--;
    h->nodes[0] = h->nodes[h->size];
    
    int current = idx_init;

    while(current < h->size && (h->nodes[node_idx_left_child(current)].priority < h->nodes[current].priority
                            || h->nodes[node_idx_right_child(current)].priority < h->nodes[current].priority)){

        if(h->nodes[node_idx_left_child(current)].priority < h->nodes[node_idx_right_child(current)].priority)
            _heapify_idx_swap(h->nodes ,node_idx_left_child(current), node_idx_right_child(current));
    }

    return pop;
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

    int aux = -1;
    CmpFunction cmp = hash_table_cmp_fn;
    double *new_val = malloc(sizeof(int));
    *new_val = priority;
    void *old = hash_table_set(heap->hash, data, new_val);

    if(old){
        //busca onde no array tá a celula
        for(int i = 0; i < heap->size; i++){
            if(cmp(heap->nodes[i].data, data) == 0)
                if(heap->nodes[i].priority > *(int *)old)
                    _heapify_down(heap->nodes, i);
                else if(heap->nodes[i].priority < *(int *)old)
                    _heapify(heap, i);
        }

        return old;
    }

    if(heap->size == heap->capacity){
        heap->capacity = 2*heap->capacity;
        realloc(heap->nodes, heap->capacity);
    }
    HeapNode node;
    node.data = data;
    node.priority = priority;

    heap->nodes[heap->size] = node;
    heap->size++;

    _heapify(heap, heap->size - 1);

    return NULL;
}

bool heap_empty(Heap *heap){return heap->size == 0;}

void *heap_min(Heap *heap){return heap->nodes[0].data;}

double heap_min_priority(Heap *heap){return heap->nodes[0].priority;}

void *heap_pop(Heap *heap){return _heapify_down(heap, 0).data;}

void heap_destroy(Heap *heap){
    free(heap->nodes);
    free(heap);
}