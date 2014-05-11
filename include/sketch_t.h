//
//  cmsketch.h
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//
#ifndef cmsketch_sketch_t_h
#define cmsketch_sketch_t_h
#define LONG_MAX 9223372036854775783

#include "heap_t.h"
#include "map_t.h"

typedef struct {
	int	k;	// Number of top-k items to keep track of
	int	width;	// Width of field
	int	depth;	// Depth of field
	int*	hashes;
	int*	field;	// Int field -- width * depth length
	heap_t*	heap;	// Heap for sorting
	map_t* top_k;
} sketch_t;

sketch_t* sketch_create(size_t data_size, double delta, double epsilon, int k, comparator cmp);
void sketch_destroy(sketch_t *s);

#endif
