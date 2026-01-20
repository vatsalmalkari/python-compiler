#include <stdio.h>
#include "pystack.h"
#include "pyqueue.h"
#include "pyobject.h"
#include "pyint.h"
#include <stdlib.h>


int main() {
    printf(" STACK TEST \n");

    pystack* s = stack_new();

    printf("push 10\n");
    stack_push(s, pyint_new(10));

    printf("push 20\n");
    stack_push(s, pyint_new(20));

    printf("push 30\n");
    stack_push(s, pyint_new(30));

    printf("peek -> ");
    pyobject_print(stack_peek(s));
    printf("\n");

    printf("pop  -> ");
    pyobject_print(stack_pop(s));
    printf("\n");

    printf("pop  -> ");
    pyobject_print(stack_pop(s));
    printf("\n");

    printf("len  -> %d\n", stack_len(s));

    stack_del(s);

    printf("\n QUEUE TEST \n");

    pyqueue* q = queue_new();

    printf("enqueue 1\n");
    queue_enqueue(q, pyint_new(1));

    printf("enqueue 2\n");
    queue_enqueue(q, pyint_new(2));

    printf("enqueue 3\n");
    queue_enqueue(q, pyint_new(3));

    printf("peek     -> ");
    pyobject_print(queue_peek(q));
    printf("\n");

    printf("dequeue  -> ");
    pyobject_print(queue_dequeue(q));
    printf("\n");

    printf("dequeue  -> ");
    pyobject_print(queue_dequeue(q));
    printf("\n");

    printf("len      -> %d\n", queue_len(q));

    queue_del(q);

    return 0;
}