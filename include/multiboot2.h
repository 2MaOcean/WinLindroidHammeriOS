#ifndef WLHIOS_MULTIBOOT2_H
#define WLHIOS_MULTIBOOT2_H
#include "types.h"

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289
#define MULTIBOOT_TAG_TYPE_END        0
#define MULTIBOOT_TAG_TYPE_MMAP       6

struct multiboot_tag {
    uint32_t type;
    uint32_t size;
};

struct multiboot_mmap_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t zero;
} __attribute__((packed));
#define MULTIBOOT_MEMORY_AVAILABLE 1

struct multiboot_tag_mmap {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    struct multiboot_mmap_entry entries[0];
};

#endif
