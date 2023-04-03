#include "../include/framebuffer.h"

void os_main() {
    fb_clear();
    fb_write_str("Welcome to SaturnOS!");
}