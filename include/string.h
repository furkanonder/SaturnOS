#ifndef __STRING_H__
#define __STRING_H__

#include "../include/stddef.h"

size_t strlen(const char *s);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *dest, int value, size_t n);

#endif