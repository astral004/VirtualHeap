#include "customHeap.c"
#include "queue.c"

int main() {
    init_pmHeap();
    init_mutex();
    init_virtualPages();
    init_physicalPages();

    queue *myQ = newQueue();
    printf("Testing init. Expected ~. First character: %c\n", pm_heap[0]);
    //testing pm_malloc
    int vpn = pm_malloc(4, myQ);
    int myInt = 4;
    pm_put(vpn, &myInt, myQ);
    printf("Testing put. Expected: 4. Actual value: %d\n", pm_heap[0]);
    pm_free(vpn);
    printf("Testing free. Expected ~. Actual: %c\n", pm_heap[0]);
    int newVPN = pm_malloc(1, myQ);
    printf("newVPN = %d\n", newVPN);
    char myChar = 'a';
    pm_put(newVPN, &myChar, myQ);
    char* returnedLocation = pm_get(newVPN, myQ);
    printf("Testing get. Expected character a. Actual value: %c\n", *returnedLocation);

    /**
     * Tried to implement thread safety. Code keeps hanging for some reason. I was quite close.
     */

//    printf("testing threads\n");
//
//    myQ->isThreadSafe = 1;
//
//    int p1VPN = pm_malloc(4, myQ);
//    int newInt = 5;
//    Payload p1;
//    p1.virtualPageNum = p1VPN;
//    p1.q = myQ;
//    p1.bytes = &newInt;
//    int* iptr;
//
//    int p2VPN = pm_malloc(1, myQ);
//    char newChar = 'z';
//    Payload p2;
//    p2.virtualPageNum = p2VPN;
//    p2.q = myQ;
//    p2.bytes = &newChar;
//    char* cptr;
//
//    pthread_t pth1, pth2;
//
//
//    pthread_create(&pth1, NULL, &thread_put, &p1);
//    pthread_create(&pth2, NULL, &thread_put, &p2);
//

    //Testing pageOut and pageIn
    usedPhysicalPages = PHYSICAL_PAGES;
    //Will generate file0.txt
    int newInt = pm_malloc(4,myQ);
    //Resetting heap
    pm_heap[0] = '~';
    printf("resetting heap. first character = %c\n", pm_heap[0]);
    //Setting up a virtual page to run a pageIn test
    virtualPages[15].size = 1;
    virtualPages[15].onDisk = 1;
    virtualPages[15].isAvailable = 0;
    pageIn(15, myQ);
    printf("Testing page in. Expected character x. Actual value: %c\n", pm_heap[0]);

    printf("End of program.\n");
}
