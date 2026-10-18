.RECIPEPREFIX := >
CC ?= gcc
LD ?= ld
CFLAGS := -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -Wextra -O2 -Iinclude
LDFLAGS := -m elf_i386 -T linker.ld
OBJS := build/boot.o build/gdt.asm.o build/interrupt.asm.o build/gdt.o build/idt.o build/pmm.o build/console.o build/permission.o build/keyboard.o build/shell.o build/main.o

all: build/wlhios-kernel.elf

build:
>mkdir -p build

build/boot.o: boot/boot.S | build
>$(CC) -m32 -c $< -o $@

build/gdt.asm.o: boot/gdt.asm | build
>nasm -f elf32 $< -o $@

build/interrupt.asm.o: boot/interrupt.asm | build
>nasm -f elf32 $< -o $@

build/console.o: kernel/console.c | build
>$(CC) $(CFLAGS) -c $< -o $@

build/gdt.o: kernel/gdt.c | build
>$(CC) $(CFLAGS) -c $< -o $@

build/idt.o: kernel/idt.c | build
>$(CC) $(CFLAGS) -c $< -o $@

build/pmm.o: kernel/pmm.c | build
>$(CC) $(CFLAGS) -c $< -o $@

build/permission.o: kernel/permission.c | build
>$(CC) $(CFLAGS) -c $< -o $@

build/keyboard.o: kernel/keyboard.c | build
>$(CC) $(CFLAGS) -c $< -o $@

build/shell.o: kernel/shell.c | build
>$(CC) $(CFLAGS) -c $< -o $@

build/main.o: kernel/main.c | build
>$(CC) $(CFLAGS) -c $< -o $@

build/wlhios-kernel.elf: $(OBJS) linker.ld
>$(LD) $(LDFLAGS) -o $@ $(OBJS)

iso: build/wlhios-kernel.elf
>mkdir -p build/iso/boot/grub
>cp build/wlhios-kernel.elf build/iso/boot/wlhios-kernel.elf
>cp grub/grub.cfg build/iso/boot/grub/grub.cfg
>grub-mkrescue -o build/wlhios.iso build/iso

clean:
>rm -rf build
