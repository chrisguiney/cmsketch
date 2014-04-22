//
//  llist_t.h
//  cmsketch
//
//  Created by Chris Guiney on 4/18/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//
#ifndef cmsketch_llist_t_h
#define cmsketch_llist_t_h
typedef struct llist_node_t {
	void* value;
	struct llist_node_t* next;
} llist_node_t;

typedef struct {
	int size;
	llist_node_t* head;
} llist_t;

llist_t* llist_create();
void llist_destroy(llist_t* list);
void llist_push(llist_t* list, void* item);
void* llist_pop(llist_t* list);

#endif