#include "console.h"
#include "permission.h"
#include "shell.h"
#include "gdt.h"
#include "idt.h"
#include "pmm.h"
#include "multiboot2.h"
#include "types.h"

void kernel_main(uint32_t magic, uint32_t mb_info) {
    console_init(); 
    console_puts("\n\nWinLindroidHammeriOS P2 Kernel\n");
    console_puts("===============================\n");

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        console_puts("[ERR] Invalid Multiboot2 magic!\n");
        while(1) __asm__ volatile("hlt");
    }

    // 解析 Multiboot2 标签
    uint8_t* mbi = (uint8_t*)mb_info;
    uint32_t mem_upper = 0;
    
    // 8 字节对齐跳过前 8 个字节（total_size 和 reserved）
    struct multiboot_tag* tag = (struct multiboot_tag*)(mbi + 8);
    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap*)tag;
            for (uint8_t* entry = (uint8_t*)mmap->entries; 
                 entry < (uint8_t*)mmap + mmap->size; 
                 entry += mmap->entry_size) {
                struct multiboot_mmap_entry* e = (struct multiboot_mmap_entry*)entry;
                if (e->type == MULTIBOOT_MEMORY_AVAILABLE) {
                    // 找到最大的可用内存块
                    if (e->addr + e->len > mem_upper) {
                        mem_upper = e->addr + e->len;
                    }
                }
            }
        }
        // 标签按 8 字节对齐
        tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    console_puts("[HW]  Detected Physical Memory: ");
    console_put_dec(mem_upper / 1024 / 1024);
    console_puts(" MB\n");

    console_puts("[CPU] Initializing GDT...\n");
    gdt_init();

    console_puts("[MEM] Initializing Physical Memory Manager (Bitmap)...\n");
    pmm_init(mem_upper);

    // 测试分配内存
    void* p1 = pmm_alloc_page();
    console_puts("[MEM] Allocated 1 page at: ");
    console_puthex32((uint32_t)p1);
    console_puts("\n");

    console_puts("[CPU] Initializing PIC & IDT...\n");
    pic_init();
    idt_init();
    pit_init();

    console_puts("[SYS] Kernel init complete. Dropping to shell.\n\n");
    shell_run();
}
