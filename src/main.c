//
//  main.c
//  cmsketch
//
//  Created by Chris Guiney on 4/16/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#include <stdio.h>
#include "../include/sketch_t.h"
#include "../include/dbg.h"
int int_cmp_asc(void* a, void* b) {
    return (int) a - (int) b;
}

int main(int argc, const char * argv[])
{

    printf("Starting\n");
    sketch_t *s = sketch_create(sizeof(int), 0.5, 0.5, 1.0, int_cmp_asc);
    
    if(s == NULL) {
        printf("Error creating sketch");
        return 1;
    }
    
    sketch_destroy(s);
    
    heap_t* heap = heap_create(10, sizeof(int), int_cmp_asc);
    

    
    debug("Pushpop 4: %d", (int) heap_pushpop(heap, (void*) 4));
    heap_push(heap, (void*) 1);
    heap_push(heap, (void*) 2);
    heap_push(heap, (void*) 3);
    heap_push(heap, (void*) 4);
    heap_push(heap, (void*) 5);
    
    debug("Pushpop 6: %d", (int) heap_pushpop(heap, (void*) 6));
    debug("Pushpop 7: %d", (int) heap_pushpop(heap, (void*) 7));
    debug("Pushpop 8: %d", (int) heap_pushpop(heap, (void*) 8));
    debug("Pushpop 9: %d", (int) heap_pushpop(heap, (void*) 9));
    debug("Pushpop 10: %d", (int) heap_pushpop(heap, (void*) 10));

    debug("pop 6: %d", (int) heap_pop(heap));
    debug("pop 7: %d", (int) heap_pop(heap));
    debug("pop 8: %d", (int) heap_pop(heap));
    debug("pop 9: %d", (int) heap_pop(heap));
    debug("pop 10: %d", (int) heap_pop(heap));    

    heap_clear(heap);
    heap_destroy(heap);

    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

