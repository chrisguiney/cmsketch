#include "minunit.h"
#include "../include/llist_t.h"
#include <assert.h>
#include <stdint.h>

char* test_create() {
	llist_t* list = llist_create(NULL);
	log_err("list address: %p", list);
	mu_assert(list != NULL, "Failed to create list");
	mu_assert(list->size == 0, "Initial size of list is not zero");
	llist_destroy(list);
	return NULL;
}

char* test_push() {
	uint32_t val = 5;
	llist_t* list = llist_create(NULL);
	llist_push(list, (void*) &val);

	mu_assert(list->size == 1, "List size did not increment");
	mu_assert(list->head != NULL, "List head did not get properly assigned");

	uint32_t* out_value = (uint32_t*) list->head->value;

	mu_assert(val == *out_value, "Stored wrong value in list node");
	llist_destroy(list);
	return NULL;
}

char* test_pop() {
	llist_t* list = llist_create(NULL);
	uintptr_t i = 0;

	for(i = 0; i < 10; i++) {
		llist_push(list, (void*) i);
	}

	mu_assert(list->size == 10, "List size was not 10");

	uintptr_t j = 0;
	for(uintptr_t i = 0; i < 10; i++) {
		debug("Popping %lu", i);
		j = (uintptr_t) llist_pop(list);
		debug("Popped %lu", j);
		mu_assert(j == (9 - i), "List item came out in wrong order");
	}
	mu_assert(list->size == 0, "List size was not 0");
	llist_destroy(list);
	return NULL;
}

char* test_destroy() {
	llist_t* list = llist_create(NULL);
	uintptr_t i = 0;

	uintptr_t val = 5;

	llist_push(list, (void*) val);

	for(i = 0; i < 10; i++) {
		llist_push(list, (void*) i);
	}

	llist_destroy(list);

	debug("Value now: %lu", val);
	return NULL;
}

char* all_tests() {
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_push);
	mu_run_test(test_pop);
	mu_run_test(test_destroy);
	return NULL;
}

RUN_TESTS(all_tests);