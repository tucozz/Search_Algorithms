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

typedef struct
{
    void *key;
    void *val;
} HashTableItem;

static int _kvp_cmp_fn(HashTableItem i1, HashTableItem i2)
{
    return hash_fn(i1.key, i2.key);
}

HashTableItem *_hash_pair_construct(void *key, void *val)
{
    HashTableItem *p = calloc(1, sizeof(HashTableItem));
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

void hash_table_set(HashTable *h, void *key, void *val)
{
    //cria um Key_Value_Pair a ser adicionado
    HashTableItem *new_kvp = _hash_pair_construct(key, val);
    //identifica qual o index real a ser adicionado na hashtable
    int new_idx = h->hash_fn(h, key)%h->table_size;
    //verifica se o item existe lá dentro (conferir se existe jeito melhor de fazer isso)
    forward_list_remove_first(h->buckets[new_idx], new_kvp, _kvp_cmp_fn);
    //adiciona no final da lista
    forward_list_push_back(h, new_kvp);

}

void *hash_table_get(HashTable *h, void *key)
{
    //cria um Key_Value_Pair por conveniencia
    HashTableItem *new_kvp = _hash_pair_construct(key, NULL);
    //identifica qual o index real a ser buscado na hashtable
    int new_idx = h->hash_fn(h, key)%h->table_size;
    //retorna o item desejado se ele existir no index, e NULL se nao.
    return forward_list_find(h->buckets[new_idx], new_kvp, _kvp_cmp_fn);
}

void *hash_table_pop(HashTable *h, void *key)
{
    //cria um Key_Value_Pair por conveniencia
    HashTableItem *new_kvp = _hash_pair_construct(key, NULL);
    //identifica qual o index real a ser buscado na hashtable
    int new_idx = h->hash_fn(h, key)%h->table_size;
    //retorna o item desejado se ele existir no index, e NULL se nao.
    Node *n = h->buckets[new_idx]->head;
    Node *prev = NULL;
    Node *new_n = NULL;
    data_type data = NULL;

    while (n != NULL)
    {
        if (h->cmp_fn(key, (void*)((HashTableItem)n->value)->val) == 0)
        {
            data = n->value;
            if (prev == NULL)
                l->head = new_n = n->next;
            else
                prev->next = new_n = n->next;

            node_destroy(n);
            n = new_n;
            l->size--;
            return data;
        }
        else
        {
            prev = n;
            n = n->next;
        }
    }
    return NULL;

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
