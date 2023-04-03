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