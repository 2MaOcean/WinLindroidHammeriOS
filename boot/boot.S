.section .multiboot
.align 8
multiboot_header_start:
    .long 0xE85250D6
    .long 0
    .long multiboot_header_end - multiboot_header_start
    .long -(0xE85250D6 + 0 + (multiboot_header_end - multiboot_header_start))
    .align 8
    .short 0
    .short 0
    .long 8
multiboot_header_end:
.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:
.section .text
.global _start
.extern kernel_main
_start:
    cli
    mov $stack_top, %esp
    push %ebx
    push %eax
    call kernel_main
1:
    hlt
    jmp 1b
.section .note.GNU-stack,"",@progbits
