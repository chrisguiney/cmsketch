#ifndef cmsketch_hash_h
#define cmsketch_hash_h
#include <stdint.h>
#include "heap_t.h"
uint32_t djb2(void* key, size_t size);

#endif