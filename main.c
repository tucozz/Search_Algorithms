
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/ed/deque.h"

typedef struct
{
    int x, y;
} Celula;

Celula *celula_construct(int x, int y)
{
    Celula *c = malloc(sizeof(Celula));
    c->x = x;
    c->y = y;
    return c;
}

void celula_free(Celula *c)
{
    free(c);
}

int main()
{
    int i, n, x, y;
    char cmd[10];
    Deque *d = deque_construct();

    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        scanf("\n%s", cmd);

        if (!strcmp(cmd, "PUSH_BACK"))
        {
            scanf("%d %d", &x, &y);
            Celula *celula = celula_construct(x, y);
            deque_push_back(d, celula);
        }
        else if (!strcmp(cmd, "PUSH_FRONT"))
        {
            scanf("%d %d", &x, &y);
            Celula *celula = celula_construct(x, y);
            deque_push_front(d, celula);
        }
        else if (!strcmp(cmd, "POP_BACK"))
        {
            Celula *c = deque_pop_back(d);
            printf("%d %d\n", c->x, c->y);
            celula_free(c);
        }
        else if (!strcmp(cmd, "POP_FRONT"))
        {
            Celula *c = deque_pop_front(d);
            printf("%d %d\n", c->x, c->y);
            celula_free(c);
        }
    }

    deque_destroy(d);
    return 0;
}