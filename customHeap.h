//
// Created by Kashyap Tare on 3/24/23.
//

#ifndef PRACTICUM1_CUSTOMHEAP_H
#define PRACTICUM1_CUSTOMHEAP_H
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "queue.h"
#define VIRTUAL_PAGES 1024
#define PHYSICAL_PAGES 512
#define PAGESIZE 256


typedef struct virtualPage_t{
    int virtualPageNum;
    int onDisk;
    int isAvailable;
    int isChanged; //todo does dirtyBit only change on pmPut? YES
    int size;
    int physPageNumber;
}virtualPage;

typedef struct physicalPage_t{
    void* physAddress;
    int isOccupied;
    int virtualPageNum;
}physicalPage;

typedef struct payload_t{
    int virtualPageNum;
    queue* q;
    void* bytes;
}Payload;

char pm_heap[PAGESIZE * PHYSICAL_PAGES];
virtualPage virtualPages[VIRTUAL_PAGES];
physicalPage physicalPages[PHYSICAL_PAGES];

int usedVirtualPages = 0;
int usedPhysicalPages = 0;

pthread_mutex_t mutex;

void init_pmHeap();
void init_mutex();
void init_virtualPages();
void init_physicalPages();

int pm_malloc(int sizeInBytes, queue *queue1);
void* pm_get(int virtualPageNum, queue *queue1);
void pm_put(int virtualPageNum, void *bytes, queue *queue1);
void pm_free(int virtualPageNum);

void* thread_get(Payload* payload);
void thread_put(Payload* payload);
void thread_free(Payload* payload);

void pageOut(queue *queue1);
void pageIn(int virtualPageNum, queue *queue1);

#endif //PRACTICUM1_CUSTOMHEAP_H
