#include "../include/framebuffer.h"
#include "../include/serial.h"
#include "../include/gdt.h"

void os_main() {
    fb_clear();
    fb_write_str("Welcome to SaturnOS!");
    serial_write_str("test");
    setup_gdt();
}