.section .text
.extern idt_handler
.global isr_common_stub

# 通用中断处理入口
isr_common_stub:
    pusha            ; 保存通用寄存器

    mov %ds, %ax     ; 保存数据段
    push %eax

    mov $0x10, %ax   ; 加载内核数据段
    mov %ds, %ax
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax

    push %esp        ; 传递当前栈指针
    call idt_handler ; 调用 C 语言处理函数
    add $4, %esp     ; 清理参数

    pop %eax         ; 恢复数据段
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa             ; 恢复通用寄存器
    add $8, %esp     ; 清理中断号和错误码
    iret             ; 中断返回
