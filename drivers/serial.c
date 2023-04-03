#include "../include/serial.h"
#include "../include/io.h"
#include "../include/string.h"

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    /* The divisor is a 16-bit number, but we can only send 8 bits at a time.
     * We must therefore send an instruction telling the serial port to first
     * expect the highest 8 bits, then the lowest 8 bits. This is done by sending
     * 0x80 to the line command port. */
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

/** serial_configure_line:
 *  Configures the line of the given serial port.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com) {
    /*  Name   | Description
     *    d    | Enables (d = 1) or disables (d = 0) DLAB(Divisor Latch Access Bit)
     *    b    | If break control is enabled (b = 1) or disabled (b = 0)
     *   prty  | The number of parity bits to use
     *    s    | The number of stop bits to use (s = 0 equals 1, s = 1 equals 1.5 or 2)
     *    dl   | Describes the length of the data */

    /* The port is set to have a data length of 8 bits, no parity bits,
     * one stop bit and break control disabled.
     *
     * Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s |  dl |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03 */

    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_configure_fifo:
 *  Configures the fifo of the given serial port.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_fifo(unsigned short com) {
    /*  Name | Description
     *   lvl | How many bytes should be stored in the FIFO buffers
     *   bs  | If the buffers should be 16 or 64 bytes large
     *    r  | Reserved for future use
     *   dma | How the serial port data should be accessed
     *   clt | Clear the transmission FIFO buffer
     *   clr | Clear the receiver FIFO buffer
     *    e  | If the FIFO buffer should be enabled or not */

    /*  We use the value 0xC7 = 11000111 that:
     *  Enables FIFO
     *  Clear both receiver and transmission FIFO queues
     *  Use 14 bytes as size of queue
     *
     * Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
     * Content: | lvl | bs | r | dma | clt | clr | e |
     * Value:   | 1 1 | 0  | 0  | 0  |  1  |  1  | 1 | = 0xC7 */

    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty 1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com) {
    /* Bit | Description
     *  0  | Data Ready
     *  1  | Overrun Error
     *  2  | Parity error
     *  3  | Framing error
     *  4  | Break Interrupt
     *  5  | Empty Transmitter Holding Register
     *  6  | Empty Data Holding Registers
     *  7  | Error in Received FIFO
     *
     * Bit 5 merely tells you that the UART is capable of receiving more characters,
     * including into the FIFO for transmitting.
     *
     * Bit:   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     * Value: | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | = 0x20 */

    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/** serial_configure_modem:
 *  Configures the modem of the given serial port.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_modem(unsigned short com) {
    /* Name | Description
     *   r  | Reserved
     *  af 	| Autoflow control enabled
     *  lb  | Loopback mode (used for debugging serial ports)
     *  ao2 | Auxiliary output 2, used for receiving interrupts
     *  ao1 | Auxiliary output 1
     *  rts | Ready To Transmit
     *  dtr | Data Terminal Ready */

    /* We don’t need to enable interrupts, because we won’t handle any received data.
     * Therefore, we use the configuration value 0x03 = 00000011 (RTS = 1 and DTS = 1).
     *
     * Bit:     | 7 | 6 | 5  | 4  |  3  |  2  |  1  |  0  |
     * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
     * Value:   | 0 | 0 | 0  | 0  |  0  |  0  |  1  |  1  | = 0x03 */

    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

// Initialize the serial
void serial_init() {
    serial_configure_baud_rate(SERIAL_COM1_BASE, 2);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_fifo(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);
}

/** serial_write_str:
*  Writes a character array to the serial.
*
*  @param buf  Character array
*/
void serial_write_str(char *buf) {
    serial_init();

    for(size_t i=0; i < strlen(buf); i++) {
        char c = buf[i];
        while (serial_is_transmit_fifo_empty(SERIAL_COM1_BASE) == 0);
        outb(SERIAL_COM1_BASE, c);
    }

}