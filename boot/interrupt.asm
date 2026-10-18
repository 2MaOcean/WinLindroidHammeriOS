section .text
extern idt_handler

; 宏：没有错误码的中断
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

global isr_common_stub
isr_common_stub:
    pusha             ; 保存通用寄存器

    mov ax, ds
    push eax          ; 保存数据段 (按 32 位压栈)

    mov ax, 0x10      ; 加载内核数据段
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp          ; 传递栈指针给 C 语言
    call idt_handler
    add esp, 4        ; 清理参数

    pop eax           ; 恢复数据段
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa              ; 恢复通用寄存器
    add esp, 8        ; 弹出中断号和错误码
    iret
