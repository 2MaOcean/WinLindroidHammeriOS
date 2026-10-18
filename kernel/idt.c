#include "idt.h"
#include "console.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtp;

extern void isr_common_stub(void);

// 读写端口
static inline void outb(uint16_t port, uint8_t val) { __asm__ volatile("outb %0, %1"::"a"(val),"Nd"(port)); }
static inline uint8_t inb(uint16_t port) { uint8_t r; __asm__ volatile("inb %1, %0":"=a"(r):"Nd"(port)); return r; }
static inline void io_wait(void) { outb(0x80, 0); }

// 设置 IDT 门
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

// 初始化 PIC (重新映射中断号到 32-47)
void pic_init(void) {
    outb(0x20, 0x11); io_wait();
    outb(0xA0, 0x11); io_wait();
    outb(0x21, 0x20); io_wait(); // 主 PIC 从 32 开始
    outb(0xA1, 0x28); io_wait(); // 从 PIC 从 40 开始
    outb(0x21, 0x04); io_wait();
    outb(0xA1, 0x02); io_wait();
    outb(0x21, 0x01); io_wait();
    outb(0xA1, 0x01); io_wait();
    outb(0x21, 0x0);  io_wait();
    outb(0xA1, 0x0);  io_wait();
}

// 初始化 PIT (时钟中断，每秒 100 次)
void pit_init(void) {
    uint32_t divisor = 1193180 / 100;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

// C 语言中断分发中心
void idt_handler(uint32_t esp) {
    uint32_t int_no = 0; // 简化处理：通过读取栈或全局变量判断，先用简单逻辑
    // 实际项目中这里需要压入 int_no
    // 暂时我们只处理时钟(32)和键盘(33)
    
    // 向 PIC 发送 EOI 结束信号
    outb(0x20, 0x20);
    if (int_no >= 40) outb(0xA0, 0x20);
}

// 简易处理
void idt_init(void) {
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;
    idtp.base = (uint32_t)&idt;
    
    // 清空
    for(int i=0; i<256; i++) idt_set_gate(i, 0, 0, 0);

    // 注册我们汇编里的通用 stub
    idt_set_gate(32, (uint32_t)isr_common_stub, 0x08, 0x8E); // 时钟
    idt_set_gate(33, (uint32_t)isr_common_stub, 0x08, 0x8E); // 键盘

    __asm__ volatile("lidt (%0)"::"r"(&idtp));
    __asm__ volatile("sti"); // 开启硬件中断
}
