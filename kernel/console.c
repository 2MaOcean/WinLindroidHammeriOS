#include "console.h"

#define SERIAL_PORT 0x3F8

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static void serial_init(void) {
    outb(SERIAL_PORT + 1, 0x00);    // 禁用中断
    outb(SERIAL_PORT + 3, 0x80);    // 启用 DLAB
    outb(SERIAL_PORT + 0, 0x01);    // 115200 波特率除数低字节
    outb(SERIAL_PORT + 1, 0x00);    // 除数高字节
    outb(SERIAL_PORT + 3, 0x03);    // 8N1
    outb(SERIAL_PORT + 2, 0xC7);    // 启用 FIFO
    outb(SERIAL_PORT + 4, 0x0B);
}

static void serial_putc(char c) {
    int timeout = 100000;
    while (timeout-- > 0 && (inb(SERIAL_PORT + 5) & 0x20) == 0); 
    outb(SERIAL_PORT, c);
}

void console_init(void) { serial_init(); }

void console_clear(void) {
    // 用打出大量换行来模拟清屏，避免 ANSI 转义序列在部分终端乱码
    for(int i = 0; i < 50; i++) {
        serial_putc('\r'); serial_putc('\n');
    }
}

void console_putc(char c) {
    if (c == '\n') serial_putc('\r');
    serial_putc(c);
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
