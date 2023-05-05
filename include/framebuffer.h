#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#define FB_BLACK          0
#define FB_BLUE           1
#define FB_GREEN          2
#define FB_CYAN           3
#define FB_RED            4
#define FB_MAGENTA        5
#define FB_BROWN          6
#define FB_LIGHT_GREY     7
#define FB_DARK_GREY      8
#define FB_LIGHT_BLUE     9
#define FB_LIGHT_GREEN    10
#define FB_LIGHT_CYAN     11
#define FB_LIGHT_RED      12
#define FB_LIGHT_MAGENTA  13
#define FB_LIGHT_BROWN    14
#define FB_WHITE          15

// 0x000B8000 is the starting address for the frame buffer
#define FRAME_BUFFER_ADDRESS 0x000B8000
#define FB_WIDTH 80
#define FB_HEIGHT 25

// The Framebuffer I/O ports
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

// Cursor Ports
#define CURSOR_LOCATION_HIGH    0x0E
#define CURSOR_LOCATION_LOW     0x0F

// The Framebuffer I/O port commands
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_write_str(char *buf);
void fb_clear();
void os_printf(const char *format, ...);

#endif