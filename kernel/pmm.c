#include "pmm.h"
#include "console.h"

// 位图：每个 bit 代表 4KB (1 页)
static uint32_t bitmap[MAX_PAGES / 32];
static uint32_t total_pages = 0;
static uint32_t used_pages = 0;

static void set_bit(uint32_t i) {
    bitmap[i / 32] |= (1 << (i % 32));
}

static void clear_bit(uint32_t i) {
    bitmap[i / 32] &= ~(1 << (i % 32));
}

static int test_bit(uint32_t i) {
    return (bitmap[i / 32] & (1 << (i % 32))) != 0;
}

void pmm_init(uint32_t mem_upper) {
    // 简单起见，假设可用内存从 1MB 开始，并且总量由 GRUB 提供
    // 这里先将所有位图清零（全部标记为可用）
    for (int i = 0; i < MAX_PAGES / 32; i++) bitmap[i] = 0;

    total_pages = mem_upper / PAGE_SIZE;
    if (total_pages > MAX_PAGES) total_pages = MAX_PAGES;

    // 0 - 1MB 以及内核自身占用的区域，先简单粗暴地全部标记为已占用
    // 真实的系统需要精确解析 multiboot mmap
    for (uint32_t i = 0; i < 256; i++) {
        set_bit(i);
        used_pages++;
    }

    console_puts("[PMM] Total memory managed: ");
    console_put_dec(total_pages * 4 / 1024);
    console_puts(" MB\n");
}

void* pmm_alloc_page(void) {
    for (uint32_t i = 256; i < total_pages; i++) {
        if (!test_bit(i)) {
            set_bit(i);
            used_pages++;
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0; // 内存耗尽
}

void pmm_free_page(void* ptr) {
    uint32_t addr = (uint32_t)ptr;
    uint32_t page = addr / PAGE_SIZE;
    if (test_bit(page)) {
        clear_bit(page);
        used_pages--;
    }
}
