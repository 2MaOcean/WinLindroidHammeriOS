#include "console.h"
#include "permission.h"
#include "shell.h"
#include "types.h"

void kernel_main(uint32_t magic, uint32_t mb_info) {
    console_init(); // 初始化串口和屏幕
    
    console_puts("\n\nWinLindroidHammeriOS P1 kernel\n");
    console_puts("===============================\n");

    console_puts("Permission model:\n");
    for (int i = PERM_RESTRICTED; i <= PERM_SUPERROOT; i++) {
        console_puts(perm_level_name((perm_level_t)i));
        console_puts("\n");
    }

    // 进入交互式命令行
    shell_run();
}
