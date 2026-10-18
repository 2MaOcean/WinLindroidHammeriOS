#ifndef WLHIOS_LIBC_H
#define WLHIOS_LIBC_H
#include "types.h"
int k_strcmp(const char* a, const char* b);
void k_strcpy(char* dest, const char* src, int max_len);
uint32_t k_hash(const char* str);
#endif
