//
//  sketch_t.c
//  sketch_t
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#include <stdlib.h>
#include <math.h>
#include "../include/sketch_t.h"
#include "../include/heap_t.h"


sketch_t* sketch_create(size_t data_size, double delta, double epsilon, int k, comparator cmp)
{
	if(delta <= 0 || delta >= 1) {
		goto error;
	}
	
	if(epsilon <= 0 || epsilon >= 1) {
		goto error;
	}
	
	if(k < 1) {
		goto error;
	}

	sketch_t *s = calloc(1, sizeof(sketch_t));
	s->width = (int) ceil(exp(1.0) / epsilon);
	s->depth = (int) ceil(log(1 / delta));
	s->k = k;
	s->field = calloc(1, (sizeof(int) * s->width) * sizeof(int) * s->depth);
	s->heap = heap_create(k, data_size, cmp);
	return s;
	
	error:
		return NULL;
}

void sketch_destroy(sketch_t *s) {
	heap_destroy(s->heap);
	free(s->field);
	free(s);
}
