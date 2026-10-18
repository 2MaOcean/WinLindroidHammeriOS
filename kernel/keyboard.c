#include "keyboard.h"
#include "input.h"
#include "types.h"

static inline uint8_t inb(uint16_t port) {
    uint8_t r;
    __asm__ volatile ("inb %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}

static int shift_pressed = 0;

// 标准 US 键盘映射表
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

void keyboard_interrupt_handler(void) {
    uint8_t scancode = inb(0x60);

    // Shift 按下与释放
    if (scancode == 0x2A || scancode == 0x36) { shift_pressed = 1; return; }
    if (scancode == 0xAA || scancode == 0xB6) { shift_pressed = 0; return; }

    // 忽略按键释放（高位为1）
    if (scancode & 0x80) return;

    char c = 0;
    if (scancode < sizeof(keymap_lower)) {
        c = shift_pressed ? keymap_upper[scancode] : keymap_lower[scancode];
    }

    if (c != 0) {
        input_push(c); // 统一送入输入子系统
    }
}

// 保留这个空函数是为了兼容旧接口，但逻辑已移至 input.c
char keyboard_getchar(void) {
    return input_getchar();
}
