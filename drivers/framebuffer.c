#include "../include/framebuffer.h"

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