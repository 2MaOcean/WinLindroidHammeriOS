.RECIPEPREFIX := >
CC ?= gcc
LD ?= ld
CFLAGS := -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -Wextra -O2 -Iinclude
LDFLAGS := -m elf_i386 -T linker.ld
OBJS := build/boot.o build/console.o build/permission.o build/main.o
.PHONY: all iso run check clean
all: build/wlhios-kernel.elf
build:
>mkdir -p build
build/boot.o: boot/boot.S | build
>$(CC) -m32 -c $< -o $@
build/console.o: kernel/console.c include/console.h include/types.h | build
>$(CC) $(CFLAGS) -c $< -o $@
build/permission.o: kernel/permission.c include/permission.h include/types.h | build
>$(CC) $(CFLAGS) -c $< -o $@
build/main.o: kernel/main.c include/console.h include/permission.h include/types.h | build
>$(CC) $(CFLAGS) -c $< -o $@
build/wlhios-kernel.elf: $(OBJS) linker.ld
>$(LD) $(LDFLAGS) -o $@ $(OBJS)
iso: build/wlhios-kernel.elf
>mkdir -p build/iso/boot/grub
>cp build/wlhios-kernel.elf build/iso/boot/wlhios-kernel.elf
>cp grub/grub.cfg build/iso/boot/grub/grub.cfg
>grub-mkrescue -o build/wlhios.iso build/iso
run: iso
>qemu-system-i386 -cdrom build/wlhios.iso -m 256M
check: all
>@echo "Build OK."
clean:
>rm -rf build
