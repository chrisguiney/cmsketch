#include <stdlib.h>
#include "../include/llist_t.h"
#include "../include/dbg.h"

llist_t* llist_create(dtor destructor)
{
	llist_t* list = NULL;
	list = calloc(1, sizeof(llist_t));
	check_mem(list);

	list->destructor = destructor;
	list->size = 0;
	list->head = NULL;

	return list;

error:
	if(list != NULL) {
		list = NULL;
		free(list);
	}
	return NULL;
}

void llist_destroy(llist_t* list)
{
	if(list == NULL) {
		return;
	}

	llist_node_t* node = list->head;
	llist_node_t* next_node = NULL;

	while(node != NULL) {
		if(list->destructor != NULL) {
			list->destructor(node->value);
		}
		next_node = node->next;
		free(node);
		node = next_node;
	}
	free(list);
}

void* llist_node_destroy(llist_node_t* node)
{
	void* value = NULL;
	if(node != NULL) {
		value = node->value;
		free(node);
		return value;
	}
	return NULL;
}

void llist_push(llist_t* list, void* item)
{
	llist_node_t* node;
	
	node = calloc(1, sizeof(llist_node_t));
	check_mem(node);

	node->value = item;
	node->next = list->head;
	list->head = node;

	list->size += 1;
	return;

error:
	if(node != NULL) {
		free(node);
		node = NULL;
	}
	return;
}

void* llist_pop(llist_t* list)
{
	check(list->head != NULL, "List head is NULL");
	
	llist_node_t* head = list->head;
	list->head = head->next;
	void* value = llist_node_destroy(head);

	list->size -= 1;

	return value;

error:
	return NULL;
}

