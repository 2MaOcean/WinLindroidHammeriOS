#include "shell.h"
#include "input.h"
#include "console.h"
#include "permission.h"
#include "vfs.h"
#include "libc.h"
#include "types.h"

void shell_run(perm_level_t current_level) {
    char buf[128];
    uint32_t len = 0;

    console_puts("\nType 'help' for commands.\n");
    
    while (1) {
        console_puts("wlhios> ");
        len = 0;
        while (1) {
            console_poll_input();
            char c = input_getchar();
            if (c == 0) { __asm__ volatile("hlt"); continue; }
            if (c == '\n' || c == '\r') {
                console_putc('\n'); buf[len] = '\0'; break;
            } else if (c == '\b' || c == 0x7F) {
                if (len > 0) { len--; console_puts("\b \b"); }
            } else if (c >= ' ' && c <= '~') {
                if (len < sizeof(buf) - 1) { buf[len++] = c; console_putc(c); }
            }
        }

        if (len == 0) continue;

        if (k_strcmp(buf, "help") == 0) console_puts("Commands: help, whoami, clear, ls, mkdir, touch, logout\n");
        else if (k_strcmp(buf, "whoami") == 0) { console_puts("Current level: "); console_puts(perm_level_name(current_level)); console_puts("\n"); }
        else if (k_strcmp(buf, "clear") == 0) console_clear();
        else if (k_strcmp(buf, "ls") == 0) vfs_list();
        else if (k_strcmp(buf, "logout") == 0) { console_puts("Logging out...\n"); return; }
        else if (buf[0]=='m' && buf[1]=='k') vfs_create(buf + 6, VFS_DIR);
        else if (buf[0]=='t' && buf[1]=='o') vfs_create(buf + 6, VFS_FILE);
        else console_puts("Unknown command.\n");
    }
}
