#include "minunit.h"
#include "../include/heap_t.h"
#include <assert.h>

int int_cmp_asc(void* a, void* b) {
    return (int) a - (int) b;
}

char* test_create() {
	heap_t* heap = heap_create(10, sizeof(int), int_cmp_asc, NULL);
	mu_assert(heap != NULL, "Failed to create heap");
	heap_destroy(heap);
	return NULL;
}

char* test_heap_push() {
	heap_t* heap = heap_create(10, sizeof(int), int_cmp_asc, NULL);
	mu_assert(heap != NULL, "Failed to create heap");

    heap_push(heap, (void*) 5);
    heap_push(heap, (void*) 1);
    heap_push(heap, (void*) 2);
    heap_push(heap, (void*) 7);
    heap_push(heap, (void*) 20);
    
    mu_assert((int) heap_pop(heap) == 1, "Heap popped out of order");
    mu_assert((int) heap_pop(heap) == 2, "Heap popped out of order");
    mu_assert((int) heap_pop(heap) == 5, "Heap popped out of order");
    mu_assert((int) heap_pop(heap) == 7, "Heap popped out of order");
    mu_assert((int) heap_pop(heap) == 20, "Heap popped out of order");

    mu_assert(heap->items == 0, "Heap should have 0 items");

    heap_clear(heap);
    heap_destroy(heap);
    return NULL;
}

char* test_heap_pushpop() {
	heap_t* heap = heap_create(10, sizeof(int), int_cmp_asc, NULL);
	debug("Pushpop 4: %d", (int) heap_pushpop(heap, (void*) 4));
    heap_push(heap, (void*) 1);
    heap_push(heap, (void*) 2);
    heap_push(heap, (void*) 3);
    heap_push(heap, (void*) 4);
    heap_push(heap, (void*) 5);
    
    int n;

    for(int i = 1; i <= 5; i++) {
	    n = (int) heap_pushpop(heap, (void*) 5 + i);
	    mu_assert(n == i, "Incorrect value pushpopped off the heap");
    }

    for(int i = 6; i <= 10; i++) {
    	n = (int) heap_pop(heap);
    	mu_assert(n == i, "Incorrect value popped off the heap");
    }
    heap_destroy(heap);
    return NULL;
}

char* all_tests() {
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_heap_push);
	mu_run_test(test_heap_pushpop);
	return NULL;
}

RUN_TESTS(all_tests);