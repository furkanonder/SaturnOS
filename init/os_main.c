#include "../include/framebuffer.h"
#include "../include/serial.h"
#include "../include/gdt.h"
#include "../include/idt.h"

void os_main() {
    fb_clear();
    fb_write_str("Welcome to SaturnOS!");
    serial_write_str("test");
    init_gdt();
    init_idt();

     asm volatile ("int $0x3");
}