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
    outb(0x20, 0x11); io_wait(); outb(0xA0, 0x11); io_wait();
    outb(0x21, 0x20); io_wait(); outb(0xA1, 0x28); io_wait();
    outb(0x21, 0x04); io_wait(); outb(0xA1, 0x02); io_wait();
    outb(0x21, 0x01); io_wait(); outb(0xA1, 0x01); io_wait();
    outb(0x21, 0x0);  io_wait(); outb(0xA1, 0x0);  io_wait();
}

void pit_init(void) {
    uint32_t divisor = 1193180 / 100; outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF)); outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

typedef struct {
    uint32_t ds; uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code; uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

extern void isr0(); extern void isr1(); extern void isr2(); extern void isr3(); extern void isr4(); extern void isr5(); extern void isr6(); extern void isr7(); 
extern void isr8(); extern void isr9(); extern void isr10(); extern void isr11(); extern void isr12(); extern void isr13(); extern void isr14(); extern void isr15(); 
extern void isr16(); extern void isr17(); extern void isr18(); extern void isr19(); extern void isr20(); extern void isr21(); extern void isr22(); extern void isr23(); 
extern void isr24(); extern void isr25(); extern void isr26(); extern void isr27(); extern void isr28(); extern void isr29(); extern void isr30(); extern void isr31();
extern void isr32(); extern void isr33(); extern void isr128();

static const char *exception_messages[] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
    "Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
    "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt",
    "Coprocessor Fault", "Alignment Check", "Machine Check", "Reserved"
};

void idt_handler(registers_t *regs) {
    if (regs->int_no < 32) {
        console_puts("\n[FATAL] CPU Exception: ");
        console_puts(exception_messages[regs->int_no]);
        console_puts("\nError Code: "); console_puthex32(regs->err_code);
        console_puts("\nEIP: "); console_puthex32(regs->eip);
        console_puts("\nSystem Halted.\n");
        while(1) __asm__ volatile("cli; hlt");
    }
    
    if (regs->int_no == 32) { /* Timer */ }
    else if (regs->int_no == 33) { keyboard_interrupt_handler(); }
    else if (regs->int_no == 128) { /* Syscall 入口 */ }
    
    if (regs->int_no >= 40) outb(0xA0, 0x20);
    if (regs->int_no >= 32) outb(0x20, 0x20);
}

void idt_init(void) {
    idtp.limit = sizeof(struct idt_entry) * 256 - 1; 
    idtp.base = (uint32_t)&idt;
    for(int i=0; i<256; i++) idt_set_gate(i, 0, 0, 0);

    uint32_t isrs[] = { 
        (uint32_t)isr0, (uint32_t)isr1, (uint32_t)isr2, (uint32_t)isr3, (uint32_t)isr4, (uint32_t)isr5, (uint32_t)isr6, (uint32_t)isr7, 
        (uint32_t)isr8, (uint32_t)isr9, (uint32_t)isr10, (uint32_t)isr11, (uint32_t)isr12, (uint32_t)isr13, (uint32_t)isr14, (uint32_t)isr15, 
        (uint32_t)isr16, (uint32_t)isr17, (uint32_t)isr18, (uint32_t)isr19, (uint32_t)isr20, (uint32_t)isr21, (uint32_t)isr22, (uint32_t)isr23, 
        (uint32_t)isr24, (uint32_t)isr25, (uint32_t)isr26, (uint32_t)isr27, (uint32_t)isr28, (uint32_t)isr29, (uint32_t)isr30, (uint32_t)isr31 
    };
    
    for(int i=0; i<32; i++) idt_set_gate(i, isrs[i], 0x08, 0x8E);

    idt_set_gate(32, (uint32_t)isr32, 0x08, 0x8E); 
    idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E);
    idt_set_gate(128, (uint32_t)isr128, 0x08, 0xEE); // 允许 Ring 3 访问

    __asm__ volatile("lidt (%0)"::"r"(&idtp));
    __asm__ volatile("sti");
}
