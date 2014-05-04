//
//  llist_t.h
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//
#ifndef cmsketch_llist_t_h
#define cmsketch_llist_t_h

#include "adt.h"

typedef struct llist_node_t {
	struct llist_node_t*	next;
	void*			value;
} llist_node_t;

typedef struct {
	int		size;
	dtor		destructor;
	llist_node_t*	head;
} llist_t;

llist_t* llist_create(dtor destructor);
void llist_destroy(llist_t* list);
void* llist_node_destroy(llist_node_t* node);
void llist_push(llist_t* list, void* item);
void* llist_pop(llist_t* list);

#endif