//
//  heap_t.h
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#ifndef cmsketch_heap_t_h
#define cmsketch_heap_t_h

typedef int (*comparator) (void* item, void* item2);

typedef struct {
    comparator cmp;                                   // Comparator function
    int size;                                         // Max size
    int items;                                        // Items currently in heap
    size_t item_size;                                 // Size of item
    void **list;                                      // Array of items
} heap_t;

heap_t* heap_create(int size, size_t type_size, comparator cmp);
void heap_clear(heap_t* heap);
void heap_destroy(heap_t* heap);
void heap_push(heap_t* heap, void* item);
void* heap_pop(heap_t* heap);
void* heap_pushpop(heap_t* heap, void* item);
void heap_sort(heap_t* heap);
#endif
