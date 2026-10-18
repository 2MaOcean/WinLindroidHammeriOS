#include "keyboard.h"
#include "types.h"
static inline uint8_t inb(uint16_t port) { uint8_t ret; __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port)); return ret; }
char keyboard_getchar(void) {
    if (inb(0x3FD) & 1) return inb(0x3F8);
    if (inb(0x64) & 1) {
        uint8_t scancode = inb(0x60);
        if (scancode < 0x80) {
            static const char keymap[] = {
                0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
                '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
                0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
                '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
            };
            if (scancode < sizeof(keymap)) return keymap[scancode];
        }
    }
    return 0;
}
