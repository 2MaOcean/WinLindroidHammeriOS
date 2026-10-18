#ifndef WLHIOS_IDT_H
#define WLHIOS_IDT_H
#include "types.h"

void idt_init(void);
void pic_init(void);
void pit_init(void);
extern void keyboard_interrupt_handler(void);

#endif
