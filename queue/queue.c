#include "queue.h"
#include <stdlib.h>
#include <stddef.h>

typedef struct Node Node;
struct Node {
    Node *next;
    void *val;
};

typedef struct Queue {
    Node *first, *last;
} Queue;

int queue_empty(Queue *q){
    return q == NULL || q->first == NULL;
}

Queue* queue(){
    Queue* a = malloc(sizeof(Queue));
    a->first = NULL;
    a->last  = NULL;
    return a;
}

static Node* node_alloc(void* elem){
    Node* a = malloc(sizeof(Node));
    a->next = NULL;
    a->val  = elem;
    return a;
}

void queue_push(Queue *q, void* elem){
    if (queue_empty(q)){
        q->last = q->first = node_alloc(elem);
        return;
    }
    Node *l = q->last;
    l->next = node_alloc(elem);
    q->last = l->next;
    return;
}

void queue_pop(Queue *q){
    if (queue_empty(q)) return;
    Node* a = q->first;
    q->first = a->next; // если они указывают на один, то след это NULL
    if (a == q->last) q->last = NULL;
    free(a);
}

void* queue_peek(Queue *q){
    if (queue_empty(q)) return NULL;
    return q->first->val;
}

void queue_foreach(Queue *q, void (*f)(void*)){
    for(Node *p = q->first; p != NULL; p = p->next){
        f(p->val);
    }
}

void queue_free(Queue **q){
    while( ! queue_empty(*q)){
        queue_pop(*q);
    }
    free(*q);
    *q = NULL;
}
