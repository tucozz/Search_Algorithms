typedef struct _Deque Deque;
typedef void* data_type;

Deque *deque_construct();

void deque_destroy(Deque *d);

void deque_push_front(Deque *d);

void deque_push_back(Deque *d);

data_type deque_pop_back(Deque *d);

data_type deque_pop_front(Deque *d);

int deque_size(Deque *d);

