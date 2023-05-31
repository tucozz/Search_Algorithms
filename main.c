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

    for(int i = 0; i < 25; i++){
        int *v = malloc(sizeof(int));
        deque_push_front(d, &v);
    }

    deque_print(d, print_deque);

    return 0;
}