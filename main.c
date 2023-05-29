#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

void print_deque(data_type data){
    int data_print = *((int *)data);
    printf("%d ", data_print);
}

int main(){
    Deque *d = deque_construct(free);

    deque_push_back(d, 3);
    deque_push_back(d, 5);
    deque_push_back(d, 7);

    deque_print(d, print_deque);

    return 0;
}