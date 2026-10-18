#include "console.h"
#include "permission.h"
#include "shell.h"
#include "gdt.h"
#include "idt.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "multiboot2.h"
#include "types.h"

void kernel_main(uint32_t magic, uint32_t mb_info) {
    console_init(); 
    console_puts("\nWinLindroidHammeriOS P2 Kernel\n");
    console_puts("===============================\n");

    uint8_t* mbi = (uint8_t*)mb_info;
    uint32_t mem_upper = 0;
    struct multiboot_tag* tag = (struct multiboot_tag*)(mbi + 8);
    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap*)tag;
            for (uint8_t* entry = (uint8_t*)mmap->entries; entry < (uint8_t*)mmap + mmap->size; entry += mmap->entry_size) {
                struct multiboot_mmap_entry* e = (struct multiboot_mmap_entry*)entry;
                if (e->type == MULTIBOOT_MEMORY_AVAILABLE && e->addr + e->len > mem_upper) mem_upper = e->addr + e->len;
            }
        }
        tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    gdt_init();
    pmm_init(mem_upper);
    vmm_init();
    
    heap_init(); // 初始化堆

    // 测试 kmalloc
    char* test_str = (char*)kmalloc(32);
    test_str[0] = 'O'; test_str[1] = 'K'; test_str[2] = '\0';
    console_puts("[TEST] kmalloc string: ");
    console_puts(test_str);
    console_puts("\n");
    kfree(test_str);

    pic_init();
    idt_init();
    pit_init();

    console_puts("\n[SYS] Kernel init complete.\n\n");
    shell_run();
}
