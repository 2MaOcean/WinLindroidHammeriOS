#include "idt.h"
#include "console.h"
#include "keyboard.h"

struct idt_entry { uint16_t base_low; uint16_t sel; uint8_t always0; uint8_t flags; uint16_t base_high; } __attribute__((packed));
struct idt_ptr { uint16_t limit; uint32_t base; } __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtp;

static inline void outb(uint16_t port, uint8_t val) { __asm__ volatile("outb %0, %1"::"a"(val),"Nd"(port)); }
static inline void io_wait(void) { outb(0x80, 0); }

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void pic_init(void) {
    outb(0x20, 0x11); io_wait();
    outb(0xA0, 0x11); io_wait();
    outb(0x21, 0x20); io_wait();
    outb(0xA1, 0x28); io_wait();
    outb(0x21, 0x04); io_wait();
    outb(0xA1, 0x02); io_wait();
    outb(0x21, 0x01); io_wait();
    outb(0xA1, 0x01); io_wait();
    outb(0x21, 0x0);  io_wait();
    outb(0xA1, 0x0);  io_wait();
}

void pit_init(void) {
    uint32_t divisor = 1193180 / 100;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

// 从汇编传过来的寄存器结构体
typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

static uint32_t tick = 0;

void idt_handler(registers_t *regs) {
    if (regs->int_no == 32) {
        tick++;
        // 时钟中断
    } else if (regs->int_no == 33) {
        keyboard_interrupt_handler(); // 调用键盘中断
    }

    // 发送 EOI
    if (regs->int_no >= 40) outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void idt_init(void) {
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;
    idtp.base = (uint32_t)&idt;
    for(int i=0; i<256; i++) idt_set_gate(i, 0, 0, 0);

    idt_set_gate(32, (uint32_t)isr32, 0x08, 0x8E); // 时钟
    idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E); // 键盘

    __asm__ volatile("lidt (%0)"::"r"(&idtp));
    __asm__ volatile("sti");
}
