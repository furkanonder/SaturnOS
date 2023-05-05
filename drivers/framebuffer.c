#include "../include/framebuffer.h"
#include "../include/string.h"
#include "../include/stdint.h"
#include "../include/io.h"
#include "../include/stdarg.h"

char *fb = (char *) FRAME_BUFFER_ADDRESS;

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i in the frame buffer.
 *
 *  @param i  The location in the frame buffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 *  Example of a frame buffer cell;
 *  Bit:     | 15 14 13 12 | 11 10 9 8 | 7 6 5 4 3 2 1 0 |
 *  Content: |      BG     |     FG    |      ASCII      |
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
    fb[i] = c;
    fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

/** fb_move_cursor:
 *  Moves the cursor of the frame buffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos) {
    /* Moving the cursor of the framebuffer is done via two different I/O ports. The cursor’s position is determined
     * with a 16 bits integer: 0 means row zero, column zero; 1 means row zero, column one; 80 means row one,column zero
     * and so on.
     *
     * Since the position is 16 bits large,and the out assembly code instruction argument is 8 bits, the position must
     * be sent in two turns, first 8 bits then the next 8 bits. The framebuffer has two I/O ports, one for accepting
     * the data,and one for describing the data being received. FB_COMMAND_PORT is the port that describes the data and
     * port FB_DATA_PORT is for the data itself */
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

/** fb_clear:
 *  Clears the frame buffer
 */
void fb_clear() {
    for(int i=0; i < FB_WIDTH * FB_HEIGHT; i++) {
        fb_write_cell(i * 2, ' ', FB_BLACK, FB_BLACK);
    }
    fb_move_cursor(0);
}

/** get_cursor_loc:
 *  Returns the cursor position.
 *
 *  @return cursor position
 */
uint16_t get_cursor_loc() {
    uint16_t pos = 0;

    outb(FB_COMMAND_PORT, CURSOR_LOCATION_LOW);
    pos |= inb(FB_DATA_PORT);
    outb(FB_COMMAND_PORT, CURSOR_LOCATION_HIGH);
    pos |= inb(FB_DATA_PORT) << 8;

    return pos;
}

/** fb_scroll_down:
 *  Shifts one row the framebuffer
 */
void fb_scroll_down() {
    // Each cell has 16 bits.
    uint16_t *fb = (uint16_t *) FRAME_BUFFER_ADDRESS;
    // One row can contain 80(FB_WIDTH) cell, shift one row framebuffer to add new line.
    memmove(fb, fb + FB_WIDTH, (FB_WIDTH * 2) * (FB_HEIGHT * 2));
}

/** fb_write_str:
 *  Writes a character array to the frame buffer.
 *
 *  @param buf Character array
 */
void fb_write_str(char *buf) {
    uint16_t cursor_position = get_cursor_loc() * 2;

    for(size_t i=0; i < strlen(buf); i++) {
        if(cursor_position >= FB_WIDTH * FB_HEIGHT * 2){
            fb_scroll_down();
            cursor_position -= FB_WIDTH * 2;
        }

        if (buf[i] == '\n') {
            cursor_position = (cursor_position + FB_WIDTH * 2) - (cursor_position % (FB_WIDTH * 2));
        }
        else {
            fb_write_cell(cursor_position, buf[i], FB_WHITE, FB_BLACK);
            cursor_position += 2;
        }
    }

    fb_move_cursor(cursor_position / 2);
}

/** fb_write_char:
 *  Writes a character to the frame buffer.
 *  @param c Character
 */
void fb_write_char(unsigned char c) {
    char buf[] = {c, '\0'};
    fb_write_str(buf);
}

/** os_printf:
 *  Writes the string pointed by format to frame buffer. If format includes format specifiers
 *  (subsequences beginning with %), the additional arguments following format are formatted and inserted in the
 *  resulting string replacing their respective specifiers.
 *
 * @param format C string that contains the text to be written to frame buffer.
 */
void os_printf(const char *format, ...) {
    char buf[20];
    va_list ap;
    va_start(ap, format);

    for(size_t i=0; i < strlen(format); i++) {
        if(format[i] == '%') {
            char *arg = '\0';

            switch(format[++i]) {
                case 'd':
                    arg = itoa(va_arg(ap, int), buf, 10);
                    break;
                case 'x':
                    arg = itoa(va_arg(ap, int), buf, 16);
                    break;
                default:
                    break;
            }
            fb_write_str(arg);
        }
        else {
            fb_write_char(format[i]);
        }
    }

    va_end(ap);
}