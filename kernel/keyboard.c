#include "keyboard.h"
#include "types.h"

#define BUF_SIZE 256
static char buffer[BUF_SIZE];
static volatile int head = 0;
static volatile int tail = 0;

// 中断处理程序调用的入队函数
void keyboard_push(char c) {
    int next = (head + 1) % BUF_SIZE;
    if (next != tail) {
        buffer[head] = c;
        head = next;
    }
}

// Shell 调用的出队函数
char keyboard_getchar(void) {
    if (head == tail) return 0;
    char c = buffer[tail];
    tail = (tail + 1) % BUF_SIZE;
    return c;
}
