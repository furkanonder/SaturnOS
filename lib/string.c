#include "../include/stddef.h"

/** strlen:
 * Returns the length of the string.
 *
 * @param s string
 * @return length of string
 */
size_t strlen(const char *s) {
    size_t i = 0;
    while(s[i] != '\0') {
        i++;
    }
    return i;
}

/** memmove:
 * Copies the values of n bytes from dst to src
 *
 * @param dst Pointer to the destination array where the content is to be copied
 * @param src Pointer to the source of data to be copied
 * @param n Number of bytes to copy.
 * @return dst
 */
void *memmove(void *dst, const void *src, size_t n) {
    char *d = (char*) dst;
    char *s = (char*) src;

    for(size_t i=0; i < n; i++)
        d[i] = s[i];

    return d;
}

/** memset:
 * Sets the first n bytes of the block of memory pointed by dest to the specified value.
 * @param dest  Pointer to the block of memory to fill.
 * @param value Value to be set.
 * @param n Number of bytes to be set to the value.
 * @return ptr
 */
void *memset(void *dest, int value, size_t n) {
    char *ptr = (char *)dest;
    while(n-- > 0) {
        *ptr++ = value;
    }
    return ptr;
}