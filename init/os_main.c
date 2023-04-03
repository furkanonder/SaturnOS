#include "../include/framebuffer.h"

void os_main() {
    fb_write_cell(0, 'H', FB_WHITE, FB_BLACK);
    fb_write_cell(1, 'i', FB_WHITE, FB_BLACK);
    fb_write_cell(2, '!', FB_WHITE, FB_BLACK);
}