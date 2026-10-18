#include "vmm.h"
#include "pmm.h"
#include "console.h"

#define PAGE_PRESENT 0x1
#define PAGE_RW      0x2

// 页目录 (Page Directory) 必须 4KB 对齐
static uint32_t page_directory[1024] __attribute__((aligned(4096)));
// 页表 (Page Table)，映射前 4MB
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void vmm_init(void) {
    // 1. 清空页目录，全部标记为不可用
    for(int i = 0; i < 1024; i++) page_directory[i] = 0x00000002; // 读写属性，未映射

    // 2. 映射前 4MB 物理内存到虚拟内存 (1:1 映射)
    for(uint32_t i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_RW;
    }

    // 3. 将页表地址挂载到页目录的第 0 项
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    // 4. 加载页目录到 CR3 寄存器
    __asm__ volatile("mov %0, %%cr3" :: "r"(page_directory));

    // 5. 开启分页（设置 CR0 寄存器的 PG 位）
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // 最高位置 1
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));

    console_puts("[VMM] Paging enabled. Virtual memory active.\n");
}
