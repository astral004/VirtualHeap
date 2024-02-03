//
// Created by Kashyap Tare on 3/25/23.
//

#ifndef PRACTICUM1_QUEUE_H
#define PRACTICUM1_QUEUE_H
#include <stdio.h>
#include <stdlib.h>

typedef struct node_t {
    struct node_t *previous;
    struct node_t *next;
    int virtualPageNum;
}node;

typedef struct queue_t {
    node *head;
    node *tail;
    int size;
    int isThreadSafe;
}queue;

queue* newQueue();
void enqueue(queue *queue1, int virtualPageNum);
int dequeue(queue *queue1);
int isInQueue(queue *queue1, int virtualPageNum);


#endif //PRACTICUM1_QUEUE_H
