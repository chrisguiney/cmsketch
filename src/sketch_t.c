//
//  sketch_t.c
//  sketch_t
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../include/sketch_t.h"
#include "../include/heap_t.h"
#include "../include/map_t.h"
#include "../include/hash.h"
#include "../include/dbg.h"

#define min(x,y) y ^ ((x ^ y) & -(x < y)); 

typedef struct {
	size_t len;
	char* str;
} sketch_str;

typedef struct {
	long count;
	sketch_str* str;
} sketch_item;

int sketch_cmp(void* a, void* b)
{
	return ((sketch_item*) a)->count - ((sketch_item*) b)->count;
}

sketch_t* sketch_create(double delta, double epsilon, int k)
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

	sketch_t* s = calloc(1, sizeof(sketch_t));
	s->width = (int) ceil(exp(1.0) / epsilon);
	s->depth = (int) ceil(log(1 / delta));
	s->k = k;
	s->hashes = calloc(2 * s->depth, sizeof(long));
	s->field = calloc(1, (sizeof(long) * s->width) * (sizeof(long) * s->depth));
	s->heap = heap_create(k, sizeof(sketch_item), sketch_cmp, NULL);
	s->top_k = map_create_simple(0.75, sizeof(sketch_str), sketch_cmp, NULL);

	check_mem(s->hashes);
	check_mem(s->field);
	check_mem(s->heap);
	check_mem(s->top_k);

	int i = 0;
	srandom(time(NULL));
	for(i = 0; i < s->depth * 2; i++) {
		s->field[i] = random();
	}

	return s;

error:
	return NULL;
}

sketch_item* sketch_item_create(sketch_str* s, long count)
{
	sketch_item* si = calloc(1, sizeof(sketch_item));
	check_mem(si);

	si->str = s;
	si->count = count;
error:
	return NULL;
}

void sketch_destroy(sketch_t *s)
{
	heap_destroy(s->heap);
	map_destroy(s->top_k);
	free(s->field);
	free(s);
}


long sketch_get(sketch_t* s, const sketch_str* ss)
{
	int i, j;
	long k;
	long v = LONG_MAX;
	for(i = 0; i < 2 * s->width; i++) {
		k = (s->hashes[i] * djb2(ss->str, ss->len)) + s->hashes[i+1] % LONG_MAX % s->width;
		v = min(s->field[i * k], v)
	}

	return v;
}

void sketch_update_heap(sketch_t* s, sketch_str* ss)
{
	long est = sketch_get(s, ss);

	if(s->heap->items != 0 || est < ((sketch_item*) s->heap->list[0])->count) {
		return;
	}

	sketch_item* si = map_get(s->top_k, ss);

	if(si != NULL) {
		si->count = est;
		return;
	} 

	si = sketch_item_create(ss, est);

	if(s->top_k->num_entries < s->k) {
		heap_push(s->heap, si);
	} else {
		sketch_item* old_si = heap_pushpop(s->heap, si);
		map_delete(s->top_k, (void*) (old_si->str));
		map_put(s->top_k, (void*) ss->str, (void*) si);
	}


}

void sketch_update(sketch_t* s, sketch_str* ss, long inc)
{
	int i, j;
	long k;

	for(i = 0; i < 2 * s->width; i++) {
		k = (s->hashes[i] * djb2(ss->str, ss->len)) + s->hashes[i+1] % LONG_MAX % s->width;
		s->field[i * k] += inc;
	}

	sketch_update_heap(s, ss);
}


