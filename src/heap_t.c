//
//  heap_t.c
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#include <stdlib.h>
#include <stdbool.h>
#include "../include/heap_t.h"
#include "../include/dbg.h"

/*
 * create a new heap...on the heap
 */

int local_int_cmp_asc(void* a, void* b) {
    return (int) a - (int) b;
}


void heap_swap(heap_t* heap, int a, int b) {
    check(a <= heap->size, "Heap position out of range");
    check(b <= heap->size, "Heap position out of range");
    void* tmp = heap->list[a];
    heap->list[a] = heap->list[b];
    heap->list[b] = tmp;

    error:
        return;
}

/*
 * Create a new fixed size heap.
 */
heap_t* heap_create(int size, size_t type_size, comparator cmp) {
    heap_t* h = NULL;
    
    check(size > 0, "Heap size must be a positive integer");
    
    h = malloc(sizeof(heap_t));

    check_mem(h);

    h->cmp = cmp;
    h->items = 0;
    h->size = size;
    h->item_size = type_size;
    h->list = calloc(1, type_size * size);

    check_mem(h->list);
    return h;

    error:
        if(h) {
            if(h->list) {
                free(h->list);
            }
            free(h);
        }
        return NULL;
}

/*
 * Decallocate every item in the heap.
 */
void heap_clear(heap_t* heap) {
    int i = 0;
    if(heap->items > 0) {
        for(i = 0; i < heap->size; i++) {
            if(heap->list[i] != NULL) {
                free(heap->list[i]);
            }
        }
    }
}

/*
 * Deallocate a heap heap
 */
void heap_destroy(heap_t* heap){
    if(heap) {
        if(heap->list) {
            free(heap->list);
        }
        free(heap);
    }
}

/*
 * Return the index of the items parent
 */
int heap_parent(const heap_t* heap, const int pos) {
    check(pos >= 0, "Heap position must be greater than 0");
    check(pos < heap->items, "Heap position is out of range");
    return (pos - 1) / 2;
    
    error:
        return -1;
}

/*
 * Determine if the element at pos is a leaf node
 */
bool heap_leaf(heap_t* heap, int pos) {
    return (pos >= heap->items / 2) && (pos < heap->items);
}

/*
 * Get the index of the left child of element at pos
 */
int heap_left(heap_t* heap, int pos) {
    check(pos < heap->items, "Heap position is out of range");
    return (2 * pos) + 1;
    
    error:
        return -1;
}

/*
 * Get index of the right child of element at pos
 */
int heap_right(heap_t* heap, int pos) {
    check(pos < heap->items, "Heap position is out of range");
    return (2 * pos) + 2;
    
    error:
        return -1;
}

/*
 * Move element at pos to its correct position within the heap
 */
void heap_sift(heap_t* heap, int pos) {
    int j;
    while(!heap_leaf(heap, pos)) {
        j = heap_left(heap, pos);
        if((j < heap->items - 1) &&
           (heap->cmp(heap->list[j], heap->list[j+1]) > 0)) {
            j++;
        }
        if(heap->cmp(heap->list[pos], heap->list[j]) <= 0) {
            return;
        }
        heap_swap(heap, pos, j);
        pos = j;
    }
}

/*
 * Push an item on to the heap, maintain proper ordering
 */
void heap_push(heap_t* heap, void* item) {

    // If the heap is full, compare item to the 0th item in the heap.
    if(heap->items == heap->size - 1) {
        if(heap->cmp(item, heap->list[0])) {
            heap_pop(heap);
        }
    }
    
    int current = heap->items++;
    int parent = heap_parent(heap, current);

    heap->list[current] = item;
    
    while(current != 0 &&
          heap->cmp(heap->list[current], heap->list[parent]) < 0) {
        heap_swap(heap, current, parent);
        current = parent;
    }
}

/*
 * Pop the smallest item off the top of the queue
 */
void* heap_pop(heap_t* heap) {
    check(heap->items, "Performing pop operation on empty heap");
    
    //Decement items, Put the smallest item at the end of the list
    heap_swap(heap, 0, --heap->items);
    
    if(heap->items != 0) {
        heap_sift(heap, 0);
    }
   
    return heap->list[heap->items];
    
    error:
        return NULL;
}

/*
 * Pop the item at the specified position in the heap.
 */
void* heap_pop_at(heap_t* heap, int pos) {
    check(heap->items >= pos, "Heap does not contain an element at %d", pos);
    
    if(pos == heap->items -1) {
        heap->items--;
    }
    else {
        heap_swap(heap, pos, --heap->items);
        while (pos > 0 &&
               heap->cmp(heap->list[pos], heap->list[heap_parent(heap, pos)])) {
            heap_swap(heap, pos, heap_parent(heap, pos));
        }
    }
    
    return heap->list[heap->items];
    
    error:
        return NULL;
}

void* heap_pushpop(heap_t* heap, void* item) {
    // If the heap is empty, we can just return item
    if(heap->items == 0) {
        return item;
    }
    
    
    int cmp = heap->cmp(item, heap->list[0]);
    
    if(cmp < 0) {
        return NULL;
    }
    
    if(cmp == 0) {
        return item;
    }
    
    void* min = heap->list[0];
    
    heap->list[0] = item;
    heap_sift(heap, 0);

    return min;
}