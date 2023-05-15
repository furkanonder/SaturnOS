#include "../drivers/framebuffer/framebuffer.h"
#include "../drivers/serial/serial.h"
#include "../mm/segmentation/gdt.h"
#include "../drivers/interrupts/idt.h"
#include "../drivers/keyboard/keyboard.h"

void os_main() {
    fb_clear();
    fb_write_str("Welcome to SaturnOS!\n");
    serial_write_str("test");
    init_gdt();
    init_idt();
    init_keyboard();
    //asm volatile ("int $0x3");
}