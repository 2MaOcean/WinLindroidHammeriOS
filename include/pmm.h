#ifndef WLHIOS_PMM_H
#define WLHIOS_PMM_H
#include "types.h"

#define PAGE_SIZE 4096
#define MAX_PAGES 1048576 // 支持最多 4GB 内存

void pmm_init(uint32_t mem_upper);
void* pmm_alloc_page(void);
void pmm_free_page(void* ptr);
uint32_t pmm_get_free_memory(void);

#endif
