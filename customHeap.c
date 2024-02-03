//
// Created by Kashyap Tare on 3/24/23.
//
#include "customHeap.h"

void init_pmHeap(){
    for (int  i = 0; i < PHYSICAL_PAGES*PAGESIZE; i+=PAGESIZE){
        pm_heap[i] = '~';
    }
}

void init_mutex() { pthread_mutex_init(&mutex, NULL); }

void init_virtualPages(){
    for (int i = 0; i < 1024; ++i) {
        virtualPages[i].virtualPageNum = i;
        virtualPages[i].onDisk = 0;
        virtualPages[i].isAvailable = 1;
        virtualPages[i].isChanged = 0;
        virtualPages[i].size = -1;
        virtualPages[i].physPageNumber = -1;
    }
}

void init_physicalPages(){
    for (int i = 0; i < PHYSICAL_PAGES; ++i) {
        physicalPages[i].isOccupied = 0;
        physicalPages[i].virtualPageNum = -1;
    }
}

int pm_malloc(int sizeInBytes, queue *queue1){
    if(sizeInBytes > PAGESIZE){
        printf("Memory not available!");
        return -1;
    }
    if (usedVirtualPages == VIRTUAL_PAGES){
        printf("Memory not available!");
        return -1;
    }
    int currentVirtualPage = 0;
    int currentPhysicalPage = 0;
    for (int i = 0; i < VIRTUAL_PAGES; ++i) {
        if (virtualPages[i].isAvailable == 1){
            currentVirtualPage = i;
            break;
        }
    }
    usedVirtualPages++;
    if(usedPhysicalPages == PHYSICAL_PAGES){
        pageOut(queue1);
        usedPhysicalPages--;
    }
    for (int i = 0; i < PHYSICAL_PAGES; ++i) {
        if(physicalPages[i].isOccupied == 0){
            currentPhysicalPage = i;
            break;
        }
    }
    usedPhysicalPages++;
    for (int i = 0; i < PAGESIZE * PHYSICAL_PAGES; i+=PAGESIZE) {
        if(pm_heap[i] == '~'){
            physicalPages[currentPhysicalPage].physAddress = &(pm_heap[i]);
            break;
        }
    }
    physicalPages[currentPhysicalPage].virtualPageNum = currentVirtualPage;
    physicalPages[currentPhysicalPage].isOccupied = 1;
    virtualPages[currentVirtualPage].size = sizeInBytes;
    virtualPages[currentVirtualPage].isAvailable = 0;
    virtualPages[currentVirtualPage].physPageNumber = currentPhysicalPage;
    enqueue(queue1, currentVirtualPage);
    return virtualPages[currentVirtualPage].virtualPageNum;
}

void* pm_get(int virtualPageNum, queue *queue1){
    if(virtualPages[virtualPageNum].onDisk == 1){
        if(usedPhysicalPages == PHYSICAL_PAGES){
            pageOut(queue1);
        }
        pageIn(virtualPageNum, queue1);
    }
    int physicalPageNum = virtualPages[virtualPageNum].physPageNumber;
    enqueue(queue1, virtualPageNum);
    return physicalPages[physicalPageNum].physAddress;
}

void pm_put(int virtualPageNum, void *bytes, queue *queue1){
    char* pmHeapLocation = pm_get(virtualPageNum, queue1);
    *pmHeapLocation = *((char *) bytes);
    virtualPages[virtualPageNum].isChanged = 1;
}

void pm_free(int virtualPageNum){
    int currentPhysicalPage = virtualPages[virtualPageNum].physPageNumber;
    char* heapLocation = physicalPages[currentPhysicalPage].physAddress;
    *heapLocation = '~';

    virtualPages[virtualPageNum].onDisk = 0;
    virtualPages[virtualPageNum].isAvailable = 1;
    virtualPages[virtualPageNum].isChanged = 0;
    virtualPages[virtualPageNum].size = -1;
    virtualPages[virtualPageNum].physPageNumber = -1;

    physicalPages[currentPhysicalPage].isOccupied = 0;
    physicalPages[currentPhysicalPage].virtualPageNum = -1;
}

void pageOut(queue *queue1){
    int virtualPageNum = dequeue(queue1);
    int physicalPageNum = virtualPages[virtualPageNum].physPageNumber;
    virtualPages[virtualPageNum].onDisk = 1;
    virtualPages[virtualPageNum].physPageNumber = -1;
    physicalPages[physicalPageNum].isOccupied = 0;
    physicalPages[physicalPageNum].virtualPageNum = -1;
    if(virtualPages[virtualPageNum].isChanged){
        char filename[13];
        snprintf(filename, 13, "file%d.txt", virtualPageNum);
        FILE *file = fopen(filename, "w");
        for (int i = 0; i < (virtualPages[virtualPageNum].size); ++i) {
            fputc((*((char *)(physicalPages[physicalPageNum].physAddress)))+i , file);
        }
    }
    usedPhysicalPages--;
    char* charAddress = physicalPages[physicalPageNum].physAddress;
    *charAddress ='~';
}

void pageIn(int virtualPageNum, queue *queue1){
    int currentPhysicalPage = 0;
    for (int i = 0; i < PHYSICAL_PAGES; ++i) {
        if(physicalPages[i].isOccupied == 0){
            currentPhysicalPage = i;
            break;
        }
    }
    usedPhysicalPages++;
    int currentHeapLocation = -1;
    for (int i = 0; i < PAGESIZE * PHYSICAL_PAGES; i+=PAGESIZE) {
        if(pm_heap[i] == '~'){
            physicalPages[currentPhysicalPage].physAddress = &(pm_heap[i]);
            currentHeapLocation = i;
            break;
        }
    }
    physicalPages[currentPhysicalPage].isOccupied = 1;
    physicalPages[currentPhysicalPage].virtualPageNum = virtualPageNum;
    char filename[13];
    snprintf(filename, 13, "file%d.txt", virtualPageNum);
    FILE *file = fopen(filename, "r");
    for (int i = 0; i < (virtualPages[virtualPageNum].size); ++i) {
        pm_heap[currentHeapLocation+i] = (char)(fgetc(file));
    }
    virtualPages[virtualPageNum].physPageNumber = currentPhysicalPage;
    virtualPages[virtualPageNum].onDisk = 0;
}

void* thread_get(Payload* payload){
    pthread_mutex_lock(&mutex);
    void* myPtr = pm_get(payload->virtualPageNum, payload->q);
    pthread_mutex_unlock(&mutex);
    return myPtr;
}

void thread_put(Payload* payload){
    pthread_mutex_lock(&mutex);
    pm_put(payload->virtualPageNum, payload->bytes, payload->q);
    pthread_mutex_unlock(&mutex);
}
void thread_free(Payload* payload){
    pthread_mutex_lock(&mutex);
    pm_free(payload->virtualPageNum);
    pthread_mutex_unlock(&mutex);
}





