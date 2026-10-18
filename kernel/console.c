#include "console.h"
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ATTR 0x07
static volatile uint16_t* const VGA = (volatile uint16_t*)0xB8000;
static uint32_t row = 0, col = 0;
static uint16_t vga_entry(char c) { return (uint16_t)(uint8_t)c | ((uint16_t)VGA_ATTR << 8); }
void console_clear(void) {
    for (uint32_t y = 0; y < VGA_HEIGHT; y++)
        for (uint32_t x = 0; x < VGA_WIDTH; x++)
            VGA[y * VGA_WIDTH + x] = vga_entry(' ');
    row = col = 0;
}
void console_putc(char c) {
    if (c == '\n') { col = 0; if (++row >= VGA_HEIGHT) console_clear(); return; }
    if (c == '\r') { col = 0; return; }
    VGA[row * VGA_WIDTH + col] = vga_entry(c);
    if (++col >= VGA_WIDTH) { col = 0; if (++row >= VGA_HEIGHT) console_clear(); }
}
void console_puts(const char* s) { while (*s) console_putc(*s++); }
void console_puthex32(uint32_t v) {
    const char* h = "0123456789ABCDEF";
    console_puts("0x");
    for (int i = 7; i >= 0; i--) console_putc(h[(v >> (i * 4)) & 0xF]);
}
void console_put_dec(uint32_t v) {
    char b[10]; int i = 0;
    if (!v) { console_putc('0'); return; }
    while (v && i < 10) { b[i++] = '0' + (v % 10); v /= 10; }
    while (i) console_putc(b[--i]);
}
