#ifndef WLHIOS_CONSOLE_H
#define WLHIOS_CONSOLE_H
#include "types.h"
void console_clear(void);
void console_putc(char c);
void console_puts(const char* s);
void console_puthex32(uint32_t v);
void console_put_dec(uint32_t v);
#endif
