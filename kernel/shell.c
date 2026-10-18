#include "shell.h"
#include "input.h"
#include "console.h"
#include "permission.h"
#include "vfs.h"
#include "types.h"

static int streq(const char* a, const char* b) { while (*a && *b) { if (*a != *b) return 0; a++; b++; } return *a == *b; }
static int startswith(const char* str, const char* prefix) {
    while (*prefix) { if (*str != *prefix) return 0; str++; prefix++; }
    return 1;
}

void shell_run(void) {
    char buf[64];
    uint32_t len = 0;
    perm_level_t current_level = PERM_USER;

    console_puts("\nWLHIOS shell initialized. Type 'help'.\n");
    
    while (1) {
        console_puts("wlhios> ");
        len = 0;

        while (1) {
            console_poll_input();
            char c = input_getchar();
            if (c == 0) { __asm__ volatile("hlt"); continue; }

            if (c == '\n' || c == '\r') {
                console_putc('\n');
                buf[len] = '\0';
                break;
            } else if (c == '\b' || c == 0x7F) {
                if (len > 0) { len--; console_puts("\b \b"); }
            } else if (c >= ' ' && c <= '~') {
                if (len < sizeof(buf) - 1) { buf[len++] = c; console_putc(c); }
            }
        }

        if (len == 0) continue;

        if (streq(buf, "help")) console_puts("Commands: help, whoami, unlock-superroot, clear, ls, mkdir <name>, touch <name>\n");
        else if (streq(buf, "whoami")) { console_puts("Current level: "); console_puts(perm_level_name(current_level)); console_puts("\n"); }
        else if (streq(buf, "clear")) console_clear();
        else if (streq(buf, "ls")) vfs_list();
        else if (startswith(buf, "mkdir ")) {
            if (vfs_create(buf + 6, VFS_DIR)) console_puts("Directory created.\n");
            else console_puts("Error: Already exists.\n");
        }
        else if (startswith(buf, "touch ")) {
            if (vfs_create(buf + 6, VFS_FILE)) console_puts("File created.\n");
            else console_puts("Error: Already exists.\n");
        }
        else if (streq(buf, "unlock-superroot")) {
            if (perm_command_allowed(current_level, buf)) { console_puts("[SYSTEM] SuperRoot verification triggered.\n"); current_level = PERM_SUPERROOT; }
            else console_puts("[ERROR] Permission denied.\n");
        } else console_puts("Unknown command.\n");
    }
}
