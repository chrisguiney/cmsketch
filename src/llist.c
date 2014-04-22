#include <stdlib.h>
#include "../include/llist_t.h"
#include "../include/dbg.h"

llist_t* llist_create() {
	llist_t* list;

	list = malloc(sizeof(llist_t));
	check_mem(list);

	list->size = 0;
	list->head = NULL;

	return list;

	error:
		return NULL;
}

void llist_destroy(llist_t* list) {
	if(list == NULL || list->head) {
		return;
	}

	llist_node_t* node = list->head;
	llist_node_t* next_node = NULL;

	while(node != NULL) {
		next_node = node->next;
		free(node->value);
		free(node);
		node = next_node;
	}
	free(list);
}

void llist_push(llist_t* list, void* item) {
	llist_node_t* node;
	
	node = calloc(1, sizeof(llist_node_t));

	check_mem(node);

	node->value = item;
	node->next = list->head;
	list->head = node;

	list->size += 1;
	return;

	error:
		if(node) {
			free(node);
		}
		return;
}

void* llist_pop(llist_t* list) {
	check(list->head != NULL, "List head is NULL");
	
	llist_node_t* head = list->head;

	void* value = head->value;
	list->head = head->next;
	list->size -= 1;

	free(head);

	return value;

	error:
		return NULL;
}

