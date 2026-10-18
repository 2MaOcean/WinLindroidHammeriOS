#include "keyboard.h"
#include "types.h"

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

char keyboard_getchar(void) {
    // 在调试阶段，只从 COM1 串口读取输入，彻底屏蔽 PS/2，防止双份输入乱码
    if (inb(0x3FD) & 1) {
        return inb(0x3F8);
    }
    return 0;
}
