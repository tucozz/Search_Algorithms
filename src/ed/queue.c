
#include "queue.h"
#include "deque.h"

struct Queue{
    Deque *deque;
};

Queue *queue_construct(void(*destroy_fn)(data_type)){
    Queue *q = malloc(sizeof(Queue));
    q->deque = deque_construct(destroy_fn);
    return q;
}

void queue_push(Queue *queue, void *data){
    deque_push_back(queue->deque, data);
}

bool queue_empty(Queue *queue){
    return (deque_size(queue->deque) == 0);
}

void *queue_pop(Queue *queue){
    void *pop = deque_pop_front(queue->deque);
    return pop;
}

void queue_destroy(Queue *queue){
    deque_destroy(queue->deque);
    free(queue);
}

// TODO!