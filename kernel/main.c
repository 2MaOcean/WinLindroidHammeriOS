#include "console.h"
#include "permission.h"
#include "types.h"
#define MULTIBOOT2_BOOT_MAGIC 0x36d76289u
static void print_level_row(perm_level_t l) {
    console_puts(perm_level_name(l));
    console_puts(" | GUI=");
    console_puts(perm_level_has_gui_entry(l) ? "yes" : "no");
    console_puts(" | hotswap=");
    console_puts(perm_level_can_attempt_kernel_hotswap(l) ? "yes" : "no");
    console_puts(" | caps=");
    console_puthex32(perm_default_capabilities(l));
    console_puts("\n");
}
void kernel_main(uint32_t magic, uint32_t mb) {
    console_clear();
    console_puts("WinLindroidHammeriOS P0 kernel\n");
    console_puts("Multiboot2 magic: ");
    console_puthex32(magic);
    console_puts(magic == MULTIBOOT2_BOOT_MAGIC ? " OK\n" : " INVALID\n");
    console_puts("Multiboot2 info: ");
    console_puthex32(mb);
    console_puts("\n\nPermission model:\n");
    for (int i = PERM_RESTRICTED; i <= PERM_SUPERROOT; i++) print_level_row((perm_level_t)i);
    console_puts("\nHidden commands: unlock-superroot, enter-restricted\n");
    console_puts("Hotswap: ");
    console_puts(perm_hotswap_policy_text());
    console_puts("\n\nP0: bootable skeleton only. System halted.\n");
}
