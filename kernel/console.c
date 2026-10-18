#include "console.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ATTR 0x07
#define SERIAL_PORT 0x3F8

static volatile uint16_t* const VGA = (volatile uint16_t*)0xB8000;
static uint32_t row = 0, col = 0;

// 汇编读写端口指令
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
    outb(SERIAL_PORT + 0, 0x03);    // 波特率除数低字节 (38400)
    outb(SERIAL_PORT + 1, 0x00);    // 波特率除数高字节
    outb(SERIAL_PORT + 3, 0x03);    // 8位数据位，无奇偶校验，1位停止位
    outb(SERIAL_PORT + 2, 0xC7);    // 启用 FIFO
    outb(SERIAL_PORT + 4, 0x0B);    // 启用 IRQ，RTS/DSR
}

static void serial_putc(char c) {
    while ((inb(SERIAL_PORT + 5) & 0x20) == 0); // 等待发送缓冲区为空
    outb(SERIAL_PORT, c);
}

static uint16_t vga_entry(char c) { return (uint16_t)(uint8_t)c | ((uint16_t)VGA_ATTR << 8); }

void console_clear(void) {
    for (uint32_t y = 0; y < VGA_HEIGHT; y++)
        for (uint32_t x = 0; x < VGA_WIDTH; x++)
            VGA[y * VGA_WIDTH + x] = vga_entry(' ');
    row = col = 0;
    serial_putc('\f'); // 清空终端屏幕 (部分终端支持)
}

void console_init(void) {
    serial_init();
    console_clear();
}

void console_putc(char c) {
    // 1. 输出到串口
    serial_putc(c);

    // 2. 输出到 VGA
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
