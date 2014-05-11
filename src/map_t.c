//
//  map_t.c
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#include "../include/adt.h"
#include "../include/llist_t.h"
#include "../include/map_t.h"
#include "../include/hash.h"
#include "../include/dbg.h"
#include <stdlib.h>
#include <stdint.h>

#define DEFAULT_BUCKET_COUNT 1
#define DEFAULT_HASHER djb2

void map_entry_destroy(map_entry_t* entry)
{
	if(entry != NULL) {
		free(entry);
	}
}

void map_bucket_destroy(map_t* map, llist_t* bucket)
{
	llist_node_t* node = NULL;
	map_entry_t* entry = NULL;
	node = bucket->head;
	while(node != NULL) {
		entry = (map_entry_t*) node->value;
		if(map->destructor != NULL) {
			map->destructor(entry->value);
		}
		map_entry_destroy(entry);
		node->value = NULL;
		node = node->next;
	}
	llist_destroy(bucket);
}

void map_destroy(map_t* map)
{
	int i;
	if(map->buckets != NULL) {
		for(i = 0; i < map->num_buckets; i++) {
			map_bucket_destroy(map, map->buckets[i]);
		}
		free(map->buckets);
	}
	free(map);
}

map_t* map_create(float load_factor,
		  size_t key_size,
		  int initial_bucket_count,
		  comparator cmp,
		  hasher hash,
		  dtor destructor)
{

	if(initial_bucket_count == NULL) {
		initial_bucket_count = DEFAULT_BUCKET_COUNT;
	}

	map_t* map = NULL;

	map = malloc(sizeof(map_t));
	map->num_buckets = initial_bucket_count;
	map->num_entries = 0;
	map->load_factor = load_factor;
	map->key_size = key_size;
	map->cmp = cmp;
	map->hash = hash;
	map->destructor = destructor;
	check_mem(map);

	map->buckets = calloc(initial_bucket_count, sizeof(llist_t));
	check_mem(map->buckets);

	for(int i = 0; i < initial_bucket_count; i++) {
		map->buckets[i] = llist_create(NULL);
	}

	return map;

error:
	if(map != NULL) {
		if(map->buckets != NULL) {
			int i;
			for(i = 0; i < initial_bucket_count; i++) {
				if(map->buckets[i] != NULL) {
					llist_destroy(map->buckets[i]);
				}
			} 
			free(map->buckets);
		}
		free(map);
	}
	return NULL;
}

map_t* map_create_simple(float load_factor, size_t key_size, comparator cmp, dtor destructor)
{
	return map_create(load_factor,
			  key_size,
			  DEFAULT_BUCKET_COUNT,
			  cmp,
			  DEFAULT_HASHER,
			  destructor);
}


map_entry_t* map_create_entry(uint32_t hash, void* key, void* value)
{
	map_entry_t* entry = calloc(1, sizeof(map_entry_t));
	check_mem(entry);

	entry->hash = hash;
	entry->key = key;
	entry->value = value;

	return entry;

error:
	if(entry != NULL) {
		free(entry);
	}
	return NULL;
}



llist_t* map_get_bucket(map_t* map, void* key, uint32_t hash)
{
	check(map, "Map was null");
	check(map->buckets, "buckets was not properly assigned");

	int bucket_number = hash % map->num_buckets;
	check(bucket_number >= 0, "Bucket out of range");
	check(map->buckets[bucket_number], "Requested bucket is NULL");

	return map->buckets[bucket_number];

error:
	return NULL;
}

map_entry_t* map_find_entry(
	map_t* map,
	llist_t* bucket,
	void* key,
	uint32_t hash)
{
	llist_node_t* node = bucket->head;
	map_entry_t* entry = NULL;
	while(node != NULL) {
		entry = (map_entry_t*) node->value;
		if(entry->hash == hash &&
			map->cmp(entry->key, key) == 0) {
			return entry;
		}
		node = node->next;
	}

	log_err("Could not find entry");
	return NULL;
}

void map_put(map_t* map, void* key, void* value)
{

	uint32_t hash = map->hash(key, map->key_size);
	llist_t* bucket = map_get_bucket(map, key, hash);
	check(bucket, "Error obtaining hash bucket");

	map_entry_t* entry = map_create_entry(hash, key, value);
	check_mem(entry);

	llist_push(bucket, (void*) entry);

	map->num_entries += 1;

	return;
error:
	return;
}

void* map_get(map_t* map, void* key)
{
	uint32_t hash = map->hash(key, map->key_size);
	llist_t* bucket = map_get_bucket(map, key, hash);

	check(bucket, "Error obtaining hash bucket");

	if(bucket == NULL) {
		log_err("Bucket is null");
		return NULL;
	}

	map_entry_t* entry = map_find_entry(map, bucket, key, hash);

	if(entry == NULL) {
		return NULL;
	}

	return entry;
error:
	return NULL;
}

void* map_delete(map_t* map, void* key)
{
	uint32_t hash = map->hash(key, map->key_size);
	
	llist_t* bucket = map_get_bucket(map, key, hash);

	if(bucket == NULL) {
		log_err("Bucket is null");
		return NULL;
	}

	if(bucket->head == NULL) {
		log_err("Bucket head is null");
		log_err("Bucket size: %d", bucket->size);
		return NULL;
	}


	llist_node_t* node = bucket->head;
	llist_node_t** prev = &bucket->head;

	map_entry_t* entry = NULL;

	while(node != NULL) {
		entry = (map_entry_t*) node->value;
		if(entry->hash == hash &&
			map->cmp(entry->key, key) == 0) {

			void* data = entry->value;
			
			(*prev) = node->next;
			bucket->size--;
			
			llist_node_destroy(node);		
			map_entry_destroy(entry);
			return data;
		}
		prev = &(*prev)->next;
		node = node->next;
	}
	return NULL;
}

