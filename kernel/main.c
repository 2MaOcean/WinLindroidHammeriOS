#include "console.h"
#include "permission.h"
#include "shell.h"
#include "gdt.h"
#include "idt.h"
#include "types.h"

void kernel_main(uint32_t magic, uint32_t mb_info) {
    (void)magic;
    (void)mb_info;

    // 第一步：初始化串口并立刻打印（如果这行都看不到，说明串口驱动坏了，或者 GRUB 根本没引导内核）
    console_init(); 
    console_puts("\n\n[TEST 0] Serial console initialized.\n");

    console_puts("[TEST 1] Initializing GDT...\n");
    gdt_init();
    console_puts("[TEST 1] GDT init OK.\n");

    console_puts("[TEST 2] Initializing PIC...\n");
    pic_init();
    console_puts("[TEST 2] PIC init OK.\n");

    console_puts("[TEST 3] Initializing IDT and enabling interrupts...\n");
    idt_init();
    console_puts("[TEST 3] IDT init OK.\n");
    
    console_puts("[TEST 4] Initializing PIT...\n");
    pit_init();
    console_puts("[TEST 4] PIT init OK.\n");

    console_puts("[SYS] All hardware initialized. Starting Shell...\n\n");
    shell_run();
}
