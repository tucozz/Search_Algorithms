#include <stdlib.h>

#include "hash.h"
#include "forward_list.h"

struct HashTable
{
    ForwardList **buckets;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
    int table_size;
    int n_elements;
};

struct HashTableIterator
{
    HashTable *h;
    Node *current;
    int current_idx;
    int visited;
};

HashTableItem *_hash_pair_construct(void *key, void *val)
{
    HashTableItem *p = malloc(sizeof(HashTableItem));
    p->key = key;
    p->val = val;
    return p;
}

void _hash_pair_destroy(HashTableItem *p)
{
    free(p);
}

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn)
{
    HashTable *hash_tbl = calloc(1, sizeof(HashTable));

    hash_tbl->table_size = table_size;
    hash_tbl->hash_fn = hash_fn;
    hash_tbl->cmp_fn = cmp_fn;
    hash_tbl->buckets = calloc(table_size, sizeof(ForwardList *));
    hash_tbl->n_elements = 0;

    return hash_tbl;
}

void *hash_table_set(HashTable *h, void *key, void *val)
{
    //cria um Key_Value_Pair a ser adicionado
    HashTableItem *new_kvp = _hash_pair_construct(key, val);
    //identifica qual o index real a ser adicionado na hashtable
    int new_idx = h->hash_fn(h, key)%h->table_size;
    //verifica se o item existe lá dentro (conferir se existe jeito melhor de fazer isso)
    if(h->buckets[new_idx] == NULL)
        h->buckets[new_idx] = forward_list_construct();
    Node *n = h->buckets[new_idx]->head;
    data_type *data_return = NULL;

    while (n != NULL)
    {
        if (h->cmp_fn(key, (void*)((HashTableItem *)n->value)->key) == 0)
        {
            data_return = ((HashTableItem *)n->value)->val;
            ((HashTableItem *)n->value)->val = val;

            free(new_kvp);
            return data_return;
        }
        else
        {
            n = n->next;
        }
    }
    //adiciona no final da lista se nao encontrou nada
    h->n_elements++;
    forward_list_push_front(h->buckets[new_idx], new_kvp);
    return data_return;
}

void *hash_table_get(HashTable *h, void *key)
{
    //identifica qual o index real a ser buscado na hashtable
    int new_idx = h->hash_fn(h, key)%h->table_size;
    //retorna o item desejado se ele existir no index, e NULL se nao.
    if(h->buckets[new_idx] == NULL)
        return NULL;
    
    Node *n = h->buckets[new_idx]->head;

    while (n != NULL)
    {
        if (!h->cmp_fn((void*)((HashTableItem *)n->value)->key, key))
            return ((HashTableItem *)n->value)->val;

        n = n->next;
    }

    return NULL;
}

void *hash_table_pop(HashTable *h, void *key)
{
    //identifica qual o index real a ser buscado na hashtable
    int new_idx = h->hash_fn(h, key)%h->table_size;
    //retorna o item desejado se ele existir no index, e NULL se nao.
    if(h->buckets[new_idx] == NULL)
        return NULL;

    Node *n = h->buckets[new_idx]->head;
    Node *prev = NULL;
    Node *new_n = NULL;
    void *data = NULL;

    while (n != NULL)
    {
        if (h->cmp_fn(key, (void*)((HashTableItem *)n->value)->key) == 0)
        {
            h->n_elements--;
            data = ((HashTableItem *)n->value)->val;
            if (prev == NULL)
                h->buckets[new_idx]->head = new_n = n->next;
            else
                prev->next = new_n = n->next;

            _hash_pair_destroy((HashTableItem *)n->value);
            node_destroy(n);
            n = new_n;
            h->buckets[new_idx]->size--;
            if(h->buckets[new_idx]->size == 0){
                forward_list_destroy(h->buckets[new_idx]);
                h->buckets[new_idx] = NULL;
            }
                
            return data;
        }
        else
        {
            prev = n;
            n = n->next;
        }
    }
    return NULL;
}

int hash_table_size(HashTable *h)
{
    return h->table_size;
}

int hash_table_num_elems(HashTable *h)
{
    return h->n_elements;
}

CmpFunction hash_table_cmp_fn(HashTable *h)
{
    return h->cmp_fn;
}

void hash_table_destroy(HashTable *h)
{
    for (int i = 0; i < h->table_size; i++)
    {
        if (h->buckets[i] != NULL)
        {
            Node *n = h->buckets[i]->head;

            while (n != NULL)
            {
                HashTableItem *pair = n->value;
                _hash_pair_destroy(pair);
                n = n->next;
            }

            forward_list_destroy(h->buckets[i]);
        }
    }

    free(h->buckets);
    free(h);
}

// cria um novo iterador para a tabela hash
HashTableIterator *hash_table_iterator(HashTable *h){
    HashTableIterator *it = malloc(sizeof(HashTableIterator));
    it->h = h;
    it->visited = 0;
    it->current = NULL;
    it->current_idx = 0;

    return it;
}

// retorna 1 se o iterador chegou ao fim da tabela hash ou 0 caso contrario
int hash_table_iterator_is_over(HashTableIterator *it){
    return it->visited >= it->h->n_elements;
}

// retorna o proximo par chave valor da tabela hash
HashTableItem *hash_table_iterator_next(HashTableIterator *it){
    //CASO: acabou de criar, busca o primeiro item
    if(it->current == NULL){
        for(int i = 0; i < it->h->table_size; i++){
            if(it->h->buckets[i] != NULL){
                it->current_idx = i;
                it->current = it->h->buckets[i]->head;
                it->visited++;
                return (HashTableItem *)it->current->value;
            }

        }
    }

    //CASO: a lunked list ainda tem coisa pra frente, entao só passa pro proximo
    if(it->current->next != NULL){
        it->current = it->current->next;
        it->visited++;
        return (HashTableItem *)it->current->value;
    }
        
    //CASO: acabou a linked list entao tem que buscar o proximo index com alguma coisa
    //nota que se nao achar mais nada, vai sair do loop sem atualizar e retornar NULL
    for(int i = it->current_idx + 1; i < it->h->table_size; i++){
        if(it->h->buckets[i] != NULL){
            it->current_idx = i;
            it->current = it->h->buckets[i]->head;
            it->visited++;
            return (HashTableItem *)it->current->value;
        }
    }

    return NULL;
}

// desaloca o iterador da tabela hash
void hash_table_iterator_destroy(HashTableIterator *it){
    free(it);
}