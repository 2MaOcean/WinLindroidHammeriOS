#include "input.h"
#include "types.h"

#define INPUT_BUF_SIZE 256
static char buffer[INPUT_BUF_SIZE];
static volatile int head = 0;
static volatile int tail = 0;

void input_push(char c) {
    int next = (head + 1) % INPUT_BUF_SIZE;
    if (next != tail) {
        buffer[head] = c;
        head = next;
    }
}

char input_getchar(void) {
    if (head == tail) return 0;
    char c = buffer[tail];
    tail = (tail + 1) % INPUT_BUF_SIZE;
    return c;
}
