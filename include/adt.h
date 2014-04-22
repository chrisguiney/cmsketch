//Abstract Data Type headers
#ifndef cmsketch_adt_h
#define cmsketch_adt_h
#include <stdint.h>

typedef int (*comparator) (void* item, void* item2);
typedef uint32_t (*hasher) (void* key);

#endif