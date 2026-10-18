#ifndef WLHIOS_CONSOLE_H
#define WLHIOS_CONSOLE_H
#include "types.h"

void console_init(void);
void console_clear(void);
void console_putc(char c);
void console_puts(const char* s);
void console_puthex32(uint32_t v);
void console_put_dec(uint32_t v);

// 新增：专门用于串口输入的轮询
char console_getchar(void);
void console_poll_input(void);

#endif
