
#include "heap.h"
#include <stdlib.h>

typedef struct
{
    void *data;
    double priority;
}HeapNode;

struct Heap
{
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

static void _heapify(Heap *h){
    if(h->size == 0)
        return;
    
    int current = h->size-1;
    while(current != 0 && h->nodes[node_idx_parent(current)].priority > h->nodes[current].priority)
        _heapify_idx_swap(h->nodes, current, node_idx_parent(current));
}

static HeapNode _heapify_down(Heap *h){
    HeapNode pop = h->nodes[0];
    h->size--;
    h->nodes[0] = h->nodes[h->size];
    
    int current = 0;

    while(current < h->size && (h->nodes[node_idx_left_child(current)].priority < h->nodes[current].priority
                            || h->nodes[node_idx_right_child(current)].priority < h->nodes[current].priority)){

        if(h->nodes[node_idx_left_child(current)].priority < h->nodes[node_idx_right_child(current)].priority)
            _heapify_idx_swap(h,node_idx_left_child(current), node_idx_right_child(current));
    }

    return pop;
}

Heap *heap_construct(){
    Heap *h = malloc(sizeof(Heap));
    h->size = 0;
    h->capacity = 10;
    h->nodes = calloc(h->capacity, sizeof(HeapNode));

    return h;
}

void heap_push(Heap *heap, void *data, double priority){
    if(heap->size == heap->capacity){
        heap->capacity = 2*heap->capacity;
        realloc(heap->nodes, heap->capacity);
    }
    HeapNode node;
    node.data = data;
    node.priority = priority;

    heap->nodes[heap->size] = node;
    heap->size++;

    _heapify(heap);
}

bool heap_empty(Heap *heap){return heap->size == 0;}

void *heap_max(Heap *heap){return heap->nodes[0].data;}

double heap_max_priority(Heap *heap){return heap->nodes[0].priority;}

void *heap_pop(Heap *heap){return _heapify_down(heap).data;}

void heap_destroy(Heap *heap){
    free(heap->nodes);
    free(heap);
}