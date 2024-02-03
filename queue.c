//
// Created by Kashyap Tare on 3/25/23.
//
#include "queue.h"

queue* newQueue(){
    queue *queue1 = (queue*) malloc(sizeof (queue));
    queue1->head = NULL;
    queue1->tail = NULL;
    queue1->size = 0;
    queue1->isThreadSafe = 0;
    return queue1;
}

void enqueue(queue* queue1, int virtualPageNum){
    if (queue1->isThreadSafe == 1){
        pthread_mutex_lock(&mutex);
    }
    if (isInQueue(queue1, virtualPageNum) == 0){
        node *node1 = (node*) malloc(sizeof(node));
        node1->virtualPageNum = virtualPageNum;
        if (queue1->size == 0) {
            queue1->head = node1;
            queue1->tail = node1;
            queue1->size = 1;
        }
        else {
            node1->previous = queue1->tail;
            node1->next = NULL;
            queue1->tail->next = node1;
            queue1->tail = node1;
            queue1->size++;
        }
    }
    if (queue1->isThreadSafe == 1){
        pthread_mutex_unlock(&mutex);
    }
}

int dequeue(queue* queue1){
    if (queue1->isThreadSafe == 1){
        pthread_mutex_lock(&mutex);
    }
    int virtualPageNum = queue1->head->virtualPageNum;
    if (queue1->size == 1) {
        queue1->head = NULL;
        queue1->tail = NULL;
        queue1->size = 0;
    }
    else{
        node *newHead = queue1->head->next;
        newHead->previous = NULL;
        queue1->head = newHead;
        queue1->size-=1;
    }
    if (queue1->isThreadSafe == 1){
        pthread_mutex_unlock(&mutex);
    }
    return virtualPageNum;
}

int isInQueue(queue *queue1, int virtualPageNum){
    if (queue1->isThreadSafe == 1){
        pthread_mutex_lock(&mutex);
    }
    node *currNode = queue1->head;
    for (int i = 0; i < queue1->size; ++i) {
        if (currNode->virtualPageNum == virtualPageNum){
            if (queue1->isThreadSafe == 1){
                pthread_mutex_unlock(&mutex);
            }
            return 1;
        }
        else{
            currNode = currNode->next;
        }
    }
    if (queue1->isThreadSafe == 1){
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}