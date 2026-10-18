#include "heap.h"
#include "pmm.h"
#include "console.h"

typedef struct block_header {
    size_t size;
    int is_free;
    struct block_header* next;
} block_header_t;

static block_header_t* head = 0;
#define BLOCK_SIZE sizeof(block_header_t)

void heap_init(void) {
    // 从物理内存管理器申请一页 (4KB) 作为初始堆
    void* page = pmm_alloc_page();
    if (!page) {
        console_puts("[HEAP] Failed to allocate initial page!\n");
        return;
    }

    head = (block_header_t*)page;
    head->size = PAGE_SIZE - BLOCK_SIZE;
    head->is_free = 1;
    head->next = 0;
    console_puts("[HEAP] Kernel heap initialized.\n");
}

void* kmalloc(size_t size) {
    if (size == 0) return 0;

    block_header_t* curr = head;
    while (curr) {
        if (curr->is_free && curr->size >= size) {
            // 如果块比需要的大很多，切分它
            if (curr->size > size + BLOCK_SIZE) {
                block_header_t* new_block = (block_header_t*)((uint8_t*)curr + BLOCK_SIZE + size);
                new_block->size = curr->size - size - BLOCK_SIZE;
                new_block->is_free = 1;
                new_block->next = curr->next;
                
                curr->size = size;
                curr->next = new_block;
            }
            curr->is_free = 0;
            return (void*)((uint8_t*)curr + BLOCK_SIZE);
        }
        curr = curr->next;
    }
    return 0; // 内存不足
}

void kfree(void* ptr) {
    if (!ptr) return;
    block_header_t* block = (block_header_t*)((uint8_t*)ptr - BLOCK_SIZE);
    block->is_free = 1;

    // 合并相邻的空闲块 (简单合并)
    block_header_t* curr = head;
    while (curr && curr->next) {
        if (curr->is_free && curr->next->is_free) {
            curr->size += BLOCK_SIZE + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}
