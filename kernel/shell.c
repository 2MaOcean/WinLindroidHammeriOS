#include "shell.h"
#include "keyboard.h"
#include "console.h"
#include "permission.h"
#include "types.h"

static int streq(const char* a, const char* b) { while (*a && *b) { if (*a != *b) return 0; a++; b++; } return *a == *b; }

void shell_run(void) {
    char buf[64];
    uint32_t len = 0;
    perm_level_t current_level = PERM_USER;

    console_puts("\nWLHIOS shell initialized. Type 'help'.\n");
    
    while (1) {
        console_puts("wlhios> ");
        len = 0;

        while (1) {
            char c = keyboard_getchar();
            if (c == 0) continue;

            // 同时兼容 \n 和 \r 作为回车
            if (c == '\n' || c == '\r') {
                console_putc('\n');
                buf[len] = '\0';
                break;
            } else if (c == '\b' || c == 0x7F) { // 兼容退格键
                if (len > 0) {
                    len--;
                    console_puts("\b \b");
                }
            } else if (c >= ' ' && c <= '~') {
                if (len < sizeof(buf) - 1) {
                    buf[len++] = c;
                    console_putc(c);
                }
            }
        }

        if (len == 0) continue;

        if (streq(buf, "help")) console_puts("Commands: help, whoami, unlock-superroot, enter-restricted, clear\n");
        else if (streq(buf, "whoami")) { console_puts("Current level: "); console_puts(perm_level_name(current_level)); console_puts("\n"); }
        else if (streq(buf, "clear")) console_clear();
        else if (streq(buf, "unlock-superroot")) {
            if (perm_command_allowed(current_level, buf)) { console_puts("[SYSTEM] SuperRoot verification triggered.\n"); current_level = PERM_SUPERROOT; }
            else console_puts("[ERROR] Permission denied.\n");
        } else if (streq(buf, "enter-restricted")) {
            if (perm_command_allowed(current_level, buf)) { console_puts("[SYSTEM] Entering Restricted mode.\n"); current_level = PERM_RESTRICTED; }
            else console_puts("[ERROR] Permission denied.\n");
        } else console_puts("Unknown command.\n");
    }
}
