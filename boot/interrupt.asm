.section .text
.extern idt_handler

# 宏：没有错误码的中断
%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push dword 0      ; 压入假错误码
    push dword %1     ; 压入中断号
    jmp isr_common_stub
%endmacro

ISR_NOERR 32          ; 时钟中断
ISR_NOERR 33          ; 键盘中断

.global isr_common_stub
isr_common_stub:
    pusha             ; 保存通用寄存器
    mov %ds, %ax
    push %eax         ; 保存数据段

    mov $0x10, %ax    ; 加载内核数据段
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %esp         ; 传递栈指针给 C 语言 (包含中断号等信息)
    call idt_handler
    add $4, %esp

    pop %eax          ; 恢复数据段
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa              ; 恢复通用寄存器
    add $8, %esp      ; 弹出中断号和错误码
    iret
