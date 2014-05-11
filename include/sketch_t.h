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
	long*	hashes; // Array of random integers to use for hash seeds
	long*	field;	// Int field -- width * depth length
	heap_t*	heap;	// Heap for sorting
	map_t* top_k;	// Dictionary of top K entries
} sketch_t;

sketch_t* sketch_create(double delta, double epsilon, int k);
void sketch_destroy(sketch_t *s);

#endif
