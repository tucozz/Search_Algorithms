
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

static void _heapify(Heap *h){
    if(h->size == 0)
        return;
    
    int current = h->size-1;
    while(current != 0 && h->nodes[node_idx_parent(current)].priority < h->nodes[current].priority){
        HeapNode aux = h->nodes[current];
        h->nodes[current] = h->nodes[node_idx_parent(current)];
        h->nodes[node_idx_parent(current)] = aux;
        

    }
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

    _heapify(heap);
}

bool heap_empty(Heap *heap){return heap->size == 0;}

void *heap_max(Heap *heap){return heap->nodes[0].data;}

double heap_max_priority(Heap *heap){return heap->nodes[0].priority;}

void *heap_pop(Heap *heap){

}

void heap_destroy(Heap *heap){

}