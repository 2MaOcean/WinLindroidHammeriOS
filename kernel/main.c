#include "console.h"
#include "permission.h"
#include "shell.h"
#include "gdt.h"
#include "types.h"

void kernel_main(uint32_t magic, uint32_t mb_info) {
    (void)magic;
    (void)mb_info;

    console_init(); 
    
    console_puts("\n\nWinLindroidHammeriOS P2 kernel\n");
    console_puts("===============================\n");

    console_puts("Initializing GDT (Ring 0 & Ring 3)...\n");
    gdt_init();
    console_puts("GDT loaded successfully.\n\n");

    console_puts("Starting interactive shell...\n");
    shell_run();
}
