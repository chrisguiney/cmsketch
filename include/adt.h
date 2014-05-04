//Abstract Data Type headers
#ifndef cmsketch_adt_h
#define cmsketch_adt_h
#include <stdlib.h>
#include <stdint.h>

typedef int (*comparator) (void* item, void* item2);
typedef uint32_t (*hasher) (void* key, size_t item);
typedef void (*dtor) (void* value);
#endif