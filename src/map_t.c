//
//  map_t.c
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#include "../include/adt.h"
#include "../include/llist_t.h"
#include "../include/dbg.h"
#include <stdlib.h>
#include <stdint.h>

#define DEFAULT_BUCKET_COUNT 8

typedef struct {
	float load_factor;
	int num_buckets;
	hasher hash;
	comparator cmp;
	llist_t** buckets;
} map_t;

typedef struct {
	void *key;
	void *value;
	uint32_t hash;
} map_entry_t;

map_t* map_create(float load_factor, comparator cmp) {
	map_t* map = NULL;

	map = malloc(sizeof(map_t));
	map->load_factor = load_factor;
	map->cmp = cmp;
	check_mem(map);

	map->buckets = malloc(sizeof(llist_t) * DEFAULT_BUCKET_COUNT);
	check_mem(map->buckets);

	for(int i = 0; i < DEFAULT_BUCKET_COUNT; i++) {
		map->buckets[i] = llist_create();
	}

	error:
		if(map) {
			if(map->buckets) {
				for(int i = 0; i < DEFAULT_BUCKET_COUNT; i++) {

				}
			}

		}
		return NULL;
}


void map_put(map_t* map, void* key, void* value) {
	// TODO: implement
}

void* map_get(map_t* map, void* key) {
	// TODO: implement
	return NULL;
}