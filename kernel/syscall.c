#include "console.h"
#include "types.h"

void syscall_handler(uint32_t syscall_num, uint32_t arg1) {
    switch (syscall_num) {
        case 1: console_putc((char)arg1); break;
        case 2: console_clear(); break;
        default: console_puts("[SYSCALL] Unknown.\n"); break;
    }
}
