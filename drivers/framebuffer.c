#include "../include/framebuffer.h"
#include "../include/string.h"
#include "../include/stdint.h"
#include "../include/io.h"

static unsigned int fb_col = 0;
static unsigned int fb_row = 0;

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
    char *fb = (char *) FRAME_BUFFER_ADDRESS;
    unsigned int loc = i * 2;

    fb[loc] = c;
    fb[loc + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
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

// Clears the frame buffer
void fb_clear() {
    for(size_t i=0; i < FB_WIDTH * FB_HEIGHT; i++) {
        fb_write_cell(i, ' ', FB_BLACK, FB_BLACK);
    }
    fb_move_cursor(0);
    fb_col = 0;
    fb_row = 0;
}

// Adds new line to frame buffer
void fb_new_line() {
    while(FB_WIDTH - 1 != fb_col) {
        fb_write_cell(fb_col, ' ', FB_BLACK, FB_BLACK);
        fb_col++;
    }
    fb_col = 0;
    fb_row++;
    fb_move_cursor(fb_col + (fb_row * FB_WIDTH));
}

// Shifts one row the framebuffer
void fb_scroll_down() {
    // Each cell has 16 bits.
    uint16_t *fb = (uint16_t *) FRAME_BUFFER_ADDRESS;
    // One row can contain 80(FB_WIDTH) cell, shift one row framebuffer to add new line.
    memmove(fb, fb + FB_WIDTH, (FB_WIDTH * 2) * (FB_HEIGHT * 2));
}

/** fb_write_str:
 *  Writes a character array to the frame buffer.
 *
 *  @param buf  Character array
 */
void fb_write_str(char *buf) {

    for(size_t i=0; i < strlen(buf); i++) {
        char c = buf[i];

        if (c == '\n') {
            fb_new_line();
        }
        else {
            fb_write_cell(fb_col + (fb_row * FB_WIDTH), c, FB_WHITE, FB_BLACK);
            fb_col++;
        }

        if (fb_col == FB_WIDTH - 1) {
            fb_row++;
            fb_col = 0;
        }
        if (fb_row == FB_HEIGHT - 1) {
            fb_scroll_down();
            fb_row--;
        }
        fb_move_cursor(fb_col + (fb_row * FB_WIDTH));
    }

}