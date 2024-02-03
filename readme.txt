To run the program just run 'make'.

You might need to play around with the file path provided for the pageOut and pageIn functions on lines 117, 118, 149,
and 150 in customHeap.c.

HOW IT WORKS:

The main functions are pm_malloc, pm_put, pm_get, and pm_free. pmMalloc will take in a size and a queue and  will
return a virtual page number(vp#). You use the vp# in tandem with a queue to call pm_get which will actually return a
pointer in the heap. Once you have that pointer, you can use it with the vp# and queue to set the value at that memory
location to one of your choosing. Once you're all done, you can call pm_free with the vp# of your choosing to release
deallocate memeory.

Note**
pageIn and pageOut are never used by the programmer. For the sake of demonstrating that they work, we are mocking
scenarios that would trigger them. All page replacement is managed within the heap.