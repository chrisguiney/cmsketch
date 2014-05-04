//
//  cmsketch.h
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//
#include "heap_t.h"
#ifndef cmsketch_sketch_t_h
#define cmsketch_sketch_t_h

typedef struct {
	int	k;	// Number of top-k items to keep track of
	int	width;	// Width of field
	int	depth;	// Depth of field
	int	*field;	// Int field -- width * depth length
	heap_t	*heap;	// Heap for sorting
} sketch_t;

sketch_t* sketch_create(size_t data_size, double delta, double epsilon, int k, comparator cmp);
void sketch_destroy(sketch_t *s);

#endif
