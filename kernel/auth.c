#include "auth.h"
#include "vfs.h"
#include "console.h"
#include "input.h"
#include "libc.h"
#include "heap.h"

static void read_line(char* buf, int max_len, int hide_chars) {
    int len = 0;
    while (1) {
        console_poll_input();
        char c = input_getchar();
        if (c == 0) { __asm__ volatile("hlt"); continue; }
        
        if (c == '\n' || c == '\r') {
            console_putc('\n'); buf[len] = '\0'; break;
        } else if (c == '\b' || c == 0x7F) {
            if (len > 0) { len--; if (!hide_chars) console_puts("\b \b"); }
        } else if (c >= ' ' && c <= '~') {
            if (len < max_len - 1) { 
                buf[len++] = c; 
                if (!hide_chars) console_putc(c); 
                else console_putc('*'); 
            }
        }
    }
}

void auth_init(void) {
    fs_node_t* etc = vfs_find("etc");
    if (!etc) etc = vfs_create("etc", VFS_DIR);
    fs_node_t* passwd = vfs_find("passwd");
    if (!passwd) passwd = vfs_create("passwd", VFS_FILE);
}

void auth_register(void) {
    char username[32];
    char password[64];
    
    console_puts("\n=== User Registration ===\n");
    console_puts("Username: ");
    read_line(username, 32, 0);
    console_puts("Password: ");
    read_line(password, 64, 1);

    fs_node_t* f = vfs_find("passwd");
    if (!f) f = vfs_create("passwd", VFS_FILE);

    char buffer[128];
    uint32_t h = k_hash(password);
    
    k_strcpy(buffer, username, 32);
    int len = 0; while(buffer[len]) len++;
    buffer[len++] = ':';
    
    char hash_str[16]; int hi = 0;
    if (h == 0) hash_str[hi++] = '0';
    else {
        char rev[16]; int ri = 0;
        while (h > 0) { rev[ri++] = '0' + (h % 10); h /= 10; }
        while (ri > 0) hash_str[hi++] = rev[--ri];
    }
    hash_str[hi] = '\0';
    
    k_strcpy(buffer + len, hash_str, 16);
    len += hi;
    buffer[len++] = '\n';
    buffer[len] = '\0';

    vfs_write(f, buffer, len);
    console_puts("[AUTH] Registration saved to VFS.\n");
}

int auth_login(perm_level_t* out_level) {
    char username[32];
    char password[64];
    
    while(1) {
        console_puts("\nWinLindroidHammeriOS Login\n");
        console_puts("Username (or 'register'): ");
        read_line(username, 32, 0);
        
        if (k_strcmp(username, "register") == 0) {
            auth_register();
            continue;
        }
        
        console_puts("Password: ");
        read_line(password, 64, 1);
        
        if (k_strcmp(username, "root") == 0 && k_strcmp(password, "admin") == 0) {
            *out_level = PERM_ADMIN;
            console_puts("[AUTH] Welcome Root (Admin).\n");
            return 1;
        } else if (k_strcmp(username, "user") == 0 && k_strcmp(password, "1234") == 0) {
            *out_level = PERM_USER;
            console_puts("[AUTH] Welcome User.\n");
            return 1;
        }
        console_puts("[ERROR] Invalid credentials.\n");
    }
}
