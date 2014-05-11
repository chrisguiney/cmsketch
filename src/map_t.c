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
	map->num_buckets = DEFAULT_BUCKET_COUNT;
	map->num_entries = 0;
	map->load_factor = load_factor;
	map->key_size = key_size;
	map->cmp = cmp;
	map->hash = hash;
	map->destructor = destructor;
	check_mem(map);

	map->buckets = calloc(DEFAULT_BUCKET_COUNT, sizeof(llist_t));
	check_mem(map->buckets);

	for(int i = 0; i < DEFAULT_BUCKET_COUNT; i++) {
		map->buckets[i] = llist_create(map->destructor);
	}

	return map;

error:
	if(map != NULL) {
		if(map->buckets != NULL) {
			for(int i = 0; i < DEFAULT_BUCKET_COUNT; i++) {
				llist_destroy(map->buckets[i]);
			}
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
	map_entry_t* entry = malloc(sizeof(map_entry_t));
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

void map_entry_destroy(map_entry_t* entry) {
	if(entry != NULL) {
		free(entry);
	}
}

llist_t* map_get_bucket(map_t* map, void* key, uint32_t hash)
{
	int bucket_number = hash % map->num_buckets;
	check(bucket_number >= 0, "Bucket out of range");

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
	log_err("Address of bucket->head: %p", bucket->head);
	while(node != NULL) {
		log_err("iterating on  node");
		entry = (map_entry_t*) node->value;
		log_err("attempting comparison");
		if(entry->hash == hash &&
			map->cmp(entry->key, key) == 0) {
			log_err("Found entry");
			return entry;
		}
		log_err("Moving from %p to next, %p", node, node->next);
		node = node->next;
	}

	log_err("Could not find entry");
	return NULL;
}

void map_put(map_t* map, void* key, void* value)
{
	uint64_t hash = map->hash(key, map->key_size);

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

	if(bucket == NULL) {
		log_err("Bucket is null");
		return NULL;
	}

	log_err("Bucket is not null");
	map_entry_t* entry = map_find_entry(map, bucket, key, hash);

	if(entry == NULL) {
		return NULL;
	}

	return entry;
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
		log_err("no match, continuing");
		prev = &(*prev)->next;
		node = node->next;
	}
	return NULL;
}