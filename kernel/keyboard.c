#include "keyboard.h"
#include "types.h"

#define BUF_SIZE 256
static char buffer[BUF_SIZE];
static volatile int head = 0;
static volatile int tail = 0;

static inline uint8_t inb(uint16_t port) { uint8_t r; __asm__ volatile("inb %1, %0":"=a"(r):"Nd"(port)); return r; }

static int shift_pressed = 0;

static const char keymap_lower[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

static const char keymap_upper[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

void keyboard_push(char c) {
    int next = (head + 1) % BUF_SIZE;
    if (next != tail) {
        buffer[head] = c;
        head = next;
    }
}

char keyboard_getchar(void) {
    // 1. 优先从 PS/2 硬件中断的环形缓冲区读取
    if (head != tail) {
        char c = buffer[tail];
        tail = (tail + 1) % BUF_SIZE;
        return c;
    }
    
    // 2. 兜底：如果在 UTM 串口终端里运行，直接轮询串口
    if (inb(0x3FD) & 1) {
        return inb(0x3F8);
    }
    
    return 0;
}

void keyboard_interrupt_handler(void) {
    uint8_t scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) { shift_pressed = 1; return; }
    if (scancode == 0xAA || scancode == 0xB6) { shift_pressed = 0; return; }
    if (scancode & 0x80) return;

    char c = 0;
    if (scancode < sizeof(keymap_lower)) {
        c = shift_pressed ? keymap_upper[scancode] : keymap_lower[scancode];
    }

    if (c != 0) {
        keyboard_push(c);
    }
}
