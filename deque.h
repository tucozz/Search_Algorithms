typedef struct _Deque Deque;
typedef void* data_type;

Deque *deque_construct(void(*destroy_fn)(data_type));

void deque_destroy(Deque *d);

void deque_push_front(Deque *d, data_type data);

void deque_push_back(Deque *d, data_type data);

data_type deque_pop_back(Deque *d);

data_type deque_pop_front(Deque *d);

int deque_size(Deque *d);

data_type deque_get(Deque *d, int idx);

void deque_print(Deque *d, void(*print_fn)(data_type));
