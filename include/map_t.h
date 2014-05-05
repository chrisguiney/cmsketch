//
//  map_t.h
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#ifndef cmsketch_map_t_h
#define cmsketch_map_t_h
#include "adt.h"
#include "llist_t.h"

typedef struct {
	int		num_buckets;
	int		num_entries;
	float		load_factor;
	size_t		key_size;
	hasher		hash;
	comparator	cmp;
	dtor		destructor;
	llist_t**	buckets;
} map_t;

typedef struct {
	uint32_t	hash;
	void*		key;
	void*		value;
} map_entry_t;

typedef struct {
	size_t	size;
	void*	value;
} map_key_t;

map_t* map_create(float load_factor, size_t key_size, int initial_bucket_count, comparator cmp, hasher hash, dtor destructor);
map_t* map_create_simple(float load_factor, size_t key_size, comparator cmp, dtor destructor);
void map_put(map_t* map, void* key, void* value);
void* map_get(map_t* map, void* key);
void* map_delete(map_t* map, void* key);
void map_destroy(map_t* map);

#endif