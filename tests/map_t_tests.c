#include "minunit.h"
#include "../include/map_t.h"
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

bool dtor_cb_check = false;

uint32_t djb2_test(void* key, size_t size)
{
	uint32_t hash = 5381;
	const char* ckey = key;

	for(size_t i = 0; i < size; i++) {
		hash = ((hash << 5) + hash) + ckey[i];
	}

	return hash;
}

void dtor_cb(void* item) {
	dtor_cb_check = true;
}

int int_cmp_asc(void* a, void* b) {
    return (int) a - (int) b;
}

int str_cmp_asc(void* a, void* b) {
	//log_err("In comparator");
	if(!a && !b) {
		return -1;
	}
	//log_err("Attempting cmp on %s and %s", a, b);
	int cmp_result = strcmp((char*) a, (char*) b);
	//log_err("Cmp result: %d", cmp_result);
	return cmp_result;
}

char* test_create() {
	map_t* map = map_create_simple(0.75, sizeof(int), int_cmp_asc, NULL);

	mu_assert(map != NULL, "Map is NULL");

	map_destroy(map);

	return NULL;
}

char* test_default_hasher() {
	map_t* map = map_create_simple(0.75, sizeof(int), int_cmp_asc, NULL);

	int val_one = 1;
	int val_two = 1;
	uint32_t h1 = map->hash(&val_one, sizeof(int));
	uint32_t h2 = map->hash(&val_two, sizeof(int));
	mu_assert(h1 == h2, "ints: Hash one and hash two were not equal");

	val_one = 1;
	val_two = 2;
	h1 = map->hash(&val_one, sizeof(int));
	h2 = map->hash(&val_two, sizeof(int));	
	
	mu_assert(h1 != h2, "ints: Hash one and hash two were equal");

	{
		char str1[] = "FOO";
		char str2[] = "FOO";
		h1 = map->hash(str1, strlen(str1));
		h2 = map->hash(str2, strlen(str2));
		mu_assert(h1 == h2, "strings: Hash one and hash two were not equal");
	} {
		char str1[] = "foobaz";
		char str2[] = "foobar";
		h1 = map->hash(str1, strlen(str1));
		h2 = map->hash(str2, strlen(str2));
		mu_assert(h1 != h2, "strings: Hash one and hash two were equal");
	}
	map_destroy(map);
	return NULL;
}

char* test_set_get() {
	map_t* map = map_create_simple(0.75, sizeof(int), int_cmp_asc, NULL);
	int key = 1234;
	char value[] = "Foo";

	int key2 = 5678;
	char value2[] = "bar123";

	map_put(map, (void*) &key, (void*) value);
	map_put(map, (void*) &key2, (void*) value2);

	char* out_value = (char*) map_get(map, &key);

	debug("Out value: %s", out_value);

	map_destroy(map);
	return NULL;
}

char* test_delete()
{
	char key[] = "foo";
	char key2[] = "bar";
	char key3[] = "baz";
	int val = 10;
	int val2 = 20;
	int val3 = 30;
	map_t* map = map_create(0.75, sizeof(char*), 1 ,str_cmp_asc, djb2_test, dtor_cb);

	map_put(map, key, &val);
	map_put(map, key3, &val3);
	map_put(map, key2, &val2);

	int* retval = (int*) map_delete(map, key);
	mu_assert(*retval == val, "Return val does not match initial value");


	void* should_be_null =  map_get(map, key);

	log_err("Performing delete 2");
	int* retval2 = (int*) map_delete(map, key2);
	log_err("Performing delete 3");
	int* retval3 = (int*) map_delete(map, key3);

	mu_assert(*retval3 == val3, "Return val 3 does not match initial value 3");

	mu_assert(*retval2 == val2, "Return val 2 does not match initial value 2");

	mu_assert(should_be_null == NULL, "Item was not successfully deleted from map");
	
	map_destroy(map);

	return NULL;
}


char* all_tests() {
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_default_hasher);
	mu_run_test(test_set_get);
	mu_run_test(test_delete);
	return NULL;
}

RUN_TESTS(all_tests);