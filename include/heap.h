#ifndef WLHIOS_HEAP_H
#define WLHIOS_HEAP_H
#include "types.h"

void heap_init(void);
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
