#include <stdlib.h>
#include <stdint.h>
/*
 * DJB2 function, created by Daniel J. Bernstien -- this particular implementation aquired here:
 * http://www.cse.yorku.ca/~oz/hash.html
 * To my knowledge, vulerable to collisions.
 * Adapted to use a for loop, and use the key_t type
 */
uint32_t djb2(void* key, size_t size)
{
	uint32_t hash = 5381;
	const char* ckey = key;

	for(size_t i = 0; i < size; i++) {
		hash = ((hash << 5) + hash) + ckey[i];
	}

	return hash;
}
