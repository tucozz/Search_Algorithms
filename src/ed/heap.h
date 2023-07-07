
#ifndef _HEAP_H_
#define _HEAP_H_

#include "types.h"
#include "hash.h"

typedef struct Heap Heap;

Heap *heap_construct(HashTable *hash);
void *heap_push(Heap *heap, void *data, double priority);
bool heap_empty(Heap *heap);
void *heap_min(Heap *heap);
double heap_min_priority(Heap *heap);
void *heap_pop(Heap *heap);
void heap_destroy(Heap *heap);
void *heap_idx_data(Heap *heap, int idx);
int heap_idx_priority(Heap *heap, int idx);

#endif