#include "console.h"
#include "permission.h"
#include "shell.h"
#include "gdt.h"
#include "idt.h"
#include "types.h"

void kernel_main(uint32_t magic, uint32_t mb_info) {
    (void)magic;
    (void)mb_info;

    console_init(); 
    console_puts("\n\nWinLindroidHammeriOS P2 kernel\n");
    console_puts("===============================\n");

    console_puts("[CPU] Initializing GDT...\n");
    gdt_init();
    
    console_puts("[CPU] Initializing PIC...\n");
    pic_init();
    
    console_puts("[CPU] Initializing IDT and enabling interrupts...\n");
    idt_init();
    
    console_puts("[HW]  Initializing PIT (100Hz)...\n");
    pit_init();

    console_puts("[SYS] All hardware initialized.\n\n");
    shell_run();
}
