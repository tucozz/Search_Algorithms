#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

void print_deque(data_type data){
    int data_print = *((int *)data);
    printf("%d ", data_print);
}

int main(){
    //ATENÇÃO: FUNÇÃO AUXILIAR APENAS PARA TESTES.
    //FAVOR MODIFICAR ANTES DA ENTREGA.


    Deque *d = deque_construct(free);

    int v1 = 3;
    deque_push_back(d, &v1);
    int v2 = 5;
    deque_push_front(d, &v2);
    int v3 = 5;
    deque_push_front(d, &v3);
    int v9 = 5;
    deque_push_front(d, &v9);
    //int v10 = 7;
    //deque_push_front(d, &v10);

    deque_print(d, print_deque);

    return 0;
}